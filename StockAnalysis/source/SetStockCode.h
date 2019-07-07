// SetStockCode.h : CSetStockCode ������

#pragma once

// ���������� 2019��5��26��, 8:14

class CSetStockCode : public CRecordset
{
public:
	CSetStockCode(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSetStockCode)

// �ֶ�/��������

// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW)��ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת��)��

  long m_ID;
	long	m_Counter;
	int	m_StockType;
	CString	m_StockCode;
	CString	m_StockName;
  long m_DayLineStartDay;   //  ���ߵ���ʼ��
	long m_DayLineEndDay;     // ��������ʷ���߷������ϵõ���������
  long m_NewestDayLineDay;  // ִ���������ߴ���õ��������գ����������ʵʱ���ݣ��������Ҫ����m_DayLineEndDay
	long m_IPOed;
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


