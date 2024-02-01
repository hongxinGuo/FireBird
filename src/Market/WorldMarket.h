#pragma once

#include"VirtualMarket.h"

#include "FinnhubWebSocket.h"
#include "TiingoIEXWebSocket.h"

#include"FinnhubCryptoSymbol.h"
#include"FinnhubForexSymbol.h"

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

	void PreparingExitMarket() final;

	void Reset();
	void ResetFinnhub();
	static void ResetQuandl();
	static void ResetTiingo();
	void ResetDataContainer();

	bool IsTimeToResetSystem(long lCurrentTime) final {
		if ((lCurrentTime > 165759) && (lCurrentTime < 170501)) return true;
		return false;
	}

	bool ProcessTask(long lCurrentTime) override; // ÿ�ն�ʱ�������,��SchedulingTaskPerSecond����

	void TaskCreateTask(long lCurrentTime);
	void TaskResetMarket(long lCurrentTime);
	bool TaskCheckMarketReady(long lCurrentTime);
	void TaskProcessWebSocketData(long lCurrentTime);
	void TaskStartAllWebSocket(long lCurrentTime);
	void TaskStopAllWebSocket(long lCurrentTime);
	void TaskUpdateStockProfileDB(long lCurrentTime);

	virtual bool CreateThreadUpdateTiingoIndustry();
	virtual bool CreateThreadUpdateSICIndustry();
	virtual bool CreateThreadUpdateNaicsIndustry();

	virtual void CreateThreadUpdateCompanyNewsDB();
	virtual void CreateThreadUpdateBasicFinancialDB();
	virtual void CreateThreadUpdateDayLineDB();
	virtual void CreateThreadUpdateForexExchangeDB();
	virtual void CreateThreadUpdateForexSymbolDB();
	virtual void CreateThreadUpdateCryptoExchangeDB();
	virtual void CreateThreadUpdateFinnhubCryptoSymbolDB();
	virtual void CreateThreadUpdateCountryListDB();
	virtual void CreateThreadUpdateEconomicCalendarDB();
	virtual void CreateThreadUpdateInsiderTransactionDB();
	virtual void CreateThreadUpdateInsiderSentimentDB();
	virtual void CreateThreadUpdateTiingoStockDB();
	virtual void CreateThreadUpdateTiingoCryptoSymbolDB();
	virtual void CreateThreadUpdateDayLineStartEndDate();
	virtual void CreateThreadUpdateStockProfileDB();
	virtual void CreateThreadUpdateForexDayLineDB(CForexSymbolPtr pSymbol);
	virtual void CreateThreadUpdateCryptoDayLineDB(CFinnhubCryptoSymbolPtr pSymbol);
	virtual void CreateThreadUpdateEPSSurpriseDB(CWorldStockPtr pStock);
	virtual void CreateThreadUpdateEPSSurpriseDB2();

	bool UpdateForexDayLineDB();
	bool UpdateCryptoDayLineDB();
	bool UpdateEPSSurpriseDB();

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

	vectorString GetFinnhubWebSocketSymbolVector();

	void StartAllWebSocket();
	virtual void StartFinnhubWebSocket();
	virtual void StartTiingoIEXWebSocket();
	virtual void StartTiingoCryptoWebSocket();
	virtual void StartTiingoForexWebSocket();
	static void DisconnectAllWebSocket(); // ֹͣWebSocket���˺����ȴ���ֹͣ�󷽷��ء���ϵͳ�˳�ǰ��׼��������
	void StopAllWebSocketIfTimeOut() const;
	static void StopFinnhubWebSocketIfTimeOut();
	static void StopTiingoIEXWebSocketIfTimeOut();
	static void StopTiingoCryptoWebSocketIfTimeOut();
	static void StopTiingoForexWebSocketIfTimeOut();

	static void ProcessWebSocketData();
	static void ProcessFinnhubWebSocketData();
	static void ProcessTiingoIEXWebSocketData();
	static void ProcessTiingoCryptoWebSocketData();
	static void ProcessTiingoForexWebSocketData();

	void UpdateWorldStockFromWebSocket();
	void UpdateWorldStockFromTiingoIEXSocket(const CTiingoIEXSocketPtr& pTiingoIEXbData);
	void UpdateWorldStockFromFinnhubSocket(const CFinnhubSocketPtr& pFinnhub);

	void UpdateMarketStatus(const CMarketStatusVectorPtr& pv) const;
	void UpdateMarketHoliday(const CMarketHolidayVectorPtr& pv) const;

	// check function

protected:
	long m_lCurrentUpdateDayLinePos; // ���ڸ���һ���������ݳ���24Сʱ���ʶ����˼���������Ϊ�������������ÿ�����á�
	long m_lCurrentUpdateEPSSurprisePos; // �˱�������ÿ�ո���

	CMarketStatusVectorPtr m_pvMarketStatus;
	CMarketHolidayVectorPtr m_pvMarketHoliday;

	CString m_strCurrentFunction; // ��ǰ����ʹ����֤ȯ����

	bool m_bFinnhubWebSiteAccessible; // ����finnhub.io��ʱ��ǽ���ʶ���Ҫ�˱�ʶ��
};

using CWorldMarketPtr = shared_ptr<CWorldMarket>;

extern CWorldMarketPtr gl_pWorldMarket; // ��Ʊ�г��� ��һʵ�����������������һ��ʵ����
