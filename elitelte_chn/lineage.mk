$(call inherit-product, device/samsung/elitelte/device.mk)
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

PRODUCT_NAME := lineage_elitelte_chn
PRODUCT_BRAND := samsung
PRODUCT_MODEL := SM-G1650
PRODUCT_DEVICE := elitelte
PRODUCT_MANUFACTURER := samsung
PRODUCT_RELEASE_NAME := elitelte

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRODUCT_NAME=elitelte \
    TARGET_DEVICE=elitelte
