## data encoding notes

### hexadecimal 

#### decimal->hexadecimal

9 -> 0x9

136 -> 0x88

247 -> 0xf7

#### CSS colors

RGB: (2^8)^3 = 256^3 = Hex: 16^6 = 16777216 colors

#### hellohex

17B = 34 hexadecimals

First five bytes into binary:

0x: 68 65 6c 6c 6f

0b: 0110 1000 0110 0101 0110 1100 0110 1100 0110 1111


### integers

#### decimal->binary

4 -> 0b100

65 -> 0b1000001

105 -> 0b1101001

255 -> 0b11111111

#### uint binary->decimal

0b10 -> 2

0b11 -> 3

0b1101100 -> 108

0b1010101 -> 85

#### unsigned binary addition

11111111 + 00001101 = 

100001100 if no overflow

00001100 if overflow

255 + 13 = 

268 if no overflow

12 if overflow

#### decimal -> 8-bit two's complement int

127 -> 01111111

-128 -> 10000000

-1 -> 11111111

1 -> 00000001

-14 -> 11110010

#### 8-bit two's complement int -> decimal 

10000011 -> -128 + 3 = -125

11000100 -> -128 + 64 + 4 = -60

#### addition of two's complement ints

```
01111111       (127)
10000000+      (-128)
----------------
11111111       (-1)
```

Negate a number in two's complement by
flipping all the bits then adding one.

Compute subtraction of two's complement numbers by
adding the negation of second number. 

Values of most significant bit (MSB):

8-bit two's complement: -128

32-bit two's complement: -2^(32-1) = ~-2 billion

n-bit two's complement: -2^(n - 1)

#### detecting integer overflow

Check for only a carry-out bit (overflowing bit)
or only a carry-in bit (overflowing into MSB).

XOR(carry_in, carry_out)


### byte ordering

9001 in big-endian is 00100011 00101001.

#### TCP

source port: 
10101111 00000000 = 44800

destination port: 
10111100 00000110 = 48134

sequence number: 
01000100 00011110 01110011 01101000 = 1142846312

acknowledgment number:
11101111 11110010 10100000 00000010 = 4025655298

specified length of TCP header:
1000 = 8 * 4 = 32 32-bit words

Implies options should exist.

#### Byte ordering and integer encoding in bitmaps



### IEEE Floating Point

Largest 64-bit float: 2^52 * 2^(2^11) - 1 = ~1.455 * 10^632

Smallest 64-bit float: 

#### deconstruction

0 10000100010 10100000000000000000

0: positive sign

10000100: exponent = 128 + 4 = 132

01010100000000000000000: significand = 0b1.010101 = 1 + 1/4 + 1/16 + 1/64 = 1.328125

1.328125 * 2 ^ (132-127) = 42.5

For the largest fixed exponent, 
11111110 == 254 - 127 = 127,
the smallest change is 1 / (2^23) * 2^127 = 2^104

For the smallest fixed exponent,
00000001 == 1 - 127 = -126,
the smallest change is 1 / (2^23) * 2^(-126) = 2^(-149)

The precision of IEEE floating point values 
is higher around zero, 
and lower at higher orders of magnitude. 
This is good for scientific measurements. 

#### float casting

Round number to next highest power of two by
converting integer to a float,
bit-shifting off the significand,
un-offset the exponent bias,
apply exponent to the integer 1 for the value rounded down to nearest power of two,
then bit-shifting left if result is less than original value.


### character encodings

