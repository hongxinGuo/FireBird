///////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݰ�, ������ʵʱ��Ʊ����ӿ�(hq.sinajs.cn\list=)Ϊ��׼
//    
// Ϊ�˼��㷽�㣬���̼۵�ʹ�÷Ŵ�1000������������ʾ��ʵ���ݣ���������Ҳ��ˡ��ڴ洢ʱ��ʹ��DECIMAL(10,3)����¼�������Ƚ����ף�������ת���������͡�
// �ҵ������ɽ����ͳɽ�����ֵ�Ȳ���ʵ����ֵ��
//
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"stdafx.h"
#include"afxinet.h"

using namespace std;
#include<memory>

class CStockRTData : public CObject {
public :
// ��ʼ��
	CStockRTData( void );

	void ReSetData( void );
	bool SetData( CStockRTData & data );
	bool SetDataAll( CStockRTData & data );

  // ���ַ�ָ�봦�������ݡ���ָ�뿪ʼ��Ϊvar hq_str_s,����\n(�س�)����
  bool CStockRTData::ReadData(char *& pCurrentPos, long & iTotalRead);

public:
  bool ReadOneValue(char *& pCurrentPos, char * buffer, long & lCounter); // ��file�ж���һ���������ݣ�����ַ�Ϊ��������
  // ��file�ж���һ���������ݣ��������еĶ��ţ�����ַ�Ϊ��������
  bool ReadOneValueExceptperiod(char *& pCurrentPos, char * buffer, long & lCounter); 

public :


#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif


// ����
public:
	// Serialized data
	time_t		m_time;									// ���׷���ʱ��ʱ��
	WORD	    m_wMarket;				      // �г���ʾ
	CString		m_strStockCode;					// ֤ȯ����, 
  int       m_iStockCode;           // ֤ȯ���루���֣�
  CString   m_strStockName;         // ֤ȯ����
	long  	  m_lOpen;								// ���տ��̡���λ��Ԫ
	long  	  m_lLastClose;				    // �������̡���λ��Ԫ
	long  	  m_lNew;									// �������¡���λ��Ԫ
	long  	  m_lHigh;								// ������ߡ���λ��Ԫ
	long  	  m_lLow;									// ������͡���λ��Ԫ
  long      m_lBuy;                // ����ۡ���λ��Ԫ
  long      m_lSell;               // �����ۡ���λ��Ԫ
	INT64	    m_lVolume;							// �ܳɽ�������λ����
	INT64     m_lAmount;							// �ܳɽ�����λ��Ԫ
	long  	  m_lPBuy[5];							// ���̼�1 -- 5����λ��Ԫ
	long 		  m_lVBuy[5];							// ������1 -- 5����λ����
	long  	  m_lPSell[5];						// ���̼�1 -- 5����λ��Ԫ
	long  		m_lVSell[5];						// ������1 -- 5����λ: ��

// �Ǵ洢����
  bool      m_fActive;              // ����Ʊ�Ƿ����ʵʱ����

};

typedef shared_ptr<CStockRTData> CStockRTDataPtr;

