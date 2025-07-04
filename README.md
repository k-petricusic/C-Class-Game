# C++-Class-Game

##  macOS/Linux building and running:

### Check for all necessary installs:

Type the following commands in, and if it gives you a clean verison, you're good to go!  
```sh
git --version  
cmake --version  
pkg-config --version
ninja --version
```

If you got an error, do this for Mac (only for the command that gave you the error):  
First check that you have brew installed:  
```sh
brew --version  
```
If you get some kind of an error, install it through this command and then restart your terminal:  
```sh
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"  
brew install git  
brew install cmake  
brew install pkg-config  
brew install ninja
```

And for linux:  
```sh
sudo apt update  
sudo apt install build-essential pkg-config cmake git ninja-build
```

### Clone vcpkg if you haven't

```sh
git clone https://github.com/microsoft/vcpkg.git ~/dev/vcpkg  
cd ~/dev/vcpkg  
./bootstrap-vcpkg.sh  
./vcpkg integrate install  
./vcpkg install libtcod
```

### Build your project

You can run:  
```sh
cd ~/C-CLASS-GAME
chmod +x build.sh  
./build.sh  
```
Or change it out for this if you want debug mode:  
```sh
./build.sh Debug
```

### Otherwise do:

```sh
cd ~/C-CLASS-GAME  
mkdir build  
cd build  
cmake .. -DCMAKE_TOOLCHAIN_FILE=~/dev/vcpkg/scripts/buildsystems/vcpkg.cmake  
cmake --build .
```

### Fixing an incorrect build

To fix a bad build, you need to clear CMakeCache.txt found inside the build directory, run this command to do that, and you'll need to run ./build.sh again afterwards:  
```sh
rm -rf build
```

### Run

```sh
./game
```

## Windows building and running:

### Check for all necessary installs:
```powershell
git --version  
cmake --version
```
If you got an error, do the option corresponding to your error:  
https://git-scm.com/download/win  
That downloads git to windows  
https://cmake.org/download/  
That downloads cmake to windows

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\dev\vcpkg  
cd C:\dev\vcpkg  
.\bootstrap-vcpkg.bat  
.\vcpkg integrate install  
.\vcpkg install libtcod  
```

### Build your project

You can run:  
```powershell
cd %USERPROFILE%\C-CLASS-GAME  
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass  
./build.ps1  
```
Or if you want debug mode, run:
```powershell
./build.ps1 Debug
```

### Otherwise:

```powershell
cd C:\C-CLASS-GAME  
mkdir build  
cd build  
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake  
cmake --build .
```

### Fixing an incorrect build

To fix a bad build, you need to clear CMakeCache.txt found inside the build directory, run this command to do that, and you'll need to run ./build.ps1 again afterwards:  
```powershell
Remove-Item -Recurse -Force build
```

### Run

```powershell
.\game.exe
```