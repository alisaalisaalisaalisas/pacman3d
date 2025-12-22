# Voxel Pac-Man 3D - Build Instructions

## Prerequisites

You need **GLFW** and **GLM** installed before building.

### Option 1: Install via vcpkg (Recommended)

```powershell
# Install vcpkg if not installed
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat

# Add to PATH (or run from vcpkg directory)
$env:PATH += ";C:\vcpkg"

# Install dependencies
vcpkg install glfw3:x64-windows glm:x64-windows

# Integrate with Visual Studio/CMake
vcpkg integrate install
```

### Option 2: Download Pre-built Binaries

**GLFW:**
1. Go to https://www.glfw.org/download.html
2. Download "64-bit Windows binaries"
3. Extract to `C:\libs\glfw-3.3.8`

**GLM:**
1. Go to https://github.com/g-truc/glm/releases
2. Download latest release
3. Extract to `C:\libs\glm`

Then build with:
```powershell
cmake -B build -DCMAKE_PREFIX_PATH="C:\libs\glfw-3.3.8;C:\libs\glm"
cmake --build build
```

---

## Build Commands

```powershell
cd c:\Users\User\Desktop\муусор\pacman3d
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Run

```powershell
.\Release\voxel_pacman.exe
# or if Debug build:
.\Debug\voxel_pacman.exe
```

## Controls
- **Arrow Keys**: Rotate cube
- **Escape**: Exit
