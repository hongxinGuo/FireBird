#pragma once

#include"nlohmannJsonDeclaration.h"
#include"nlohmann/json.hpp"

#include"VirtualStock.h"
#include"TiingoStock.h"

#include"FinnhubStockBasicFinancial.h"

#include"DayLine.h"
#include"ContainerWorldStockDayLine.h"
#include "EPSSurprise.h"
#include"FinnhubCompanyNews.h"
#include "InsiderSentiment.h"
#include "InsiderTransaction.h"
#include "SECFiling.h"

#include"SetWorldStock.h"

class CWorldStock : public CVirtualStock {
public:
	CWorldStock();
	// �������ƺ͸�ֵ��
	CWorldStock(const CWorldStock&) = delete;
	CWorldStock& operator=(const CWorldStock&) = delete;
	CWorldStock(const CWorldStock&&) noexcept = delete;
	CWorldStock& operator=(const CWorldStock&&) noexcept = delete;
	~CWorldStock() override;

	void ResetAllUpdateDate() override;
	int GetRatio() const final { return 1000; }

	void Load(CSetWorldStock& setWorldStock);
	void CheckUpdateStatus(long lTodayDate);
	void CheckProfileUpdateStatus(long lTodayDate);
	bool CheckCompanyNewsUpdateStatus(long lTodayDate);
	bool CheckBasicFinancialUpdateStatus(long lTodayDate);
	bool CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek);
	void Save(CSetWorldStock& setWorldStock) const;
	void Update(CSetWorldStock& setWorldStock) const;
	void Append(CSetWorldStock& setWorldStock) const;
	void SaveDayLine() { m_dataDayLine.SaveDB(m_strSymbol); }
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
	long GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	CDayLinePtr GetDayLine(const long lIndex) const { return dynamic_pointer_cast<CDayLine>(m_dataDayLine.GetData(lIndex)); }
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
	bool IsUpdateBasicFinancialDBAndClearFlag() { return m_fUpdateFinnhubBasicFinancialDB.exchange(false); }

	bool UpdateBasicFinancial(const CFinnhubStockBasicFinancialPtr& pFinnhubStockBasicFinancial);
	CFinnhubStockBasicFinancialPtr GetBasicFinancial() noexcept { return m_pBasicFinancial; }

	bool IsUpdateEPSSurprise() const noexcept { return m_fUpdateEPSSurprise; }
	void SetUpdateEPSSurprise(const bool fFlag) noexcept { m_fUpdateEPSSurprise = fFlag; }
	bool CheckEPSSurpriseStatus(long lCurrentDate);
	bool IsUpdateEPSSurpriseDB() const noexcept { return m_fUpdateEPSSurpriseDB; }
	void SetUpdateEPSSurpriseDB(const bool fFlag) noexcept { m_fUpdateEPSSurpriseDB = fFlag; }
	bool IsUpdateEPSSurpriseDBAndClearFlag() { return m_fUpdateEPSSurpriseDB.exchange(false); }

	bool IsUpdatePeer() const noexcept { return m_fUpdateFinnhubPeer; }
	void SetUpdatePeer(bool fFlag) noexcept { m_fUpdateFinnhubPeer = fFlag; }
	bool CheckPeerStatus(long lCurrentDate);

	bool IsSECFilingsUpdated() const noexcept { return m_fSECFilingsUpdated; }
	void SetSECFilingsUpdated(bool fFlag) noexcept { m_fSECFilingsUpdated = fFlag; }
	bool CheckSECFilingsStatus(long lCurrentDate);
	bool IsUpdateSECFilingsDB() const noexcept { return m_fUpdateSECFilingsDB; }
	void SetUpdateSECFilingsDB(const bool fFlag) noexcept { m_fUpdateSECFilingsDB = fFlag; }
	bool IsUpdateSECFilingsDBAndClearFlag() { return m_fUpdateSECFilingsDB.exchange(false); }

	bool HaveInsiderTransaction() const noexcept { return !m_vInsiderTransaction.empty(); }
	void UnloadInsiderTransaction() { m_vInsiderTransaction.resize(0); }
	void UpdateInsiderTransaction(const vector<CInsiderTransactionPtr>& vInsiderTransaction);
	bool IsUpdateInsiderTransaction() const noexcept { return m_fUpdateFinnhubInsiderTransaction; }
	void SetUpdateInsiderTransaction(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderTransaction = fFlag; }
	bool CheckInsiderTransactionStatus(long lCurrentDate);
	bool IsUpdateInsiderTransactionDB() const noexcept { return m_fUpdateFinnhubInsiderTransactionDB; }
	void SetUpdateInsiderTransactionDB(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderTransactionDB = fFlag; }
	bool IsUpdateInsiderTransactionDBAndClearFlag() { return m_fUpdateFinnhubInsiderTransactionDB.exchange(false); }

	bool HaveInsiderSentiment() const noexcept { return !m_vInsiderSentiment.empty(); }
	void UnloadInsiderSentiment() { m_vInsiderSentiment.resize(0); }
	void UpdateInsiderSentiment(const vector<CInsiderSentimentPtr>& vInsiderSentiment);
	bool IsUpdateInsiderSentiment() const noexcept { return m_fUpdateFinnhubInsiderSentiment; }
	void SetUpdateInsiderSentiment(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderSentiment = fFlag; }
	bool CheckInsiderSentimentStatus(long lCurrentDate);
	bool IsUpdateInsiderSentimentDB() const noexcept { return m_fUpdateFinnhubInsiderSentimentDB; }
	void SetUpdateInsiderSentimentDB(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderSentimentDB = fFlag; }
	bool IsUpdateInsiderSentimentDBAndClearFlag() { return m_fUpdateFinnhubInsiderSentimentDB.exchange(false); }

	CString GetType() const { return m_strType; }
	void SetType(const CString& strType) { m_strType = strType; }
	CString GetMic() const { return m_strMic; }
	void SetMic(const CString& strMic) { m_strMic = strMic; }
	CString GetFigi() const { return m_strFigi; }
	void SetFigi(const CString& strFigi) { m_strFigi = strFigi; }
	CString GetShareClassFIGI() const { return m_strShareClassFIGI; }
	void SetShareClassFIGI(const CString& strFigi) { m_strShareClassFIGI = strFigi; }
	CString GetSymbol2() const { return m_strSymbol2; }
	void SetSymbol2(const CString& str) { m_strSymbol2 = str; }
	CString GetCurrency() const { return m_strCurrency; }
	void SetCurrency(const CString& strCurrency) { m_strCurrency = strCurrency; }

	CString GetAddress() const { return m_strAddress; }
	void SetAddress(const CString& strAddress) { m_strAddress = strAddress; }
	CString GetCity() const { return m_strCity; }
	void SetCity(const CString& strCity) { m_strCity = strCity; }
	CString GetCountry() const { return m_strCountry; }
	void SetCountry(const CString& strCountry) { m_strCountry = strCountry; }
	CString GetCusip() const { return m_strCusip; }
	void SetCusip(const CString& strCusip) { m_strCusip = strCusip; }
	CString GetSedol() const { return m_strSedol; }
	void SetSedol(const CString& strSedol) { m_strSedol = strSedol; }
	long GetEmployeeTotal() const noexcept { return m_lEmployeeTotal; }
	void SetEmployeeTotal(const long lEmployeeTotal) noexcept { m_lEmployeeTotal = lEmployeeTotal; }
	CString GetGgroup() const { return m_strGgroup; }
	void SetGgroup(const CString& strGgroup) { m_strGgroup = strGgroup; }
	CString GetGind() const { return m_strGind; }
	void SetGind(const CString& strGind) { m_strGind = strGind; }
	CString GetGsector() const { return m_strGsector; }
	void SetGsector(const CString& strGsector) { m_strGsector = strGsector; }
	CString GetGsubind() const { return m_strGsubind; }
	void SetGsubind(const CString& strGsubind) { m_strGsubind = strGsubind; }
	CString GetIPODate() const { return m_strIPODate; }
	void SetIPODate(const CString& strIPODate) { m_strIPODate = strIPODate; }
	CString GetIsin() const { return m_strIsin; }
	void SetIsin(const CString& strIsin) { m_strIsin = strIsin; }
	double GetMarketCapitalization() const noexcept { return m_dMarketCapitalization; }
	void SetMarketCapitalization(double dMarketCapitalization) noexcept { m_dMarketCapitalization = dMarketCapitalization; }
	CString GetNaics() const { return m_strNaics; }
	void SetNaics(const CString& strNaics) { m_strNaics = strNaics; }
	CString GetNaicsNationalIndustry() const { return m_strNaicsNationalIndustry; }
	void SetNaicsNationalIndustry(const CString& strNaicsNationalIndustry) { m_strNaicsNationalIndustry = strNaicsNationalIndustry; }
	CString GetNaicsSector() const { return m_strNaicsSector; }
	void SetNaicsSector(const CString& strNaicsSector) { m_strNaicsSector = strNaicsSector; }
	CString GetNaicsSubsector() const { return m_strNaicsSubsector; }
	void SetNaicsSubsector(const CString& strNaicsSubsector) { m_strNaicsSubsector = strNaicsSubsector; }
	CString GetName() const { return m_strName; }
	void SetName(const CString& strName) { m_strName = strName; }
	CString GetPhone() const { return m_strPhone; }
	void SetPhone(const CString& strPhone) { m_strPhone = strPhone; }
	double GetShareOutstanding() const noexcept { return m_dShareOutstanding; }
	void SetShareOutstanding(const double dShareOutstanding) noexcept { m_dShareOutstanding = dShareOutstanding; }
	CString GetState() const { return m_strState; }
	void SetState(const CString& strState) { m_strState = strState; }
	CString GetTicker() const { return m_strTicker; }
	void SetTicker(const CString& strTicker) { m_strTicker = strTicker; }
	CString GetWebURL() const { return m_strWebURL; }
	void SetWebURL(const CString& strWebURL) { m_strWebURL = strWebURL; }
	CString GetLogo() const { return m_strLogo; }
	void SetLogo(const CString& strLogo) { m_strLogo = strLogo; }
	CString GetFinnhubIndustry() const { return m_strFinnhubIndustry; }
	void SetFinnhubIndustry(const CString& strFinnhubIndustry) { m_strFinnhubIndustry = strFinnhubIndustry; }
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

	CString GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const;
	CString GetTiingoDayLineInquiryParam(long lStartDate, long lCurrentDate) const;
	CString GetFinnhubInsiderTransactionInquiryParam(time_t tCurrentTime);

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
	bool m_fSECFilingsUpdated{ false };
	atomic_bool m_fUpdateSECFilingsDB{ false };

protected:
	// Finnhub symbol��Ϣ
	CString m_strType{ _T(" ") };
	CString m_strSymbol2{ _T(" ") }; // Alternative ticker for exchanges with multiple tickers for 1 stock such as BSE
	CString m_strMic{ _T(" ") };
	CString m_strFigi{ _T(" ") }; // FIGI identifier
	CString m_strShareClassFIGI{ _T(" ") }; // Global Share Class FIGI
	CString m_strCurrency{ _T(" ") };

	// Finnhub profile��Ϣ
	CString m_strAddress{ _T(" ") };
	CString m_strCity{ _T(" ") };
	CString m_strCountry{ _T(" ") };
	CString m_strCusip{ _T(" ") };
	CString m_strSedol{ _T(" ") };
	long m_lEmployeeTotal{ 0 };
	//CString m_strListedExchange{ _T("US") }; // ʹ��virtualStock�е�ExchangeCode
	CString m_strGgroup{ _T(" ") };
	CString m_strGind{ _T(" ") };
	CString m_strGsector{ _T(" ") };
	CString m_strGsubind{ _T(" ") };
	CString m_strIPODate{ _T(" ") };
	CString m_strIsin{ _T(" ") };
	double m_dMarketCapitalization{ 0 };
	CString m_strNaics{ _T(" ") };
	CString m_strNaicsNationalIndustry{ _T(" ") };
	CString m_strNaicsSector{ _T(" ") };
	CString m_strNaicsSubsector{ _T(" ") };
	CString m_strName{ _T(" ") };
	CString m_strPhone{ _T(" ") };
	double m_dShareOutstanding{ 0 };
	CString m_strState{ _T(" ") };
	CString m_strTicker{ _T(" ") };
	CString m_strWebURL{ _T(" ") };
	CString m_strLogo{ _T(" ") };
	CString m_strFinnhubIndustry{ _T(" ") };
	json m_jsonPeer{ json({}) }; // ��ȡ����Peer����Ϊjson��ʽ

	// ϵͳ������Ϣ
	CContainerWorldStockDayLine m_dataDayLine;

	vector<CCompanyNewsPtr> m_vCompanyNews;

	CFinnhubStockBasicFinancialPtr m_pBasicFinancial{ nullptr };

	// ����洢������
	bool m_fUpdateCompanyProfile{ true }; // ���¹�˾���
	bool m_fUpdateCompanyNews{ true }; // ���¹�˾����
	bool m_fUpdateBasicFinancial{ true }; // ���»�������
	bool m_fUpdateFinnhubPeer{ true }; // ����ͬҵ��˾����
	bool m_fUpdateFinnhubInsiderTransaction{ true }; // ���¹�˾�ڲ���������
	bool m_fUpdateFinnhubInsiderSentiment{ true }; // ���¹�˾�ڲ�������������

	atomic_bool m_fUpdateFinnhubBasicFinancialDB{ false }; // ��������������Ҫ����
	atomic_bool m_fUpdateFinnhubInsiderTransactionDB{ false }; // �ڲ�����������Ҫ�洢
	atomic_bool m_fUpdateFinnhubInsiderSentimentDB{ false }; // �ڲ���������������Ҫ�洢
};

using CWorldStockPtr = shared_ptr<CWorldStock>;
using CWorldStocksPtr = shared_ptr<vector<CWorldStockPtr>>;
