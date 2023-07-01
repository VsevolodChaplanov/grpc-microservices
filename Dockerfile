FROM gcc:latest as build

RUN apt update && \
    apt install -y \
      python3-pip \
      cmake
RUN pip3 install conan --break-system-packages

COPY conanfile.txt /app/
RUN conan profile detect
RUN mkdir /app/build && cd /app/ && \
    conan install . --output-folder=build --build=missing --settings=build_type=Release

COPY ./src /app/src
COPY ./pingpong /app/pingpong
COPY CMakeLists.txt /app/ 

RUN cd /app/build && \
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release 
RUN ./conanbuild.sh
RUN cmake --build . -j6
RUN ./deactivate_conanbuild.sh

# FROM ubuntu:22.04 as run

# RUN groupadd -r www && useradd -r -g www www
# USER www 

# COPY --from=build /app/build/bin/* /app/

# WORKDIR /app/
