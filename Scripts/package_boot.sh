#!/usr/bin/env sh
set -eu

mode="${1:-multi}"
arch="${2:-x86_64}"
kernel="${3:-}"
initrd="${4:-}"
output="${5:-build/boot-image}"

case "$mode" in single|multi) ;; *) echo "mode must be single or multi" >&2; exit 1 ;; esac
case "$arch" in x86_64|aarch64|riscv64) ;; *) echo "arch must be x86_64, aarch64, or riscv64" >&2; exit 1 ;; esac

repo_root="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
out_root="$repo_root/$output"
boot_root="$out_root/boot"
arch_root="$boot_root/$arch"
cfg="$repo_root/boot/config/$mode-$arch.cfg"

[ -f "$cfg" ] || { echo "Missing boot config template: $cfg" >&2; exit 1; }

mkdir -p "$arch_root"
cp "$cfg" "$boot_root/boot.cfg"

if [ -n "$kernel" ]; then
    [ -f "$kernel" ] || { echo "Kernel file not found: $kernel" >&2; exit 1; }
    if [ "$arch" = "aarch64" ]; then
        cp "$kernel" "$arch_root/terrarium-kernel.efi"
    else
        cp "$kernel" "$arch_root/terrarium-kernel.bin"
    fi
fi

if [ -n "$initrd" ]; then
    [ -f "$initrd" ] || { echo "Initrd file not found: $initrd" >&2; exit 1; }
    cp "$initrd" "$boot_root/initrd.img"
fi

echo "Staged TerrariumOS boot files:"
echo "  Mode:   $mode"
echo "  Arch:   $arch"
echo "  Output: $out_root"
