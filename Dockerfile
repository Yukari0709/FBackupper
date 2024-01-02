FROM ubuntu:20.04

WORKDIR /FBackupper

COPY . /FBackupper

ENV TZ="Asia/Shanghai"
RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list

RUN apt-get update && \
    apt-get install -y build-essential cmake libssl-dev

RUN rm -rf build && mkdir build
RUN cd build && cmake .. && make && make install

