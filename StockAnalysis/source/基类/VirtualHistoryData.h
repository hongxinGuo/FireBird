////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ʷ���ݵĻ��ࡣ
// �����࣬����������ʵ����
// Ϊ�˱�֤���Գɹ����ʶ�û������Ϊ�����ࡣ
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"Accessory.h"

using namespace std;
#include<memory>

class CVirtualHistoryData : public CObject {
public:
  CVirtualHistoryData();
  ~CVirtualHistoryData();
  void Reset(void); // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

public:
  virtual int GetRatio(void) = 0; // �˺���Ӧ������Ϊ���麯������������Ҫ���Դ˻��࣬�ʶ���ִ���塣�о���������Ϊ���麯��Ϊ�ѡ�
  virtual void SetRatio(int iRatio = 1) = 0; // �˺�����Ҫ�̳������ʵ��

  long GetFormatedMarketDate(void) noexcept { return m_lDate; }
  void SetDate(long lDate) noexcept { m_lDate = lDate; }
  time_t GetFormatedMarketTime(void) noexcept { return m_time; }
  void SetTime(time_t t) noexcept { m_time = t; }
  CString GetExchange(void) { return m_strExchange; }
  void SetExchange(CString strExchange) { m_strExchange = strExchange; }
  CString GetStockSymbol(void) { return m_strStockSymbol; }
  void SetStockSymbol(CString str) { m_strStockSymbol = str; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }

  long GetLastClose(void) noexcept { return m_lLastClose; }
  void SetLastClose(long lValue) noexcept { m_lLastClose = lValue; }
  long GetOpen(void) noexcept { return m_lOpen; }
  void SetOpen(long lValue) noexcept { m_lOpen = lValue; }
  long GetHigh(void) noexcept { return m_lHigh; }
  void SetHigh(long lValue) noexcept { m_lHigh = lValue; }
  long GetLow(void) noexcept { return m_lLow; }
  void SetLow(const char* buffer) noexcept { m_lLow = static_cast<long>(atof(buffer)); }
  void SetLow(long lValue) noexcept { m_lLow = lValue; }
  long GetClose(void) noexcept { return m_lClose; }
  void SetClose(const char* buffer) noexcept { m_lClose = static_cast<long>(atof(buffer)); }
  void SetClose(long lValue) noexcept { m_lClose = lValue; }
  double GetUpDown(void) noexcept { return m_dUpDown; }
  void SetUpDown(const char* buffer) noexcept { m_dUpDown = atof(buffer); }
  void SetUpDown(double dValue) noexcept { m_dUpDown = dValue; }
  double GetUpDownRate(void) noexcept { return m_dUpDownRate; }
  void SetUpDownRate(const char* buffer) noexcept { m_dUpDownRate = atof(buffer); }
  void SetUpDownRate(double dValue) noexcept { m_dUpDownRate = dValue; }
  double GetChangeHandRate(void) noexcept { return m_dChangeHandRate; }
  void SetChangeHandRate(const char* buffer) noexcept { m_dChangeHandRate = atof(buffer); }
  void SetChangeHandRate(double dValue) noexcept { m_dChangeHandRate = dValue; }
  INT64 GetVolume(void) noexcept { return m_llVolume; }
  void SetVolume(const char* buffer) noexcept { m_llVolume = static_cast<INT64>(atof(buffer)); }
  void SetVolume(INT64 llValue) noexcept { m_llVolume = llValue; }
  INT64 GetAmount(void) noexcept { return m_llAmount; }
  void SetAmount(const char* buffer) noexcept { m_llAmount = static_cast<INT64>(atof(buffer)); }
  void SetAmount(INT64 llValue) noexcept { m_llAmount = llValue; }
  INT64 GetTotalValue(void) noexcept { return m_llTotalValue; }
  void SetTotalValue(const char* buffer) noexcept { m_llTotalValue = static_cast<INT64>(atof(buffer)); }
  void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
  INT64 GetCurrentValue(void) noexcept { return m_llCurrentValue; }
  void SetCurrentValue(const char* buffer) noexcept { m_llCurrentValue = static_cast<INT64>(atof(buffer)); }
  void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }

protected:
  // need to save
  long m_lDate; // ����(YYYYMMDD)
  time_t m_time;
  CString m_strExchange;
  CString m_strStockSymbol;
  CString m_strStockName;

  // �ڲ�ͬ���г��У��������ݻ�Ŵ����ɱ����ٱ���ǧ���ȣ���ȡ���ڼ۸�ʹ�ü���С����
  long m_lLastClose; // ǰ���̡���λ��0.001Ԫ
  long m_lOpen; // ���̼�
  long m_lHigh; // ��߼�
  long m_lLow; // ��ͼ�
  long m_lClose; // ���̼�

  // ������ֵ��ʵ��ֵ
  double m_dUpDown;	// �ǵ���
  double m_dUpDownRate;
  double m_dChangeHandRate;	// ������
  INT64 m_llVolume;	// �ɽ���,��λ:��
  INT64	m_llAmount;	// �ɽ����,��λ:Ԫ/��Ԫ�����̣�
  INT64	m_llTotalValue;	// ����ֵ����λ����Ԫ
  INT64 m_llCurrentValue; // ��ͨ��ֵ����λ����Ԫ

private:
  // ����ĸ��̳��඼��Ҫ�и��Ե�Ratio���ʶ���Ҫ���˾�̬��������Ϊprivate����������̳���ʹ�á�
};

typedef shared_ptr<CVirtualHistoryData> CVittualHistoryDataPtr;
