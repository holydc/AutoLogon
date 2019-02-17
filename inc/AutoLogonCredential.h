#pragma once

#include <credentialprovider.h>

class AutoLogonCredential : public ICredentialProviderCredential {
public:
  // IUnknown
  IFACEMETHODIMP_(ULONG) AddRef() override;
  IFACEMETHODIMP QueryInterface(REFIID aIid, void **oInterface) override;
  IFACEMETHODIMP_(ULONG) Release() override;

  // ICredentialProviderCredential
  IFACEMETHODIMP Advise(ICredentialProviderCredentialEvents *aCredentialEvents) override;
  IFACEMETHODIMP CommandLinkClicked(DWORD aFieldId) override;
  IFACEMETHODIMP GetBitmapValue(DWORD aFieldId, HBITMAP *oValue) override;
  IFACEMETHODIMP GetCheckboxValue(DWORD aFieldId, BOOL *oChecked, PWSTR *oValue) override;
  IFACEMETHODIMP GetComboBoxValueAt(DWORD aFieldId, DWORD aItemIndex, PWSTR *oValue) override;
  IFACEMETHODIMP GetComboBoxValueCount(DWORD aFieldId, DWORD *oNumItems, DWORD *oSelectedItemIndex) override;
  IFACEMETHODIMP GetFieldState(DWORD aFieldId, CREDENTIAL_PROVIDER_FIELD_STATE *oFieldState, CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE *oFieldInteractiveState) override;
  IFACEMETHODIMP GetSerialization(CREDENTIAL_PROVIDER_GET_SERIALIZATION_RESPONSE *oResponse, CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION *oSerialization, PWSTR *oOptionalStatusText, CREDENTIAL_PROVIDER_STATUS_ICON *oOptionalStatusIcon) override;
  IFACEMETHODIMP GetStringValue(DWORD aFieldId, PWSTR *oValue) override;
  IFACEMETHODIMP GetSubmitButtonValue(DWORD aFieldId, DWORD *oAdjacentTo) override;
  IFACEMETHODIMP ReportResult(NTSTATUS aStatus, NTSTATUS aSubstatus, PWSTR *oOptionalStatusText, CREDENTIAL_PROVIDER_STATUS_ICON *oOptionalStatusIcon) override;
  IFACEMETHODIMP SetCheckboxValue(DWORD aFieldId, BOOL aChecked) override;
  IFACEMETHODIMP SetComboBoxSelectedValue(DWORD aFieldId, DWORD aSelectedItemIndex) override;
  IFACEMETHODIMP SetDeselected() override;
  IFACEMETHODIMP SetSelected(BOOL *oAutoLogon) override;
  IFACEMETHODIMP SetStringValue(DWORD aFieldId, PCWSTR aValue) override;
  IFACEMETHODIMP UnAdvise() override;

  // AutoLogonCredential
  AutoLogonCredential();

private:
  ULONG mRefCnt = 1;

  ~AutoLogonCredential();
}; // class AutoLogonCredential
