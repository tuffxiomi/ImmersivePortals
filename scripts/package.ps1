param(
    [ValidateSet("arm64-v8a")]
    [string]$Abi = "arm64-v8a",
    [switch]$Clean,
    [string]$Ndk = ""
)
$ErrorActionPreference = "Stop"

$projectFiles = Get-ChildItem -Recurse -File -Path (Split-Path -Parent $PSScriptRoot)
$legacyPattern = ("bedrock" + "tools") + "|" + ("bedrock_" + "tools")
if ($projectFiles | Where-Object { $_.Name -match $legacyPattern }) {
    throw "Legacy runtime files are not permitted in this mod project."
}

$root = Split-Path -Parent $PSScriptRoot
$build = Join-Path $root "build-$Abi"

if ($Clean -and (Test-Path $build)) { Remove-Item -Recurse -Force $build }

if (-not $Ndk) {
    $Ndk = $env:ANDROID_NDK_HOME
    if (-not $Ndk) { $Ndk = $env:ANDROID_NDK_ROOT }
    if (-not $Ndk -and $env:ANDROID_HOME) { $Ndk = Join-Path $env:ANDROID_HOME "ndk\28.2.13676358" }
    if (-not $Ndk -and $env:ANDROID_SDK_ROOT) { $Ndk = Join-Path $env:ANDROID_SDK_ROOT "ndk\28.2.13676358" }
}
if (-not (Test-Path $Ndk)) { throw "Android NDK 28.2.13676358 was not found. Set ANDROID_NDK_HOME or pass -Ndk." }

$toolchain = Join-Path $Ndk "build\cmake\android.toolchain.cmake"
cmake -S $root -B $build -G Ninja `
    -DCMAKE_TOOLCHAIN_FILE="$toolchain" `
    -DANDROID_ABI=$Abi `
    -DANDROID_PLATFORM=android-28 `
    -DANDROID_STL=c++_shared `
    -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) { throw "CMake configure failed" }
cmake --build $build --target levi_package
if ($LASTEXITCODE -ne 0) { throw "Android build failed" }
Write-Host "Package: $build\dist\$Abi\immersive_portals.levipack"
