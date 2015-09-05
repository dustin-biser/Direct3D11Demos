#pragma once

#include <windows.h>
#include <sal.h>

#ifdef __cplusplus
extern "C" {
#endif

void WINAPI DebugMessage (
	_In_z_ const WCHAR* strFile,
	_In_ DWORD dwLine,
	_In_opt_ const WCHAR* strMsg
);

//---------------------------------------------------------------------------------------
// Helper Macro
#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_MSG(str)	DebugMessage( __FILEW__, (DWORD)__LINE__, str)
#else
#define DEBUG_MSG(str)	(0L)
#endif

#ifdef __cplusplus
}
#endif
