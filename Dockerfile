#FROM alpine:latest as build
#RUN sed -i 's/dl-cdn.alpinelinux.org/mirrors.aliyun.com/g' /etc/apk/repositories\
#  && apk add --no-cache build-base
#WORKDIR /app/src
#COPY src .
# amd64
#RUN make && make DIST=/app/output install 
# arm
#RUN make CROSS_TOOL && make DIST=/app/output install 


FROM alpine:latest
#COPY --from=build /app/output /usr/local
COPY src/memtestd /usr/local/bin

ENTRYPOINT ["/usr/local/bin/memtestd"]

EXPOSE 5555
