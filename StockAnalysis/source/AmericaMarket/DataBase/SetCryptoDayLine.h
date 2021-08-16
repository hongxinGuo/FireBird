// CSetCryptoDayLine.h : CSetCryptoDayLine ������

#pragma once

#include"afxdb.h"

// ���������� 2019��6��2��, 5:10

class CSetCryptoDayLine : public CRecordset
{
public:
	CSetCryptoDayLine(CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetCryptoDayLine)

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
	CString m_Exchange;	//�г����Ϻ������ڣ�
	CString m_Symbol;	//
	CString m_LastClose;	//�������̼ۡ���λ��0.001Ԫ
	CString m_Open;	//���쿪�̼�
	CString m_High;	//������߼�
	CString m_Low;	//������ͼ�
	CString m_Close;	//�������̼�
	CString m_Volume;	//�����ܳɽ���������λ���ɡ�

// ��д
	// �����ɵ��麯����д
public:
	virtual CString GetDefaultConnect();	// Ĭ�������ַ���

	virtual CString GetDefaultSQL(); 	// ��¼����Ĭ�� SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
