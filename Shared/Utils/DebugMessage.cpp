#include "DebugMessage.hpp"

#include <stdio.h>


void WINAPI DebugMessage (
	_In_z_ const WCHAR* strFile,
	_In_ DWORD dwLine,
	_In_opt_ const WCHAR* strMsg 
) {
	const size_t BUFFER_SIZE(1000);
	WCHAR strBufferLine[128];
	WCHAR strBuffer[BUFFER_SIZE];

	swprintf_s(strBufferLine, 128, L"%lu", dwLine);

	WCHAR strBufferFile[MAX_PATH];
	wcscpy_s(strBufferFile, MAX_PATH, L"");
	if (strFile)
		wcscpy_s(strBufferFile, MAX_PATH, strFile);

	WCHAR strBufferMsg[1024];
	wcscpy_s( strBufferMsg, 1024, L"" );
    size_t nMsgLen = (strMsg) ? wcsnlen_s( strMsg, 1024 ) : 0;
	if( nMsgLen > 0 )
		swprintf_s( strBufferMsg, 1024, L"%ls", strMsg );

	swprintf_s(strBuffer, BUFFER_SIZE, 
		L"%ls\n\n"
		L"File: %ls\n"
		L"Line: %ls\n"
		L"\nDo you want pause the application and debug?",
		strBufferMsg, strBufferFile, strBufferLine);

	int nResult = MessageBoxW(GetForegroundWindow(), strBuffer, L"Debug Message", 
		MB_YESNO | MB_ICONEXCLAMATION);
	if (nResult == IDYES)
		DebugBreak();
}
