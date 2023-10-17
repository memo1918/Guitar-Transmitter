FROM ubuntu:22.04

RUN apt-get update &&\
    apt-get install python3 -y &&\
    apt-get install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential -y

WORKDIR /app

ENTRYPOINT ["./build.sh"]
