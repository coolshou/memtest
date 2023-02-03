# memtest
memory consumer app for test propose

# Build
```
git clone https://github.com/coolshou/memtest.git
cd memtest/src
make
```

## for cross compile arm64-musl
```
make CROSS_TOOL=aarch64-linux-musl-
```
# RUN
 run server
```
memtestd
```
use the client to tell server how many memory (eq: 100M) will consumer
 
the memory usage will a little more then specify value
```
memtester 127.0.0.1 100
```

# Help
```
v20230201
Usage:
 memtester <ip address> [port] <num of MB>
        <ip address>: target ipv4 address
        [port]: target port number, default 5555 (option)
        <num of MB>: How many memory will consume in MB
```