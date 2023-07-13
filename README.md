```bash
conan install . --output-folder=build --build=missing --settings=build_type=Release
cd build
sh Release/generators/conanbuild.sh
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR=Ninja -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake
sh Release/generators/deactivate_conanbuild.sh
cmake --install .. --config Release -j10
```

link to check
https://github.com/TrevorAttema/ASIO-Microservices-Template/tree/master