#pragma once

#include"stdafx.h"
#include"globeSymbolDef.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CVirtualStock : public CObject {
public:
  CVirtualStock();
  ~CVirtualStock();
  virtual void Reset(void);

public:
  bool IsTodayNewStock(void) noexcept { return m_fTodayNewStock; }
  void SetTodayNewStock(bool fFlag) noexcept { m_fTodayNewStock = fFlag; }
  bool IsUpdateStockProfileDB(void) noexcept { return m_fUpdateStockProfileDB; }
  bool IsUpdateStockProfileDBAndClearFlag(void) noexcept { const bool fNeedSaving = m_fUpdateStockProfileDB.exchange(false); return fNeedSaving; }
  void SetUpdateStockProfileDB(bool fFlag) noexcept { m_fUpdateStockProfileDB = fFlag; }
  bool IsActive(void) noexcept { return m_fActive; }
  void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }
  
  long GetDayLineStartDate(void) noexcept { return m_lDayLineStartDate; }
  void SetDayLineStartDate(long lDate) noexcept { m_lDayLineStartDate = lDate; }
  long GetDayLineEndDate(void) noexcept { return m_lDayLineEndDate; }
  void SetDayLineEndDate(long lDate) noexcept { m_lDayLineEndDate = lDate; }

  long GetIPOStatus(void) noexcept { return m_lIPOStatus; }
  void SetIPOStatus(long lValue) noexcept { m_lIPOStatus = lValue; }
  bool IsDelisted(void) noexcept { return (m_lIPOStatus == __STAKE_DELISTED__); }
  bool IsNullStock(void) noexcept { return (m_lIPOStatus == __STAKE_NULL__); }
  bool IsIPOed(void) noexcept { return(m_lIPOStatus == __STAKE_IPOED__); }
  bool IsNotChecked(void) noexcept { return(m_lIPOStatus == __STAKE_NOT_CHECKED__); }

  // ���ڴ���������ʷ���ݵĺ���λ�ڲ�ͬ���߳��У��ʶ���Ҫͬ���������ñ�ʶ
  bool IsDayLineNeedUpdate(void) noexcept { return m_fDayLineNeedUpdate; }
  void SetDayLineNeedUpdate(bool fFlag) noexcept { m_fDayLineNeedUpdate = fFlag; }
  bool IsDayLineNeedProcess(void) noexcept { return m_fDayLineNeedProcess; }
  void SetDayLineNeedProcess(bool fFlag) noexcept { m_fDayLineNeedProcess = fFlag; }
  bool IsDayLineNeedSaving(void) noexcept { return m_fDayLineNeedSaving; }
  void SetDayLineNeedSaving(bool fFlag) noexcept { m_fDayLineNeedSaving = fFlag; }
  bool IsDayLineNeedSavingAndClearFlag(void) noexcept { const bool fNeedSaving = m_fDayLineNeedSaving.exchange(false); return fNeedSaving; }


protected:
  bool m_fTodayNewStock; // �����·��ֵĹ�Ʊ
  atomic_bool m_fUpdateStockProfileDB; 
  bool m_fActive;	// �Ƿ����������ݶ��롣������ʵʱ���鴦����������������ʷ���ݴ����������á�
  long m_lDayLineStartDate;	// ����������ʼ�ա�����Ǵ���������ʷ����ʱ�õ�����ʼ�����գ�
  long m_lDayLineEndDate;	// �������ݸ����ա�����Ǵ���������ʷ����ʱ�õ��������գ�
  long m_lIPOStatus; // ͨ��������ʷ���߲�ѯ�����ֻ��ǰ׺��Ϣ��û��ʵ�����ݣ�����ȷ��û��ʵ�ʽ��ס�����������£�����ʵʱ���������ݣ�ֻ��Ϊ����ѡ�Ĭ�������Ϊ������
               // δ���У���Ч��Ʊ���룩Ϊ__STAKE_NULL__������Ϊ__STAKE_IPOED__����ͨ��IPO����δ���л�����Ϊ__STAKE_DELISTED�����������δ���֣������Ժ���

  atomic_bool m_fDayLineNeedUpdate; // ������Ҫ���¡�Ĭ��Ϊ��
  atomic_bool m_fDayLineNeedProcess; // �Ѵ������϶�ȡ��������ʷ���ݣ��ȴ�����
  atomic_bool m_fDayLineNeedSaving; // ������ʷ�����Ѵ����ȴ��洢��


};

typedef shared_ptr<CVirtualStock> CVirtualStockPtr;
