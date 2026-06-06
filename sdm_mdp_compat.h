#pragma once

#include <stdint.h>

#ifndef MDP_DESTSCALER_ENABLE
#define MDP_DESTSCALER_ENABLE (1 << 0)
#endif

#ifndef MDP_DESTSCALER_SCALE_UPDATE
#define MDP_DESTSCALER_SCALE_UPDATE (1 << 1)
#endif

#ifndef MDP_DESTSCALER_ENHANCER_UPDATE
#define MDP_DESTSCALER_ENHANCER_UPDATE (1 << 2)
#endif

#ifndef ELITELTE_MDP_DESTINATION_SCALER_DATA_COMPAT
#define ELITELTE_MDP_DESTINATION_SCALER_DATA_COMPAT
struct mdp_destination_scaler_data {
  uint32_t flags;
  uint32_t dest_scaler_ndx;
  uint32_t lm_width;
  uint32_t lm_height;
  uint64_t scale;
};
#endif
