# android_device_samsung_elitelte_kor

LineageOS 15.1 device tree for the Korean Samsung Galaxy Folder2 (`elitelte_kor`, SM-G160N).

This tree is part of an Android 8.1 bring-up for a 32-bit ARM LineageOS build. It describes the board, product, boot image layout, ramdisk files, recovery fstab, and device-side init configuration used by the ROM build.

## Device Summary

| Item | Value |
| --- | --- |
| Device | Samsung Galaxy Folder2 |
| Model | SM-G160N |
| Codename | `elitelte_kor` |
| Platform | Qualcomm MSM8937 / MSM8917 family |
| Android target | LineageOS 15.1 / Android 8.1 |
| Architecture | 32-bit ARM (`armeabi-v7a`) |
| Kernel image | Source-built `zImage` |
| Vendor path | `vendor/samsung/elitelte_kor` |
| Kernel path | `kernel/samsung/msm8917_elitelte_kor` |

## What This Tree Provides

- Lineage product target: `lineage_elitelte_kor`.
- Board configuration for a 32-bit ARM userspace and kernel.
- Source-built kernel integration through `TARGET_KERNEL_SOURCE`.
- Samsung boot image handling with separated `dt.img` and `SEANDROIDENFORCE` trailer.
- Legacy boot ramdisk configuration for Android 8.1.
- Device init scripts, fstab files, USB configuration, recovery fstab, and root mount points.
- Vendor inheritance for proprietary files under `vendor/samsung/elitelte_kor`.

## Expected Source Layout

Place the three device-specific repositories under a LineageOS 15.1 source tree like this:

```text
device/samsung/elitelte_kor
vendor/samsung/elitelte_kor
kernel/samsung/msm8917_elitelte_kor
```

## Build

From the root of the LineageOS 15.1 source tree:

```sh
source build/envsetup.sh
lunch lineage_elitelte_kor-userdebug
make clean
mka bacon
```

For boot image testing only:

```sh
source build/envsetup.sh
lunch lineage_elitelte_kor-userdebug
mka bootimage
```

For recovery image testing only:

```sh
source build/envsetup.sh
lunch lineage_elitelte_kor-userdebug
mka recoveryimage
```

## Bring-up Notes

- This tree is for the Korean `elitelte_kor` target, not the Chinese `elitelte` variants.
- The intended ROM target is 32-bit LineageOS 15.1.
- The kernel is built from source; this tree is not designed around a prebuilt kernel.
- The boot image uses the standard LineageOS build flow, with only Samsung-specific boot image requirements layered in through `custom_bootimg.mk`.
- The device is still under bring-up. Treat boot, radio, camera, audio, and power behavior as active validation areas.

## Related Repositories

- `android_vendor_samsung_elitelte_kor`
- `android_kernel_samsung_msm8917_elitelte_kor`

## Branch

Use the `lineage-15.1` branch for Android 8.1 work.
