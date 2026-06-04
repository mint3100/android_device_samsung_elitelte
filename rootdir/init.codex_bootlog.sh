#!/system/bin/sh

stage="$1"
dir=/cache/codex_bootlog
mkdir -p "$dir"

{
    echo "stage=$stage"
    echo "--- date ---"
    date
    echo "--- cmdline ---"
    cat /proc/cmdline
    echo "--- mounts ---"
    cat /proc/mounts
    echo "--- getprop ---"
    getprop
    echo "--- ps ---"
    ps -A || ps
    echo "--- key paths ---"
    ls -al /system /system/bin /vendor /data /data/property /data/system /cache 2>&1
    echo "--- dmesg ---"
    dmesg
} > "$dir/$stage.txt" 2>&1

sync
