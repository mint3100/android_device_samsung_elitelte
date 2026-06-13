#!/bin/bash
#
# Copyright (C) 2026 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

DEVICE=elitelte
VENDOR=samsung

MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "${MY_DIR}" ]]; then
    MY_DIR="${PWD}"
fi

LINEAGE_ROOT="${MY_DIR}/../../.."
HELPER="${LINEAGE_ROOT}/vendor/lineage/build/tools/extract_utils.sh"

if [[ ! -f "${HELPER}" ]]; then
    echo "Unable to find extract_utils.sh at ${HELPER}" >&2
    exit 1
fi

SECTION=
if [[ "${1}" == "--section" ]]; then
    SECTION="${2}"
    shift 2
fi

SRC="${1:-adb}"

. "${HELPER}"

setup_vendor "${DEVICE}" "${VENDOR}" "${LINEAGE_ROOT}" false false
extract "${MY_DIR}/proprietary-files.txt" "${SRC}" "${SECTION}"
"${MY_DIR}/setup-makefiles.sh"
