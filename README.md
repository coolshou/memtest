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
wget https://musl.cc/aarch64-linux-musl-cross.tgz
tar xvf aarch64-linux-musl-cross.tgz
export PATH=<path to toolchain>/aarch64-linux-musl-cross/bin/:$PATH
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
# Docker
```
docker build --platform linux/arm64 -t memeat:1.0 .
docker save -o memeat_arm64.tar memeat:1.0
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