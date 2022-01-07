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