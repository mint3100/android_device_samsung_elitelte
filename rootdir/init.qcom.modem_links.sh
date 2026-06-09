#!/system/bin/sh
# Copyright (c) 2014, The Linux Foundation. All rights reserved.
#
# Stock Qualcomm firmware link setup, adjusted for Samsung's split
# apnhlos (/firmware) and modem (/firmware-modem) partitions.

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

link_firmware() {
    src_dir="$1"
    pattern="$2"

    [ -d "$src_dir" ] || return

    for imgfile in "$src_dir"/$pattern; do
        [ -f "$imgfile" ] || continue
        base="${imgfile##*/}"
        ln -sf "$imgfile" "/system/etc/firmware/$base" 2>/dev/null
    done
}

mkdir -p /system/etc/firmware

link_firmware /firmware/image 'wcnss*'
link_firmware /firmware/image 'venus*'
link_firmware /firmware-modem/image 'modem*'
link_firmware /firmware-modem/image 'adsp*'
link_firmware /firmware-modem/image 'mba*'

cd /
