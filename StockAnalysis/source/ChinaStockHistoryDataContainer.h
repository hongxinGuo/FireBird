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

  virtual bool SaveData(CString strStockSymbol) { TRACE(_T("�����˻���SaveData\n")); return false; }
  virtual bool LoadData(CString strStockSymbol) { TRACE(_T("�����˻���LoadData\n")); return false; }

  void UpdateData(vector<CChinaStockHistoryDataPtr>& vTempLine);

  void ShowData(CDC* pDC, CRect rectClient);

public:
  vector<CChinaStockHistoryDataPtr>* GetContainer(void) noexcept { return &m_vHistoryData; }

  size_t GetDataSize(void) const noexcept { return m_vHistoryData.size(); }
  CChinaStockHistoryDataPtr GetData(long lIndex) const { return m_vHistoryData.at(lIndex); }
  void Unload(void) noexcept { m_vHistoryData.clear(); m_fDataLoaded = false; }
  bool StoreData(CChinaStockHistoryDataPtr pData) { m_vHistoryData.push_back(pData); return true; }

  bool IsDatabaseTodayUpdated(void) const noexcept { return (m_fDatabaseTodayUpdated); }
  void SetDatabaseTodayUpdated(bool fUpdate) noexcept { m_fDatabaseTodayUpdated = fUpdate; }
  bool IsDataLoaded(void) const noexcept { return m_fDataLoaded; }
  void SetDataLoaded(bool fFlag) noexcept { m_fDataLoaded = fFlag; }

  bool CalculateRS0(void);
  virtual bool CalculateRS1(INT64 lNumber);
  bool CalculateRSIndex0(void);
  virtual bool CalculateRSIndex1(INT64 lNumber);
  bool CalculateRSLogarithm0(void);
  virtual bool CalculateRSLogarithm1(INT64 lNumber);

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