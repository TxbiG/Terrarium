param(
    [string]$Preset = "windows-clang-ninja-smoke"
)

$ErrorActionPreference = "Stop"

cmake --preset windows-clang-ninja
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

cmake --build --preset $Preset
exit $LASTEXITCODE