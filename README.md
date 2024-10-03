# USAGE
## Default options
### run example

```sh
mkdir build
cd build
cmake ..
make
./lirs
```
 or
```sh
./perfect
```

 ### run tests
running tests from build directory
```sh
ctest
```

## Additional options

### run 10 million elements test

```sh
cmake -DENABLE_EXTRA=ON ..
make
ctest
```
