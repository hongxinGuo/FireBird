// SetRealTimeData.h : CSetRealTimeData ������

#pragma once

// ���������� 2019��5��26��, 8:09

class CSetRealTimeData : public CRecordset
{
public:
	CSetRealTimeData(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSetRealTimeData)

// �ֶ�/��������

// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW)��ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת��)��

	long	m_Time;	//�ɽ�ʱ�䣬������㣬��1970��1��1����
  long  m_lMarket; // �Ϻ�Ϊ1������Ϊ2�� ������
	CString	m_StockName;	//��λ��Ʊ���
	CString	m_StockCode;	//��λ��Ʊ����
	double	m_LastClose;	//�������̼�
	double	m_Open;	//���쿪�̼�
	double	m_High;	//������߼�
	double	m_Low;	//������ͼ�
	double	m_CurrentPrice;	//���³ɽ��۸�
	long	m_Volume;	//Ŀǰ�ܳɽ�����
	double	m_Amount;	//��ǰ�ܳɽ����
	long	m_Stroke;	//��ǰ�ܳɽ�����
	double	m_PBuy5;	//���5
	long	m_VBuy5;	//���5�ϵ�����
	double	m_PBuy4;
	long	m_VBuy4;
	double	m_PBuy3;
	long	m_VBuy3;
	double	m_PBuy2;
	long	m_VBuy2;
	double	m_PBuy1;
	long	m_VBuy1;
	double	m_PSell1;
	long	m_VSell1;
	double	m_PSell2;
	long	m_VSell2;
	double	m_PSell3;
	long	m_VSell3;
	double	m_PSell4;
	long	m_VSell4;
	double	m_PSell5;	//����5
	long	m_VSell5;	//����5�ϵ�����

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


