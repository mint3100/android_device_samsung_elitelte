# android_device_samsung_elitelte

LineageOS 15.1 device tree for Samsung Galaxy Folder2 elitelte variants.

This tree is shared by the Korean SM-G160N target and the Chinese/HK SM-G1650
target.  Region-specific kernel selection is handled from `BoardConfig.mk`
based on the selected lunch product.

## Device Summary

| Item | Value |
| --- | --- |
| Device | Samsung Galaxy Folder2 |
| Models | SM-G160N, SM-G1650 |
| Codename | `elitelte` |
| Platform | Qualcomm MSM8937 / MSM8917 family |
| Android target | LineageOS 15.1 / Android 8.1 |
| Architecture | 32-bit ARM (`armeabi-v7a`) |
| Vendor path | `vendor/samsung/elitelte` |
| Kernel path | `kernel/samsung/msm8917_elitelte` |

## Products

```sh
lunch lineage_elitelte-userdebug
lunch lineage_elitelte_chn-userdebug
```

`lineage_elitelte` is the current KOR-oriented default target and uses
`msm8937_sec_elitelte_kor_defconfig`.  `lineage_elitelte_chn` uses the stock
CHN/HK `msm8937_sec_elitelte_chn_defconfig`.

## Source Layout

```text
device/samsung/elitelte
vendor/samsung/elitelte
kernel/samsung/msm8917_elitelte
```

## Bring-up Notes

- The kernel is built from source; do not replace it with a prebuilt kernel.
- The boot image uses a separate `dt.img` and legacy ramdisk layout.
- The vendor tree currently contains a shared bring-up blob set.  Add CHN-only
  proprietary files only after a CHN stock dump or device log proves they are
  needed.
- Boot, radio, camera, audio, display, Wi-Fi, and power behavior remain active
  validation areas.

## Branch

Use the `lineage-15.1` branch for Android 8.1 work.
