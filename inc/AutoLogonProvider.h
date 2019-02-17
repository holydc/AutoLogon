#pragma once

#include <credentialprovider.h>

#include "AutoLogonCredential.h"

class AutoLogonProvider : public ICredentialProvider {
public:
  // IUnknown
  IFACEMETHODIMP_(ULONG) AddRef() override;
  IFACEMETHODIMP QueryInterface(REFIID aIid, void **oInterface) override;
  IFACEMETHODIMP_(ULONG) Release() override;

  // ICredentialProvider
  IFACEMETHODIMP Advise(ICredentialProviderEvents *aProviderEvents, UINT_PTR aAdviseContext) override;
  IFACEMETHODIMP GetCredentialAt(DWORD aIndex, ICredentialProviderCredential **oCredential) override;
  IFACEMETHODIMP GetCredentialCount(DWORD *oCount, DWORD *oDefaultIndex, BOOL *oAutoLogonWithDefaultIndex) override;
  IFACEMETHODIMP GetFieldDescriptorAt(DWORD aIndex, CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR **oDescriptor) override;
  IFACEMETHODIMP GetFieldDescriptorCount(DWORD *oCount) override;
  IFACEMETHODIMP SetSerialization(const CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION *aSerialization) override;
  IFACEMETHODIMP SetUsageScenario(CREDENTIAL_PROVIDER_USAGE_SCENARIO aUsageScenario, DWORD aFlags) override;
  IFACEMETHODIMP UnAdvise() override;

  // AutoLogonProvider
  static HRESULT CreateInstance(REFIID aIid, void **oInstance);

private:
  UINT_PTR mAdviseContext = NULL;
  AutoLogonCredential *mCredential = nullptr;
  ICredentialProviderEvents *mProviderEvents = nullptr;
  ULONG mRefCnt = 1;
  bool mTimeout = false;

  AutoLogonProvider();
  ~AutoLogonProvider();
}; // class AutoLogonProvider
