#include <stdio.h>
#include <sys/types.h>

extern "C" {

void *sdm_legacy_fopen asm("_ZN3sdm3Sys6fopen_E") =
    reinterpret_cast<void *>(fopen);

void *sdm_legacy_fclose asm("_ZN3sdm3Sys7fclose_E") =
    reinterpret_cast<void *>(fclose);

void *sdm_legacy_getline asm("_ZN3sdm3Sys8getline_E") =
    reinterpret_cast<void *>(getline);

}
