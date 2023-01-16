

# MR Simulator

MR Robot Simulator
a configuration file motorSim.yaml is used to configure the system

```yaml
motors:  # a list of motors initial position
  - 5
  - 3
  - 6
  - 12
positions: # a list of initial positions (should match the motors ammount)
  - 1
  - 2
  - 3
  - 4
functions: # a list of Net functions that will be used. the .so files should be copied into the run folder
           # in order to add a function you should specify its name and .so file
           # the function is assumed to match a specific signature 
           # extern "C" std::vector<float> function(std::vector<float> i_positions, std::vector<float> i_motorPosition) 
    - NF1:
      funcName: pos0
      funcObject: 'NetFuncs1.so'
    - NF2:
      funcName: inc3
      funcObject: 'NetFuncs2.so'

```

## How to compile the code

```
git clone --recursive https://github.com/dimazhevelev/MR.git 
cd MR
mkdir build
cd build
cmake ..
make
```

## How to compile Net Functions

```
./compileNetFunc.sh <input cpp file> <output so file> 
```
for example
```
./compileNetFunc.sh NetFuncs1.cpp NetFuncs1.so
./compileNetFunc.sh NetFuncs2.cpp NetFuncs2.so 
```


## Example test run

```
python3 launchSimulator.py -i motorSim.yaml
```

The log of the run will be located in the run folder under the name log.log
