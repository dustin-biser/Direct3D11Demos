#pragma once

#include <vector>

/**
 *	Converts C style null terminated string to a wide Unicode string, where storage
 *	of wide string is held by 'dest'.
 */
inline const wchar_t * toWideString (
	std::vector<wchar_t> & dest,
	const char * sz_source
) {
	size_t len = strlen(sz_source);
	dest.resize(len + 1);
	mbstowcs(&dest[0], sz_source, len);
	return &dest[0];
}
