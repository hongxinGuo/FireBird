////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �й���Ʊʵ�������������������������������ȣ��Ļ��ࡣ
// �����࣬����������ʵ����
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"Accessory.h"
#include"ChinaStockHistoryData.h"

using namespace std;
#include<vector>
#include<memory>
#include<atomic>

class CChinaStockHistoryDataContainer : public CObject {
public:
  CChinaStockHistoryDataContainer();
  ~CChinaStockHistoryDataContainer();
  void Reset(void); // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

  virtual bool SaveData(void) { TRACE(_T("�����˻���SaveData\n")); return false; }
  virtual bool LoadData(void) { TRACE(_T("�����˻���LoadData\n")); return false; }

  void UpdateData(vector<CChinaStockHistoryDataPtr>& vTempLine);

public:
  size_t GetDataSize(void) { return m_vHistoryData.size(); }
  CChinaStockHistoryDataPtr GetData(long lIndex) { return m_vHistoryData.at(lIndex); }
  void Unload(void) noexcept { m_vHistoryData.clear(); m_fDataLoaded = false; }
  bool StoreData(CChinaStockHistoryDataPtr pData) noexcept { m_vHistoryData.push_back(pData); return true; }

  bool IsDatabaseTodayUpdated(void) noexcept { return (m_fDatabaseTodayUpdated); }
  void SetDatabaseTodayUpdated(bool fUpdate) noexcept { m_fDatabaseTodayUpdated = fUpdate; }
  bool IsDataLoaded(void) noexcept { return m_fDataLoaded; }
  void SetDataLoaded(bool fFlag) noexcept { m_fDataLoaded = fFlag; }

  bool CalculateRelativeStrong(void);
  virtual bool CalculateRS(INT64 lNumber);
  bool CalculateRelativeStrongIndex(void);
  virtual bool CalculateRSIndex(INT64 lNumber);
  bool CalculateRelativeStrongLogarithm(void);
  virtual bool CalculateRSLogarithm(INT64 lNumber);

  void GetRS1(vector<double>& vRS);
  void GetRSIndex1(vector<double>& vRS);
  void GetRSLogarithm1(vector<double>& vRS);
  void GetRS3(vector<double>& vRS);
  void GetRS5(vector<double>& vRS);
  void GetRS10(vector<double>& vRS);
  void GetRS30(vector<double>& vRS);
  void GetRS60(vector<double>& vRS);
  void GetRS120(vector<double>& vRS);

protected:
  vector<CChinaStockHistoryDataPtr> m_vHistoryData;

  atomic_bool m_fDataLoaded; // ����װ������ʶ

  bool m_fDatabaseTodayUpdated; // ���ݿ�����Ƿ��Ѹ��±�ʶ

protected:
  bool m_fLoadDataFirst;
};

typedef shared_ptr<CChinaStockHistoryDataContainer> CChinaStockHistoryDataContainerPtr;