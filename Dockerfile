FROM amd64/alpine:latest

COPY amd64/memtestd /usr/local/bin

ENTRYPOINT ["/usr/local/bin/memtestd"]

EXPOSE 5555
