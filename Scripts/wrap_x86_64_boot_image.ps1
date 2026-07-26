param(
    [ValidateSet("raw-fat", "iso")]
    [string]$Format = "raw-fat",

    [string]$StagedRoot = "build\boot-image-x86_64",
    [string]$Output = "build\terrarium-x86_64.img",
    [int]$SizeMiB = 64,
    [switch]$DryRun,
    [switch]$PrintCommandsOnly
)

$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent $PSScriptRoot
$stagePath = if ([System.IO.Path]::IsPathRooted($StagedRoot)) { $StagedRoot } else { Join-Path $repoRoot $StagedRoot }
$outputPath = if ([System.IO.Path]::IsPathRooted($Output)) { $Output } else { Join-Path $repoRoot $Output }
$bootPath = Join-Path $stagePath "boot"

function Require-Command($Name, $Hint) {
    if (-not (Get-Command $Name -ErrorAction SilentlyContinue)) {
        Write-Error "Required tool not found: $Name. $Hint"
        exit 1
    }
}

function Write-WrapperHeader {
    Write-Host "TerrariumOS boot image wrapper"
    Write-Host "  Format: $Format"
    Write-Host "  Staged: $stagePath"
    Write-Host "  Output: $outputPath"
}

function Write-RawFatCommands {
    Write-Host "Dry run commands:"
    Write-Host "  fsutil file createnew `"$outputPath`" $($SizeMiB * 1024 * 1024)"
    Write-Host "  mformat -i `"$outputPath`" -F ::"
    Write-Host "  mcopy -i `"$outputPath`" -s `"$bootPath`" ::/"
}

function Write-IsoCommands {
    Write-Host "Dry run command:"
    Write-Host "  xorriso -as mkisofs -o `"$outputPath`" -R -J `"$stagePath`""
}

if ($PrintCommandsOnly) {
    Write-WrapperHeader
    if ($Format -eq "raw-fat") { Write-RawFatCommands } else { Write-IsoCommands }
    exit 0
}

if (-not (Test-Path -LiteralPath $bootPath)) {
    Write-Error "Staged boot tree not found: $bootPath. Run Scripts\package_x86_64_emulator.ps1 first."
    exit 1
}

Write-WrapperHeader

if ($Format -eq "raw-fat") {
    Write-Host "  Tools:  mformat, mcopy"
    if ($DryRun) {
        Write-RawFatCommands
        exit 0
    }

    Require-Command "mformat" "Install mtools or use -Format iso with xorriso."
    Require-Command "mcopy" "Install mtools or use -Format iso with xorriso."

    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $outputPath) | Out-Null
    if (Test-Path -LiteralPath $outputPath) {
        Remove-Item -LiteralPath $outputPath -Force
    }
    fsutil file createnew $outputPath ($SizeMiB * 1024 * 1024) | Out-Null
    & mformat -i $outputPath -F "::"
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    & mcopy -i $outputPath -s $bootPath "::/"
    exit $LASTEXITCODE
}

if ($Format -eq "iso") {
    Write-Host "  Tools:  xorriso"
    if ($DryRun) {
        Write-IsoCommands
        exit 0
    }

    Require-Command "xorriso" "Install xorriso or use -Format raw-fat with mtools."
    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $outputPath) | Out-Null
    & xorriso -as mkisofs -o $outputPath -R -J $stagePath
    exit $LASTEXITCODE
}
