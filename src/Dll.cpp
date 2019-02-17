#include "Dll.h"

#include <Windows.h>

#include "ClassFactory.h"

namespace {
ULONG gRefCnt = 0;
} // namespace

void DllAddRef() {
  InterlockedIncrement(&gRefCnt);
}

void DllRelease() {
  InterlockedDecrement(&gRefCnt);
}

STDAPI_(BOOL) DllMain(HINSTANCE aInst, DWORD aReason, void *) {
  switch (aReason) {
  case DLL_PROCESS_ATTACH:
    DisableThreadLibraryCalls(aInst);
    break;

  default:
    break;
  }

  return TRUE;
}


STDAPI DllGetClassObject(REFCLSID aClsid, REFIID aIid, void **oClassObject) {
  return ClassFactory::CreateInstance(aClsid, aIid, oClassObject);
}

STDAPI DllCanUnloadNow() {
  return (gRefCnt > 0) ? S_FALSE : S_OK;
}
