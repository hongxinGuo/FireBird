#pragma once

#include"globeSymbolDef.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CVirtualStock : public CObject {
public:
  CVirtualStock();
  virtual ~CVirtualStock();
  virtual void Reset(void);

public:
  CString GetSymbol(void) { return m_strSymbol; }
  void SetSymbol(CString str) { m_strSymbol = str; }

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
  void SetUpDownRate(double dValue) noexcept { m_dUpDownRate = dValue; }
  double GetChangeHandRate(void) noexcept { return m_dChangeHandRate; }
  void SetChangeHandRate(double dValue) noexcept { m_dChangeHandRate = dValue; }
  void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
  INT64 GetTotalValue(void) noexcept { return m_llTotalValue; }
  void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }
  INT64 GetCurrentValue(void) noexcept { return m_llCurrentValue; }

  //
  bool IsTodayNewStock(void) noexcept { return m_fTodayNewStock; }
  void SetTodayNewStock(bool fFlag) noexcept { m_fTodayNewStock = fFlag; }
  bool IsUpdateProfileDB(void) noexcept { return m_fUpdateProfileDB; }
  bool IsUpdateProfileDBAndClearFlag(void) noexcept { const bool fNeedSaving = m_fUpdateProfileDB.exchange(false); return fNeedSaving; }
  void SetUpdateProfileDB(bool fFlag) noexcept { m_fUpdateProfileDB = fFlag; }
  bool IsActive(void) noexcept { return m_fActive; }
  void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }

  long GetDayLineStartDate(void) noexcept { return m_lDayLineStartDate; }
  void SetDayLineStartDate(long lDate) noexcept { m_lDayLineStartDate = lDate; }
  long GetDayLineEndDate(void) noexcept { return m_lDayLineEndDate; }
  void SetDayLineEndDate(long lDate) noexcept { m_lDayLineEndDate = lDate; }

  long GetIPOStatus(void) noexcept { return m_lIPOStatus; }
  void SetIPOStatus(long lValue) noexcept { m_lIPOStatus = lValue; }
  bool IsDelisted(void) noexcept { return (m_lIPOStatus == __STOCK_DELISTED__); }
  bool IsNullStock(void) noexcept { return (m_lIPOStatus == __STOCK_NULL__); }
  bool IsIPOed(void) noexcept { return(m_lIPOStatus == __STOCK_IPOED__); }
  bool IsNotChecked(void) noexcept { return(m_lIPOStatus == __STOCK_NOT_CHECKED__); }

  // ���ڴ���������ʷ���ݵĺ���λ�ڲ�ͬ���߳��У��ʶ���Ҫͬ���������ñ�ʶ
  bool IsDayLineNeedUpdate(void) noexcept { return m_fDayLineNeedUpdate; }
  void SetDayLineNeedUpdate(bool fFlag) noexcept { m_fDayLineNeedUpdate = fFlag; }
  bool IsDayLineNeedProcess(void) noexcept { return m_fDayLineNeedProcess; }
  void SetDayLineNeedProcess(bool fFlag) noexcept { m_fDayLineNeedProcess = fFlag; }
  bool IsDayLineNeedSaving(void) noexcept { return m_fDayLineNeedSaving; }
  void SetDayLineNeedSaving(bool fFlag) noexcept { m_fDayLineNeedSaving = fFlag; }
  bool IsDayLineNeedSavingAndClearFlag(void) noexcept { const bool fNeedSaving = m_fDayLineNeedSaving.exchange(false); return fNeedSaving; }

protected:
  CString m_strSymbol; // ��Ʊ���롣ʮλ���ڣ�ǰ��λΪ���ִ��룬����λΪ�г�ǰ׺����600601.SS��000001.SZ

  // ʵʱ������
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

  bool m_fTodayNewStock; // �����·��ֵĹ�Ʊ
  atomic_bool m_fUpdateProfileDB;
  bool m_fActive;	// �Ƿ����������ݶ��롣������ʵʱ���鴦����������������ʷ���ݴ����������á�
  long m_lDayLineStartDate;	// ����������ʼ�ա�����Ǵ���������ʷ����ʱ�õ�����ʼ�����գ�
  long m_lDayLineEndDate;	// �������ݸ����ա�����Ǵ���������ʷ����ʱ�õ��������գ�
  long m_lIPOStatus; // ͨ��������ʷ���߲�ѯ�����ֻ��ǰ׺��Ϣ��û��ʵ�����ݣ�����ȷ��û��ʵ�ʽ��ס�����������£�����ʵʱ���������ݣ�ֻ��Ϊ����ѡ�Ĭ�������Ϊ������
               // δ���У���Ч��Ʊ���룩Ϊ__STOCK_NULL__������Ϊ__STOCK_IPOED__����ͨ��IPO����δ���л�����Ϊ__STOCK_DELISTED�����������δ���֣������Ժ���

  atomic_bool m_fDayLineNeedUpdate; // ������Ҫ���¡�Ĭ��Ϊ��
  atomic_bool m_fDayLineNeedProcess; // �Ѵ������϶�ȡ��������ʷ���ݣ��ȴ�����
  atomic_bool m_fDayLineNeedSaving; // ������ʷ�����Ѵ����ȴ��洢��
};

typedef shared_ptr<CVirtualStock> CVirtualStockPtr;
