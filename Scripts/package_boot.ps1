param(
    [ValidateSet("single", "multi")]
    [string]$Mode = "multi",

    [ValidateSet("x86_64", "aarch64", "riscv64")]
    [string]$Arch = "x86_64",

    [string]$Kernel = "",
    [string]$Initrd = "",
    [string]$Output = "build\boot-image"
)

$repoRoot = Split-Path -Parent $PSScriptRoot
$outRoot = Join-Path $repoRoot $Output
$bootRoot = Join-Path $outRoot "boot"
$archRoot = Join-Path $bootRoot $Arch
$cfg = Join-Path $repoRoot "boot\config\$Mode-$Arch.cfg"

if (-not (Test-Path -LiteralPath $cfg)) {
    Write-Error "Missing boot config template: $cfg"
    exit 1
}

New-Item -ItemType Directory -Force -Path $archRoot | Out-Null
Copy-Item -LiteralPath $cfg -Destination (Join-Path $bootRoot "boot.cfg") -Force

if ($Kernel -ne "") {
    if (-not (Test-Path -LiteralPath $Kernel)) {
        Write-Error "Kernel file not found: $Kernel"
        exit 1
    }

    $kernelName = if ($Arch -eq "aarch64") { "terrarium-kernel.efi" } else { "terrarium-kernel.bin" }
    Copy-Item -LiteralPath $Kernel -Destination (Join-Path $archRoot $kernelName) -Force
}

if ($Initrd -ne "") {
    if (-not (Test-Path -LiteralPath $Initrd)) {
        Write-Error "Initrd file not found: $Initrd"
        exit 1
    }
    Copy-Item -LiteralPath $Initrd -Destination (Join-Path $bootRoot "initrd.img") -Force
}

Write-Host "Staged TerrariumOS boot files:"
Write-Host "  Mode:   $Mode"
Write-Host "  Arch:   $Arch"
Write-Host "  Output: $outRoot"
