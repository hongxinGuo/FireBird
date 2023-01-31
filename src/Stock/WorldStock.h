#pragma once

#include"nlohmannJsonDeclaration.h"

#include"VirtualStock.h"
#include"TiingoStock.h"

#include"FinnhubStockBasicFinancial.h"

#include"DayLine.h"
#include"DataWorldStockDayLine.h"
#include"FinnhubCompanyNews.h"

#include"SetWorldStock.h"

class CWorldStock : public CVirtualStock {
public:
	CWorldStock();
	// 不允许复制和赋值。
	CWorldStock(const CWorldStock&) = delete;
	CWorldStock& operator=(const CWorldStock&) = delete;
	CWorldStock(const CWorldStock&&) noexcept = delete;
	CWorldStock& operator=(const CWorldStock&&) noexcept = delete;
	~CWorldStock() override;

	void Reset(void) override;
	void ResetAllUpdateDate(void);
	int GetRatio(void) const final { return 1000; }

public:
	void Load(CSetWorldStock& setWorldStock);
	void CheckUpdateStatus(long lTodayDate);
	void CheckProfileUpdateStatus(long lTodayDate);
	bool CheckCompanyNewsUpdateStatus(long lTodayDate);
	bool CheckBasicFinancialUpdateStatus(long lTodayDate);
	bool CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek);
	void Save(CSetWorldStock& setWorldStock);
	void Update(CSetWorldStock& setWorldStock);
	void Append(CSetWorldStock& setWorldStock);
	void SaveDayLine(void) { m_dataDayLine.SaveDB(m_strSymbol); }
	void SaveInsiderTransaction(void);
	void SaveInsiderSentiment(void);
	virtual bool UpdateCompanyNewsDB(void);
	virtual bool UpdateEPSSurpriseDB(void);
	virtual bool UpdateDayLineDB(void);

	void UpdateBasicFinancialMetric(CSetFinnhubStockBasicFinancialMetric& set) const { m_pBasicFinancial->UpdateMetric(set); }
	void AppendBasicFinancialMetric(CSetFinnhubStockBasicFinancialMetric& set) const { m_pBasicFinancial->AppendMetric(set); }
	void AppendBasicFinancialAnnual(void);
	void AppendBasicFinancialQuarter(void);

	void UpdateDayLine(vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UpdateCompanyNews(CCompanyNewsVectorPtr pvCompanyNews);
	void UpdateEPSSurprise(vector<CEPSSurprisePtr>& vEPSSurprise);

	bool IsNeedUpdateProfile(CTiingoStockPtr pTiingoStock);
	void UpdateStockProfile(CTiingoStockPtr pTiingoStock);

	void UpdateDayLineStartEndDate(void);
	long GetDayLineSize(void) const noexcept { return m_dataDayLine.Size(); }
	CDayLinePtr GetDayLine(const long lIndex) const { return dynamic_pointer_cast<CDayLine>(m_dataDayLine.GetData(lIndex)); }
	void UnloadDayLine(void) { m_dataDayLine.Unload(); }

	bool HaveNewDayLineData(void);

	bool IsUpdateCompanyProfile(void) const noexcept { return m_fUpdateCompanyProfile; }
	void SetUpdateCompanyProfile(const bool fFlag) noexcept { m_fUpdateCompanyProfile = fFlag; }

	bool IsUpdateCompanyNews(void) const noexcept { return m_fUpdateCompanyNews; }
	void SetUpdateCompanyNews(const bool fFlag) noexcept { m_fUpdateCompanyNews = fFlag; }
	size_t GetCompanyNewsSize(void) const noexcept { return m_vCompanyNews.size(); }
	long long GetCompanyNewsDateTime(const int iIndex) const { return m_vCompanyNews.at(iIndex)->m_llDateTime; }

	bool IsUpdateBasicFinancial(void) const noexcept { return m_fUpdateBasicFinancial; }
	void SetUpdateBasicFinancial(const bool fFlag) noexcept { m_fUpdateBasicFinancial = fFlag; }
	bool IsUpdateBasicFinancialDB(void) const noexcept { return m_fUpdateFinnhubBasicFinancialDB; }
	void SetUpdateBasicFinancialDB(const bool fFlag) noexcept { m_fUpdateFinnhubBasicFinancialDB = fFlag; }

	bool IsUpdateBasicFinancialDBAndClearFlag(void) {
		const bool fNeedSave = m_fUpdateFinnhubBasicFinancialDB.exchange(false);
		return fNeedSave;
	}

	bool UpdateBasicFinancial(CFinnhubStockBasicFinancialPtr pFinnhubStockBasicFinancial);
	CFinnhubStockBasicFinancialPtr GetBasicFinancial(void) noexcept { return m_pBasicFinancial; }

	bool IsEPSSurpriseUpdated(void) const noexcept { return m_fEPSSurpriseUpdated; }
	void SetEPSSurpriseUpdated(const bool fFlag) noexcept { m_fEPSSurpriseUpdated = fFlag; }
	bool CheckEPSSurpriseStatus(long lCurrentDate);
	bool IsEPSSurpriseNeedSave(void) const noexcept { return m_fEPSSurpriseNeedSave; }
	void SetEPSSurpriseNeedSave(const bool fFlag) noexcept { m_fEPSSurpriseNeedSave = fFlag; }
	bool IsEPSSurpriseNeedSaveAndClearFlag(void);

	bool IsUpdatePeer(void) const noexcept { return m_fUpdateFinnhubPeer; }
	void SetUpdatePeer(bool fFlag) noexcept { m_fUpdateFinnhubPeer = fFlag; }
	bool CheckPeerStatus(long lCurrentDate);

	bool HaveInsiderTransaction(void) const noexcept {
		if (!m_vInsiderTransaction.empty()) return true;
		return false;
	}

	void UnloadInsiderTransaction(void) { m_vInsiderTransaction.resize(0); }
	void UpdateInsiderTransaction(vector<CInsiderTransactionPtr>& vInsiderTransaction);
	bool IsUpdateInsiderTransaction(void) const noexcept { return m_fUpdateFinnhubInsiderTransaction; }
	void SetUpdateInsiderTransaction(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderTransaction = fFlag; }
	bool CheckInsiderTransactionStatus(long lCurrentDate);
	bool IsSaveInsiderTransaction(void) const noexcept { return m_fSaveFinnhubInsiderTransaction; }
	void SetSaveInsiderTransaction(const bool fFlag) noexcept { m_fSaveFinnhubInsiderTransaction = fFlag; }

	bool IsSaveInsiderTransactionAndClearFlag(void) {
		const bool fNeedSave = m_fSaveFinnhubInsiderTransaction.exchange(false);
		return fNeedSave;
	}

	bool HaveInsiderSentiment(void) const noexcept {
		if (m_vInsiderSentiment.size() > 0) return true;
		return false;
	}

	void UnloadInsiderSentiment(void) { m_vInsiderSentiment.resize(0); }
	void UpdateInsiderSentiment(const vector<CInsiderSentimentPtr>& vInsiderSentiment);
	bool IsUpdateInsiderSentiment(void) const noexcept { return m_fUpdateFinnhubInsiderSentiment; }
	void SetUpdateInsiderSentiment(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderSentiment = fFlag; }
	bool CheckInsiderSentimentStatus(long lCurrentDate);
	bool IsSaveInsiderSentiment(void) const noexcept { return m_fSaveFinnhubInsiderSentiment; }
	void SetSaveInsiderSentiment(const bool fFlag) noexcept { m_fSaveFinnhubInsiderSentiment = fFlag; }

	bool IsSaveInsiderSentimentAndClearFlag(void) {
		const bool fNeedSave = m_fSaveFinnhubInsiderSentiment.exchange(false);
		return fNeedSave;
	}

	CString GetType(void) const { return m_strType; }
	void SetType(const CString& strType) { m_strType = strType; }
	CString GetMic(void) const { return m_strMic; }
	void SetMic(const CString& strMic) { m_strMic = strMic; }
	CString GetFigi(void) const { return m_strFigi; }
	void SetFigi(const CString& strFigi) { m_strFigi = strFigi; }
	CString GetShareClassFIGI(void) const { return m_strShareClassFIGI; }
	void SetShareClassFIGI(const CString& strFigi) { m_strShareClassFIGI = strFigi; }
	CString GetSymbol2(void) const { return m_strSymbol2; }
	void SetSymbol2(const CString& str) { m_strSymbol2 = str; }
	CString GetCurrency(void) const { return m_strCurrency; }
	void SetCurrency(const CString& strCurrency) { m_strCurrency = strCurrency; }

	CString GetAddress(void) const { return m_strAddress; }
	void SetAddress(const CString& strAddress) { m_strAddress = strAddress; }
	CString GetCity(void) const { return m_strCity; }
	void SetCity(const CString& strCity) { m_strCity = strCity; }
	CString GetCountry(void) const { return m_strCountry; }
	void SetCountry(const CString& strCountry) { m_strCountry = strCountry; }
	CString GetCusip(void) const { return m_strCusip; }
	void SetCusip(const CString& strCusip) { m_strCusip = strCusip; }
	CString GetSedol(void) const { return m_strSedol; }
	void SetSedol(const CString& strSedol) { m_strSedol = strSedol; }
	long GetEmployeeTotal(void) const noexcept { return m_lEmployeeTotal; }
	void SetEmployeeTotal(const long lEmployeeTotal) noexcept { m_lEmployeeTotal = lEmployeeTotal; }
	CString GetListedExchange(void) const { return m_strListedExchange; }
	void SetListedExchange(const CString& strListedExchange) { m_strListedExchange = strListedExchange; }
	CString GetGgroup(void) const { return m_strGgroup; }
	void SetGgroup(const CString& strGgroup) { m_strGgroup = strGgroup; }
	CString GetGind(void) const { return m_strGind; }
	void SetGind(const CString& strGind) { m_strGind = strGind; }
	CString GetGsector(void) const { return m_strGsector; }
	void SetGsector(const CString& strGsector) { m_strGsector = strGsector; }
	CString GetGsubind(void) const { return m_strGsubind; }
	void SetGsubind(const CString& strGsubind) { m_strGsubind = strGsubind; }
	CString GetIPODate(void) const { return m_strIPODate; }
	void SetIPODate(const CString& strIPODate) { m_strIPODate = strIPODate; }
	CString GetIsin(void) const { return m_strIsin; }
	void SetIsin(const CString& strIsin) { m_strIsin = strIsin; }
	double GetMarketCapitalization(void) const noexcept { return m_dMarketCapitalization; }
	void SetMarketCapitalization(double dMarketCapitalization) noexcept { m_dMarketCapitalization = dMarketCapitalization; }
	CString GetNaics(void) const { return m_strNaics; }
	void SetNaics(const CString& strNaics) { m_strNaics = strNaics; }
	CString GetNaicsNationalIndustry(void) const { return m_strNaicsNationalIndustry; }
	void SetNaicsNationalIndustry(const CString& strNaicsNationalIndustry) { m_strNaicsNationalIndustry = strNaicsNationalIndustry; }
	CString GetNaicsSector(void) const { return m_strNaicsSector; }
	void SetNaicsSector(const CString& strNaicsSector) { m_strNaicsSector = strNaicsSector; }
	CString GetNaicsSubsector(void) const { return m_strNaicsSubsector; }
	void SetNaicsSubsector(const CString& strNaicsSubsector) { m_strNaicsSubsector = strNaicsSubsector; }
	CString GetName(void) const { return m_strName; }
	void SetName(const CString& strName) { m_strName = strName; }
	CString GetPhone(void) const { return m_strPhone; }
	void SetPhone(const CString& strPhone) { m_strPhone = strPhone; }
	double GetShareOutstanding(void) const noexcept { return m_dShareOutstanding; }
	void SetShareOutstanding(const double dShareOutstanding) noexcept { m_dShareOutstanding = dShareOutstanding; }
	CString GetState(void) const { return m_strState; }
	void SetState(const CString& strState) { m_strState = strState; }
	CString GetTicker(void) const { return m_strTicker; }
	void SetTicker(const CString& strTicker) { m_strTicker = strTicker; }
	CString GetWebURL(void) const { return m_strWebURL; }
	void SetWebURL(const CString& strWebURL) { m_strWebURL = strWebURL; }
	CString GetLogo(void) const { return m_strLogo; }
	void SetLogo(const CString& strLogo) { m_strLogo = strLogo; }
	CString GetFinnhubIndustry(void) const { return m_strFinnhubIndustry; }
	void SetFinnhubIndustry(const CString& strFinnhubIndustry) { m_strFinnhubIndustry = strFinnhubIndustry; }
	json GetPeer(void) { return m_jsonPeer; }
	void SetPeer(const json& jsonPeer) { m_jsonPeer = jsonPeer; }
	long GetProfileUpdateDate();
	void SetProfileUpdateDate(long lProfileUpdateDate) noexcept;
	long GetCompanyNewsUpdateDate();
	void SetCompanyNewsUpdateDate(long lCompanyNewsUpdateDate) noexcept;
	long GetBasicFinancialUpdateDate();
	void SetBasicFinancialUpdateDate(long lBasicFinancialUpdateDate) noexcept;
	long GetLastRTDataUpdateDate();
	void SetLastRTDataUpdateDate(long lDate) noexcept;
	long GetPeerUpdateDate();
	void SetPeerUpdateDate(long lDate) noexcept;
	long GetInsiderTransactionUpdateDate();
	void SetInsiderTransactionUpdateDate(long lDate) noexcept;
	long GetInsiderSentimentUpdateDate();
	void SetInsiderSentimentUpdateDate(long lDate) noexcept;
	long GetLastEPSSurpriseUpdateDate();
	void SetLastEPSSurpriseUpdateDate(long lDate) noexcept;

	CString GetTiingoPermaTicker(void) noexcept { return m_strTiingoPermaTicker; }
	void SetTiingoPermaTicker(const CString& strTiingoPermaTicker) noexcept { m_strTiingoPermaTicker = strTiingoPermaTicker; }
	bool TiingoIsActive(void) const noexcept { return m_fIsActive; }
	void TiingoSetActive(const bool fIsActive) noexcept { m_fIsActive = fIsActive; }
	bool IsADR(void) const noexcept { return m_fIsADR; }
	void SetADR(const bool fIsADR) noexcept { m_fIsADR = fIsADR; }
	INT32 GetSICCode(void) const noexcept { return m_iSICCode; }
	void SetSICCode(const INT32 iSICCode) noexcept { m_iSICCode = iSICCode; }
	CString GetSICIndustry(void) noexcept { return m_strSICIndustry; }
	void SetSICIndustry(const CString& strSICIndustry) noexcept { m_strSICIndustry = strSICIndustry; }
	CString GetSICSector(void) noexcept { return m_strSICSector; }
	void SetSICSector(const CString& strSICSector) noexcept { m_strSICSector = strSICSector; }
	CString GetTiingoIndustry(void) noexcept { return m_strTiingoIndustry; }
	void SetTiingoIndustry(const CString& strTiingoIndustry) noexcept { m_strTiingoIndustry = strTiingoIndustry; }
	CString GetTiingoSector(void) noexcept { return m_strTiingoSector; }
	void SetTiingoSector(const CString& strTiingoSector) noexcept { m_strTiingoSector = strTiingoSector; }
	CString GetCompanyWebSite(void) noexcept { return m_strCompanyWebSite; }
	void SetCompanyWebSite(const CString& strCompanyWebSite) noexcept { m_strCompanyWebSite = strCompanyWebSite; }
	CString GetSECFilingWebSite(void) noexcept { return m_strSECFilingWebSite; }
	void SetSECFilingWebSite(const CString& strSECFilingWebSite) noexcept { m_strSECFilingWebSite = strSECFilingWebSite; }

	long GetTiingoStatementUpdateDate();
	void SetTiingoStatementUpdateDate(long lStatementUpdateDate) noexcept;
	long GetTiingoDailyDataUpdateDate();
	void SetTiingoDailyDataUpdateDate(long lDailyDataUpdateDate) noexcept;
	CString GetFinnhubDayLineInquiryParam(time_t tCurrentTime);
	CString GetTiingoDayLineInquiryParam(long lStartDate, long lCurrentDate);

	bool IsUSMarket(void) const;

public:
	vector<CEPSSurprisePtr> m_vEPSSurprise;
	bool m_fEPSSurpriseUpdated;
	atomic_bool m_fEPSSurpriseNeedSave;

	vector<CInsiderTransactionPtr> m_vInsiderTransaction;
	long m_lInsiderTransactionStartDate;

	vector<CInsiderSentimentPtr> m_vInsiderSentiment;
	long m_lInsiderSentimentStartDate;

protected:
	// Finnhub symbol信息
	CString m_strType;
	CString m_strSymbol2; // Alternative ticker for exchanges with multiple tickers for 1 stock such as BSE
	CString m_strMic;
	CString m_strFigi; // FIGI identifier
	CString m_strShareClassFIGI; // Global Share Class FIGI
	CString m_strCurrency;

	// Finnhub profile信息
	CString m_strAddress;
	CString m_strCity;
	CString m_strCountry;
	CString m_strCusip;
	CString m_strSedol;
	long m_lEmployeeTotal;
	CString m_strListedExchange;
	CString m_strGgroup;
	CString m_strGind;
	CString m_strGsector;
	CString m_strGsubind;
	CString m_strIPODate;
	CString m_strIsin;
	double m_dMarketCapitalization;
	CString m_strNaics;
	CString m_strNaicsNationalIndustry;
	CString m_strNaicsSector;
	CString m_strNaicsSubsector;
	CString m_strName;
	CString m_strPhone;
	double m_dShareOutstanding;
	CString m_strState;
	CString m_strTicker;
	CString m_strWebURL;
	CString m_strLogo;
	CString m_strFinnhubIndustry;
	json m_jsonPeer; // 读取到的Peer数据为json制式
	json m_jsonUpdateDate; // 存储所有的更新日期（json格式）。使用这种方式存储后，当增加或减少更新日期时，无需修改相应数据表的结构。

	// Tiingo Symbol信息
	CString m_strTiingoPermaTicker; // Tiingo永久代码标识
	bool m_fIsActive; //
	bool m_fIsADR;
	INT32 m_iSICCode;
	CString m_strSICIndustry;
	CString m_strSICSector;
	CString m_strTiingoIndustry;
	CString m_strTiingoSector;
	CString m_strCompanyWebSite;
	CString m_strSECFilingWebSite;

	// 系统生成信息
	CDataWorldStockDayLine m_dataDayLine;

	vector<CCompanyNewsPtr> m_vCompanyNews;

	CFinnhubStockBasicFinancialPtr m_pBasicFinancial;

	// 无需存储数据区
	bool m_fUpdateCompanyProfile; // 更新公司简介
	bool m_fUpdateCompanyNews; // 更新公司新闻
	bool m_fUpdateBasicFinancial; // 更新基本财务
	bool m_fUpdateFinnhubPeer; // 更新同业公司数据
	bool m_fUpdateFinnhubInsiderTransaction; // 公司内部交易数据已更新
	bool m_fUpdateFinnhubInsiderSentiment; // 公司内部交易情绪数据已更新
	atomic_bool m_fUpdateFinnhubBasicFinancialDB; // 基本财务数据需要保存
	atomic_bool m_fSaveFinnhubInsiderTransaction; // 内部交易数据需要存储
	atomic_bool m_fSaveFinnhubInsiderSentiment; // 内部交易情绪数据需要存储
};

using CWorldStockPtr = shared_ptr<CWorldStock>;
using CWorldStockVectorPtr = shared_ptr<vector<CWorldStockPtr>>;
