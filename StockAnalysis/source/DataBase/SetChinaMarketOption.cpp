#include"pch.h"
#include"accessory.h"

#include "SetChinaMarketOption.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(SetChinaMarketOption, CRecordset)

SetChinaMarketOption::SetChinaMarketOption(CDatabase* pdb)
	: CRecordset(pdb) {
	m_ID = 0;
	m_RTDataServerIndex = 0; // Ĭ��ʹ������ʵʱ���ݷ�����
	m_RTDataInquiryTime = 400; // Ĭ�ϲ�ѯʱ��Ϊ400����
	m_nFields = 2;
}
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û�������֤��
CString SetChinaMarketOption::GetDefaultConnect() {
	return GetChinaMarketSchemaConnect();
}

CString SetChinaMarketOption::GetDefaultSQL() {
	return _T("[option_china_stockmarket]");
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
	CRecordset::AssertValid();
}

void SetChinaMarketOption::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG