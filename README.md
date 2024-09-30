# USAGE
## Default options
### run example

```
mkdir build
```
```
cd build
```
```
cmake ..
```
```
make
```

```
./lirs
```
 or
```
./perfect
```

 ### run tests

```
cmake ..
```
```
make`
```
```
ctest
```

## Additional options

### run 10 million elements test

```
cmake -DENABLE_EXTRA=ON ..
```
```
make
```
```
ctest
```
