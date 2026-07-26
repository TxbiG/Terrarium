param(
    [switch]$AcceptAgreements
)

$packages = @(
    @{ Id = "Kitware.CMake"; Name = "CMake" },
    @{ Id = "Ninja-build.Ninja"; Name = "Ninja" },
    @{ Id = "LLVM.LLVM"; Name = "LLVM/Clang" }
)

function Test-CommandAvailable {
    param([string]$Command)
    return $null -ne (Get-Command $Command -ErrorAction SilentlyContinue)
}

if (-not (Test-CommandAvailable "winget")) {
    Write-Error "winget was not found. Install App Installer from the Microsoft Store, then run this script again."
    exit 1
}

$agreementArgs = @()
if ($AcceptAgreements) {
    $agreementArgs = @("--accept-package-agreements", "--accept-source-agreements")
}

foreach ($package in $packages) {
    Write-Host "Installing $($package.Name) ($($package.Id))..."
    winget install --id $package.Id --exact @agreementArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Failed to install $($package.Id)"
        exit $LASTEXITCODE
    }
}

Write-Host ""
Write-Host "Installed build tools. Open a new PowerShell window, then verify:"
Write-Host "  cmake --version"
Write-Host "  ninja --version"
Write-Host "  clang --version"
