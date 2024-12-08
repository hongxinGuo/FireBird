#pragma once
import FireBird.Set.VirtualRecordset;

class CSetOption : public CVirtualRecordset {
public:
	CSetOption(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("options"), CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��
	long m_ID{ 0 };
	long m_RSEndDate{ 0 };
	long m_RSStartDate{ 0 };
	long m_LastLoginDate{ 0 };
	long m_LastLoginTime{ 0 };
	long m_UpdatedDateFor10DaysRS1{ 0 };
	long m_UpdatedDateFor10DaysRS2{ 0 };
	long m_UpdatedDateFor10DaysRS{ 0 };

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
