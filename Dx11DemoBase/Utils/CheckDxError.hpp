#pragma once

#include "dxerr.h"

#ifdef _DEBUG
	#ifndef CHECK_DX_ERRORS
	#define CHECK_DX_ERROR(x) \
	{ \
		HRESULT hr = x; \
		if (FAILED(hr)) { \
			/* Stringify to argument x and pass to DXTrace as a wide string */ \
			DXTrace(__FILEW__, __LINE__, hr, L#x, true); \
		} \
	}
	#endif 
#else
	#ifndef CHECK_DX_ERRORS
	#define CHECK_DX_ERROR(x) x;
	#endif
#endif

