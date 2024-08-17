#pragma once

#include"VirtualMarket.h"

#include "FinnhubWebSocket.h"
#include "TiingoIEXWebSocket.h"

#include "MarketStatus.h"
#include"MarketHoliday.h"

class CWorldMarket : public CVirtualMarket {
public:
	CWorldMarket();
	// ֻ����һ��ʵ��,������ֵ������
	CWorldMarket(const CWorldMarket&) = delete;
	CWorldMarket& operator=(const CWorldMarket&) = delete;
	CWorldMarket(const CWorldMarket&&) noexcept = delete;
	CWorldMarket& operator=(const CWorldMarket&&) noexcept = delete;
	~CWorldMarket() override;

	void ResetMarket() final;
	long GetResetTime() override { return gl_systemConfiguration.GetWorldMarketResettingTime(); }

	void PrepareToCloseMarket() final;

	void Reset();
	void ResetFinnhub();
	static void ResetQuandl();
	static void ResetTiingo();
	void ResetDataContainer();

	bool IsTimeToResetSystem(long lCurrentTime) final { return (lCurrentTime > 165759) && (lCurrentTime < 170501); }
	int ProcessTask(long lCurrentTime) override; // ÿ�ն�ʱ�������,��ScheduleTask����
	int ProcessCurrentImmediateTask(long lMarketTime) override; // ��ʱ������ȣ���ScheduleTask����

	void TaskCreateTask(long lCurrentTime);
	void TaskResetMarket(long lCurrentTime);
	bool TaskCheckMarketReady(long lCurrentTime);
	void TaskProcessWebSocketData(long lCurrentTime);
	void TaskMonitorWebSocket(long lCurrentTime);
	void TaskUpdateWorldMarketDB(long lCurrentTime);

	bool TaskUpdateTiingoIndustry();
	bool TaskUpdateSICIndustry();
	bool TaskUpdateNaicsIndustry();

	bool TaskUpdateForexDayLineDB();
	bool TaskUpdateCryptoDayLineDB();

	void TaskPerSecond(long lCurrentTime);

	bool UpdateEPSSurpriseDB();
	void UpdateSECFilingsDB();

	// ����״̬

	CString GetCurrentFunction() { return m_strCurrentFunction; }
	void SetCurrentFunction(const CString& str) { m_strCurrentFunction = str; }

	static bool UpdateToken();

	// ���ݿ����
	virtual bool UpdateCompanyNewsDB();
	virtual bool UpdateStockDayLineDB();
	virtual bool UpdateInsiderTransactionDB();
	virtual bool UpdateInsiderSentimentDB();
	virtual bool UpdateTiingoIndustry();
	virtual bool UpdateSICIndustry();
	virtual bool UpdateNaicsIndustry();

	void RebuildStockDayLineDB();
	virtual void UpdateStockDayLineStartEndDate();
	void RebuildEPSSurprise();
	void RebuildPeer();
	void RebuildBasicFinancial();

	vectorString GetFinnhubWebSocketSymbols();

	static void DisconnectAllWebSocket(); // ֹͣWebSocket���˺����ȴ���ֹͣ�󷽷��ء���ϵͳ�˳�ǰ��׼��������

	static void ProcessWebSocketData();
	static void ProcessFinnhubWebSocketData();
	static void ProcessTiingoIEXWebSocketData();
	static void ProcessTiingoCryptoWebSocketData();
	static void ProcessTiingoForexWebSocketData();

	void UpdateWorldStockFromWebSocket();
	void UpdateWorldStockFromTiingoIEXSocket(const CTiingoIEXSocketPtr& pTiingoIEXbData);
	void UpdateWorldStockFromFinnhubSocket(const CFinnhubSocketPtr& pFinnhub);

	void UpdateMarketStatus(const CMarketStatussPtr& pv) const;
	void UpdateMarketHoliday(const CMarketHolidaysPtr& pv) const;

	// check function
	bool IsReadyToInquireWebData(long lCurrentMarketTime) override;

protected:
	long m_lCurrentUpdateDayLinePos; // ���ڸ���һ���������ݳ���24Сʱ���ʶ����˼���������Ϊ�������������ÿ�����á�
	long m_lCurrentUpdateEPSSurprisePos; // �˱�������ÿ�ո���

	CMarketStatussPtr m_pvMarketStatus;
	CMarketHolidaysPtr m_pvMarketHoliday;

	CString m_strCurrentFunction; // ��ǰ����ʹ����֤ȯ����

	bool m_bFinnhubWebSiteAccessible; // ����finnhub.io��ʱ��ǽ���ʶ���Ҫ�˱�ʶ��
};

using CWorldMarketPtr = shared_ptr<CWorldMarket>;

extern CWorldMarketPtr gl_pWorldMarket; // ��Ʊ�г��� ��һʵ�����������������һ��ʵ����
