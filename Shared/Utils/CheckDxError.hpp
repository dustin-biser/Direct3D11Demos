#pragma once

#include "dxerr.h"


/**
 *	Can be used with any DirectX function that returns an HRESULT.
 *	Checks the HRESULT x. If FAILED(x), a MessageBox is displayed with DirectX
 *	error message, and an option to drop into debug mode at the line of failure.
 */

#ifdef _DEBUG
	#ifndef CHECK_DX_ERROR
	#define CHECK_DX_ERROR(x) \
	do { \
		HRESULT hr = x; \
		if (FAILED(hr)) { \
			/* Stringify argument x and pass to DXTrace as a wide string */ \
			DXTrace(__FILEW__, __LINE__, hr, L#x, true); \
		} \
	} \
	while(0)
	#endif 
#else
	#ifndef CHECK_DX_ERROR
	#define CHECK_DX_ERROR(x) x;
	#endif
#endif

