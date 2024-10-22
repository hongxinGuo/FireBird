// VirtualSetHistoryCandleBasic.cpp : CVirtualSetHistoryCandleBasic ���ʵ��

#include"pch.h"

#include "VirtualSetHistoryCandleBasic.h"

CVirtualSetHistoryCandleBasic::CVirtualSetHistoryCandleBasic(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 20;
}

void CVirtualSetHistoryCandleBasic::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[Date]"), m_Date);
	RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Text(pFX, _T("[DisplaySymbol]"), m_DisplaySymbol);
	RFX_Text(pFX, _T("[LastClose]"), m_LastClose);
	RFX_Text(pFX, _T("[Open]"), m_Open);
	RFX_Text(pFX, _T("[High]"), m_High);
	RFX_Text(pFX, _T("[Low]"), m_Low);
	RFX_Text(pFX, _T("[Close]"), m_Close);
	RFX_Text(pFX, _T("[Volume]"), m_Volume);
	RFX_Text(pFX, _T("[Amount]"), m_Amount);
	RFX_Text(pFX, _T("[UpAndDown]"), m_UpAndDown);
	RFX_Text(pFX, _T("[UpDownRate]"), m_UpDownRate);
	RFX_Text(pFX, _T("[ChangeHandRate]"), m_ChangeHandRate);
	RFX_Text(pFX, _T("[TotalValue]"), m_TotalValue);
	RFX_Text(pFX, _T("[CurrentValue]"), m_CurrentValue);
	RFX_Text(pFX, _T("[RelativeStrong]"), m_RS);	//��������г���ǿ��
	RFX_Text(pFX, _T("[RelativeStrongIndex]"), m_RSIndex);	//������г�ָ����ǿ��
	RFX_Text(pFX, _T("[RelativeStrongBackup]"), m_RSBackup);	//������г�ָ����ǿ��
}
