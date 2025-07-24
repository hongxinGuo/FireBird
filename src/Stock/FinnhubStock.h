#pragma once

#include"nlohmannJsonDeclaration.h"
#include"nlohmann/json.hpp"

#include"VirtualStock.h"

#include"FinnhubStockBasicFinancial.h"

#include"DayLine.h"
#include"ContainerFinnhubStockDayLine.h"
#include "EPSSurprise.h"
#include"FinnhubCompanyNews.h"
#include "InsiderSentiment.h"
#include "InsiderTransaction.h"
#include "SECFiling.h"

#include"SetFinnhubStock.h"

class CFinnhubStock : public CVirtualStock {
public:
	CFinnhubStock();
	// 不允许复制和赋值。
	CFinnhubStock(const CFinnhubStock&) = delete;
	CFinnhubStock& operator=(const CFinnhubStock&) = delete;
	CFinnhubStock(const CFinnhubStock&&) noexcept = delete;
	CFinnhubStock& operator=(const CFinnhubStock&&) noexcept = delete;
	~CFinnhubStock() override;

	void ResetAllUpdateDate() override;
	int GetRatio() const final { return 1000; }

	void Load(const CSetFinnhubStock& setFinnhubStock);
	void CheckUpdateStatus(long lTodayDate);
	void CheckProfileUpdateStatus(long lTodayDate);
	bool CheckCompanyNewsUpdateStatus(long lTodayDate);
	bool CheckBasicFinancialUpdateStatus(long lTodayDate);
	bool CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek);
	void Save(CSetFinnhubStock& setFinnhubStock) const;
	void Update(CSetFinnhubStock& setFinnhubStock) const;
	void Append(CSetFinnhubStock& setFinnhubStock) const;
	void SaveDayLineDB() { m_dataDayLine.SaveDB(m_strSymbol); }
	void UpdateInsiderTransactionDB();
	void UpdateInsiderSentimentDB();
	virtual bool UpdateCompanyNewsDB();
	virtual bool UpdateEPSSurpriseDB();
	bool UpdateSECFilingsDB() const;
	virtual bool UpdateDayLineDB();

	void UpdateBasicFinancialMetric(CSetFinnhubStockBasicFinancialMetric& set) const { m_pBasicFinancial->UpdateMetric(set); }
	void AppendBasicFinancialMetric(CSetFinnhubStockBasicFinancialMetric& set) const { m_pBasicFinancial->AppendMetric(set); }
	void AppendBasicFinancialAnnual() const;
	void AppendBasicFinancialQuarter() const;

	void UpdateDayLine(const vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UpdateCompanyNews(const CCompanyNewssPtr& pvCompanyNews);
	void UpdateEPSSurprise(const vector<CEPSSurprisePtr>& vEPSSurprise);

	void UpdateDayLineStartEndDate();
	auto GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	CDayLinePtr GetDayLine(const size_t lIndex) const { return dynamic_pointer_cast<CDayLine>(m_dataDayLine.GetData(lIndex)); }
	void UnloadDayLine() { m_dataDayLine.Unload(); }

	bool HaveNewDayLineData();

	bool IsUpdateCompanyProfile() const noexcept { return m_fUpdateCompanyProfile; }
	void SetUpdateCompanyProfile(const bool fFlag) noexcept { m_fUpdateCompanyProfile = fFlag; }

	bool IsUpdateCompanyNews() const noexcept { return m_fUpdateCompanyNews; }
	void SetUpdateCompanyNews(const bool fFlag) noexcept { m_fUpdateCompanyNews = fFlag; }
	size_t GetCompanyNewsSize() const noexcept { return m_vCompanyNews.size(); }
	long long GetCompanyNewsDateTime(const int iIndex) const { return m_vCompanyNews.at(iIndex)->m_llDateTime; }

	bool IsUpdateBasicFinancial() const noexcept { return m_fUpdateBasicFinancial; }
	void SetUpdateBasicFinancial(const bool fFlag) noexcept { m_fUpdateBasicFinancial = fFlag; }
	bool IsUpdateBasicFinancialDB() const noexcept { return m_fUpdateFinnhubBasicFinancialDB; }
	void SetUpdateBasicFinancialDB(const bool fFlag) noexcept { m_fUpdateFinnhubBasicFinancialDB = fFlag; }

	bool UpdateBasicFinancial(const CFinnhubStockBasicFinancialPtr& pFinnhubStockBasicFinancial);
	CFinnhubStockBasicFinancialPtr GetBasicFinancial() noexcept { return m_pBasicFinancial; }

	bool IsUpdateEPSSurprise() const noexcept { return m_fUpdateEPSSurprise; }
	void SetUpdateEPSSurprise(const bool fFlag) noexcept { m_fUpdateEPSSurprise = fFlag; }
	bool CheckEPSSurpriseStatus(long lCurrentDate);
	bool IsUpdateEPSSurpriseDB() const noexcept { return m_fUpdateEPSSurpriseDB; }
	void SetUpdateEPSSurpriseDB(const bool fFlag) noexcept { m_fUpdateEPSSurpriseDB = fFlag; }

	bool IsUpdatePeer() const noexcept { return m_fUpdateFinnhubPeer; }
	void SetUpdatePeer(bool fFlag) noexcept { m_fUpdateFinnhubPeer = fFlag; }
	bool CheckPeerStatus(long lCurrentDate);

	bool IsUpdateSECFilings() const noexcept { return m_fUpdateSECFilings; }
	void SetUpdateSECFilings(bool fFlag) noexcept { m_fUpdateSECFilings = fFlag; }
	bool CheckSECFilingsStatus(long lCurrentDate);
	bool IsUpdateSECFilingsDB() const noexcept { return m_fUpdateSECFilingsDB; }
	void SetUpdateSECFilingsDB(const bool fFlag) noexcept { m_fUpdateSECFilingsDB = fFlag; }

	bool HaveInsiderTransaction() const noexcept { return !m_vInsiderTransaction.empty(); }
	void UnloadInsiderTransaction() { m_vInsiderTransaction.resize(0); }
	void UpdateInsiderTransaction(const vector<CInsiderTransactionPtr>& vInsiderTransaction);
	bool IsUpdateInsiderTransaction() const noexcept { return m_fUpdateFinnhubInsiderTransaction; }
	void SetUpdateInsiderTransaction(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderTransaction = fFlag; }
	bool CheckInsiderTransactionStatus(long lCurrentDate);
	bool IsUpdateInsiderTransactionDB() const noexcept { return m_fUpdateFinnhubInsiderTransactionDB; }
	void SetUpdateInsiderTransactionDB(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderTransactionDB = fFlag; }

	bool HaveInsiderSentiment() const noexcept { return !m_vInsiderSentiment.empty(); }
	void UnloadInsiderSentiment() { m_vInsiderSentiment.resize(0); }
	void UpdateInsiderSentiment(const vector<CInsiderSentimentPtr>& vInsiderSentiment);
	bool IsUpdateInsiderSentiment() const noexcept { return m_fUpdateFinnhubInsiderSentiment; }
	void SetUpdateInsiderSentiment(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderSentiment = fFlag; }
	bool CheckInsiderSentimentStatus(long lCurrentDate);
	bool IsUpdateInsiderSentimentDB() const noexcept { return m_fUpdateFinnhubInsiderSentimentDB; }
	void SetUpdateInsiderSentimentDB(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderSentimentDB = fFlag; }

	string GetType() const { return m_strType; }
	void SetType(const string& strType) { m_strType = strType; }
	string GetMic() const { return m_strMic; }
	void SetMic(const string& strMic) { m_strMic = strMic; }
	string GetFigi() const { return m_strFigi; }
	void SetFigi(const string& strFigi) { m_strFigi = strFigi; }
	string GetShareClassFIGI() const { return m_strShareClassFIGI; }
	void SetShareClassFIGI(const string& strFigi) { m_strShareClassFIGI = strFigi; }
	string GetSymbol2() const { return m_strSymbol2; }
	void SetSymbol2(const string& str) { m_strSymbol2 = str; }
	string GetCurrency() const { return m_strCurrency; }
	void SetCurrency(const string& strCurrency) { m_strCurrency = strCurrency; }

	string GetAddress() const { return m_strAddress; }
	void SetAddress(const string& strAddress) { m_strAddress = strAddress; }
	string GetCity() const { return m_strCity; }
	void SetCity(const string& strCity) { m_strCity = strCity; }
	string GetCountry() const { return m_strCountry; }
	void SetCountry(const string& strCountry) { m_strCountry = strCountry; }
	string GetCusip() const { return m_strCusip; }
	void SetCusip(const string& strCusip) { m_strCusip = strCusip; }
	string GetSedol() const { return m_strSedol; }
	void SetSedol(const string& strSedol) { m_strSedol = strSedol; }
	long GetEmployeeTotal() const noexcept { return m_lEmployeeTotal; }
	void SetEmployeeTotal(const long lEmployeeTotal) noexcept { m_lEmployeeTotal = lEmployeeTotal; }
	string GetGgroup() const { return m_strGgroup; }
	void SetGgroup(const string& strGgroup) { m_strGgroup = strGgroup; }
	string GetGind() const { return m_strGind; }
	void SetGind(const string& strGind) { m_strGind = strGind; }
	string GetGsector() const { return m_strGsector; }
	void SetGsector(const string& strGsector) { m_strGsector = strGsector; }
	string GetGsubind() const { return m_strGsubind; }
	void SetGsubind(const string& strGsubind) { m_strGsubind = strGsubind; }
	string GetIPODate() const { return m_strIPODate; }
	void SetIPODate(const string& strIPODate) { m_strIPODate = strIPODate; }
	string GetIsin() const { return m_strIsin; }
	void SetIsin(const string& strIsin) { m_strIsin = strIsin; }
	double GetMarketCapitalization() const noexcept { return m_dMarketCapitalization; }
	void SetMarketCapitalization(double dMarketCapitalization) noexcept { m_dMarketCapitalization = dMarketCapitalization; }
	string GetNaics() const { return m_strNaics; }
	void SetNaics(const string& strNaics) { m_strNaics = strNaics; }
	string GetNaicsNationalIndustry() const { return m_strNaicsNationalIndustry; }
	void SetNaicsNationalIndustry(const string& strNaicsNationalIndustry) { m_strNaicsNationalIndustry = strNaicsNationalIndustry; }
	string GetNaicsSector() const { return m_strNaicsSector; }
	void SetNaicsSector(const string& strNaicsSector) { m_strNaicsSector = strNaicsSector; }
	string GetNaicsSubsector() const { return m_strNaicsSubsector; }
	void SetNaicsSubsector(const string& strNaicsSubsector) { m_strNaicsSubsector = strNaicsSubsector; }
	string GetName() const { return m_strName; }
	void SetName(const string& strName) { m_strName = strName; }
	string GetPhone() const { return m_strPhone; }
	void SetPhone(const string& strPhone) { m_strPhone = strPhone; }
	double GetShareOutstanding() const noexcept { return m_dShareOutstanding; }
	void SetShareOutstanding(const double dShareOutstanding) noexcept { m_dShareOutstanding = dShareOutstanding; }
	string GetState() const { return m_strState; }
	void SetState(const string& strState) { m_strState = strState; }
	string GetTicker() const { return m_strTicker; }
	void SetTicker(const string& strTicker) { m_strTicker = strTicker; }
	string GetWebURL() const { return m_strWebURL; }
	void SetWebURL(const string& strWebURL) { m_strWebURL = strWebURL; }
	string GetLogo() const { return m_strLogo; }
	void SetLogo(const string& strLogo) { m_strLogo = strLogo; }
	string GetFinnhubIndustry() const { return m_strFinnhubIndustry; }
	void SetFinnhubIndustry(const string& strFinnhubIndustry) { m_strFinnhubIndustry = strFinnhubIndustry; }
	json GetPeer() { return m_jsonPeer; }
	void SetPeer(const json& jsonPeer) { m_jsonPeer = jsonPeer; }
	void SetSECFilings(const CSECFilingsPtr& pv) noexcept { m_pvSECFilings = pv; }
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
	void SetSECFilingsUpdateDate(long lDate) noexcept;
	long GetSECFilingsUpdateDate();

	string GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const;
	string GetTiingoDayLineInquiryParam(long lStartDate, long lCurrentDate) const;
	string GetFinnhubInsiderTransactionInquiryParam(time_t tCurrentTime);

	bool IsUSMarket() const;

public:
	vector<CEPSSurprisePtr> m_vEPSSurprise;
	bool m_fUpdateEPSSurprise{ true };
	atomic_bool m_fUpdateEPSSurpriseDB{ false };

	vector<CInsiderTransactionPtr> m_vInsiderTransaction;
	long m_lInsiderTransactionEndDate{ 19800101 };

	vector<CInsiderSentimentPtr> m_vInsiderSentiment;
	long m_lInsiderSentimentStartDate{ 19800101 };

	CSECFilingsPtr m_pvSECFilings{ nullptr };
	bool m_fUpdateSECFilings{ true };
	atomic_bool m_fUpdateSECFilingsDB{ false };

protected:
	// Finnhub symbol信息
	string m_strType{ _T(" ") };
	string m_strSymbol2{ _T(" ") }; // Alternative ticker for exchanges with multiple tickers for 1 stock such as BSE
	string m_strMic{ _T(" ") };
	string m_strFigi{ _T(" ") }; // FIGI identifier
	string m_strShareClassFIGI{ _T(" ") }; // Global Share Class FIGI
	string m_strCurrency{ _T(" ") };

	// Finnhub profile信息
	string m_strAddress{ _T(" ") };
	string m_strCity{ _T(" ") };
	string m_strCountry{ _T(" ") };
	string m_strCusip{ _T(" ") };
	string m_strSedol{ _T(" ") };
	long m_lEmployeeTotal{ 0 };
	//string m_strListedExchange{ _T("US") }; // 使用virtualStock中的ExchangeCode
	string m_strGgroup{ _T(" ") };
	string m_strGind{ _T(" ") };
	string m_strGsector{ _T(" ") };
	string m_strGsubind{ _T(" ") };
	string m_strIPODate{ _T(" ") };
	string m_strIsin{ _T(" ") };
	double m_dMarketCapitalization{ 0 };
	string m_strNaics{ _T(" ") };
	string m_strNaicsNationalIndustry{ _T(" ") };
	string m_strNaicsSector{ _T(" ") };
	string m_strNaicsSubsector{ _T(" ") };
	string m_strName{ _T(" ") };
	string m_strPhone{ _T(" ") };
	double m_dShareOutstanding{ 0 };
	string m_strState{ _T(" ") };
	string m_strTicker{ _T(" ") };
	string m_strWebURL{ _T(" ") };
	string m_strLogo{ _T(" ") };
	string m_strFinnhubIndustry{ _T(" ") };
	json m_jsonPeer{ json({}) }; // 读取到的Peer数据为json制式

	// 系统生成信息
	CContainerFinnhubStockDayLine m_dataDayLine;

	vector<CCompanyNewsPtr> m_vCompanyNews;

	CFinnhubStockBasicFinancialPtr m_pBasicFinancial{ nullptr };

	// 无需存储数据区
	bool m_fUpdateCompanyProfile{ true }; // 更新公司简介
	bool m_fUpdateCompanyNews{ true }; // 更新公司新闻
	bool m_fUpdateBasicFinancial{ true }; // 更新基本财务
	bool m_fUpdateFinnhubPeer{ true }; // 更新同业公司数据
	bool m_fUpdateFinnhubInsiderTransaction{ true }; // 更新公司内部交易数据
	bool m_fUpdateFinnhubInsiderSentiment{ true }; // 更新公司内部交易情绪数据

	atomic_bool m_fUpdateFinnhubBasicFinancialDB{ false }; // 基本财务数据需要保存
	atomic_bool m_fUpdateFinnhubInsiderTransactionDB{ false }; // 内部交易数据需要存储
	atomic_bool m_fUpdateFinnhubInsiderSentimentDB{ false }; // 内部交易情绪数据需要存储
};

using CFinnhubStockPtr = shared_ptr<CFinnhubStock>;
using CFinnhubStocksPtr = shared_ptr<vector<CFinnhubStockPtr>>;
