FROM ubuntu:20.04

WORKDIR /FBackupper

COPY . /FBackupper

RUN apt-get update && \
    apt-get install -y build-essential cmake libssl-dev && \

RUN rm -rf build && mkdir build
RUN cd build && cmake .. && make && make install

