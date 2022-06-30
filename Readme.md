

## How to build

```sh
sgd_a_star$ cmake -S . -B build
-- ...
-- Configuring done
-- Generating done
-- Build files have been written to: /home/pascal/cpp_dev_ws/src/sgd_a_star/build

sgd_a_star$ cmake --build build
Scanning dependencies of target gtest
...
Scanning dependencies of target lib_a_star
...
[100%] Built target gmock_main

sgd_a_star$ cd build && ctest
Test project .../sgd_a_star/build
    Start 1: ...
1/3 Test #1: <Test>..................   Passed    0.00 sec
...
```
