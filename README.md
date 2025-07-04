# C++-Class-Game

Important notes:  
If you install a tool but the command still isn't found, try restarting your terminal or logging out and back in.  
It's typical for downloads to take a few minutes, just be patient

##  macOS/Linux building and running:

### Check for all necessary installs:

Type the following commands in, and if it gives you a clean verison, you're good to go!  
```sh
git --version  
cmake --version  
pkg-config --version
ninja --version
```

If you got an error on any of those steps, do this for Mac:  
First check that you have brew installed:  
```sh
brew --version  
```
If you get an error when checking brew, install it through this command and then restart your terminal:  
```sh
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" 
```
Now, install the component that gave you the error after --version
```sh 
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
If git --version gives you an error, download it here:  
https://git-scm.com/download/win  

If `cmake --version` gives you an error, install CMake:  
1. Go to https://cmake.org/download/  
2. Download the correct version for your computer  
3. Run the installer to finish downloading and **choose the option to add CMake to your system PATH**  
4. After installation, open a new Command Prompt and run `cmake --version` to verify.

### Getting the Installer (vcpkg package manager)

These commands:
1. Clones the installer's repository  
2. Moves into the cloned repository on your computer  
3. Compiles installer to allow you to install new libraries  
4. Allows CMake (used to compile thie game) to use the installer to access those new libraries  
5. The installer installs a new library

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\dev\vcpkg  
cd C:\dev\vcpkg  
.\bootstrap-vcpkg.bat  
.\vcpkg integrate install  
.\vcpkg install libtcod  
```

If you got a error triplet error after running `.\vcpkg install libtcod` do these steps:  
1. Go to this website: https://visualstudio.microsoft.com/visual-cpp-build-tools/  
2. Click "Download Build Tools" and follow steps  
3. When following steps, select "Desktop development with C++"  
4. Restart your terminal/PowerShell once it's installed  
5. Run `cd C:\dev\vcpkg` to make sure you're in the right place  
6. Run `.\vcpkg install libtcod` and it should work now

### Build your project

You can run:  
```powershell
cd C:\C-CLASS-GAME  
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass  
./build.ps1  
```

Or if you want debug mode, run:
```powershell
./build.ps1 Debug
```

If the first command `cd C:\C-CLASS-GAME` doesn't work, search for the C-Class-Game file in your finder then right click the top search and select "copy address as text". Then type cd and paste and follow the next steps the same.

If you get a permissions error with Set-ExecutionPolicy, try running PowerShell as Administrator.

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
.\Debug\game.exe
```

If that doesn't work, look around the build folder for another folder called Debug. Inside there should be a game.exe file for you to double click on and open manually.