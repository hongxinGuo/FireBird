#pragma once

#include <afxpropertygridctrl.h>

class CGridProperty : public CMFCPropertyGridProperty {
public:
	CGridProperty(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE) : CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList) {}
	CGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = nullptr, DWORD_PTR dwData = 0,
								LPCTSTR lpszEditMask = nullptr, LPCTSTR lpszEditTemplate = nullptr, LPCTSTR lpszValidChars = nullptr) :
		CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars) {
	}

	~CGridProperty() override = default;

public:
	void SetValue(const string& strValue) { CMFCPropertyGridProperty::SetValue(static_cast<_variant_t>(CA2W(strValue.c_str()))); }
};
