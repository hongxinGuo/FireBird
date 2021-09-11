#include"pch.h"

#include "SetChinaMarketOption.h"

IMPLEMENT_DYNAMIC(SetChinaMarketOption, CVirtualRecordset)

SetChinaMarketOption::SetChinaMarketOption(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_RTDataServerIndex = 0; // Ĭ��ʹ������ʵʱ���ݷ�����
	m_RTDataInquiryTime = 400; // Ĭ�ϲ�ѯʱ��Ϊ400����
	m_nFields = 2;
}

void SetChinaMarketOption::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[RealTimeDataServer]"), m_RTDataServerIndex);
	RFX_Long(pFX, _T("[RealTimeDataInquiryTime]"), m_RTDataInquiryTime);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode ���

#ifdef _DEBUG
void SetChinaMarketOption::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void SetChinaMarketOption::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG