$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)
$(call inherit-product, device/samsung/elitelte_kor/device.mk)
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

PRODUCT_NAME := lineage_elitelte_kor
PRODUCT_DEVICE := elitelte_kor
PRODUCT_BRAND := samsung
PRODUCT_MODEL := SM-G160N
PRODUCT_MANUFACTURER := samsung
PRODUCT_RELEASE_NAME := elitelte_kor

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRODUCT_NAME=eliteltekx \
    TARGET_DEVICE=eliteltekx \
    BUILD_FINGERPRINT=samsung/eliteltekx/eliteltekx:6.0.1/MMB29M/G160NKOU1BUD2:user/release-keys \
    PRIVATE_BUILD_DESC="eliteltekx-user 6.0.1 MMB29M G160NKOU1BUD2 release-keys"
