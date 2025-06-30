# C++-Class-Game

##  macOS/Linux building and running:

### Clone vcpkg if you haven’t

git clone https://github.com/microsoft/vcpkg.git ~/dev/vcpkg  
cd ~/dev/vcpkg  
./bootstrap-vcpkg.sh  
./vcpkg integrate install  
./vcpkg install libtcod

### Build your project

Or you can run:  
chmod +x build.sh  
./build.sh

### Otherwise do:

cd ~/dev/C-CLASS-GAME  
mkdir build  
cd build  
cmake .. -DCMAKE_TOOLCHAIN_FILE=~/dev/vcpkg/scripts/buildsystems/vcpkg.cmake  
cmake --build .

### Run
./game

## Windows building and running:

git clone https://github.com/microsoft/vcpkg.git C:\dev\vcpkg  
cd C:\dev\vcpkg  
.\bootstrap-vcpkg.bat  
.\vcpkg integrate install  
.\vcpkg install libtcod  

### Build your project

Or you can run:  
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass  
./build.ps1

### Otherwise :

cd C:\dev\C-CLASS-GAME  
mkdir build  
cd build  
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake  
cmake --build .

### Run

.\game.exe
