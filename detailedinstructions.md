# Install Instructions for Neurovectorizer on Ubuntu 18.04
----
#### Table of Contents:
1. [Rise Specific Directory Setup](#directory-setup) 
2. [Installing Anaconda, CMAKE, and LLVM](#install-anaconda,-cmake,-and-llvm)
3. [Install Neurovectorizer Dependencies](#install-neurovectorizer-dependencies)  
4. [Edit configure&#46;sh](#edit-configure.sh)

####  Directory Setup: 
----
We've discovered that the default working directory /home/eecs/[username] is far slower than working inside of /data directory so this should be your working directory.
 ```
 cd /data/
 mkdir [your username]
 cd [your username]
 mkdir dependencies
```
You may want to change your $HOME to point to /data/[username] for ease of access in the future.   


#### Install Anaconda, CMAKE, and LLVM: 
----
Skip any dependency you already have.
```
wget https://repo.anaconda.com/archive/Anaconda3-2020.02-Linux-x86_64.sh
bash Anaconda3-2020.02-Linux-x86_64.sh  # Make sure to install in /data/username/ when asked rather than HOME
wget https://github.com/Kitware/CMake/releases/download/v3.17.1/cmake-3.17.1-Linux-x86_64.tar.gz
tar xzf cmake-3.17.0.tar.gz
cd cmake-3.17.0
./bootstrap
make
make install
cd ..
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
mkdir build
cd build
cmake -DLLVM_ENABLE_PROJECTS=clang -G "Unix Makefiles" ../llvm
make
```


#### Install Neurovectorizer Dependencies: 
----
First make sure you're in your anaconda environment.
```
conda install -c conda-forge clang
conda install -c conda-forge tensorflow
pip install tensorflow
pip install ray
pip install ray[rllib]
pip install clang
```  

#### Edit configure.sh:
----
Note the following is using the Rise Specific Directory Setup, so make adjustments as necessary. Change the folllowing lines:
```
CLANG_PATH=/data/[username]/dependencies/llvm-project/build/lib/libclang.so
CLANG_BIN_PATH=/data/[username]/anaconda3/bin/clang
```
