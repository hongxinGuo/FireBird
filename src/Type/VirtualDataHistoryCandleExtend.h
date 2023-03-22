////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �й���Ʊʵ�������������������������������ȣ��Ļ��ࡣ
// �����࣬����������ʵ����
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualHistoryCandleExtend.h"

#include<vector>
#include<memory>
#include<atomic>
using std::vector;
using std::shared_ptr;
using std::atomic_bool;

class CVirtualDataHistoryCandleExtend {
public:
	CVirtualDataHistoryCandleExtend();
	virtual ~CVirtualDataHistoryCandleExtend() = default;
	void Reset(); // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

	// ���е����������Ҫ����������洢����ȡ��������������ô˻��ຯ��
	virtual bool SaveDB(const CString&) {
		TRACE(_T("�����˻���SaveDB\n"));
		return false;
	}

	virtual bool LoadDB(const CString&) {
		TRACE(_T("�����˻���LoadDB\n"));
		return false;
	}

	bool SaveExtendDB(CVirtualSetHistoryCandleExtend* pSetHistoryCandleExtend);

	bool LoadBasicDB(CVirtualSetHistoryCandleBasic* pSetHistoryCandleBasic);
	bool LoadExtendDB(CVirtualSetHistoryCandleExtend* pSetHistoryCandleExtend);

	void UpdateData(vector<CVirtualHistoryCandleExtendPtr>& vTempData);
	void UpdateData(vector<CDayLinePtr>& vTempDayLine);

	void ShowData(CDC* pDC, CRect rectClient);

protected:
	bool UpdateBasicDB(CVirtualSetHistoryCandleBasic* pSetHistoryCandleBasic, const CString& strStockSymbol = _T(""));

public:
	vector<CVirtualHistoryCandleExtendPtr>* GetContainer() noexcept { return &m_vHistoryData; }

	long Size() const noexcept { return static_cast<long>(m_vHistoryData.size()); }
	bool GetStartEndDate(long& lStartDate, long& lEndDate);

	void Unload() noexcept {
		m_vHistoryData.clear();
		m_fDataLoaded = false;
	}

	CVirtualHistoryCandleExtendPtr GetData(const long lIndex) const { return m_vHistoryData.at(lIndex); }

	bool StoreData(const CVirtualHistoryCandleExtendPtr pData) {
		m_vHistoryData.push_back(pData);
		return true;
	}

	bool IsDatabaseTodayUpdated() const noexcept { return (m_fDatabaseTodayUpdated); }
	void SetDatabaseTodayUpdated(const bool fUpdate) noexcept { m_fDatabaseTodayUpdated = fUpdate; }
	bool IsDataLoaded() const noexcept { return m_fDataLoaded; }
	void SetDataLoaded(const bool fFlag) noexcept { m_fDataLoaded = fFlag; }

	bool CalculateRS0();
	virtual bool CalculateRS1(INT64 lNumber);
	bool CalculateRSIndex0();
	virtual bool CalculateRSIndex1(INT64 lNumber);
	bool CalculateRSLogarithm0();
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
	vector<CVirtualHistoryCandleExtendPtr> m_vHistoryData;

	atomic_bool m_fDataLoaded; // ����װ������ʶ

	bool m_fDatabaseTodayUpdated; // ���ݿ�����Ƿ��Ѹ��±�ʶ

protected:
	bool m_fLoadDataFirst;
};

using CVirtualDataHistoryCandleExtendPtr = shared_ptr<CVirtualDataHistoryCandleExtend>;
