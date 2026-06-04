INSTALLED_DTIMAGE_TARGET := $(PRODUCT_OUT)/dt.img
DTBTOOL := $(HOST_OUT_EXECUTABLES)/dtbToolLineage$(HOST_EXECUTABLE_SUFFIX)
CUSTOM_KERNEL_OUT := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ
CUSTOM_KERNEL_ARCH := $(TARGET_KERNEL_ARCH)
possible_dtb_dirs := $(CUSTOM_KERNEL_OUT)/arch/$(CUSTOM_KERNEL_ARCH)/boot/

define build-dtimage-target
	$(call pretty,"Target dt image: $@")
	$(hide) for dir in $(possible_dtb_dirs); do \
		if [ -d "$$dir" ]; then \
			dtb_dir="$$dir"; \
			break; \
		fi; \
	done; \
	$(DTBTOOL) $(BOARD_DTBTOOL_ARGS) -o $@ -s $(BOARD_KERNEL_PAGESIZE) -p $(CUSTOM_KERNEL_OUT)/scripts/dtc/ "$$dtb_dir";
	$(hide) chmod a+r $@
endef

$(INSTALLED_DTIMAGE_TARGET): $(DTBTOOL) $(INSTALLED_KERNEL_TARGET)
	$(build-dtimage-target)
	@echo "Made DT image: $@"

ALL_DEFAULT_INSTALLED_MODULES += $(INSTALLED_DTIMAGE_TARGET)

.PHONY: dtimage
dtimage: $(INSTALLED_DTIMAGE_TARGET)

$(INSTALLED_BOOTIMAGE_TARGET): $(MKBOOTIMG) $(INTERNAL_BOOTIMAGE_FILES) $(BOOTIMAGE_EXTRA_DEPS)
	@echo "----- Making Samsung boot image ------"
	$(hide) $(MKBOOTIMG) $(INTERNAL_BOOTIMAGE_ARGS) $(INTERNAL_MKBOOTIMG_VERSION_ARGS) $(BOARD_MKBOOTIMG_ARGS) --output $@
	$(hide) echo -n "SEANDROIDENFORCE" >> $@
	$(hide) $(call assert-max-image-size,$@,$(BOARD_BOOTIMAGE_PARTITION_SIZE))

$(INSTALLED_RECOVERYIMAGE_TARGET): $(MKBOOTIMG) $(recovery_ramdisk) $(recovery_kernel) $(RECOVERYIMAGE_EXTRA_DEPS)
	@echo "----- Making Samsung recovery image ------"
	$(call build-recoveryimage-target, $@)
	$(hide) echo -n "SEANDROIDENFORCE" >> $@
	$(hide) $(call assert-max-image-size,$@,$(BOARD_RECOVERYIMAGE_PARTITION_SIZE))
