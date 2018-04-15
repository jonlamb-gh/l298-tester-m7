# l298-tester-m7
l298-tester running on STM32F7

## Debugging

```
make debug
```

```
make st-semihost
```

```
make gdb
```

Debug/stdout is routed to `uart1`, often shows up as `ACM` device, ie `/dev/ttyACM0`.
