# Samsung Galaxy Folder2 SM-G160N TWRP device tree

Device tree for building 64-bit TWRP for the Korean Samsung Galaxy Folder2.

- Device: Samsung Galaxy Folder2
- Model: SM-G160N
- Codename: `eliteltekor` / `elitelte_kor`
- Platform: Qualcomm MSM8937/MSM8917 family
- Recovery target: TWRP 3.1.1 / Android 7.1 tree
- Architecture: arm64 recovery with 32-bit secondary arch

## Checkout

Clone this repository into the TWRP source tree at:

```sh
device/samsung/eliteltekor
```

## Build

```sh
source build/envsetup.sh
lunch omni_eliteltekor-userdebug
make clean
make -j$(nproc) recoveryimage
```

## Included prebuilts

This tree includes the required recovery kernel and QCDT image under
`prebuilt/`.

- `prebuilt/kernel`: 64-bit `Image.gz`
- `prebuilt/qcdt.img`: QCDT matched to the known-working recovery header

No separate device-specific proprietary vendor tree is required for this
minimal TWRP recovery build.

## Known-good recovery header

The board name, cmdline, QCDT, and `SEANDROIDENFORCE` trailer are matched to a
known-working temporary port for SM-G160N.
