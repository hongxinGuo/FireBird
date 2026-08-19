#pragma once
#include <afxdialogex.h>

class CDlgGetDate : public CDialogEx {
	DECLARE_DYNAMIC(CDlgGetDate)

	COleDateTime m_date;

public:
	CDlgGetDate(CWnd* pParent = NULL);
	virtual ~CDlgGetDate();

	BOOL OnInitDialog() override;
#ifdef AFX_DESIGN_TIME
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;

protected:
	DECLARE_MESSAGE_MAP()
};
