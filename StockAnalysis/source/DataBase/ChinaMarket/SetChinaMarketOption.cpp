#include"pch.h"

#include "SetChinaMarketOption.h"

IMPLEMENT_DYNAMIC(CSetChinaMarketOption, CVirtualRecordset)

CSetChinaMarketOption::CSetChinaMarketOption(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_RTDataServerIndex = 0; // Ĭ��ʹ������ʵʱ���ݷ�����
	m_RTDataInquiryTime = 400; // Ĭ�ϲ�ѯʱ��Ϊ400����
	m_nFields = 3;
}

void CSetChinaMarketOption::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[RealTimeDataServer]"), m_RTDataServerIndex);
	RFX_Long(pFX, _T("[RealTimeDataInquiryTime]"), m_RTDataInquiryTime);
}
/////////////////////////////////////////////////////////////////////////////
// CSetChinaMarketOption ���

#ifdef _DEBUG
void CSetChinaMarketOption::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetChinaMarketOption::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG