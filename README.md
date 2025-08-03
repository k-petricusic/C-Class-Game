# C++-Class-Game

[Home](README.md) | [Credits](CREDITS.md)

A terminal-based C++ project.

---

## Table of Contents
- [Project Overview](#project-overview)
- [Getting Started](#getting-started)
  - [Cloning the Repository](#cloning-the-repository)
- [Prerequisites](#prerequisites)
- [Building the Project](#building-the-project)
  - [macOS/Linux](#macoslinux)
  - [Windows](#windows)
- [Running the Game](#running-the-game)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)

---

## Project Overview
This is a stealth game written using C++. Try to avoid the guard's sight lines as you make your way to the exit!

---

## Getting Started

### Cloning the Repository
First, you need to get a copy of this project on your computer. Open your terminal (or PowerShell on Windows) and run:

#### macOS/Linux
```sh
# Clone the repository into your home directory for easy access
cd ~
git clone https://github.com/k-petricusic/C-Class-Game.git C-CLASS-GAME
cd C-CLASS-GAME
```

#### Windows
```powershell
# Clone the repository into your C: drive for easy access
cd C:\
git clone https://github.com/k-petricusic/C-Class-Game.git C-CLASS-GAME
cd C:\C-CLASS-GAME
```

---

## Prerequisites
Before building, make sure you have the following tools installed:

### MacOS

Check if you have the tools (macOS):
```sh
git --version
cmake --version
pkg-config --version
ninja --version
```
If any command fails, install the missing tool as shown below.

> **Tip:** On macOS, use [Homebrew](https://brew.sh/) to install packages. If you don't have Homebrew, install it with:
> ```sh
> /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
> ```

| Tool         | What it's for                        | Install Command              |
|--------------|--------------------------------------|------------------------------|
| git          | Downloading the project              | `brew install git`           |
| cmake        | Build system generator               | `brew install cmake`         |
| ninja        | Fast build tool                      | `brew install ninja`         |
| pkg-config   | Library configuration                | `brew install pkg-config`    |
| vcpkg        | C++ package manager                  | See below                    |
| libtcod      | Terminal graphics library            | Installed via vcpkg          |

### Linux

Check if you have the tools (Linux):
```sh
git --version
cmake --version
pkg-config --version
ninja --version
```
If any command fails, install the missing tool as shown below.

> **Tip:** On Linux, use your package manager (e.g., apt for Ubuntu/Debian) to install packages.

| Tool         | What it's for                        | Install Command              |
|--------------|--------------------------------------|------------------------------|
| git          | Downloading the project              | `sudo apt install git`           |
| cmake        | Build system generator               | `sudo apt install cmake`         |
| ninja        | Fast build tool                      | `sudo apt install ninja-build`    |
| pkg-config   | Library configuration                | `sudo apt install pkg-config`    |
| build-essential | Compiler and build tools           | `sudo apt install build-essential`|
| vcpkg        | C++ package manager                  | See below                    |
| libtcod      | Terminal graphics library            | Installed via vcpkg          |

And for Linux, you can install all the essentials with:
```sh
sudo apt update
sudo apt install build-essential pkg-config cmake git ninja-build
```

### Windows

Check if you have the tools (Windows):
```powershell
git --version
cmake --version
```
If any command fails, install the missing tool as shown above.

| Tool         | What it's for                        | Install Command/Link         |
|--------------|--------------------------------------|------------------------------|
| git          | Downloading the project              | [Download](https://git-scm.com/download/win) |
| cmake        | Build system generator               | [Download](https://cmake.org/download/) |
| ninja        | Fast build tool                      | Unnecessary for Windows      |
| pkg-config   | Library configuration                | (comes with vcpkg)           |
| vcpkg        | C++ package manager                  | See below                    |
| libtcod      | Terminal graphics library            | Installed via vcpkg          |

> **Tip:** On Windows, use the provided links to download and install the tools.

---

## Building the Project

### macOS/Linux

1. **Install vcpkg and libtcod**
   ```sh
   git clone https://github.com/microsoft/vcpkg.git ~/dev/vcpkg
   cd ~/dev/vcpkg
   ./bootstrap-vcpkg.sh
   ./vcpkg integrate install
   ./vcpkg install libtcod
   ```
   > This sets up the C++ package manager and installs the graphics library.

2. **Build the project**
   - **Quick build:**
     ```sh
     cd ~/C-CLASS-GAME
     chmod +x build.sh
     ./build.sh
     ```
     > This script configures and builds the project using CMake and Ninja.

   - **Debug build:**
     ```sh
     ./build.sh Debug
     ```
     > Builds with debug symbols for easier debugging.

   - **Manual build:**
     ```sh
     mkdir build
     cd build
     cmake .. -DCMAKE_TOOLCHAIN_FILE=~/dev/vcpkg/scripts/buildsystems/vcpkg.cmake
     cmake --build .
     ```
     > This is the manual way to configure and build if you want more control.

### Windows

1. **Install vcpkg and libtcod**
   ```powershell
   git clone https://github.com/microsoft/vcpkg.git C:\dev\vcpkg
   cd C:\dev\vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg integrate install
   .\vcpkg install libtcod
   ```
   > This sets up the C++ package manager and installs the graphics library.

   If you get a "triplet error" after installing libtcod, install Visual C++ Build Tools:
   1. Go to https://visualstudio.microsoft.com/visual-cpp-build-tools/
   2. Follow the steps to install, then click on the "Desktop development with C++" option when it pops up in the top left
   3. Restart your terminal and try again.

2. **Build the project**
   - **Quick build:**
     ```powershell
     cd C:\C-CLASS-GAME  
     Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
     ./build.ps1
     ```
     > This script configures and builds the project using CMake and Ninja.

   - **Debug build:**
     ```powershell
     ./build.ps1 Debug
     ```
     > Builds with debug symbols for easier debugging.

   - **Manual build:**
     ```powershell
     mkdir build
     cd build
     cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake
     cmake --build .
     ```
     > This is the manual way to configure and build if you want more control.

---

## Running the Game

After building, run the game from the build directory:

### macOS/Linux
```sh
./game
```

### Windows
```powershell
.\Debug\game.exe
```
If that doesn't work, look for a `Debug` folder inside `build` and double click `game.exe` from there.

---

## Troubleshooting

- **Build errors or strange behavior?**
  - Clear the build cache and rebuild:
    - macOS/Linux:
      ```sh
      rm -rf build
      ./build.sh
      ```
    - Windows:
      ```powershell
      Remove-Item -Recurse -Force build
      ./build.ps1
      ```
- If you see errors about `xcodebuild` or missing compilers, make sure you have the full Xcode app installed (search Xcode in the app store). Then fully enable it:
  ```sh
  sudo xcode-select -s /Applications/Xcode.app/Contents/Developer
  ```
- If you get errors about outdated tools, update them:
  ```sh
  softwareupdate --all --install --force
  ```
- If vcpkg fails to detect the compiler, try cleaning the build trees:
  ```sh
  rm -rf buildtrees
  ```
- **Command not found?**
  - Make sure the tool is installed and your terminal is restarted.
- **Permissions error on Windows?**
  - Try running PowerShell as Administrator.
- **Still stuck?**
  - Check the issues page or open a new issue on GitHub.

---

## Contributing

**How to contribute via PR**

Contributions should follow these guidelines:
1. Fork the repo  
2. Clone your fork  
3. Create a new branch  
4. Make your changes, then `git push origin your-branch-name`  
5. On GitHub, click "New pull request" to propose your changes. If you need to compare across forks, there's a button for that at the top of the new pull request window.


Become a beta tester by playing our game and filling out this form:  
[https://docs.google.com/forms/d/e/1FAIpQLSe6YKlQTI7Qc4jxVJvfyrBUSd9kNMXNq38Cy_ZB0f-7iAq42w/viewform?usp=dialog
](https://forms.gle/Zk5BLCc7XLkx7AW88)
