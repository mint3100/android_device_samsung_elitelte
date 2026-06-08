/*
 * libOmxVenc needs getMetaData() for timestamp and color-space metadata.
 * Samsung's stock libqdMetaData blob exports setMetaData() only, so provide
 * the read side here without changing platform or vendor blobs.
 */

#include <errno.h>
#include <string.h>
#include <sys/mman.h>

#include <cutils/log.h>
#include <gralloc_priv.h>
#include <qdMetaData.h>

static int validateAndMapForRead(private_handle_t *handle) {
    if (private_handle_t::validate(handle)) {
        ALOGE("%s: invalid private handle %p", __func__, handle);
        return -EINVAL;
    }

    if (handle->fd_metadata == -1) {
        ALOGE("%s: invalid metadata fd for handle %p", __func__, handle);
        return -EINVAL;
    }

    if (!handle->base_metadata) {
        unsigned long size = ROUND_UP_PAGESIZE(sizeof(MetaData_t));
        void *base = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,
                          handle->fd_metadata, 0);
        if (base == reinterpret_cast<void *>(MAP_FAILED)) {
            ALOGE("%s: metadata mmap failed for handle %p fd %d: %s",
                  __func__, handle, handle->fd_metadata, strerror(errno));
            return -errno;
        }
        handle->base_metadata = reinterpret_cast<uintptr_t>(base);
    }

    return 0;
}

extern "C" int getMetaData(private_handle_t *handle,
                           DispFetchParamType paramType, void *param) {
    int ret = validateAndMapForRead(handle);
    if (ret != 0) {
        return ret;
    }

    MetaData_t *data = reinterpret_cast<MetaData_t *>(handle->base_metadata);
    ret = -EINVAL;

    switch (paramType) {
        case GET_PP_PARAM_INTERLACED:
            if (data->operation & PP_PARAM_INTERLACED) {
                *reinterpret_cast<int32_t *>(param) = data->interlaced;
                ret = 0;
            }
            break;
        case GET_BUFFER_GEOMETRY:
            if (data->operation & UPDATE_BUFFER_GEOMETRY) {
                *reinterpret_cast<BufferDim_t *>(param) = data->bufferDim;
                ret = 0;
            }
            break;
        case GET_REFRESH_RATE:
            if (data->operation & UPDATE_REFRESH_RATE) {
                *reinterpret_cast<float *>(param) = data->refreshrate;
                ret = 0;
            }
            break;
        case GET_COLOR_SPACE:
            if (data->operation & UPDATE_COLOR_SPACE) {
                *reinterpret_cast<ColorSpace_t *>(param) = data->colorSpace;
                ret = 0;
            }
            break;
        case GET_MAP_SECURE_BUFFER:
            if (data->operation & MAP_SECURE_BUFFER) {
                *reinterpret_cast<int32_t *>(param) = data->mapSecureBuffer;
                ret = 0;
            }
            break;
        case GET_S3D_FORMAT:
            if (data->operation & S3D_FORMAT) {
                *reinterpret_cast<uint32_t *>(param) = data->s3dFormat;
                ret = 0;
            }
            break;
        case GET_LINEAR_FORMAT:
            if (data->operation & LINEAR_FORMAT) {
                *reinterpret_cast<uint32_t *>(param) = data->linearFormat;
                ret = 0;
            }
            break;
        case GET_IGC:
            if (data->operation & SET_IGC) {
                *reinterpret_cast<IGC_t *>(param) = data->igc;
                ret = 0;
            }
            break;
        case GET_SINGLE_BUFFER_MODE:
            if (data->operation & SET_SINGLE_BUFFER_MODE) {
                *reinterpret_cast<uint32_t *>(param) = data->isSingleBufferMode;
                ret = 0;
            }
            break;
        case GET_VT_TIMESTAMP:
            if (data->operation & SET_VT_TIMESTAMP) {
                *reinterpret_cast<uint64_t *>(param) = data->vtTimeStamp;
                ret = 0;
            }
            break;
        default:
            ALOGE("%s: unknown metadata param %d", __func__, paramType);
            break;
    }

    return ret;
}
