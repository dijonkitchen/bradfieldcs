# BradfieldCS 

## Software Systems: Behind the Abstractions 2021-01

### clock speed

```sh
clang clock_speed.c -o clock_speed
./clock_speed
```

Modify to 10 million iterations
```sh
clang -O0 -S clock_speed_nop_loop.c 
```
Add 2000 `nop` codes in iteration

```sh
clang  clock_speed.s -o clock_speed_nop_loop
time ./nop_loop
```

Check math to get better clock speed. 
20 billion / time / 1 billion hertz... 

