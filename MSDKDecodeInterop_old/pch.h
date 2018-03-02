#pragma once

#ifdef MEDIASDK_UWP_SUPPORT
#include <collection.h>
#include <ppltasks.h>
#include "mfxvideo++.h"
#include "sample_defs.h"
#include "d3d11_device.h"
#include "base_allocator.h"
#include "d3d11_allocator.h"
#endif

// Disable debug string output on non-debug build
#if !_DEBUG
#define DebugMessage(x)
#else
#define DebugMessage(x) OutputDebugString(x)
#endif

