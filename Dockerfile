FROM silkeh/clang:latest as build

RUN apt update && \
    apt install -y \
      python3-pip \
      cmake \
      python3.11-venv
RUN python3 -m venv .venv
RUN .venv/bin/pip install conan

COPY conanfile.txt /app/
RUN mkdir /app/build && cd /app/build && \
    conan install .. --build=missing -s compiler.libcxx=libstdc++11 -s build_type=Release

COPY ./src /app/src
COPY ./pingpong /app/pingpong
COPY CMakeLists.txt /app/ 

RUN cd /app/build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build . -j6

# FROM ubuntu:22.04 as run

# RUN groupadd -r www && useradd -r -g www www
# USER www 

# COPY --from=build /app/build/bin/* /app/

# WORKDIR /app/
