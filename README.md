# C++-Class-Game

##  macOS/Linux building and running:

### Check for all necessary installs:

Type the following commands in, and if it gives you a clean verison, you're good to go!  
git --version  
cmake --version  
pkg-config --version

If you got an error, do this for Mac (only for the command that gave you the error):  
First check that you have brew installed:  
brew --version  
If you get some kind of an error, install it through this command and then restart your terminal:  
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"  
brew install git  
brew install cmake  
brew install pkg-config

And for linux:  
sudo apt update  
sudo apt install build-essential pkg-config cmake git

### Clone vcpkg if you haven’t

git clone https://github.com/microsoft/vcpkg.git ~/dev/vcpkg  
cd ~/dev/vcpkg  
./bootstrap-vcpkg.sh  
./vcpkg integrate install  
./vcpkg install libtcod

### Build your project

You can run:  
chmod +x build.sh  
./build.sh  
Or change it out for this if you want debug mode:  
./build.sh Debug

### Otherwise do:

cd ~/dev/C-CLASS-GAME  
mkdir build  
cd build  
cmake .. -DCMAKE_TOOLCHAIN_FILE=~/dev/vcpkg/scripts/buildsystems/vcpkg.cmake  
cmake --build .

### Run

./game

## Windows building and running:

### Check for all necessary installs:
git --version  
cmake --version

If you got an error, do the option corresponding to your error:  
https://git-scm.com/download/win  
That downloads git to windows  
https://cmake.org/download/  
That downloads cmake to windows

git clone https://github.com/microsoft/vcpkg.git C:\dev\vcpkg  
cd C:\dev\vcpkg  
.\bootstrap-vcpkg.bat  
.\vcpkg integrate install  
.\vcpkg install libtcod  

### Build your project

You can run:  
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass  
./build.ps1  
Or if you want debug mode, run:
./build.ps1 Debug

### Otherwise :

cd C:\dev\C-CLASS-GAME  
mkdir build  
cd build  
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake  
cmake --build .

### Run

.\game.exe
