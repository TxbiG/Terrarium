param(
    [string]$DiskImage = "build\terrarium-x86_64.img",
    [string]$Qemu = "qemu-system-x86_64",
    [int]$MemoryMiB = 512,
    [switch]$DryRun
)

$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent $PSScriptRoot
$diskPath = if ([System.IO.Path]::IsPathRooted($DiskImage)) { $DiskImage } else { Join-Path $repoRoot $DiskImage }
$commandLine = "$Qemu -m ${MemoryMiB}M -serial stdio -no-reboot -no-shutdown -drive format=raw,file=`"$diskPath`""

Write-Host "TerrariumOS x86_64 emulator command:"
Write-Host "  $commandLine"

if ($DryRun) {
    exit 0
}

if (-not (Get-Command $Qemu -ErrorAction SilentlyContinue)) {
    Write-Error "QEMU executable not found: $Qemu. Install QEMU or pass -Qemu <path>."
    exit 1
}

if (-not (Test-Path -LiteralPath $diskPath)) {
    Write-Error "Disk image not found: $diskPath. Stage files with Scripts\package_x86_64_emulator.ps1, then wrap them into this raw disk image."
    exit 1
}

& $Qemu -m "${MemoryMiB}M" -serial stdio -no-reboot -no-shutdown -drive "format=raw,file=$diskPath"
exit $LASTEXITCODE