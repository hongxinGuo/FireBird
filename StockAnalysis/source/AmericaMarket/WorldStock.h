#pragma once

#include"VirtualStock.h"
#include"TiingoStock.h"

#include"FinnhubStockBasicFinancial.h"

#include"DayLine.h"
#include"DataWorldStockDayLine.h"
#include"EPSSurprise.h"
#include"InsiderTransaction.h"

#include"SetWorldStock.h"
#include"SetWorldStockDayLine.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CWorldStock : public CVirtualStock {
public:
	CWorldStock();
	// 不允许复制和赋值。
	CWorldStock(const CWorldStock&) = delete;
	CWorldStock& operator=(const CWorldStock&) = delete;
	CWorldStock(const CWorldStock&&) noexcept = delete;
	CWorldStock& operator=(const CWorldStock&&) noexcept = delete;
	virtual ~CWorldStock();

	virtual void Reset(void) override;
	virtual int GetRatio(void) const override final { return 1000; }

public:
	void Load(CSetWorldStock& setWorldStock);
	void CheckUpdateStatus(long lTodayDate);
	bool CheckProfileUpdateStatus(long lTodayDate);
	bool CheckBasicFinancialUpdateStatus(long lTodayDate);
	bool CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek);
	void Save(CSetWorldStock& setWorldStock);
	void Update(CSetWorldStock& setWorldStock);
	void Append(CSetWorldStock& setWorldStock);
	void SaveDayLine(void) { m_dataDayLine.SaveDB(m_strSymbol); }
	void SaveInsiderTransaction(void);
	virtual bool UpdateEPSSurpriseDB(void);
	virtual bool UpdateDayLineDB(void);

	void UpdateBasicFinancialMetric(CSetFinnhubStockBasicFinancialMetric& set) { m_pBasicFinancial->UpdateMetric(set); }
	void AppendBasicFinancialMetric(CSetFinnhubStockBasicFinancialMetric& set) { m_pBasicFinancial->AppendMetric(set); }
	void AppendBasicFinancialAnnual(void);
	void AppendBasicFinancialQuarter(void);

	void UpdateDayLine(vector<CDayLinePtr>& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UpdateEPSSurprise(vector<CEPSSurprisePtr>& vEPSSurprise);

	bool IsNeedUpdateProfile(CTiingoStockPtr pTiingoStock);
	void UpdateStockProfile(CTiingoStockPtr pTiingoStock);

	void UpdateDayLineStartEndDate(void);
	long GetDayLineSize(void) const noexcept { return m_dataDayLine.Size(); }
	CDayLinePtr GetDayLine(long lIndex) const { return dynamic_pointer_cast<CDayLine>(m_dataDayLine.GetData(lIndex)); }
	void UnloadDayLine(void) { m_dataDayLine.Unload(); }

	bool HaveNewDayLineData(void);

	bool IsProfileUpdated(void) const noexcept { return m_fProfileUpdated; }
	void SetProfileUpdated(bool fFlag) noexcept { m_fProfileUpdated = fFlag; }

	bool IsBasicFinancialUpdated(void) const noexcept { return m_fBasicFinancialUpdated; }
	void SetBasicFinancialUpdated(bool fFlag) noexcept { m_fBasicFinancialUpdated = fFlag; }
	bool IsUpdateBasicFinancialDB(void) const noexcept { return m_fUpdateFinnhubBasicFinancialDB; }
	void SetUpdateBasicFinancialDB(bool fFlag) noexcept { m_fUpdateFinnhubBasicFinancialDB = fFlag; }
	bool IsUpdateBasicFinancialDBAndClearFlag(void) { const bool fNeedSave = m_fUpdateFinnhubBasicFinancialDB.exchange(false); return fNeedSave; }
	bool UpdateBasicFinancial(CFinnhubStockBasicFinancialPtr pFinnhubStockBasicFinancial);
	CFinnhubStockBasicFinancialPtr GetBasicFinancial(void) noexcept { return m_pBasicFinancial; }

	bool IsEPSSurpriseUpdated(void) const noexcept { return m_fEPSSurpriseUpdated; }
	void SetEPSSurpriseUpdated(bool fFlag) noexcept { m_fEPSSurpriseUpdated = fFlag; }
	bool CheckEPSSurpriseStatus(long lCurrentDate);
	bool IsEPSSurpriseNeedSave(void) const noexcept { return m_fEPSSurpriseNeedSave; }
	void SetEPSSurpriseNeedSave(bool fFlag) noexcept { m_fEPSSurpriseNeedSave = fFlag; }
	bool IsEPSSurpriseNeedSaveAndClearFlag(void);

	bool IsPeerUpdated(void) const noexcept { return m_fFinnhubPeerUpdated; }
	void SetPeerUpdated(bool fFlag) noexcept { m_fFinnhubPeerUpdated = fFlag; }
	bool CheckPeerStatus(long lCurrentDate);

	bool HaveInsiderTransaction(void) noexcept { if (m_vInsiderTransaction.size() > 0) return true; else return false; }
	void UnloadInsiderTransaction(void) { m_vInsiderTransaction.resize(0); }
	void UpdateInsiderTransaction(vector<CInsiderTransactionPtr>& vInsiderTransaction);
	bool IsInsiderTransactionNeedUpdate(void) const noexcept { return m_fFinnhubInsiderTransactionNeedUpdate; }
	void SetInsiderTransactionNeedUpdate(bool fFlag) noexcept { m_fFinnhubInsiderTransactionNeedUpdate = fFlag; }
	bool CheckInsiderTransactionStatus(long lCurrentDate);
	bool IsInsiderTransactionNeedSave(void) const noexcept { return m_fFinnhubInsiderTransactionNeedSave; }
	void SetInsiderTransactionNeedSave(bool fFlag) noexcept { m_fFinnhubInsiderTransactionNeedSave = fFlag; }
	bool IsInsiderTransactionNeedSaveAndClearFlag(void) { const bool fNeedSave = m_fFinnhubInsiderTransactionNeedSave.exchange(false); return fNeedSave; }

	CString GetType(void) const { return m_strType; }
	void SetType(CString strType) { m_strType = strType; }
	CString GetMic(void) const { return m_strMic; }
	void SetMic(CString strMic) { m_strMic = strMic; }
	CString GetFigi(void) const { return m_strFigi; }
	void SetFigi(CString strFigi) { m_strFigi = strFigi; }
	CString GetShareClassFIGI(void) const { return m_strShareClassFIGI; }
	void SetShareClassFIGI(CString strFigi) { m_strShareClassFIGI = strFigi; }
	CString GetSymbol2(void) const { return m_strSymbol2; }
	void SetSymbol2(CString str) { m_strSymbol2 = str; }
	CString GetCurrency(void) const { return m_strCurrency; }
	void SetCurrency(CString strCurrency) { m_strCurrency = strCurrency; }

	CString GetAddress(void) const { return m_strAddress; }
	void SetAddress(CString strAddress) { m_strAddress = strAddress; }
	CString GetCity(void) const { return m_strCity; }
	void SetCity(CString strCity) { m_strCity = strCity; }
	CString GetCountry(void) const { return m_strCountry; }
	void SetCountry(CString strCountry) { m_strCountry = strCountry; }
	CString GetCusip(void) const { return m_strCusip; }
	void SetCusip(CString strCusip) { m_strCusip = strCusip; }
	CString GetSedol(void) const { return m_strSedol; }
	void SetSedol(CString strSedol) { m_strSedol = strSedol; }
	long GetEmployeeTotal(void) const noexcept { return m_lEmployeeTotal; }
	void SetEmployeeTotal(long lEmployeeTotal) noexcept { m_lEmployeeTotal = lEmployeeTotal; }
	CString GetListedExchange(void) const { return m_strListedExchange; }
	void SetListedExchange(CString strListedExchange) { m_strListedExchange = strListedExchange; }
	CString GetGgroup(void) const { return m_strGgroup; }
	void SetGgroup(CString strGgroup) { m_strGgroup = strGgroup; }
	CString GetGind(void) const { return m_strGind; }
	void SetGind(CString strGind) { m_strGind = strGind; }
	CString GetGsector(void) const { return m_strGsector; }
	void SetGsector(CString strGsector) { m_strGsector = strGsector; }
	CString GetGsubind(void) const { return m_strGsubind; }
	void SetGsubind(CString strGsubind) { m_strGsubind = strGsubind; }
	CString GetIPODate(void) const { return m_strIPODate; }
	void SetIPODate(CString strIPODate) { m_strIPODate = strIPODate; }
	CString GetIsin(void) const { return m_strIsin; }
	void SetIsin(CString strIsin) { m_strIsin = strIsin; }
	double GetMarketCapitalization(void) const noexcept { return m_dMarketCapitalization; }
	void SetMarketCapitalization(double dMarketCapitalization) noexcept { m_dMarketCapitalization = dMarketCapitalization; }
	CString GetNaics(void) const { return m_strNaics; }
	void SetNaics(CString strNaics) { m_strNaics = strNaics; }
	CString GetNaicsNationalIndustry(void) const { return m_strNaicsNationalIndustry; }
	void SetNaicsNationalIndustry(CString strNaicsNationalIndustry) { m_strNaicsNationalIndustry = strNaicsNationalIndustry; }
	CString GetNaicsSector(void) const { return m_strNaicsSector; }
	void SetNaicsSector(CString strNaicsSector) { m_strNaicsSector = strNaicsSector; }
	CString GetNaicsSubsector(void) const { return m_strNaicsSubsector; }
	void SetNaicsSubsector(CString strNaicsSubsector) { m_strNaicsSubsector = strNaicsSubsector; }
	CString GetName(void) const { return m_strName; }
	void SetName(CString strName) { m_strName = strName; }
	CString GetPhone(void) const { return m_strPhone; }
	void SetPhone(CString strPhone) { m_strPhone = strPhone; }
	double GetShareOutstanding(void) const noexcept { return m_dShareOutstanding; }
	void SetShareOutstanding(double dShareOutstanding) noexcept { m_dShareOutstanding = dShareOutstanding; }
	CString GetState(void) const { return m_strState; }
	void SetState(CString strState) { m_strState = strState; }
	CString GetTicker(void) const { return m_strTicker; }
	void SetTicker(CString strTicker) { m_strTicker = strTicker; }
	CString GetWebURL(void) const { return m_strWebURL; }
	void SetWebURL(CString strWebURL) { m_strWebURL = strWebURL; }
	CString GetLogo(void) const { return m_strLogo; }
	void SetLogo(CString strLogo) { m_strLogo = strLogo; }
	CString GetFinnhubIndustry(void) const { return m_strFinnhubIndustry; }
	void SetFinnhubIndustry(CString strFinnhubIndustry) { m_strFinnhubIndustry = strFinnhubIndustry; }
	CString GetPeer(void) const { return m_strPeer; }
	void SetPeer(CString strPeer) { m_strPeer = strPeer; }
	long GetProfileUpdateDate(void) const noexcept { return m_lProfileUpdateDate; }
	void SetProfileUpdateDate(long lProfileUpdateDate) noexcept { m_lProfileUpdateDate = lProfileUpdateDate; }
	long GetBasicFinancialUpdateDate(void) const noexcept { return m_lBasicFinancialUpdateDate; }
	void SetBasicFinancialUpdateDate(long lBasicFinancialUpdateDate) noexcept { m_lBasicFinancialUpdateDate = lBasicFinancialUpdateDate; }
	long GetLastRTDataUpdateDate(void) const noexcept { return m_lLastRTDataUpdateDate; }
	void SetLastRTDataUpdateDate(long lDate) noexcept { m_lLastRTDataUpdateDate = lDate; }
	long GetPeerUpdateDate(void) const noexcept { return m_lPeerUpdateDate; }
	void SetPeerUpdateDate(long lDate) noexcept { m_lPeerUpdateDate = lDate; }
	long GetInsiderTransactionUpdateDate(void) const noexcept { return m_lInsiderTransactionUpdateDate; }
	void SetInsiderTransactionUpdateDate(long lDate) noexcept { m_lInsiderTransactionUpdateDate = lDate; }
	long GetLastEPSSurpriseUpdateDate(void) const noexcept { return m_lLastEPSSurpriseUpdateDate; }
	void SetLastEPSSurpriseUpdateDate(long lDate) noexcept { m_lLastEPSSurpriseUpdateDate = lDate; }

	CString GetTiingoPermaTicker(void) noexcept { return m_strTiingoPermaTicker; }
	void SetTiingoPermaTicker(CString strTiingoPermaTicker) noexcept { m_strTiingoPermaTicker = strTiingoPermaTicker; }
	bool IsActive(void) noexcept { return m_fIsActive; }
	void SetActive(bool fIsActive) noexcept { m_fIsActive = fIsActive; }
	bool IsADR(void) noexcept { return m_fIsADR; }
	void SetADR(bool fIsADR) noexcept { m_fIsADR = fIsADR; }
	INT32 GetSICCode(void) noexcept { return m_iSICCode; }
	void SetSICCode(INT32 iSICCode) noexcept { m_iSICCode = iSICCode; }
	CString GetSICIndustry(void) noexcept { return m_strSICIndustry; }
	void SetSICIndustry(CString strSICIndustry) noexcept { m_strSICIndustry = strSICIndustry; }
	CString GetSICSector(void) noexcept { return m_strSICSector; }
	void SetSICSector(CString strSICSector) noexcept { m_strSICSector = strSICSector; }
	CString GetTiingoIndustry(void) noexcept { return m_strTiingoIndustry; }
	void SetTiingoIndustry(CString strTiingoIndustry) noexcept { m_strTiingoIndustry = strTiingoIndustry; }
	CString GetTiingoSector(void) noexcept { return m_strTiingoSector; }
	void SetTiingoSector(CString strTiingoSector) noexcept { m_strTiingoSector = strTiingoSector; }
	CString GetCompanyWebSite(void) noexcept { return m_strCompanyWebSite; }
	void SetCompanyWebSite(CString strCompanyWebSite) noexcept { m_strCompanyWebSite = strCompanyWebSite; }
	CString GetSECFilingWebSite(void) noexcept { return m_strSECFilingWebSite; }
	void SetSECFilingWebSite(CString strSECFilingWebSite) noexcept { m_strSECFilingWebSite = strSECFilingWebSite; }
	long GetStatementUpdateDate(void) noexcept { return m_lStatementUpdateDate; }
	void SetStatementUpdateDate(long lStatementUpdateDate) noexcept { m_lStatementUpdateDate = lStatementUpdateDate; }
	long GetDailyDataUpdateDate(void) noexcept { return m_lDailyDataUpdateDate; }
	void SetDailyDataUpdateDate(long lDailyDataUpdateDate) noexcept { m_lDailyDataUpdateDate = lDailyDataUpdateDate; }

	CString GetFinnhubDayLineInquiryString(time_t tCurrentTime);
	CString GetTiingoDayLineInquiryString(long lCurrentDate);

	bool IsUSMarket(void);

public:
	vector<CEPSSurprisePtr> m_vEPSSurprise;
	bool m_fEPSSurpriseUpdated;
	atomic_bool m_fEPSSurpriseNeedSave;

	vector<CInsiderTransactionPtr> m_vInsiderTransaction;
	long m_lInsiderTransactionStartDate;

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
	CString m_strPeer;
	long m_lProfileUpdateDate; // 最新简介更新日期
	long m_lBasicFinancialUpdateDate; // 最新基本财务更新日期
	long m_lLastRTDataUpdateDate; // 最新实时数据更新日期
	long m_lPeerUpdateDate; // 最新竞争对手数据更新日期
	long m_lInsiderTransactionUpdateDate; // 最新内部人员交易数据更新日期
	long m_lLastEPSSurpriseUpdateDate; // 最新EPS Surprise更新日期

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
	long m_lStatementUpdateDate;
	long m_lDailyDataUpdateDate;

	// 系统生成信息
	CDataWorldStockDayLine m_dataDayLine;

	CFinnhubStockBasicFinancialPtr m_pBasicFinancial;

	// 无需存储数据区
	bool m_fProfileUpdated; // 公司简介已更新
	bool m_fBasicFinancialUpdated; // 基本财务已更新
	bool m_fFinnhubPeerUpdated; // 同业公司数据已更新
	bool m_fFinnhubInsiderTransactionNeedUpdate; // 公司内部交易数据已更新
	atomic_bool m_fUpdateFinnhubBasicFinancialDB; // 基本财务数据需要保存
	atomic_bool m_fFinnhubInsiderTransactionNeedSave; // 内部交易数据需要存储
};

typedef shared_ptr<CWorldStock> CWorldStockPtr;
typedef shared_ptr<vector<CWorldStockPtr>> CWorldStockVectorPtr;