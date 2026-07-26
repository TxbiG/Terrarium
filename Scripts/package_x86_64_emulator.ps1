param(
    [string]$Kernel = "build\windows-clang-ninja\kernel\kernel.elf",
    [string]$Initrd = "",
    [string]$Output = "build\boot-image-x86_64",
    [switch]$MultiBootMenu
)

$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent $PSScriptRoot
$mode = if ($MultiBootMenu) { "multi" } else { "single" }
$kernelPath = Join-Path $repoRoot $Kernel
$args = @(
    "-Mode", $mode,
    "-Arch", "x86_64",
    "-Kernel", $kernelPath,
    "-Output", $Output
)

if ($Initrd -ne "") {
    $args += @("-Initrd", (Join-Path $repoRoot $Initrd))
}

& (Join-Path $PSScriptRoot "package_boot.ps1") @args
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

$staged = Join-Path $repoRoot $Output
Write-Host ""
Write-Host "x86_64 emulator staging complete: $staged"
Write-Host "Next emulator milestone once a bootable image wrapper exists:"
Write-Host "  qemu-system-x86_64 -m 512M -serial stdio -drive format=raw,file=<disk.img>"