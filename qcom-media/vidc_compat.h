/*
 * Compatibility definitions for building Oreo CAF media against the
 * Samsung Marshmallow msm8917 kernel headers.
 *
 * Values are taken from Qualcomm's Oreo msm_vidc UAPI and are only supplied
 * when the stock kernel header does not already define them.
 */

#ifndef MSM_VIDC_BT709_5
#define MSM_VIDC_BT709_5 1
#endif

#ifndef MSM_VIDC_UNSPECIFIED
#define MSM_VIDC_UNSPECIFIED 2
#endif

#ifndef MSM_VIDC_BT470_6_M
#define MSM_VIDC_BT470_6_M 4
#endif

#ifndef MSM_VIDC_BT601_6_625
#define MSM_VIDC_BT601_6_625 5
#endif

#ifndef MSM_VIDC_BT470_6_BG
#define MSM_VIDC_BT470_6_BG MSM_VIDC_BT601_6_625
#endif

#ifndef MSM_VIDC_BT601_6_525
#define MSM_VIDC_BT601_6_525 6
#endif

#ifndef MSM_VIDC_SMPTE_240M
#define MSM_VIDC_SMPTE_240M 7
#endif

#ifndef MSM_VIDC_GENERIC_FILM
#define MSM_VIDC_GENERIC_FILM 8
#endif

#ifndef MSM_VIDC_BT2020
#define MSM_VIDC_BT2020 9
#endif

typedef int msm_vidc_h264_color_primaries_values;

#ifndef MSM_VIDC_CS_UNKNOWN
#define MSM_VIDC_CS_UNKNOWN 0
#endif

#ifndef MSM_VIDC_CS_BT_601
#define MSM_VIDC_CS_BT_601 1
#endif

#ifndef MSM_VIDC_CS_BT_709
#define MSM_VIDC_CS_BT_709 2
#endif

#ifndef MSM_VIDC_CS_SMPTE_170
#define MSM_VIDC_CS_SMPTE_170 3
#endif

#ifndef MSM_VIDC_CS_SMPTE_240
#define MSM_VIDC_CS_SMPTE_240 4
#endif

#ifndef MSM_VIDC_CS_BT_2020
#define MSM_VIDC_CS_BT_2020 5
#endif

#ifndef MSM_VIDC_CS_RESERVED
#define MSM_VIDC_CS_RESERVED 6
#endif

#ifndef MSM_VIDC_CS_RGB
#define MSM_VIDC_CS_RGB 7
#endif

#ifndef MSM_VIDC_MATRIX_RGB
#define MSM_VIDC_MATRIX_RGB 0
#endif

#ifndef MSM_VIDC_MATRIX_BT_709_5
#define MSM_VIDC_MATRIX_BT_709_5 1
#endif

#ifndef MSM_VIDC_MATRIX_UNSPECIFIED
#define MSM_VIDC_MATRIX_UNSPECIFIED 2
#endif

#ifndef MSM_VIDC_MATRIX_RESERVED
#define MSM_VIDC_MATRIX_RESERVED 3
#endif

#ifndef MSM_VIDC_MATRIX_FCC_47
#define MSM_VIDC_MATRIX_FCC_47 4
#endif

#ifndef MSM_VIDC_MATRIX_601_6_625
#define MSM_VIDC_MATRIX_601_6_625 5
#endif

#ifndef MSM_VIDC_MATRIX_BT470_BG
#define MSM_VIDC_MATRIX_BT470_BG MSM_VIDC_MATRIX_601_6_625
#endif

#ifndef MSM_VIDC_MATRIX_601_6_525
#define MSM_VIDC_MATRIX_601_6_525 6
#endif

#ifndef MSM_VIDC_MATRIX_SMPTE_170M
#define MSM_VIDC_MATRIX_SMPTE_170M MSM_VIDC_MATRIX_601_6_525
#endif

#ifndef MSM_VIDC_MATRIX_SMPTE_240M
#define MSM_VIDC_MATRIX_SMPTE_240M 7
#endif

#ifndef MSM_VIDC_MATRIX_Y_CG_CO
#define MSM_VIDC_MATRIX_Y_CG_CO 8
#endif

#ifndef MSM_VIDC_MATRIX_BT_2020
#define MSM_VIDC_MATRIX_BT_2020 9
#endif

#ifndef MSM_VIDC_MATRIX_BT_2020_CONST
#define MSM_VIDC_MATRIX_BT_2020_CONST 10
#endif

typedef int msm_vidc_h264_matrix_coeff_values;

#ifndef MSM_VIDC_TRANSFER_BT709_5
#define MSM_VIDC_TRANSFER_BT709_5 1
#endif

#ifndef MSM_VIDC_TRANSFER_UNSPECIFIED
#define MSM_VIDC_TRANSFER_UNSPECIFIED 2
#endif

#ifndef MSM_VIDC_TRANSFER_BT_470_6_M
#define MSM_VIDC_TRANSFER_BT_470_6_M 4
#endif

#ifndef MSM_VIDC_TRANSFER_BT_470_6_BG
#define MSM_VIDC_TRANSFER_BT_470_6_BG 5
#endif

#ifndef MSM_VIDC_TRANSFER_601_6_625
#define MSM_VIDC_TRANSFER_601_6_625 6
#endif

#ifndef MSM_VIDC_TRANSFER_601_6_525
#define MSM_VIDC_TRANSFER_601_6_525 MSM_VIDC_TRANSFER_601_6_625
#endif

#ifndef MSM_VIDC_TRANSFER_SMPTE_240M
#define MSM_VIDC_TRANSFER_SMPTE_240M 7
#endif

#ifndef MSM_VIDC_TRANSFER_LINEAR
#define MSM_VIDC_TRANSFER_LINEAR 8
#endif

#ifndef MSM_VIDC_TRANSFER_LOG_100_1
#define MSM_VIDC_TRANSFER_LOG_100_1 9
#endif

#ifndef MSM_VIDC_TRANSFER_LOG_100_SQRT10_1
#define MSM_VIDC_TRANSFER_LOG_100_SQRT10_1 10
#endif

#ifndef MSM_VIDC_TRANSFER_IEC_61966
#define MSM_VIDC_TRANSFER_IEC_61966 11
#endif

#ifndef MSM_VIDC_TRANSFER_BT_1361
#define MSM_VIDC_TRANSFER_BT_1361 12
#endif

#ifndef MSM_VIDC_TRANSFER_SRGB
#define MSM_VIDC_TRANSFER_SRGB 13
#endif

#ifndef MSM_VIDC_TRANSFER_BT_2020_10
#define MSM_VIDC_TRANSFER_BT_2020_10 14
#endif

#ifndef MSM_VIDC_TRANSFER_BT_2020_12
#define MSM_VIDC_TRANSFER_BT_2020_12 15
#endif

typedef int msm_vidc_h264_transfer_chars_values;

struct msm_vidc_vc1_seqdisp_payload {
    unsigned int prog_seg_format;
    unsigned int uv_sampl_fmt;
    unsigned int color_format;
    unsigned int color_primaries;
    unsigned int transfer_char;
    unsigned int matrix_coeffs;
    unsigned int aspect_ratio;
    unsigned int aspect_horiz;
    unsigned int aspect_vert;
};

struct msm_vidc_vpx_colorspace_payload {
    unsigned int color_space;
    unsigned int yuv_range_flag;
    unsigned int sumsampling_x;
    unsigned int sumsampling_y;
};

struct msm_vidc_vui_display_info_payload {
    unsigned int video_signal_present_flag;
    unsigned int video_format;
    unsigned int bit_depth_y;
    unsigned int bit_depth_c;
    unsigned int video_full_range_flag;
    unsigned int color_description_present_flag;
    unsigned int color_primaries;
    unsigned int transfer_characteristics;
    unsigned int matrix_coefficients;
    unsigned int chroma_location_info_present_flag;
    unsigned int chroma_format_idc;
    unsigned int separate_color_plane_flag;
    unsigned int chroma_sample_loc_type_top_field;
    unsigned int chroma_sample_loc_type_bottom_field;
};

#ifndef MSM_VIDC_EXTRADATA_VPX_COLORSPACE_INFO
#define MSM_VIDC_EXTRADATA_VPX_COLORSPACE_INFO 0x00000014
#endif

#ifndef MSM_VIDC_EXTRADATA_VUI_DISPLAY_INFO
#define MSM_VIDC_EXTRADATA_VUI_DISPLAY_INFO 0x7F100006
#endif

#ifndef V4L2_MPEG_VIDC_EXTRADATA_VUI_DISPLAY
#define V4L2_MPEG_VIDC_EXTRADATA_VUI_DISPLAY 29
#endif

#ifndef V4L2_MPEG_VIDC_EXTRADATA_VPX_COLORSPACE
#define V4L2_MPEG_VIDC_EXTRADATA_VPX_COLORSPACE 30
#endif

#ifndef V4L2_CID_MPEG_VIDC_VIDEO_COLOR_SPACE
#define V4L2_CID_MPEG_VIDC_VIDEO_COLOR_SPACE \
    (V4L2_CID_MPEG_MSM_VIDC_BASE + 60)
#endif

#ifndef V4L2_CID_MPEG_VIDC_VIDEO_FULL_RANGE
#define V4L2_CID_MPEG_VIDC_VIDEO_FULL_RANGE \
    (V4L2_CID_MPEG_MSM_VIDC_BASE + 61)
#endif

#ifndef V4L2_CID_MPEG_VIDC_VIDEO_TRANSFER_CHARS
#define V4L2_CID_MPEG_VIDC_VIDEO_TRANSFER_CHARS \
    (V4L2_CID_MPEG_MSM_VIDC_BASE + 62)
#endif

#ifndef V4L2_CID_MPEG_VIDC_VIDEO_MATRIX_COEFFS
#define V4L2_CID_MPEG_VIDC_VIDEO_MATRIX_COEFFS \
    (V4L2_CID_MPEG_MSM_VIDC_BASE + 63)
#endif
