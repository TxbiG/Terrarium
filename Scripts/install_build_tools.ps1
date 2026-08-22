param(
    [switch]$AcceptAgreements
)

$ErrorActionPreference = 'Stop'

$packages = @(
    @{ Id = "Kitware.CMake"; Name = "CMake"; Command = "cmake" },
    @{ Id = "Ninja-build.Ninja"; Name = "Ninja"; Command = "ninja" },
    @{ Id = "LLVM.LLVM"; Name = "LLVM/Clang"; Command = "clang" }
)

function Test-CommandAvailable {
    param([string]$Command)
    return $null -ne (Get-Command $Command -ErrorAction SilentlyContinue)
}

if (-not (Test-CommandAvailable "winget")) {
    Write-Error "winget was not found. Install App Installer from the Microsoft Store, then run this script again."
    exit 1
}

# Always accept agreements non-interactively in CI (windows-2022 runner has no human to click through them).
if ($env:CI -eq 'true') {
    $AcceptAgreements = $true
}

$agreementArgs = @()
if ($AcceptAgreements) {
    $agreementArgs = @("--accept-package-agreements", "--accept-source-agreements")
}

foreach ($package in $packages) {
    if (Test-CommandAvailable $package.Command) {
        Write-Host "$($package.Name) already available, skipping install."
        continue
    }

    Write-Host "Installing $($package.Name) ($($package.Id))..."
    winget install --id $package.Id --exact --silent @agreementArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Failed to install $($package.Id) (exit code $LASTEXITCODE)"
        exit $LASTEXITCODE
    }
}

# winget updates the machine/user PATH but this process won't see it.
# Refresh from the registry so verification below (and any later step in
# THIS SAME process/session) can find the newly installed tools.
$machinePath = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
$userPath    = [System.Environment]::GetEnvironmentVariable("Path", "User")
$env:Path    = "$machinePath;$userPath"

Write-Host ""
Write-Host "Verifying installed tools:"
cmake --version
ninja --version
clang --version
