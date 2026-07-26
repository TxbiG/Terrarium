#!/usr/bin/env sh
set -eu

mode="${1:-single}"
arch="${2:-x86_64}"

case "$mode" in
    single|multi) ;;
    *) echo "Usage: $0 [single|multi] [x86_64|aarch64|riscv64]" >&2; exit 1 ;;
esac

case "$arch" in
    x86_64|aarch64|riscv64) ;;
    *) echo "Usage: $0 [single|multi] [x86_64|aarch64|riscv64]" >&2; exit 1 ;;
esac

repo_root="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
source="$repo_root/boot/config/$mode-$arch.cfg"
target="$repo_root/boot/boot.cfg"

if [ ! -f "$source" ]; then
    echo "Boot config not found: $source" >&2
    exit 1
fi

cp "$source" "$target"
echo "Configured TerrariumOS boot: mode=$mode arch=$arch"
echo "Wrote $target"
