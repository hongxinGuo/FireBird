#pragma once

#include"Accessory.h"
#include"RTData.h"
#include"SetDayLineToday.h"

using namespace std;
#include<array>

class CStockBasicInfo
{
public:
  CStockBasicInfo();
  ~CStockBasicInfo();
  void Reset(void);

  // ���ݿ����
  void StoreBasicInfo(CSetDayLine& psetDayLine);
  void StoreTempInfo(CSetDayLineToday& setDayLineToday);

  void UpdateStatus(CRTDataPtr pRTData);

  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wValue) noexcept { m_wMarket = wValue; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) noexcept { m_strStockCode = str; }
  CString GetStockName(void) noexcept { return m_strStockName; }
  void SetStockName(CString str) noexcept { m_strStockName = str; }

  long GetOffset(void) noexcept { return m_nOffsetInContainer; }
  void SetOffset(long lValue) noexcept { m_nOffsetInContainer = lValue; }
  long GetDayLineStartDay(void) noexcept { return m_lDayLineStartDay; }
  void SetDayLineStartDay(long lDay) noexcept { m_lDayLineStartDay = lDay; }
  long GetDayLineEndDay(void) noexcept { return m_lDayLineEndDay; }
  void SetDayLineEndDay(long lDay) noexcept { m_lDayLineEndDay = lDay; }
  long GetIPOStatus(void) noexcept { return m_lIPOed; }
  void SetIPOStatus(long lValue) noexcept { m_lIPOed = lValue; }

  // ����ʵʱ���ݣ���Ҫ����
  time_t GetTransactionTime(void) noexcept { return m_TransactionTime; }
  void SetTransactionTime(time_t time) noexcept { m_TransactionTime = time; }
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
  long GetUpDown(void) noexcept { return m_lUpDown; }
  void SetUpDown(long lValue) noexcept { m_lUpDown = lValue; }
  double GetUpDownRate(void) noexcept { return m_dUpDownRate; }
  void SetUpDownRate(double dValue) { m_dUpDownRate = dValue; }
  double GetChangeHandRate(void) noexcept { return m_dChangeHandRate; }
  void SetChangeHandRate(double dValue) noexcept { m_dChangeHandRate = dValue; }
  void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
  INT64 GetTotalValue(void) noexcept { return m_llTotalValue; }
  void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }
  INT64 GetCurrentValue(void) noexcept { return m_llCurrentValue; }
  long GetPBuy(int iIndex) { return m_lPBuy.at(iIndex); }
  long GetVBuy(int iIndex) { return m_lVBuy.at(iIndex); }
  long GetPSell(int iIndex) { return m_lPSell.at(iIndex); }
  long GetVSell(int iIndex) { return m_lVSell.at(iIndex); }
  void SetPBuy(int iIndex, long value) { m_lPBuy.at(iIndex) = value; }
  void SetVBuy(int iIndex, long value) { m_lVBuy.at(iIndex) = value; }
  void SetPSell(int iIndex, long value) { m_lPSell.at(iIndex) = value; }
  void SetVSell(int iIndex, long value) { m_lVSell.at(iIndex) = value; }
  double GetRelativeStrong(void) { return m_dRelativeStrong; }
  void SetRelativeStrong(double value) { m_dRelativeStrong = value; }

protected:
  WORD m_wMarket;	// 1���Ϻ��г���������ϸ�ࣩ��2�������г���������ϸ�ࣩ��3���Ϻ�ָ����4������ָ�������Ϻ����棻6��������С�壻
                  // 7���Ϻ�B�ɣ�8������B�ɣ�9���Ϻ��ƴ���; 10�����ڴ�ҵ�壻
                  // �Ϻ��г����õ�����ʾ�������г�����˫����ʾ��Ŀǰ��ʱ���ã��Ȳ鿴�漰����Щ������Ҫ�޸ġ�
  CString m_strStockCode; // ��Ʊ���롣��λ��ǰ��λΪ�г�ǰ׺������λΪ���ִ��롣��sh600601��sz000001
  CString m_strStockName; // ��Ʊ����
  CStringW m_strStockNameReadIn; // ����Ĺ�Ʊ���ƣ�UniCode��ʽ��Ŀǰ��δʹ�ã�
  long m_nOffsetInContainer;	// �������е�ƫ����
  long m_lDayLineStartDay;	// ����������ʼ�ա�����Ǵ���������ʷ����ʱ�õ�����ʼ�����գ�
  long m_lDayLineEndDay;	// �������ݸ����ա�����Ǵ���������ʷ����ʱ�õ��������գ�
  long m_lIPOed; // ͨ��������ʷ���߲�ѯ�����ֻ��ǰ׺��Ϣ��û��ʵ�����ݣ�����ȷ��û��ʵ�ʽ��ס�����������£�����ʵʱ���������ݣ�ֻ��Ϊ����ѡ�Ĭ�������Ϊ������
                 // δ���У���Ч��Ʊ���룩Ϊ__STOCK_NULL__������Ϊ__STOCK_IPOED__����ͨ��IPO����δ���л�����Ϊ__STOCK_DELISTED�����������δ���֣������Ժ���
  short	m_nHand;	// ÿ�ֹ���

  // ʵʱ����,��Ҫ���ϸ���
  time_t m_TransactionTime; // ʵʱ���ݽ���ʱ��
  long m_lLastClose; // ��0.001Ԫ�Ƶ����̼�
  long m_lOpen; // ��0.001Ԫ�ƵĿ��̼�
  long m_lHigh; // ��0.001Ԫ�Ƶ���߼�
  long m_lLow; // ��0.001Ԫ�Ƶ���ͼ�
  long m_lNew; // ��0.001Ԫ�Ƶ����¼�
  long m_lUpDown; // �ǵ�ֵ
  double m_dUpDownRate; // �ǵ���
  INT64 m_llVolume;	// ��1�ɼƵĳɽ���
  INT64 m_llAmount; // ��Ԫ�Ƶĳɽ����
  double m_dChangeHandRate; // ������
  INT64 m_llTotalValue;	// ����ֵ����λ����Ԫ
  INT64 m_llCurrentValue;	// ��ͨ��ֵ����λ����Ԫ
  array<long, 5> m_lPBuy;	// ���̼ۡ���λ��0.001Ԫ
  array<long, 5> m_lVBuy;	// ����������λ����
  array<long, 5> m_lPSell; // ���̼ۡ���λ��0.001Ԫ
  array<long, 5> m_lVSell; // ����������λ����
  double m_dRelativeStrong; // ��λ��1%
};
