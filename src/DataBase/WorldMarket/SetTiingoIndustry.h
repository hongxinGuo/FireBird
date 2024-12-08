// CSetTiingoIndustry.h : CSetTiingoIndustry ������
#pragma once

import FireBird.Set.VirtualRecordset;

class CSetTiingoIndustry : public CVirtualRecordset {
public:
	CSetTiingoIndustry(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_industry"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };  // �����ã��Զ����ɡ�
	CString m_Industry{ _T("") };
	CString m_Sector{ _T("") };

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
