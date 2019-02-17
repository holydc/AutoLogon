#pragma once

#include <Windows.h>

class ClassFactory : public IClassFactory {
public:
  // IUnknown
  IFACEMETHODIMP_(ULONG) AddRef() override;
  IFACEMETHODIMP QueryInterface(REFIID aIid, void **oInterface) override;
  IFACEMETHODIMP_(ULONG) Release() override;

  // IClassFactory
  IFACEMETHODIMP CreateInstance(IUnknown *aOuter, REFIID aIid, void **oInstance) override;
  IFACEMETHODIMP LockServer(BOOL aLock) override;

  // ClassFactory
  static HRESULT CreateInstance(REFCLSID aClsid, REFIID aIid, void **oInstance);

private:
  ULONG mRefCnt = 1;

  ClassFactory() = default;
  ~ClassFactory() = default;
}; // class ClassFactory
