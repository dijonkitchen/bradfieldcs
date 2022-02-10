package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"log"
	"os"
	"sort"
	"strings"
	"time"
)

const (
	PcapFileHeaderLength      = 24
	PcapPerPacketHeaderLength = 16
	EthernetHeaderLength      = 14
	IPHeaderDefaultLength     = 12 // May be longer with options
	TCPHeaderDefaultLength    = 20 // May be longer with options
	WebServerPort             = uint16(80)
)

type PcapFileHeader struct {
	MagicNumber      uint32
	MajorVersion     uint16
	MinorVersion     uint16
	TimezoneOffset   uint32
	TimezoneAccuracy uint32
	SnapshotLength   uint32
	LinkLayerFormat  uint32
}

type PcapPacketHeader struct {
	TimestampSeconds      uint32
	TimestampMicroseconds uint32
	PayloadLength         uint32
	UntruncatedLength     uint32
}

type EthernetHeader struct {
	DestinationMac []byte
	SourceMac      []byte
	EtherType      uint16
}

type IPHeader struct {
	Version        uint8
	HeaderLength   uint8
	DSCP           uint8
	ECN            uint8
	TotalLength    uint16
	Identification uint16
	Flags          uint16
	FragmentOffset uint16
	TTL            uint8
	Protocol       uint8
	Checksum       uint16
	SourceIP       []byte
	DestinationIP  []byte
}

type TCPHeader struct {
	SourcePort      uint16
	DestinationPort uint16
	SequenceNumber  uint32
	AckNumber       uint32
	DataOffset      uint16
	Flags           map[string]bool
	WindowSize      uint16
	Checksum        uint16
	UrgentPointer   uint16
}

func newPcapFileHeader(data []byte) *PcapFileHeader {
	return &PcapFileHeader{
		binary.LittleEndian.Uint32(data[:4]),
		binary.LittleEndian.Uint16(data[4:6]),
		binary.LittleEndian.Uint16(data[6:8]),
		binary.LittleEndian.Uint32(data[8:12]),
		binary.LittleEndian.Uint32(data[12:16]),
		binary.LittleEndian.Uint32(data[16:20]),
		binary.LittleEndian.Uint32(data[20:24]),
	}
}

func (hdr *PcapFileHeader) Verify() {
	assertEqual(uint32(0xa1b2c3d4), hdr.MagicNumber) // Little endian
	assertEqual(uint16(2), hdr.MajorVersion)
	assertEqual(uint16(4), hdr.MinorVersion)
	assertEqual(uint32(1), hdr.LinkLayerFormat) // LINKTYPE_ETHERNET
}

func (hdr *PcapFileHeader) String() string {
	return fmt.Sprintf("pcap format version %d.%d, snapshot length %d", hdr.MajorVersion, hdr.MinorVersion, hdr.SnapshotLength)
}

func newPcapPacketHeader(data []byte) *PcapPacketHeader {
	return &PcapPacketHeader{
		binary.LittleEndian.Uint32(data[:4]),
		binary.LittleEndian.Uint32(data[4:8]),
		binary.LittleEndian.Uint32(data[8:12]),
		binary.LittleEndian.Uint32(data[12:16]),
	}
}

func (hdr *PcapPacketHeader) Verify() {
	assertEqual(hdr.PayloadLength, hdr.UntruncatedLength) // Packet wasn't truncated
}

func (hdr *PcapPacketHeader) String() string {
	t := time.Unix(int64(hdr.TimestampSeconds), 1000*int64(hdr.TimestampMicroseconds))
	return fmt.Sprintf("Packet: %4dB captured at %v", hdr.PayloadLength, t)
}

func newEthernetHeader(data []byte) *EthernetHeader {
	return &EthernetHeader{
		data[:6],
		data[6:12],
		binary.BigEndian.Uint16(data[12:14]),
	}
}

func (hdr *EthernetHeader) Verify() {
	assertEqual(uint16(0x0800), hdr.EtherType) // IPv4
}

const hexDigit = "0123456789abcdef"

func formatMac(bs []byte) string {
	buf := make([]byte, 0, len(bs)*3-1)
	for i, b := range bs {
		if i > 0 {
			buf = append(buf, ':')
		}
		buf = append(buf, hexDigit[b>>4])
		buf = append(buf, hexDigit[b&0xF])
	}
	return string(buf)
}

func (hdr *EthernetHeader) String() string {
	return fmt.Sprintf("%s -> %s", formatMac(hdr.SourceMac), formatMac(hdr.DestinationMac))
}

func newIPHeader(data []byte) *IPHeader {
	b7_8 := binary.LittleEndian.Uint16(data[6:8])
	return &IPHeader{
		Version:        data[0] >> 4,
		HeaderLength:   (data[0] & 0x0f) << 2,
		DSCP:           data[1] >> 2,
		ECN:            data[1] & 0x03,
		TotalLength:    binary.BigEndian.Uint16(data[2:4]),
		Identification: binary.BigEndian.Uint16(data[4:6]),
		Flags:          b7_8 >> 13,
		FragmentOffset: b7_8 & 0x1fff,
		TTL:            uint8(data[8]),
		Protocol:       uint8(data[9]),
		Checksum:       binary.BigEndian.Uint16(data[10:12]),
		SourceIP:       data[12:16],
		DestinationIP:  data[16:20],
	}
}

func (hdr *IPHeader) Verify() {
	assertEqual(uint8(4), hdr.Version)
	assertEqual(uint8(0), hdr.ECN)
	assertEqual(uint8(6), hdr.Protocol) // indicates TCP
}

func formatIP(bs []byte) string {
	return fmt.Sprintf("%d.%d.%d.%d", bs[0], bs[1], bs[2], bs[3])
}

func (hdr *IPHeader) String() string {
	return fmt.Sprintf("%s -> %s", formatIP(hdr.SourceIP), formatIP(hdr.DestinationIP))
}

func newTCPHeader(data []byte) *TCPHeader {
	b12_13 := binary.BigEndian.Uint16(data[12:14])
	return &TCPHeader{
		SourcePort:      binary.BigEndian.Uint16(data[0:2]),
		DestinationPort: binary.BigEndian.Uint16(data[2:4]),
		SequenceNumber:  binary.BigEndian.Uint32(data[4:8]),
		AckNumber:       binary.BigEndian.Uint32(data[8:12]),
		DataOffset:      uint16(data[12]) >> 4,
		Flags: map[string]bool{
			"NS":  (b12_13 & (1 << 8)) > 0,
			"CWR": (b12_13 & (1 << 7)) > 0,
			"ECE": (b12_13 & (1 << 6)) > 0,
			"URG": (b12_13 & (1 << 5)) > 0,
			"ACK": (b12_13 & (1 << 4)) > 0,
			"PSH": (b12_13 & (1 << 3)) > 0,
			"RST": (b12_13 & (1 << 2)) > 0,
			"SYN": (b12_13 & (1 << 1)) > 0,
			"FIN": (b12_13 & (1 << 0)) > 0,
		},
		WindowSize:    binary.BigEndian.Uint16(data[14:16]),
		Checksum:      binary.BigEndian.Uint16(data[16:18]),
		UrgentPointer: binary.BigEndian.Uint16(data[18:20]),
	}
}

func (hdr *TCPHeader) String() string {
	keys := make([]string, 0, len(hdr.Flags))
	for k, v := range hdr.Flags {
		if v {
			keys = append(keys, k)
		}
	}
	return fmt.Sprintf(":%d -> :%d %s", hdr.SourcePort, hdr.DestinationPort, strings.Join(keys, " "))
}

func Checksum(data []byte) uint16 {
	// Ones' complement of ones' complement sum of pairs of octets
	total := 0
	for i := 0; i < len(data); i += 2 {
		total += int(binary.LittleEndian.Uint16(data[i : i+2]))
	}
	wrapped := uint16((total & 0xffff) + (total >> 16))
	return wrapped ^ 0xffff
}

func main() {
	f, e := os.Open("net.cap")
	check(e)

	// Parse pcap file header
	data := make([]byte, PcapFileHeaderLength)
	n, e := f.Read(data)
	check(e)
	assertEqual(PcapFileHeaderLength, n)
	fileHeader := newPcapFileHeader(data)
	fileHeader.Verify()
	fmt.Printf("%v\n", fileHeader)

	// Parse each captured packet to extract HTTP fragment
	responseParts := map[uint32][]byte{}
	data = make([]byte, PcapPerPacketHeaderLength)
	for {
		n, e := f.Read(data)
		if e != nil {
			if e == io.EOF {
				break
			}
			panic(e)
		}
		assertEqual(PcapPerPacketHeaderLength, n)
		packetHeader := newPcapPacketHeader(data)
		packetHeader.Verify()
		fmt.Printf("%v\n", packetHeader)

		// Read entire Ethernet frame
		frame := make([]byte, packetHeader.PayloadLength)
		n, e = f.Read(frame)
		check(e)
		assertEqual(int(packetHeader.PayloadLength), n)
		ethernetHeader := newEthernetHeader(frame[:EthernetHeaderLength])
		ethernetHeader.Verify()
		fmt.Printf("    %v\n", ethernetHeader)

		// Parse the IP datagram
		datagram := frame[EthernetHeaderLength:]
		ipHeader := newIPHeader(datagram[:IPHeaderDefaultLength])
		ipHeader.Verify()
		assertEqual(uint16(0), Checksum(datagram[:ipHeader.HeaderLength]))
		fmt.Printf("        %v\n", ipHeader)

		// Parse the TCP segment and extract HTTP fragment,
		// if part of the response
		segment := datagram[ipHeader.HeaderLength:]
		tcpHeader := newTCPHeader(segment[:TCPHeaderDefaultLength])
		fmt.Printf("          %v\n", tcpHeader)
		if tcpHeader.SourcePort == WebServerPort && !tcpHeader.Flags["SYN"] {
			responseParts[tcpHeader.SequenceNumber] = segment[tcpHeader.DataOffset<<2:]
		}

		fmt.Println()
	}

	// Sort by sequence number
	keys := make([]uint32, 0, len(responseParts))
	values := make([][]byte, len(responseParts))
	for k := range responseParts {
		keys = append(keys, k)
	}
	sort.Slice(keys, func(i, j int) bool { return keys[i] < keys[j] })
	for i, k := range keys {
		values[i] = responseParts[k]
	}

	// Combine fragments
	response := bytes.Join(values, []byte{})

	// Split into HTTP header and body
	parts := bytes.SplitN(response, []byte{'\r', '\n', '\r', '\n'}, 2)
	fmt.Println(string(parts[0]))

	// Write output
	out, e := os.Create("out.jpeg")
	check(e)
	out.Write(parts[1])
	out.Close()
	fmt.Println("\nOK, result writen to out.jpeg")
}

func assertEqual(expected, actual interface{}) {
	if actual != expected {
		log.Fatalf("Expected %v to be %v\n", actual, expected)
	}
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}
