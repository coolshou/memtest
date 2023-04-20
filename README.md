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

## cross compile mingw x86/x64 (TODO, current not work)

```
# x64
make CROSS_TOOL=x86_64-w64-mingw32-
# x86
make CROSS_TOOL=i686-w64-mingw32-
```

# RUN

 run server

```
memtestd
```

use the client to tell server how many memory (eq: 100M) will consumer

the memory usage will a little more then specify value

```
memtester -c 127.0.0.1 -n 100
```

# Docker

```
# for amd64 (same as Linux Host)
docker build -t memeat:1.0 .
docker save -o memeat.tar memeat:1.0
# for arm64 platform, other platform need modify "Dockerfile" COPY command
docker build --platform linux/arm64 -f Dockerfile-arm64-musl -t memeat:1.0 .
docker save -o memeat_arm64.tar memeat:1.0
# test
docker run -it memeat:1.0
```

# Help

```
v20230321
Usage:
 ./amd64/memtester -c <ip address> -n <num of MB> -d [delay] -p [port]
        -c <ip address>: target ipv4 address
        -n <num of MB>: How many memory will consume in MB
        -p [port]: target port number, default 5555 (option)
        -d [delay]:  delay time on consume memory, default 0 (sec)
        -w [wait]:  wait time to free the consume memory, default 30 (sec)
```
