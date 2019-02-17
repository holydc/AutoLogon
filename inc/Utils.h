#pragma once

#include <Windows.h>
#include <NTSecAPI.h>

USHORT InitString(LPCSTR aSource, STRING *oDestination);
USHORT InitString(LPCWSTR aSource, UNICODE_STRING *oDestination);

USHORT MoveStringToBuffer(void *aRoot, STRING *oSource, BYTE **oDestination);
USHORT MoveStringToBuffer(void *aRoot, UNICODE_STRING *oSource, BYTE **oDestination);
