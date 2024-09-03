# COMS 4156 Mini Project

## Build

```
mkdir build && cd build
cmake ..
make
```

## Run

### Setting up the database

```
cd build
./IndividualMiniproject setup
```

### Running the web server

```
cd build
./IndividualMiniproject
```

## `clangd` integration

```
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
mv compile_commands.json ../compile_commands.json
```

This should create a file called `compile_commands.json` in the root directory.
