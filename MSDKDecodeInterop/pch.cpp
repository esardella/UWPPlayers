#include "pch.h"


#include "sample_types.h"
#include "mfxvideo.h"

msdk_string StatusToString(mfxStatus sts)
{
	switch (sts)
	{
	case MFX_ERR_NONE:
		return msdk_string(MSDK_STRING("MFX_ERR_NONE"));
	case MFX_ERR_UNKNOWN:
		return msdk_string(MSDK_STRING("MFX_ERR_UNKNOWN"));
	case MFX_ERR_NULL_PTR:
		return msdk_string(MSDK_STRING("MFX_ERR_NULL_PTR"));
	case MFX_ERR_UNSUPPORTED:
		return msdk_string(MSDK_STRING("MFX_ERR_UNSUPPORTED"));
	case MFX_ERR_MEMORY_ALLOC:
		return msdk_string(MSDK_STRING("MFX_ERR_MEMORY_ALLOC"));
	case MFX_ERR_NOT_ENOUGH_BUFFER:
		return msdk_string(MSDK_STRING("MFX_ERR_NOT_ENOUGH_BUFFER"));
	case MFX_ERR_INVALID_HANDLE:
		return msdk_string(MSDK_STRING("MFX_ERR_INVALID_HANDLE"));
	case MFX_ERR_LOCK_MEMORY:
		return msdk_string(MSDK_STRING("MFX_ERR_LOCK_MEMORY"));
	case MFX_ERR_NOT_INITIALIZED:
		return msdk_string(MSDK_STRING("MFX_ERR_NOT_INITIALIZED"));
	case MFX_ERR_NOT_FOUND:
		return msdk_string(MSDK_STRING("MFX_ERR_NOT_FOUND"));
	case MFX_ERR_MORE_DATA:
		return msdk_string(MSDK_STRING("MFX_ERR_MORE_DATA"));
	case MFX_ERR_MORE_SURFACE:
		return msdk_string(MSDK_STRING("MFX_ERR_MORE_SURFACE"));
	case MFX_ERR_ABORTED:
		return msdk_string(MSDK_STRING("MFX_ERR_ABORTED"));
	case MFX_ERR_DEVICE_LOST:
		return msdk_string(MSDK_STRING("MFX_ERR_DEVICE_LOST"));
	case MFX_ERR_INCOMPATIBLE_VIDEO_PARAM:
		return msdk_string(MSDK_STRING("MFX_ERR_INCOMPATIBLE_VIDEO_PARAM"));
	case MFX_ERR_INVALID_VIDEO_PARAM:
		return msdk_string(MSDK_STRING("MFX_ERR_INVALID_VIDEO_PARAM"));
	case MFX_ERR_UNDEFINED_BEHAVIOR:
		return msdk_string(MSDK_STRING("MFX_ERR_UNDEFINED_BEHAVIOR"));
	case MFX_ERR_DEVICE_FAILED:
		return msdk_string(MSDK_STRING("MFX_ERR_DEVICE_FAILED"));
	case MFX_ERR_MORE_BITSTREAM:
		return msdk_string(MSDK_STRING("MFX_ERR_MORE_BITSTREAM"));
	case MFX_ERR_INCOMPATIBLE_AUDIO_PARAM:
		return msdk_string(MSDK_STRING("MFX_ERR_INCOMPATIBLE_AUDIO_PARAM"));
	case MFX_ERR_INVALID_AUDIO_PARAM:
		return msdk_string(MSDK_STRING("MFX_ERR_INVALID_AUDIO_PARAM"));
	case MFX_ERR_GPU_HANG:
		return msdk_string(MSDK_STRING("MFX_ERR_GPU_HANG"));
	case MFX_ERR_REALLOC_SURFACE:
		return msdk_string(MSDK_STRING("MFX_ERR_REALLOC_SURFACE"));
	case MFX_WRN_IN_EXECUTION:
		return msdk_string(MSDK_STRING("MFX_WRN_IN_EXECUTION"));
	case MFX_WRN_DEVICE_BUSY:
		return msdk_string(MSDK_STRING("MFX_WRN_DEVICE_BUSY"));
	case MFX_WRN_VIDEO_PARAM_CHANGED:
		return msdk_string(MSDK_STRING("MFX_WRN_VIDEO_PARAM_CHANGED"));
	case MFX_WRN_PARTIAL_ACCELERATION:
		return msdk_string(MSDK_STRING("MFX_WRN_PARTIAL_ACCELERATION"));
	case MFX_WRN_INCOMPATIBLE_VIDEO_PARAM:
		return msdk_string(MSDK_STRING("MFX_WRN_INCOMPATIBLE_VIDEO_PARAM"));
	case MFX_WRN_VALUE_NOT_CHANGED:
		return msdk_string(MSDK_STRING("MFX_WRN_VALUE_NOT_CHANGED"));
	case MFX_WRN_OUT_OF_RANGE:
		return msdk_string(MSDK_STRING("MFX_WRN_OUT_OF_RANGE"));
	case MFX_WRN_FILTER_SKIPPED:
		return msdk_string(MSDK_STRING("MFX_WRN_FILTER_SKIPPED"));
	case MFX_WRN_INCOMPATIBLE_AUDIO_PARAM:
		return msdk_string(MSDK_STRING("MFX_WRN_INCOMPATIBLE_AUDIO_PARAM"));
	case MFX_TASK_WORKING:
		return msdk_string(MSDK_STRING("MFX_TASK_WORKING"));
	case MFX_TASK_BUSY:
		return msdk_string(MSDK_STRING("MFX_TASK_BUSY"));
	case MFX_ERR_MORE_DATA_SUBMIT_TASK:
		return msdk_string(MSDK_STRING("MFX_ERR_MORE_DATA_SUBMIT_TASK"));
	default:
		return msdk_string(MSDK_STRING("[Unknown status]"));
	}
}