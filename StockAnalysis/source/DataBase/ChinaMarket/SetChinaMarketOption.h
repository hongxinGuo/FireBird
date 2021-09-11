#pragma once

#include"VirtualRecordset.h"

class SetChinaMarketOption : public CVirtualRecordset
{
public:
	SetChinaMarketOption(CString strSchema = _T("ChinaMarket"), CString strTable = _T("option_china_stockmarket"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(SetChinaMarketOption)

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��
	long m_ID;
	long m_RTDataServerIndex; // ʵʱ���ݷ�����
	long m_RTDataInquiryTime; // ����ʵʱ������ѯ��ѯʱ�䡣Ĭ��Ϊ400����

	// ��д
		// �����ɵ��麯����д
public:
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
