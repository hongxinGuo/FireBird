///////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݰ�, ������ʵʱ��Ʊ����ӿ�(hq.sinajs.cn\list=)����Ѷʵʱ��Ʊ����ӿ�Ϊ��׼
//
// Ϊ�˼��㷽�㣬���̼۵�ʹ�÷Ŵ�1000������������ʾ��ʵ���ݣ���������Ҳ��ˡ��ڴ洢ʱ��ʹ��DECIMAL(10,3)����¼�������Ƚ����ף�������ת���������͡�
// �ҵ������ɽ����ͳɽ�����ֵ�Ȳ���ʵ����ֵ��
//
// ����ʵʱ������ʽ��hq.sinajs.cn\list=sh600001,sz002389
// var hq_str_sh601006=��������·,27.55,27.25,26.91,27.55,26.20,26.91,26.92,
//                     22114263,589824680,4695,26.91,57590,26.90,14700,26.89,14300,
//                     26.88,15100,26.87,3100,26.92,8900,26.93,14230,26.94,25150,26.95,15220,26.96,2008-01-11,15:05:32��;
//
// ����ַ������������ƴ����һ�𣬲�ͬ����������ö��Ÿ����ˣ����ճ���Ա��˼·��˳��Ŵ�0��ʼ��
// 0����������·������Ʊ���֣�
// 1����27.55�壬���տ��̼ۣ�
// 2����27.25�壬�������̼ۣ�
// 3����26.91�壬��ǰ�۸�
// 4����27.55�壬������߼ۣ�
// 5����26.20�壬������ͼۣ�
// 6����26.91�壬����ۣ�������һ�����ۣ�
// 7����26.92�壬�����ۣ�������һ�����ۣ�
// 8����22114263�壬�ɽ��Ĺ�Ʊ�������ڹ�Ʊ������һ�ٹ�Ϊ������λ��������ʹ��ʱ��ͨ���Ѹ�ֵ����һ�٣�
// 9����589824680�壬�ɽ�����λΪ��Ԫ����Ϊ��һĿ��Ȼ��ͨ���ԡ���Ԫ��Ϊ�ɽ����ĵ�λ������ͨ���Ѹ�ֵ����һ��
// 10����4695�壬����һ������4695�ɣ���47�֣�
// 11����26.91�壬����һ�����ۣ�
// 12����57590�壬�������
// 13����26.90�壬�������
// 14����14700�壬��������
// 15����26.89�壬��������
// 16����14300�壬�����ġ�
// 17����26.88�壬�����ġ�
// 18����15100�壬�����塱
// 19����26.87�壬�����塱
// 20����3100�壬����һ���걨3100�ɣ���31�֣�
// 21����26.92�壬����һ������
// (22, 23), (24, 25), (26, 27), (28, 29)�ֱ�Ϊ���������������ĵ������
// 30����2008 - 01 - 11�壬���ڣ�
// 31����15:05 : 32�壬ʱ�䣻
//
//
//
// ��Ѷ�������飬�������ݽӿڣ�http://qt.gtimg.cn/q=sz002818,sh600001
// v_sz000001 = "51~ƽ������~000001~15.59~15.90~15.75~1046363~518391~527971~
//              15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~
//              20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~
//              15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~0.73~-12617~15.73~9.82~12.19~
//              ~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82";
//
// 0: 51����1��  1 ��A�� 51 ��A
// 1 : ƽ������  ����
// 2 : 000001   ����
// 3 : 15.59    �ּ�
// 4 : 15.90    ����
// 5 : 15.75    ��
// 6 : 1046363  �ɽ������֣�
// 7 : 518391   ����
// 8 : 527971   ���� �������һ�н�����
// 9 : 15.58    ��һ
// 10 : 2365    ��һ�����֣�
// 11 - 18 : ��� - ����
// 19 : 15.59   ��һ
// 20 : 661     ��һ��
// 21 - 28 : ���� - ����
// 29 : ���գ�   �����ʳɽ����������˶�Ϊ�գ�����ڶ��н�����
// 30 : 20190930154003  ʱ��
// 31 : 0.31    �ǵ�
// 32 : 1.95    �ǵ� %
// 33 : 15.89   ���
// 34 : 15.57   ���
// 35 : 15.59/1046363/1645828527    �۸� / �ɽ������֣� / �ɽ��Ԫ��
// 36 : 1046363 �ɽ������֣�
// 37 : 164583  �ɽ����
// 38 : 0.54    ������
// 39 : 11.27   ��ӯ�ʣ�TTM��
// 40 : ��      ����      (��������н�����
// 41 : 15.89   ���
// 42 : 15.57   ���
// 43 : 2.01    ���
// 44 : 3025.38 ��ͨ��ֵ���ڣ�
// 45 : 3025.38 ����ֵ���ڣ�
// 46 : 1.15    �о���
// 47 : 17.49   ��ͣ��
// 48 : 14.31   ��ͣ��
// 49 ��0.73    ����
// 50 ��-12617  ί��
// 51 ��15.73   ����
// 52 �� 9.82    ��̬��ӯ��
// 53 �� 12.19   ��̬��ӯ��   ����������н�����
//
// �����ֶκ��岻����δ�ҵ��ٷ�˵����
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

  // ���ַ�ָ�봦����������ʽ���ݡ���ָ�뿪ʼ��Ϊvar hq_str_s,����\n(�س�)����
  bool CStockRTData::ReadSinaData(char*& pCurrentPos, long& lTotalRead);

  // ���ַ�ָ�봦������Ѷ��ʽ���ݡ���ָ�뿪ʼ��Ϊv_s,����\n(�س�)����
  bool CStockRTData::ReadTengxunData(char*& pCurrentPos, long& lTotalRead);

  // �Ƚϴ�С�����
  bool operator < (const CStockRTData& rtData) const {
    return m_time < rtData.m_time;
  }

  // �Ƚϴ�С�����
  bool operator < (const CStockRTDataPtr rtDataPtr) const {
    return m_time < rtDataPtr->m_time;
  }

public:
  // ��ȡ����ʵʱ���ݺ���
  bool ReadSinaOneValue(char*& pCurrentPos, long& lReturnValue, long& lTotalRead); // ��file�ж���һ��������
  bool ReadSinaOneValue(char*& pCurrentPos, INT64& llReturnValue, long& lTotalRead); // ��file�ж���һ��������
  bool ReadSinaOneValue(char*& pCurrentPos, char* buffer, long& lTotalRead); // ��file�ж���һ���������ݣ�����ַ�Ϊ��������
  // ��file�ж���һ���������ݣ��������еĶ��ţ�����ַ�Ϊ��������
  bool ReadSinaOneValueExceptPeriod(char*& pCurrentPos, long& lReturnValue, long& lCounter);
  bool ReadSinaOneValueExceptPeriod(char*& pCurrentPos, char* buffer, long& lCounter);
  
  // ��ȡ��Ѷʵʱ���ݺ���
  bool ReadTengxunOneValue(char*& pCurrentPos, long& lReturnValue, long& lTotalRead); // ��file�ж���һ��������
  bool ReadTengxunOneValue(char*& pCurrentPos, INT64& llReturnValue, long& lTotalRead); // ��file�ж���һ��INT64����
  bool ReadTengxunOneValue(char*& pCurrentPos, double& dReturnValue, long& lTotalRead); // ��file�ж���һ��������
  bool ReadTengxunOneValue(char*& pCurrentPos, char* buffer, long& lTotalRead); // ��file�ж���һ���������ݣ�����ַ�Ϊ��~����

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
  INT64 GetAmount(void) noexcept { return m_llAmount; }
  void SetAmount(INT64 llValue) noexcept { m_llAmount = llValue; }
  INT64 GetVolume(void) noexcept { return m_llVolume; }
  void SetVolume(INT64 llValue) noexcept { m_llVolume = llValue; }
  void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
  INT64 GetTotalValue(void) noexcept { return m_llTotalValue; }
  void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }
  INT64 GetCurrentValue(void) noexcept { return m_llCurrentValue; }
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
  WORD	    m_wMarket;				      // �г���ʾ.ĿǰΪ__SHANGHAI_MARKET__��__SHENZHEN_MARKET__�����г�
  CString		m_strStockCode;					// ֤ȯ����, sh600001��sz002389��
  int       m_iStockCode;           // ֤ȯ���루���֣���600001, 002389
  CString   m_strStockName;         // ֤ȯ����
  long  	  m_lOpen;								// ���տ��̡���λ��0.001Ԫ
  long  	  m_lLastClose;				    // �������̡���λ��0.001Ԫ
  long  	  m_lNew;									// �������¡���λ��0.001Ԫ
  long  	  m_lHigh;								// ������ߡ���λ��0.001Ԫ
  long  	  m_lLow;									// ������͡���λ��0.001Ԫ
  long      m_lBuy;                 // ����ۡ���λ��0.001Ԫ
  long      m_lSell;                // �����ۡ���λ��0.001Ԫ
  INT64	    m_llVolume;							// �ܳɽ�������λ����
  INT64     m_llAmount;							// �ܳɽ�����λ��Ԫ
  INT64	    m_llTotalValue;					// ����ֵ����λ����Ԫ
  INT64     m_llCurrentValue;				// ��ͨ��ֵ����λ����Ԫ
  array<long, 5>  	  m_lPBuy;			// ���̼�1--5����λ��0.001Ԫ
  array<long, 5> 		  m_lVBuy;			// ������1--5����λ����
  array<long, 5>  	  m_lPSell;			// ���̼�1--5����λ��0.001Ԫ
  array<long, 5>  		m_lVSell;			// ������1--5����λ: ��

// �Ǵ洢����
  bool      m_fActive;              // ����Ʊ�Ƿ����ʵʱ����
};
