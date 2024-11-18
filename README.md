# README.md
Unitree A1 motor uses DUP to communicate VOFA+ and uses YAML file to adjust parameters
### Notice

support motor: A1 motor

You need to download the YAML package first
```bash
git clone https://github.com/jbeder/yaml-cpp.git
cd yaml-cpp
mkdir build && cd build
cmake .. -DYAML_BUILD_SHARED_LIBS=ON
make
sudo make install
```

### Build
```bash
mkdir build
cd build
cmake ..
make
```

### Run
If the compilation is successful, many C++ example executable files will be generated in the build folder. Then run the examples with 'sudo', for example:
```bash
sudo ./example_a1_motor
```

If you want to give serial port permissions continuously
```bash
sudo chmod 777 /dev/ttyUSB0
```
or shell script
```bash
mkdir chmod.sh
sudo chmod +x chmod.sh
```
chmod.sh file input：
sudo chmod 777 /dev/ttyUSB0
```bash
./chmod.sh
```
