#include "ClassFactory.h"

#include <Shlwapi.h>

#include "AutoLogonProvider.h"
#include "Dll.h"
#include "Guid.h"

IFACEMETHODIMP_(ULONG) ClassFactory::AddRef() {
  return InterlockedIncrement(&mRefCnt);
}

IFACEMETHODIMP ClassFactory::QueryInterface(REFIID aIid, void **oInterface) {
  static const QITAB qit[] = {
    QITABENT(ClassFactory, IClassFactory),
    {0},
  };

  return QISearch(this, qit, aIid, oInterface);
}

IFACEMETHODIMP_(ULONG) ClassFactory::Release() {
  ULONG refCnt = InterlockedDecrement(&mRefCnt);
  if (refCnt == 0) {
    delete this;
  }
  return refCnt;
}

IFACEMETHODIMP ClassFactory::CreateInstance(IUnknown *aOuter, REFIID aIid, void **oInstance) {
  if (aOuter != nullptr) {
    *oInstance = nullptr;
    return CLASS_E_NOAGGREGATION;
  }

  return AutoLogonProvider::CreateInstance(aIid, oInstance);
}

IFACEMETHODIMP ClassFactory::LockServer(BOOL aLock) {
  if (aLock) {
    DllAddRef();
  } else {
    DllRelease();
  }
  return S_OK;
}

HRESULT ClassFactory::CreateInstance(REFCLSID aClsid, REFIID aIid, void **oInstance) {
  if (aClsid != kGuid) {
    return CLASS_E_CLASSNOTAVAILABLE;
  }

  auto *instance = new ClassFactory();
  if (instance == nullptr) {
    return E_OUTOFMEMORY;
  }

  HRESULT hr = instance->QueryInterface(aIid, oInstance);
  instance->Release();
  return hr;
}
