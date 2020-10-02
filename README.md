# Cinderella
Cinderella language and its compiler, built with LLVM.

## How to use on MacOS?

#### Caveats:

* Only support **LLVM 10**;
* LLVM should be compiled beforehand by yourself;

#### Build source code with CMake:
```shell
# Config CMake (the placeholder {YOUR_LLVM_DIR} shoule be replaced by your LLVM folder);
cd build && LLVM_BUILT_DIR="{YOUR_LLVM_DIR}" cmake ..

# Compile;
make -j4
```

#### Run Compiler in Command Line:
```shell 
# REPL mode;
./cinderella

# Compiling mode (try build/sources);
./cinderella <source.hs> -o <output.o> 

# Compile source file into WebAssembly binary module (.wasm):
./cinderella <source.hs> -t WASM -o <output.wasm> 
```

#### Sample Code:
```hangScript
def add(x, y) x + y;
def average(x, y) (x + y) / 2;
```
