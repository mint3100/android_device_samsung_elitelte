#!/bin/bash
#
# Copyright (C) 2026 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

cat <<'EOF'
This device keeps a curated vendor/samsung/elitelte/elitelte-vendor.mk.
Do not regenerate vendor makefiles with extract_utils.sh because the LineageOS
15.1 tree needs hand-kept copy destinations for legacy vendor HALs.

Use ./extract-files.sh <adb|mounted-system-dir|ota.zip> to refresh blobs.
EOF
