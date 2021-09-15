// VirtualSetHistoryCandle.h : CVirtualSetHistoryCandle ������
//
// ���е���ʷ���ݿ⣬��׼��ʹ�ô���Ϊ���࣬�Լ򻯴洢���̡�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualRecordset.h"

class CVirtualSetHistoryCandle : public CVirtualRecordset
{
public:
	CVirtualSetHistoryCandle(CString strSchema, CString strTable, CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CVirtualSetHistoryCandle)

	// �ֶ�/��������

	// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
	// �������͵� CStringW)��ʵ���������͡�
	//  ����Ϊ��ֹ ODBC ��������ִ�п���
	// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
	// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
	// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
	// ��ͬʱ֧�� Unicode ����Щת��)��

	long m_ID;  // �����ã��Զ����ɡ�
	long m_Date;	//�ɽ�ʱ�䣬�����ڱ�ʾ����20090222��2009��2��22��
	CString m_Exchange; // ���������ƣ���Ʊ�ǽ���������λ���룬Forex��Crypto���ǽ��������ƣ�
	CString m_Symbol;	//����
	CStringW m_DisplaySymbol;	//��Ʊ���ƣ��й���Ʊ�����������ơ�   ����������CStringW(Unicode)��ʽ��������Щ�����޷���ʾ���������ݿ��޷��洢
	CString m_LastClose;	//�������̼ۡ���λ��0.001Ԫ
	CString m_Open;	//���쿪�̼�
	CString m_High;	//������߼�
	CString m_Low;	//������ͼ�
	CString m_Close;	//�������̼�
	CString m_Volume;	//�����ܳɽ���������λ���ɡ�
	CString m_Amount;	//�����ܳɽ�����λ��Ԫ
	CString m_UpAndDown;	//�����ǵ�
	CString m_UpDownRate; // �����ǵ����ȣ���λ1%��
	CString m_ChangeHandRate; // ���ջ�����
	CString m_TotalValue;	//����ֵ����λ��Ԫ
	CString m_CurrentValue;	//��ͨ��ֵ����λ��Ԫ
	CString m_RS;	//��������г���ǿ��
	CString m_RSIndex;	//������г�ָ����ǿ��
	CString m_RSBackup;	//��������

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