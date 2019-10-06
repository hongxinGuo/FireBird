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
#include<array>

class CStockRTData;

typedef shared_ptr<CStockRTData> CStockRTDataPtr;

class CStockRTData : public CObject {
public:
  // ��ʼ��
  CStockRTData(void);

  void Reset(void);
  bool SetData(CStockRTData& data);
  bool SetDataAll(CStockRTData& data);

  // ���ַ�ָ�봦����������ʽ���ݡ���ָ�뿪ʼ��Ϊvar hq_str_s,����\n(�س�)����
  bool CStockRTData::ReadSinaData(char*& pCurrentPos, long& iTotalRead);

  // ���ַ�ָ�봦������Ѷ��ʽ���ݡ���ָ�뿪ʼ��Ϊvar hq_str_s,����\n(�س�)����
  bool CStockRTData::ReadTengxunData(char*& pCurrentPos, long& iTotalRead);

  // �Ƚϴ�С�����
  bool operator < (const CStockRTData& rtData) const {
    return m_time < rtData.m_time;
  }

  // �Ƚϴ�С�����
  bool operator < (const CStockRTDataPtr rtDataPtr) const {
    return m_time < rtDataPtr->m_time;
  }

public:
  bool ReadOneValue(char*& pCurrentPos, char* buffer, long& lCounter); // ��file�ж���һ���������ݣ�����ַ�Ϊ��������
  // ��file�ж���һ���������ݣ��������еĶ��ţ�����ַ�Ϊ��������
  bool ReadOneValueExceptperiod(char*& pCurrentPos, char* buffer, long& lCounter);

public:
  time_t GetTransactionTime(void) noexcept { return m_time; }
  void SetTransactionTime(time_t time) noexcept { m_time = time; }
  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wMarket) noexcept { m_wMarket = wMarket; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) { m_strStockCode = str; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }
  int GetCode(void) noexcept { return m_iStockCode; }
  void SetCode(int iStockCode) noexcept { m_iStockCode = iStockCode; }
  long GetLastClose(void) noexcept { return m_lLastClose; }
  void SetLastClose(long lValue) noexcept { m_lLastClose = lValue; }
  long GetOpen(void) noexcept { return m_lOpen; }
  void SetOpen(long lValue) noexcept { m_lOpen = lValue; }
  long GetHigh(void) noexcept { return m_lHigh; }
  void SetHigh(long lValue) noexcept { m_lHigh = lValue; }
  long GetLow(void) noexcept { return m_lLow; }
  void SetLow(long lValue) noexcept { m_lLow = lValue; }
  long GetNew(void) noexcept { return m_lNew; }
  void SetNew(long lValue) noexcept { m_lNew = lValue; }
  INT64 GetAmount(void) noexcept { return m_lAmount; }
  void SetAmount(INT64 llValue) noexcept { m_lAmount = llValue; }
  INT64 GetVolume(void) noexcept { return m_lVolume; }
  void SetVolume(INT64 llValue) noexcept { m_lVolume = llValue; }
  long GetBuy(void) noexcept { return m_lBuy; }
  void SetBuy(long lValue) noexcept { m_lBuy = lValue; }
  long GetSell(void) { return m_lSell; }
  void SetSell(long lValue) { m_lSell = lValue; }
  long GetPBuy(int iIndex) { return m_lPBuy.at(iIndex); }
  void SetPBuy(int iIndex, long lValue) { m_lPBuy.at(iIndex) = lValue; }
  long GetVBuy(int iIndex) { return m_lVBuy.at(iIndex); }
  void SetVBuy(int iIndex, long lValue) { m_lVBuy.at(iIndex) = lValue; }
  long GetPSell(int iIndex) { return m_lPSell.at(iIndex); }
  void SetPSell(int iIndex, long lValue) { m_lPSell.at(iIndex) = lValue; }
  long GetVSell(int iIndex) { return m_lVSell.at(iIndex); }
  void SetVSell(int iIndex, long lValue) { m_lVSell.at(iIndex) = lValue; }

  bool IsActive(void) noexcept { return m_fActive; }
  //void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif

  // ����
public:

protected:
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
  array<long, 5>  	  m_lPBuy;							// ���̼�1 -- 5����λ��Ԫ
  array<long, 5> 		  m_lVBuy;							// ������1 -- 5����λ����
  array<long, 5>  	  m_lPSell;						// ���̼�1 -- 5����λ��Ԫ
  array<long, 5>  		m_lVSell;						// ������1 -- 5����λ: ��

// �Ǵ洢����
  bool      m_fActive;              // ����Ʊ�Ƿ����ʵʱ����
};
