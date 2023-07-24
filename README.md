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

https://www.codingwiththomas.com/blog/c-static-dynamic-polymorphism-crtp-and-c20s-concepts

https://www.codingwiththomas.com/blog/boost-asio-server-client-example

atomic expected desired
atomic.compare_exchange_strong(expected, desired);

Если атомик равен excpected 

if (atomic == expected) {
    atomic = desired
    return true
} else {
    excpected = desired
    return false;
} 
    

atomic = 1
expected = 2
desired = 3

while(!atomic.compare_exchange_strong(expected, desired))

Входим в цикл 
    atomic 1  expected 2  desired 3

Первая итерация
    atomic 1  expected 1  desired 3

Second
    atomic 3  expected 1  desired 3
    