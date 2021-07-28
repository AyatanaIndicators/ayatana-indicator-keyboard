# Build and installation instructions

## Compile-time build dependencies

 - cmake (>= 3.13)
 - cmake-extras
 - glib-2.0 (>= 2.36)
 - gio-2.0 (>=2.36)
 - x11 (>=1.6.7)
 - libxklavier (>=5.4)
 - intltool
 - systemd
 - gcovr (>= 2.4)
 - lcov (>= 1.9)
 - gtest (>= 1.6.0)

## For end-users and packagers

```
cd ayatana-indicator-keyboard-X.Y.Z
mkdir build
cd build
cmake ..
make
sudo make install
```

**The install prefix defaults to `/usr`, change it with `-DCMAKE_INSTALL_PREFIX=/some/path`**

## For testers - unit tests only

```
cd ayatana-indicator-keyboard-X.Y.Z
mkdir build
cd build
cmake .. -DENABLE_TESTS=ON
make
make test
make cppcheck
```

## For testers - both unit tests and code coverage

```
cd ayatana-indicator-keyboard-X.Y.Z
mkdir build-coverage
cd build-coverage
cmake .. -DENABLE_COVERAGE=ON
make
make coverage-html
```
