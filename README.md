# Cinderella
Cinderella language and its compiler, built with LLVM.


## How to use on MacOS?

#### 1、Build LLVM toolchain:
```commandline
# Fetch LLVM from remote server;
wget http://releases.llvm.org/6.0.0/llvm-6.0.0.src.tar.xz

# Rename;
mv llvm-6.0.0.src.tar.xz llvm.tar.xz

# Uncompress;
xz -d llvm.tar.xz
tar -xzvf llvm.tar

# Configure CMake and build;
cd llvm && mkdir build
cd build && cmake .. && make
```

#### 2、Build source code with CMake:
```commandline
cd build && cmake .. && make && make install
```

#### 3、Run Compiler in Command Line:
```commandline 
# Command line mode;
Cinderella

# Compiling mode (try build/sources);
Cinderella -o <output.o> <source.hs>

# Compile ".hs" source file into WebAssembly binary module (.wasm):
Cinderella -t WASM -o <output.wasm> <source.hs>
```

#### 4、Sample Code:
```hangScript
def add(x, y) x + y;
```

#### 5、Capability and TODO:
- [x] Define simple function with binary expression;
- [x] Embed on WebAssembly (LLVM-IR to Wat/Wasm);
- [ ] Add standard control flow support;
- [ ] Add customized local variable support;
- [ ] Optimize compiler structure;

