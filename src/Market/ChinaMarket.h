#pragma once

#include"VirtualMarket.h"
import FireBird.Container.ChinaStock;

import FireBird.HistoryCandle.VirtualExtend;
import FireBird.Stock.ChinaStock;
import FireBird.RelativeStrongReference;
import FireBird.Container.ChinaStockDayLine;
using std::set;
using std::atomic_int64_t;

constexpr int c_SelectedStockStartPosition = 0;
constexpr int c_10DaysRSStockSetStartPosition = 10; // ʮ�����ǿ�ȹ�Ʊ����ʼλ�ã�10-19Ϊʮ�����ǿ�Թ�Ʊ������ʮ����

class CChinaMarket;
using CChinaMarketPtr = shared_ptr<CChinaMarket>;

// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDayִ�о��������������8�������̡߳�
UINT ThreadBuildDayLineRS(const CChinaMarketPtr& pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ11, ����Ϊ��ǰ����������
// �����Ʊ���ǿ���̡߳����̵߳����߳�ThreadCalculateRSAtThisDateִ�о��������������8�������̡߳�
UINT ThreadBuildWeekLineRS(const CChinaMarketPtr& pMarket, long startCalculatingDate); // �˹����̷߳���ֵΪ30, ����Ϊ��ǰ����������

//���ּ����ù����߳�
// ����10��ǿ��Ʊ����ʹ���ⲿpRef�ṩ�Ĳ�����
UINT ThreadChoice10RSStrongStockSet(CRSReference* pRef, int iIndex); // ���̷߳���ֵΪ103
// �����Ʊ��10��ǿ�����
UINT ThreadCalculate10RSStrongStock(vector<CChinaStockPtr>* pv10RSStrongStock, const CRSReference* pRef, const CChinaStockPtr& pStock); // ���̷߳���ֵΪ104
// �����Ʊ��10��ǿ�����1
UINT ThreadCalculate10RSStrong1Stock(vector<CChinaStockPtr>* pv10RSStrongStock, const CChinaStockPtr& pStock); // ���̷߳���ֵΪ105
// �����Ʊ��10��ǿ�����2
UINT ThreadCalculate10RSStrong2Stock(vector<CChinaStockPtr>* pv10RSStrongStock, const CChinaStockPtr& pStock); // ���̷߳���ֵΪ106

class CChinaMarket : public CVirtualMarket {
public:
	CChinaMarket();
	// ֻ����һ��ʵ��,������ֵ��
	CChinaMarket(const CChinaMarket&) = delete;
	CChinaMarket& operator=(const CChinaMarket&) = delete;
	CChinaMarket(const CChinaMarket&&) noexcept = delete;
	CChinaMarket& operator=(const CChinaMarket&&) noexcept = delete;
	~CChinaMarket() override;

	void ResetMarket() final;
	long GetResetTime() final { return 30000; } // chinaMarket����ʱ��Ϊÿ��91300��92600��������ͣ������Ϊ�賿3�㼴�ɡ�
	void Reset();

	void PrepareToCloseMarket() final;

	bool IsTimeToResetSystem(long lCurrentTime) final;
	bool IsOrdinaryTradeTime() final { return IsOrdinaryTradeTime(GetMarketTime()); } // �ճ�����ʱ��
	bool IsOrdinaryTradeTime(long lTime) final;
	bool IsWorkingTime() final { return IsWorkingTime(GetMarketTime()); }
	bool IsWorkingTime(long lTime) final;
	bool IsDummyTime() final { return !IsWorkingTime(); }
	bool IsDummyTime(long lTime) final { return !IsWorkingTime(lTime); }

	int XferMarketTimeToIndex() override;

	int ProcessTask(long lCurrentTime) override; // ÿ�ն�ʱ�������,�ɻ����ScheduleTask����
	int ProcessCurrentImmediateTask(long lMarketTime) override; // ��ʱ������ȣ���ScheduleTask����

	// ��������
	virtual void TaskCreateTask(long lCurrentTime);
	virtual void TaskExitSystem(long lCurrentTime);
	bool TaskCheckMarketReady(long lCurrentTime);
	virtual bool TaskResetMarket(long lCurrentTime);
	void TaskDistributeAndCalculateRTData(long lCurrentTime);
	void TaskProcessAndSaveDayLine(long lCurrentTime);
	void TaskUpdateTempRTDB(long lCurrentTime);
	void TaskLoadCurrentStockHistoryData() const;// װ�ص�ǰ��Ʊ����
	void TaskPerSecond(long lCurrentTime); // ÿ��һ�εĸ�������
	void TaskAccessoryPerMinuteTask(long lCurrentTime); // ÿ�����ظ�ִ�еĸ�������
	void TaskPreparingMarketOpen(long lCurrentTime);
	void TaskChoiceRSSet(long lCurrentTime);
	void TaskSetCurrentStock();

	bool SetCheckActiveStockFlag(long lCurrentTime);
	bool TaskChoice10RSStrong1StockSet(long lCurrentTime);
	bool TaskChoice10RSStrong2StockSet(long lCurrentTime);
	bool TaskChoice10RSStrongStockSet(long lCurrentTime);
	bool TaskProcessTodayStock(long lCurrentTime);

	void ProcessTodayStock();
	bool IsTaskOfSavingDayLineDBFinished();
	bool CheckFastReceivingData(long lCurrentTime);
	static bool IsWebBusy();
	bool CheckMarketOpen(long lCurrentTime);

	bool TaskUpdateStockProfileDB(long lCurrentTime);
	bool TaskUpdateOptionDB(long lCurrentTime);
	bool TaskUpdateChosenStockDB();

	bool TaskUpdateStockSection(); //

	bool ProcessDayLine();

	// �������̵߳��ð�������
	virtual void Choice10RSStrongStockSet();

	//�������Ʊ��ʵʱ���ݣ�����ҵ��仯�ȡ�
	void DistributeRTData();
	void CalculateRTData();

	// ϵͳ״̬��
	bool IsFastReceivingRTData() noexcept { return m_fFastReceivingRTData; }

	// ʵʱ���ݶ�ȡ
	CString GetSinaStockInquiringStr(long lTotalNumber, bool fUsingTotalStockSet);
	CString GetNeteaseStockInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet);
	bool CheckValidOfNeteaseDayLineInquiringStr(const CString& str) const;

	static size_t IncreaseStockInquiringIndex(size_t& lIndex, size_t lEndPosition);

	// �õ���ǰ��ʾ��Ʊ
	CChinaStockPtr GetCurrentStock() const noexcept { return m_pCurrentStock; }
	void SetCurrentStock(const CString& strStockCode);
	void SetCurrentStock(const CChinaStockPtr& pStock);
	void ClearCurrentStock() noexcept { m_pCurrentStock = nullptr; }
	bool IsCurrentStockChanged() const noexcept { return m_fCurrentStockChanged; }
	void SetCurrentStockChanged(const bool fFlag) noexcept { m_fCurrentStockChanged = fFlag; }

	long GetMinLineOffset(time_t tUTC) const;

	auto GetCurrentSelectedPosition() const noexcept { return m_lCurrentSelectedPosition; }
	void SetCurrentSelectedPosition(const long lIndex) noexcept { m_lCurrentSelectedPosition = lIndex; }
	auto GetCurrentSelectedStockSet() const noexcept { return m_lCurrentSelectedStockSet; }
	void SetCurrentSelectedStockSet(const long lIndex) noexcept { m_lCurrentSelectedStockSet = lIndex; }
	CChinaStockPtr GetCurrentSelectedStock();

	bool IsChosen10RSStrong1StockSet() const noexcept { return m_fChosen10RSStrong1StockSet; }
	void SetChosen10RSStrong1StockSet(const bool fFlag) noexcept { m_fChosen10RSStrong1StockSet = fFlag; }
	bool IsChosen10RSStrong2StockSet() const noexcept { return m_fChosen10RSStrong2StockSet; }
	void SetChosen10RSStrong2StockSet(const bool fFlag) noexcept { m_fChosen10RSStrong2StockSet = fFlag; }
	bool IsChosen10RSStrongStockSet() const noexcept { return m_fChosen10RSStrongStockSet; }
	void SetChosen10RSStrongStockSet(const bool fFlag) noexcept { m_fChosen10RSStrongStockSet = fFlag; }
	bool IsCalculateChosen10RS() const noexcept { return m_fCalculateChosen10RS; }
	void SetCalculateChosen10RS(const bool fFlag) noexcept { m_fCalculateChosen10RS = fFlag; }

	// ���ݿ��ȡ�洢����

	virtual void UpdateOptionDB();
	void LoadOptionDB();
	void UpdateChosenStockDB() const;
	virtual void AppendChosenStockDB();
	void LoadChosenStockDB();
	bool TaskLoadTempRTData(long lTheDate, long lCurrentTime);
	virtual void LoadTempRTData(long lTheDate);
	bool LoadDayLine(CContainerChinaDayLine& dataChinaDayLine, long lDate) const;

	static bool ChangeDayLineStockCodeTypeToStandard();

	virtual bool DeleteWeekLine();
	static void DeleteWeekLineBasicInfo();
	static void DeleteWeekLineExtendInfo();
	virtual void DeleteWeekLine(long lMonday);
	void DeleteWeekLineBasicInfo(long lMonday) const;
	void DeleteWeekLineExtendInfo(long lMonday) const;

	void DeleteDayLine(long lDate) const;
	void DeleteDayLineBasicInfo(long lDate) const;
	void DeleteDayLineExtendInfo(long lDate) const;

	virtual bool DeleteCurrentWeekWeekLine();
	static bool DeleteCurrentWeekWeekLineBeforeTheDate(long lCutOffDate);

	bool Load10DaysRSStrong1StockSet();
	bool Load10DaysRSStrong2StockSet();

	bool LoadCalculatingRSOption();
	void SaveCalculatingRSOption() const;

	bool Load10DaysRSStrongStockDB();
	bool LoadOne10DaysRSStrongStockDB(long lIndex);

	virtual bool BuildWeekLineOfCurrentWeek();
	static bool CreateStockCodeSet(set<CString>& setStockCode, vector<CVirtualHistoryCandleExtendPtr>* pvData);
	virtual bool BuildCurrentWeekWeekLineTable(); // ʹ�����߱�����ǰ�����߱�

	// ��Ʊ��ʷ���ݴ���

	static bool IsDayLineNeedProcess() { return gl_qDayLine.size_approx() > 0; }
	bool IsSelectedStockLoaded() const noexcept { return m_fSelectedStockLoaded; }
	void SetSelectedStockLoaded(const bool fLoad) noexcept { m_fSelectedStockLoaded = fLoad; }

	bool IsCheckingActiveStock() const noexcept { return m_fCheckActiveStock; }
	void SetCheckActiveStock(const bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

	bool IsTodayTempRTDataLoaded() const noexcept { return m_fTodayTempDataLoaded; }
	void SetTodayTempRTDataLoaded(const bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

	long GetRSStartDate() const noexcept { return m_lRSStartDate; }
	void SetRSStartDate(const long lDate) noexcept { m_lRSStartDate = lDate; }
	long GetRSEndDate() const noexcept { return m_lRSEndDate; }
	void SetRSEndDate(const long lDate) noexcept { m_lRSEndDate = lDate; }
	long GetLastLoginDate() const noexcept { return m_lLastLoginDate; }
	void SetLastLoginDate(const long lDate) noexcept { m_lLastLoginDate = lDate; }
	void SetLastLoginTime(const long lTime) noexcept { m_lLastLoginTime = lTime; }
	long GetUpdatedDateFor10DaysRS1() const noexcept { return m_lUpdatedDateFor10DaysRS1; }
	void SetUpdatedDateFor10DaysRS1(const long lDate) noexcept { m_lUpdatedDateFor10DaysRS1 = lDate; }
	long GetUpdatedDateFor10DaysRS2() const noexcept { return m_lUpdatedDateFor10DaysRS2; }
	void SetUpdatedDateFor10DaysRS2(const long lDate) noexcept { m_lUpdatedDateFor10DaysRS2 = lDate; }
	long GetUpdatedDateFor10DaysRS() const noexcept { return m_lUpdatedDateFor10DaysRS; }
	void SetUpdatedDateFor10DaysRS(const long lDate) noexcept { m_lUpdatedDateFor10DaysRS = lDate; }

	//����ʵʱ��Ʊ�仯��
	bool DistributeRTDataToStock(const CWebRTDataPtr& pRTData);

	void ResetCurrentEffectiveRTDataRatio() noexcept {
		m_lRTDataReceivedInCurrentMinute = 0;
		m_lNewRTDataReceivedInCurrentMinute = 0;
	}
	double GetCurrentEffectiveRTDataRatio() const noexcept {
		if (m_lRTDataReceivedInCurrentMinute == 0) return 0.0;
		return static_cast<double>(m_lNewRTDataReceivedInCurrentMinute) / m_lRTDataReceivedInCurrentMinute;
	}

	// ״̬����
	bool IsUsingSinaRTDataReceiver() const noexcept { return m_fUsingSinaRTDataReceiver; }
	bool IsUsingNeteaseRTDataReceiver() const noexcept { return m_fUsingNeteaseRTDataReceiver; }
	void SetUsingNeteaseRTDataReceiver(const bool fFlag) noexcept { m_fUsingNeteaseRTDataReceiver = fFlag; }
	bool IsUsingTengxunRTDataReceiver() const noexcept { return m_fUsingTengxunRTDataReceiver; }
	void SetUsingTengxunRTDataReceiver(const bool fFlag) noexcept { m_fUsingTengxunRTDataReceiver = fFlag; }
	int GetCountDownTengxunNumber() const noexcept { return m_iCountDownTengxunNumber; }
	void SetCountDownTengxunNumber(const int iValue) noexcept { m_iCountDownTengxunNumber = iValue; }

	void SetTransactionTime(chrono::sys_seconds time) noexcept { m_tpNewTransactionTime = time; }
	void SetTransactionTime(const time_t tt) noexcept { m_tpNewTransactionTime = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(tt)); }
	auto GetTransactionTimePoint() const noexcept { return m_tpNewTransactionTime; }
	time_t GetTransactionTime() const noexcept { return m_tpNewTransactionTime.time_since_epoch().count(); }
	bool IsMarketOpened() const noexcept { return m_fMarketOpened; }
	void SetMarketOpened(const bool fFlag) noexcept { m_fMarketOpened = fFlag; }
	bool IsFastReceivingRTData() const noexcept { return m_fFastReceivingRTData; }
	bool IsRTDataSetCleared() const noexcept { return m_fRTDataSetCleared; }
	void SetRTDataSetCleared(const bool fFlag) noexcept { m_fRTDataSetCleared = fFlag; }
	bool IsUpdateTempDataDB() const noexcept { return m_fUpdateTempDataDB; }
	void SetUpdateTempDataDB(const bool fFlag) noexcept { m_fUpdateTempDataDB = fFlag; }

	bool IsCurrentEditStockChanged() const noexcept { return m_fCurrentEditStockChanged; }
	void SetCurrentEditStockChanged(const bool fFlag) noexcept { m_fCurrentEditStockChanged = fFlag; }

	// ʵʱ������Ҫ������������
	void SetRTDataNeedCalculate(const bool fFlag) noexcept { m_RTDataNeedCalculate = fFlag; }
	bool IsRTDataNeedCalculate() const noexcept { return m_RTDataNeedCalculate; }
	// �����������������ǿ����������
	void SetCalculatingDayLineRS(const bool fFlag) noexcept { m_CalculatingDayLineRS = fFlag; }
	bool IsCalculatingDayLineRS() const noexcept { return m_CalculatingDayLineRS; }
	// �����������������ǿ����������
	void SetCalculatingWeekLineRS(const bool fFlag) noexcept { m_CalculatingWeekLineRS = fFlag; }
	bool IsCalculatingWeekLineRS() const noexcept { return m_CalculatingWeekLineRS; }

	bool AddChosenStock(const CChinaStockPtr& pStock);
	bool DeleteChosenStock(const CChinaStockPtr& pStock);
	size_t GetChosenStockSize() const { return m_avChosenStock.at(0).size(); }
	size_t GetChosenStockSize(const long lIndex) const { return m_avChosenStock.at(lIndex).size(); }
	void ClearChoiceStockContainer() { m_avChosenStock.at(0).clear(); }

	void SetUpdateOptionDB(const bool fFlag) noexcept { m_fUpdateOptionDB = fFlag; }

	bool IsUpdateOptionDB() const noexcept { return m_fUpdateOptionDB; }

	void SetUpdateChosenStockDB(const bool fFlag) noexcept { m_fUpdateChosenStockDB = fFlag; }
	bool IsUpdateChosenStockDB() const noexcept { return m_fUpdateChosenStockDB; }

	INT64 GetRTDataReceived() const noexcept { return m_llRTDataReceived.load(); }
	void SetRTDataReceived(const INT64 llValue) noexcept { m_llRTDataReceived = llValue; }
	void IncreaseRTDataReceived(const INT64 llValue = 1) noexcept { m_llRTDataReceived += llValue; }

	void ChangeToNextStock();
	void ChangeToPrevStock();
	void ChangeToPrevStockSet();
	void ChangeToNextStockSet();

	bool IsTotalStockSetSelected() const noexcept {
		if (m_lCurrentSelectedStockSet == -1) return true;
		return false;
	}

	size_t GetCurrentStockSetSize() const;

	void CreateStock(const CString& strStockCode, const CString& strStockName, bool fProcessRTData);

	void SetCurrentRSStrongIndex(const long lIndex) noexcept { m_lCurrentRSStrongIndex = lIndex; }

	void ResetEffectiveRTDataRatio();

	void SetDistributeAndCalculateTime(time_t tt) { m_ttDistributeAndCalculateTime = tt; }

public:
	atomic_int64_t m_ttDistributeAndCalculateTime; // ʵʱ���ݷ��估����ʱ��

protected:
	vector<CChinaStockPtr> m_v10RSStrong1Stock; // 10��ǿ�ƹ�Ʊ��
	vector<CChinaStockPtr> m_v10RSStrong2Stock; // 10��ǿ�ƹ�Ʊ��
	vector<CRSReference> m_aRSStrongOption; // ���ڼ���RS�Ĳ��������ʮ����
	vector<vector<CChinaStockPtr>> m_avChosenStock; // ����ѡ��Ĺ�Ʊ����0-9����ѡ��Ʊ����10-19��10��RS��Ʊ����20-29���ɼ۱仯��Ʊ��
	long m_lCurrentSelectedPosition; // ��ǰ��Ʊ����λ��
	long m_lCurrentRSStrongIndex; // �����ڴ��ݵ�ǰ��λ�ã�������ѡ����ȷ�����ݱ�
	long m_lCurrentSelectedStockSet; // ��ǰѡ��Ĺ�Ʊ����-1Ϊ�����Ʊ����1-10Ϊ10��RS���Թ�Ʊ�����Դ����ƣ���
	bool m_fChosen10RSStrong1StockSet; // ���յ�10��ǿ�ƹ�Ʊ���Ѽ������
	bool m_fChosen10RSStrong2StockSet; // ���յ�10��ǿ�ƹ�Ʊ���Ѽ������
	bool m_fChosen10RSStrongStockSet; // ���յ�10��ǿ�ƹ�Ʊ���Ѽ������
	bool m_fCalculateChosen10RS;

	atomic_int64_t m_llRTDataReceived; // ���յ���ʵʱ��������

	bool m_RTDataNeedCalculate;
	bool m_CalculatingDayLineRS;
	bool m_CalculatingWeekLineRS;

	bool m_fCurrentEditStockChanged;
	bool m_fMarketOpened; // �Ƿ���
	bool m_fFastReceivingRTData; // �Ƿ�ʼ����ʵʱ����
	bool m_fRTDataSetCleared; // ʵʱ���ݿ��������ʶ���ŵ���ʮ��֮ǰΪ�٣�֮������Ϊ�档
	bool m_fUpdateTempDataDB; // �洢��ʱʵʱ���ݱ�ʶ
	CChinaStockPtr m_pCurrentStock; // ��ǰ��ʾ�Ĺ�Ʊ

	chrono::sys_seconds m_tpNewTransactionTime;

	bool m_fUsingSinaRTDataReceiver; // ʹ������ʵʱ������ȡ��
	bool m_fUsingNeteaseRTDataReceiver; // ʹ������ʵʱ������ȡ��
	bool m_fUsingTengxunRTDataReceiver; // ʹ����Ѷʵʱ������ȡ��
	int m_iCountDownTengxunNumber;

	// Option��ѡ��
	long m_lRSStartDate;
	long m_lRSEndDate;
	long m_lLastLoginDate; // �ϴε�¼���ڡ����������Ϊ���յĻ�������������������ʷ����
	long m_lLastLoginTime;
	long m_lUpdatedDateFor10DaysRS2;
	long m_lUpdatedDateFor10DaysRS1;
	long m_lUpdatedDateFor10DaysRS;

	bool m_fSelectedStockLoaded;

	bool m_fCurrentStockChanged; // ��ǰѡ��Ĺ�Ʊ�ı���
	INT64 m_lTotalMarketBuy; // �����г��е�A������������
	INT64 m_lTotalMarketSell; // �����г��е�A�������������

	// ϵͳ״̬��
	bool m_fCheckActiveStock; // �Ƿ��ѯ���ջ�Ծ��Ʊ����
	bool m_fTodayTempDataLoaded; //�����ݴ����ʱ�����Ƿ���ر�ʶ��

	// ���¹�Ʊ�������ݿ��ʶ
	atomic_bool m_fUpdateOptionDB;
	bool m_fUpdateChosenStockDB;

private:
	long m_lRTDataReceivedInCurrentMinute; // ÿ���ӽ��յ���ʵʱ��������
	long m_lNewRTDataReceivedInCurrentMinute; // ÿ���ӽ��յ�����ʵʱ��������
};

extern CChinaMarketPtr gl_pChinaMarket; // �й���Ʊ�г������л�Ծ�Ĺ�Ʊ��λ�����У���һʵ�����������������һ��ʵ����
