param(
    [switch]$AcceptAgreements
)

$ErrorActionPreference = 'Stop'

$packages = @(
    @{ Id = "cmake"; Name = "CMake"; Command = "cmake" },
    @{ Id = "ninja"; Name = "Ninja"; Command = "ninja" },
    @{ Id = "llvm";  Name = "LLVM/Clang"; Command = "clang" }
)

function Test-CommandAvailable {
    param([string]$Command)
    return $null -ne (Get-Command $Command -ErrorAction SilentlyContinue)
}

if (-not (Test-CommandAvailable "choco")) {
    Write-Error "choco (Chocolatey) was not found. Install it from https://chocolatey.org/install, then run this script again."
    exit 1
}

foreach ($package in $packages) {
    if (Test-CommandAvailable $package.Command) {
        Write-Host "$($package.Name) already available, skipping install."
        continue
    }

    Write-Host "Installing $($package.Name) ($($package.Id))..."
    choco install $package.Id -y --no-progress
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Failed to install $($package.Id) (exit code $LASTEXITCODE)"
        exit $LASTEXITCODE
    }
}

# Chocolatey updates the machine PATH but this process won't see it until
# we refresh from the registry.
$machinePath = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
$userPath    = [System.Environment]::GetEnvironmentVariable("Path", "User")
$env:Path    = "$machinePath;$userPath"

Write-Host ""
Write-Host "Verifying installed tools:"
cmake --version
ninja --version
clang --version
