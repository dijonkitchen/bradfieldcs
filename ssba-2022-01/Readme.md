# BradfieldCS 

## Software Systems: Behind the Abstractions 2021-01

### clock speed

#### C / Assembly

```sh
clang clock_speed.c -o clock_speed
time ./clock_speed
```

C took 1.5 seconds, measuring 0.7 GHz.

Modify to 10 million iterations
```sh
clang -O0 -S clock_speed_nop_loop.c 
```
Add 2000 `nop` codes in iteration

```sh
clang  clock_speed.s -o clock_speed_nop_loop
time ./clock_speed_nop_loop
```

Check math to get better clock speed. 
20 billion / time / 1 billion hertz... 

Modified Assembly took ~1.5 seconds, measuring 1.7 GHz. 

#### Python

```shell
time python clock_speed.py
```

Took ~30 seconds on Python, measuring 0.03 GHz. 