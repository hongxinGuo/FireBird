// CSetNaicsIndustry.h : CSetNaicsIndustry ������

#pragma once

import FireBird.Set.VirtualRecordset;

class CSetNaicsIndustry : public CVirtualRecordset {
public:
	CSetNaicsIndustry(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("naics_industry"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };  // �����ã��Զ����ɡ�
	CString m_Naics{ _T("") };
	CString m_NationalIndustry{ _T("") };
	CString m_Sector{ _T("") };
	CString m_SubSector{ _T("") };

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
