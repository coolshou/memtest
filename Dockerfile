FROM alpine:latest

# amd64
# COPY amd64/memtestd /usr/local/bin
#  arm64-musl
COPY arm64-musl/memtestd /usr/local/bin

ENTRYPOINT ["/usr/local/bin/memtestd"]

EXPOSE 5555
