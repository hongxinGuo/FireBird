// VirtualSetHistoryCandleBasic.h : CVirtualSetHistoryCandleBasic ������
//
// ���е���ʷ���ݿ�Ļ������ݱ���׼��ʹ�ô���Ϊ���࣬�Լ򻯴洢���̡�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualRecordset.h"

class CVirtualSetHistoryCandleBasic : public CVirtualRecordset {
public:
	CVirtualSetHistoryCandleBasic(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID{ 0 };  // �����ã��Զ����ɡ�
	long m_Date{ 0 };	//�ɽ�ʱ�䣬�����ڱ�ʾ����20090222��2009��2��22��
	CString m_Exchange{ _T("") }; // ���������ƣ���Ʊ�ǽ���������λ���룬Forex��Crypto���ǽ��������ƣ�
	CString m_Symbol{ _T("") };	//����
	CStringW m_DisplaySymbol{ _T("") };	//��Ʊ���ƣ��й���Ʊ�����������ơ�   ����������CStringW(Unicode)��ʽ��������Щ�����޷���ʾ���������ݿ��޷��洢
	CString m_LastClose{ _T("") };	//�������̼ۡ���λ��0.001Ԫ
	CString m_Open{ _T("") };	//���쿪�̼�
	CString m_High{ _T("") };	//������߼�
	CString m_Low{ _T("") };	//������ͼ�
	CString m_Close{ _T("") };	//�������̼�
	CString m_Volume{ _T("") };	//�����ܳɽ���������λ���ɡ�
	CString m_Amount{ _T("") };	//�����ܳɽ�����λ��Ԫ
	CString m_dividend{ _T("") }; // ���չ�Ϣ����λ0.001Ԫ
	CString m_splitFactor{ _T("") }; // ���ղ�֡� ��λδ������
	CString m_UpAndDown{ _T("") };	//�����ǵ�
	CString m_UpDownRate{ _T("") }; // �����ǵ����ȣ���λ1%��
	CString m_ChangeHandRate{ _T("") }; // ���ջ�����
	CString m_TotalValue{ _T("") };	//����ֵ����λ��Ԫ
	CString m_CurrentValue{ _T("") };	//��ͨ��ֵ����λ��Ԫ
	CString m_RS{ _T("") };	//��������г���ǿ��
	CString m_RSIndex{ _T("") };	//������г�ָ����ǿ��
	CString m_RSBackup{ _T("") };	//��������

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
