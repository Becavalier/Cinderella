# Cinderella
Cinderella language and its compiler, built under LLVM.


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
cd build && cmake .. && make
```

#### 3、Run:
```commandline
./Cinderella
```