Write-Host "Building C++ Game..."

$Mode = "Release"
if ($args.Count -gt 0) { # if there is an argument, use it as the mode instead
    $Mode = $args[0]
}

New-Item -ItemType Directory -Force -Path build
Set-Location build

cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=$Mode
cmake --build .
Write-Host "Build complete."