param(
    [ValidateSet("single", "multi")]
    [string]$Mode = "single",

    [ValidateSet("x86_64", "aarch64", "riscv64")]
    [string]$Arch = "x86_64"
)

$repoRoot = Split-Path -Parent $PSScriptRoot
$source = Join-Path $repoRoot "boot\config\$Mode-$Arch.cfg"
$target = Join-Path $repoRoot "boot\boot.cfg"

if (-not (Test-Path -LiteralPath $source)) {
    Write-Error "Boot config not found: $source"
    exit 1
}

Copy-Item -LiteralPath $source -Destination $target -Force
Write-Host "Configured TerrariumOS boot: mode=$Mode arch=$Arch"
Write-Host "Wrote $target"
