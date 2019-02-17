#include "AutoLogonCredential.h"

#include <NTSecAPI.h>
#include <Shlwapi.h>

#include "Constants.h"
#include "Dll.h"
#include "Guid.h"
#include "Utils.h"

IFACEMETHODIMP_(ULONG) AutoLogonCredential::AddRef() {
  return InterlockedIncrement(&mRefCnt);
}

IFACEMETHODIMP AutoLogonCredential::QueryInterface(REFIID aIid, void **oInterface) {
  static const QITAB qit[] = {
    QITABENT(AutoLogonCredential, ICredentialProviderCredential),
    {0},
  };

  return QISearch(this, qit, aIid, oInterface);
}

IFACEMETHODIMP_(ULONG) AutoLogonCredential::Release() {
  ULONG refCnt = InterlockedDecrement(&mRefCnt);
  if (refCnt == 0) {
    delete this;
  }
  return refCnt;
}

IFACEMETHODIMP AutoLogonCredential::Advise(ICredentialProviderCredentialEvents *aCredentialEvents) {
  UNREFERENCED_PARAMETER(aCredentialEvents);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::CommandLinkClicked(DWORD aFieldId) {
  UNREFERENCED_PARAMETER(aFieldId);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::GetBitmapValue(DWORD aFieldId, HBITMAP *oValue) {
  UNREFERENCED_PARAMETER(aFieldId);
  UNREFERENCED_PARAMETER(oValue);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::GetCheckboxValue(DWORD aFieldId, BOOL *oChecked, PWSTR *oValue) {
  UNREFERENCED_PARAMETER(aFieldId);
  UNREFERENCED_PARAMETER(oChecked);
  UNREFERENCED_PARAMETER(oValue);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::GetComboBoxValueAt(DWORD aFieldId, DWORD aItemIndex, PWSTR *oValue) {
  UNREFERENCED_PARAMETER(aFieldId);
  UNREFERENCED_PARAMETER(aItemIndex);
  UNREFERENCED_PARAMETER(oValue);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::GetComboBoxValueCount(DWORD aFieldId, DWORD *oNumItems, DWORD *oSelectedItemIndex) {
  UNREFERENCED_PARAMETER(aFieldId);
  UNREFERENCED_PARAMETER(oNumItems);
  UNREFERENCED_PARAMETER(oSelectedItemIndex);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::GetFieldState(DWORD aFieldId, CREDENTIAL_PROVIDER_FIELD_STATE *oFieldState, CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE *oFieldInteractiveState) {
  UNREFERENCED_PARAMETER(aFieldId);
  UNREFERENCED_PARAMETER(oFieldState);
  UNREFERENCED_PARAMETER(oFieldInteractiveState);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::GetSerialization(CREDENTIAL_PROVIDER_GET_SERIALIZATION_RESPONSE *oResponse, CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION *oSerialization, PWSTR *oOptionalStatusText, CREDENTIAL_PROVIDER_STATUS_ICON *oOptionalStatusIcon) {
  UNREFERENCED_PARAMETER(oOptionalStatusText);
  UNREFERENCED_PARAMETER(oOptionalStatusIcon);

  NTSTATUS status;
  HRESULT hr;

  HANDLE lsa;
  status = LsaConnectUntrusted(&lsa);
  hr = LsaNtStatusToWinError(status);
  if (FAILED(hr)) {
    LsaDeregisterLogonProcess(lsa);
    return hr;
  }

  LSA_STRING packageName;
  InitString(MSV1_0_PACKAGE_NAME, &packageName);

  ULONG authenticationPackage;
  status = LsaLookupAuthenticationPackage(lsa, &packageName, &authenticationPackage);
  hr = LsaNtStatusToWinError(status);
  if (FAILED(hr)) {
    LsaDeregisterLogonProcess(lsa);
    return hr;
  }

  MSV1_0_INTERACTIVE_LOGON logon;
  ZeroMemory(&logon, sizeof(logon));
  logon.MessageType = MsV1_0InteractiveLogon;

  ULONG bufferLength = sizeof(logon);
  bufferLength += InitString(Constants::kLogonDomainName, &logon.LogonDomainName);
  bufferLength += InitString(Constants::kUserName, &logon.UserName);
  bufferLength += InitString(Constants::kPassword, &logon.Password);

  auto *buffer = reinterpret_cast<BYTE *>(CoTaskMemAlloc(bufferLength));
  if (buffer == nullptr) {
    LsaDeregisterLogonProcess(lsa);
    return E_OUTOFMEMORY;
  }

  CopyMemory(buffer, &logon, sizeof(logon));

  auto *pLogon = reinterpret_cast<MSV1_0_INTERACTIVE_LOGON *>(buffer);
  buffer += sizeof(*pLogon);
  buffer += MoveStringToBuffer(pLogon, &pLogon->LogonDomainName, &buffer);
  buffer += MoveStringToBuffer(pLogon, &pLogon->UserName, &buffer);
  buffer += MoveStringToBuffer(pLogon, &pLogon->Password, &buffer);

  oSerialization->ulAuthenticationPackage = authenticationPackage;
  oSerialization->clsidCredentialProvider = kGuid;
  oSerialization->rgbSerialization = reinterpret_cast<BYTE *>(pLogon);
  oSerialization->cbSerialization = bufferLength;
  *oResponse = CPGSR_RETURN_CREDENTIAL_FINISHED;

  LsaDeregisterLogonProcess(lsa);
  return S_OK;
}

IFACEMETHODIMP AutoLogonCredential::GetStringValue(DWORD aFieldId, PWSTR *oValue) {
  UNREFERENCED_PARAMETER(aFieldId);
  UNREFERENCED_PARAMETER(oValue);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::GetSubmitButtonValue(DWORD aFieldId, DWORD *oAdjacentTo) {
  UNREFERENCED_PARAMETER(aFieldId);
  UNREFERENCED_PARAMETER(oAdjacentTo);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::ReportResult(NTSTATUS aStatus, NTSTATUS aSubstatus, PWSTR *oOptionalStatusText, CREDENTIAL_PROVIDER_STATUS_ICON *oOptionalStatusIcon) {
  UNREFERENCED_PARAMETER(aStatus);
  UNREFERENCED_PARAMETER(aSubstatus);
  UNREFERENCED_PARAMETER(oOptionalStatusText);
  UNREFERENCED_PARAMETER(oOptionalStatusIcon);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::SetCheckboxValue(DWORD aFieldId, BOOL aChecked) {
  UNREFERENCED_PARAMETER(aFieldId);
  UNREFERENCED_PARAMETER(aChecked);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::SetComboBoxSelectedValue(DWORD aFieldId, DWORD aSelectedItemIndex) {
  UNREFERENCED_PARAMETER(aFieldId);
  UNREFERENCED_PARAMETER(aSelectedItemIndex);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::SetDeselected() {
  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::SetSelected(BOOL *oAutoLogon) {
  UNREFERENCED_PARAMETER(oAutoLogon);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::SetStringValue(DWORD aFieldId, PCWSTR aValue) {
  UNREFERENCED_PARAMETER(aFieldId);
  UNREFERENCED_PARAMETER(aValue);

  return E_NOTIMPL;
}

IFACEMETHODIMP AutoLogonCredential::UnAdvise() {
  return E_NOTIMPL;
}

AutoLogonCredential::AutoLogonCredential() {
  DllAddRef();
}

AutoLogonCredential::~AutoLogonCredential() {
  DllRelease();
}
