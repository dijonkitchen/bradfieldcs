package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"sort"
)

const (
	PcapFileHeaderLength      = 24
	PcapPerPacketHeaderLength = 16
	EthernetHeaderLength      = 14
	HttpSourcePort            = uint16(80)
)

func main() {
	f, _ := os.Open("net.cap")

	// Skip capture file header
	f.Seek(PcapFileHeaderLength, io.SeekStart)

	// Parse each captured packet to extract HTTP fragments
	responseParts := map[uint32][]byte{}
	pch := make([]byte, PcapPerPacketHeaderLength)
	for {
		_, e := f.Read(pch)
		if e != nil {
			if e == io.EOF {
				break
			}
			panic(e)
		}
		payloadLength := binary.LittleEndian.Uint32(pch[8:12])

		// Read entire Ethernet frame
		frame := make([]byte, payloadLength)
		f.Read(frame)

		// Parse the IP datagram
		datagram := frame[EthernetHeaderLength:]
		ihl := (datagram[0] & 0x0f) << 2

		// Extract HTTP fragment, if this is part of the response
		segment := datagram[ihl:]
		sourcePort := binary.BigEndian.Uint16(segment[:2])
		if sourcePort == HttpSourcePort {
			seq := binary.BigEndian.Uint32(segment[4:8])
			tcpHeaderLength := (segment[12] >> 4) << 2
			responseParts[seq] = segment[tcpHeaderLength:]
		}

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

	// Write output
	out, _ := os.Create("out.jpeg")
	out.Write(parts[1])
	out.Close()
	fmt.Println("OK, result writen to out.jpeg")
}
