#include"pch.h"
#include "DlgGetDate.h"

#include "resource.h"

IMPLEMENT_DYNAMIC(CDlgGetDate, CDialogEx)

BEGIN_MESSAGE_MAP(CDlgGetDate, CDialogEx)
END_MESSAGE_MAP()

CDlgGetDate::CDlgGetDate(CWnd* pParent) : CDialogEx(IDD_DIALOG_GET_DATE2, pParent) {
	m_date = COleDateTime::GetCurrentTime();
}

CDlgGetDate::~CDlgGetDate() {
}

BOOL CDlgGetDate::OnInitDialog() {
	return CDialogEx::OnInitDialog();
}

void CDlgGetDate::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);

	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_date);
}
