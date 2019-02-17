#include "Utils.h"

#include <Shlwapi.h>
#include <wincred.h>

template<class> struct StrLenHelper;

template<>
struct StrLenHelper<CHAR> {
  static constexpr auto strlen = &lstrlenA;
};

template<>
struct StrLenHelper<WCHAR> {
  static constexpr auto strlen = &lstrlenW;
};

template<class StringType, class CharType>
USHORT InitStringImpl(const CharType *aSource, StringType *oDestination) {
  if (aSource == nullptr) {
    oDestination->Length = 0;
    oDestination->MaximumLength = 0;
    oDestination->Buffer = nullptr;
    return 0;
  }

  int l = StrLenHelper<CharType>::strlen(aSource);
  size_t w = sizeof(*aSource);
  oDestination->Length = static_cast<USHORT>(l * w);
  oDestination->MaximumLength = static_cast<USHORT>(oDestination->Length + w);
  oDestination->Buffer = reinterpret_cast<decltype(oDestination->Buffer)>(CoTaskMemAlloc(oDestination->MaximumLength));
  CopyMemory(oDestination->Buffer, aSource, oDestination->MaximumLength);
  return oDestination->Length;
}

USHORT InitString(LPCSTR aSource, STRING *oDestination) {
  return InitStringImpl(aSource, oDestination);
}

USHORT InitString(LPCWSTR aSource, UNICODE_STRING *oDestination) {
  return InitStringImpl(aSource, oDestination);
}

template<class StringType>
USHORT MoveStringToBufferImpl(void *aRoot, StringType *oSource, BYTE **oDestination) {
  if ((oSource->Buffer == nullptr) || (*oDestination == nullptr)) {
    return 0;
  }

  CopyMemory(*oDestination, oSource->Buffer, oSource->Length);
  CoTaskMemFree(oSource->Buffer);
  oSource->Buffer = reinterpret_cast<decltype(oSource->Buffer)>(*oDestination - reinterpret_cast<BYTE *>(aRoot));
  return oSource->Length;
}

USHORT MoveStringToBuffer(void *aRoot, STRING *oSource, BYTE **oDestination) {
  return MoveStringToBufferImpl(aRoot, oSource, oDestination);
}

USHORT MoveStringToBuffer(void *aRoot, UNICODE_STRING *oSource, BYTE **oDestination) {
  return MoveStringToBufferImpl(aRoot, oSource, oDestination);
}
