#include "AutoLogonProvider.h"

#include <chrono>
#include <thread>

#include <Shlwapi.h>

#include "Constants.h"
#include "Dll.h"

IFACEMETHODIMP_(ULONG) AutoLogonProvider::AddRef() {
  return InterlockedIncrement(&mRefCnt);
}

IFACEMETHODIMP AutoLogonProvider::QueryInterface(REFIID aIid, void **oInterface) {
  static const QITAB qit[] = {
    QITABENT(AutoLogonProvider, ICredentialProvider),
    {0},
  };

  return QISearch(this, qit, aIid, oInterface);
}

IFACEMETHODIMP_(ULONG) AutoLogonProvider::Release() {
  ULONG refCnt = InterlockedDecrement(&mRefCnt);
  if (refCnt == 0) {
    delete this;
  }
  return refCnt;
}

IFACEMETHODIMP AutoLogonProvider::Advise(ICredentialProviderEvents *aProviderEvents, UINT_PTR aAdviseContext) {
  if (mProviderEvents != nullptr) {
    mProviderEvents->Release();
  }
  mProviderEvents = aProviderEvents;
  mProviderEvents->AddRef();
  mAdviseContext = aAdviseContext;

  std::thread t([this] () {
    std::this_thread::sleep_for(std::chrono::seconds(Constants::kTimeout));
    mTimeout = true;
    mProviderEvents->CredentialsChanged(mAdviseContext);
  });
  t.detach();

  return S_OK;
}

IFACEMETHODIMP AutoLogonProvider::GetCredentialAt(DWORD aIndex, ICredentialProviderCredential **oCredential) {
  if (!mTimeout) {
    return E_NOTIMPL;
  }

  if ((aIndex != 0) || (oCredential == nullptr)) {
    return E_INVALIDARG;
  }

  return mCredential->QueryInterface(IID_ICredentialProviderCredential, reinterpret_cast<void **>(oCredential));
}

IFACEMETHODIMP AutoLogonProvider::GetCredentialCount(DWORD *oCount, DWORD *oDefaultIndex, BOOL *oAutoLogonWithDefaultIndex) {
  if (!mTimeout) {
    *oCount = 0;
    return S_OK;
  }

  *oCount = 1;
  *oDefaultIndex = 0;
  *oAutoLogonWithDefaultIndex = TRUE;
  return S_OK;
}

IFACEMETHODIMP AutoLogonProvider::GetFieldDescriptorAt(DWORD aIndex, CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR **oDescriptor) {
  UNREFERENCED_PARAMETER(aIndex);
  UNREFERENCED_PARAMETER(oDescriptor);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonProvider::GetFieldDescriptorCount(DWORD *oCount) {
  *oCount = 0;
  return S_OK;
}

IFACEMETHODIMP AutoLogonProvider::SetSerialization(const CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION *aSerialization) {
  UNREFERENCED_PARAMETER(aSerialization);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonProvider::SetUsageScenario(CREDENTIAL_PROVIDER_USAGE_SCENARIO aUsageScenario, DWORD aFlags) {
  UNREFERENCED_PARAMETER(aFlags);

  switch (aUsageScenario) {
  case CPUS_LOGON:
  case CPUS_UNLOCK_WORKSTATION:
    mCredential = new AutoLogonCredential();
    if (mCredential == nullptr) {
      return E_OUTOFMEMORY;
    }

    return S_OK;

  case CPUS_CHANGE_PASSWORD:
  case CPUS_CREDUI:
    return E_NOTIMPL;

  default:
    return E_INVALIDARG;
  }
}

IFACEMETHODIMP AutoLogonProvider::UnAdvise() {
  if (mProviderEvents != nullptr) {
    mProviderEvents->Release();
    mProviderEvents = nullptr;
  }

  return S_OK;
}

HRESULT AutoLogonProvider::CreateInstance(REFIID aIid, void **oInstance) {
  auto *instance = new AutoLogonProvider();
  if (instance == nullptr) {
    return E_OUTOFMEMORY;
  }

  HRESULT hr = instance->QueryInterface(aIid, oInstance);
  instance->Release();
  return hr;
}

AutoLogonProvider::AutoLogonProvider() {
  DllAddRef();
}

AutoLogonProvider::~AutoLogonProvider() {
  if (mCredential != nullptr) {
    mCredential->Release();
    mCredential = nullptr;
  }

  DllRelease();
}
