FROM ubuntu:18.04

ENV LLVM_VERSION=6.0

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        vim build-essential wget \
        python3-dev virtualenv \
        llvm-${LLVM_VERSION} \
        llvm-${LLVM_VERSION}-dev \
        clang-${LLVM_VERSION} && \
    rm -rf /var/lib/apt/lists/*

RUN wget https://bootstrap.pypa.io/get-pip.py && \
    python3 get-pip.py && rm -f get-pip.py

RUN pip3 install cmake==3.12.0
