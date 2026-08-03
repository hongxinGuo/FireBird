module;

export module FireBirdLib.Stock.FinnhubStock;

import FireBirdLib.Stock;
import FireBirdLib.Container.HistoryCandle.FinnhubStockDayLine;
import FireBirdLib.Accessory.NlohmannJsonDeclaration;
import FireBirdLib.Type.FinnhubCompanyNews;
import FireBirdLib.EPSSurprise;
import FireBirdLib.Type.InsiderTransaction;
import FireBirdLib.Type.InsiderSentiment;
import FireBirdLib.HistoryCandle.DayLine;
import FireBirdLib.Type.SECFiling;

import std;
using std::chrono::weekday;
using std::chrono::sys_seconds;
using std::chrono::local_days;
using std::chrono::local_seconds;
using std::shared_ptr;
using std::vector;
using std::string;
using std::atomic_bool;
using std::time_t;


export {
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

		void CheckUpdateStatus(local_days todayDate);
		void CheckProfileUpdateStatus(local_days todayDate);
		bool CheckCompanyNewsUpdateStatus(local_days todayDate);
		bool CheckBasicFinancialUpdateStatus(local_days todayDate);
		bool CheckDayLineUpdateStatus(local_days todayDate, local_days lLastTradeDate, local_seconds lTime, weekday lDayOfWeek);
		void SaveDayLineDB() { m_dataDayLine.SaveDB(m_strSymbol); }
		void UpdateInsiderTransactionDB();
		void UpdateInsiderSentimentDB();
		virtual bool UpdateCompanyNewsDB();
		virtual bool UpdateEPSSurpriseDB();
		bool UpdateSECFilingsDB() const;
		virtual bool UpdateDayLineDB();

		void UpdateDayLine(const CDayLinesPtr& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
		void UpdateCompanyNews(const shared_ptr<vector<CFinnhubCompanyNews>>& pvCompanyNews);
		void UpdateEPSSurprise(const shared_ptr<vector<CEPSSurprise>>& vEPSSurprise);

		void UpdateDayLineStartEndDate();
		auto GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
		CDayLine* GetDayLine(const size_t lIndex) { return m_dataDayLine.GetData(lIndex); }
		void UnloadDayLine() { m_dataDayLine.Unload(); }

		bool HaveNewDayLineData(); //Todo: 移至VirtualStock中，合并其他股票类型的同名函数

		bool IsUpdateCompanyProfile() const noexcept { return m_fUpdateCompanyProfile; }
		void SetUpdateCompanyProfile(const bool fFlag) noexcept { m_fUpdateCompanyProfile = fFlag; }

		bool IsUpdateCompanyNews() const noexcept { return m_fUpdateCompanyNews; }
		void SetUpdateCompanyNews(const bool fFlag) noexcept { m_fUpdateCompanyNews = fFlag; }
		size_t GetCompanyNewsSize() const noexcept;
		sys_seconds GetCompanyNewsDateTime(int iIndex) const;
		void ClearCompanyNews();

		bool IsUpdateBasicFinancial() const noexcept { return m_fUpdateBasicFinancial; }
		void SetUpdateBasicFinancial(const bool fFlag) noexcept { m_fUpdateBasicFinancial = fFlag; }
		bool IsUpdateBasicFinancialDB() const noexcept { return m_fUpdateFinnhubBasicFinancialDB; }
		void SetUpdateBasicFinancialDB(const bool fFlag) noexcept { m_fUpdateFinnhubBasicFinancialDB = fFlag; }

		bool IsUpdateEPSSurprise() const noexcept { return m_fUpdateEPSSurprise; }
		void SetUpdateEPSSurprise(const bool fFlag) noexcept { m_fUpdateEPSSurprise = fFlag; }
		bool CheckEPSSurpriseStatus(local_days lCurrentDate);
		bool IsUpdateEPSSurpriseDB() const noexcept { return m_fUpdateEPSSurpriseDB; }
		void SetUpdateEPSSurpriseDB(const bool fFlag) noexcept { m_fUpdateEPSSurpriseDB = fFlag; }

		bool IsUpdatePeer() const noexcept { return m_fUpdateFinnhubPeer; }
		void SetUpdatePeer(bool fFlag) noexcept { m_fUpdateFinnhubPeer = fFlag; }
		bool CheckPeerStatus(local_days lCurrentDate);

		bool IsUpdateSECFilings() const noexcept { return m_fUpdateSECFilings; }
		void SetUpdateSECFilings(bool fFlag) noexcept { m_fUpdateSECFilings = fFlag; }
		bool CheckSECFilingsStatus(local_days lCurrentDate);
		bool IsUpdateSECFilingsDB() const noexcept { return m_fUpdateSECFilingsDB; }
		void SetUpdateSECFilingsDB(const bool fFlag) noexcept { m_fUpdateSECFilingsDB = fFlag; }

		bool HaveInsiderTransaction() const noexcept;
		void UnloadInsiderTransaction();
		void UpdateInsiderTransaction(const shared_ptr<vector<CInsiderTransaction>>& pvInsiderTransaction);
		bool IsUpdateInsiderTransaction() const noexcept { return m_fUpdateFinnhubInsiderTransaction; }
		void SetUpdateInsiderTransaction(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderTransaction = fFlag; }
		bool CheckInsiderTransactionStatus(local_days lCurrentDate);
		bool IsUpdateInsiderTransactionDB() const noexcept { return m_fUpdateFinnhubInsiderTransactionDB; }
		void SetUpdateInsiderTransactionDB(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderTransactionDB = fFlag; }

		bool HaveInsiderSentiment() const noexcept;
		void UpdateInsiderSentiment(const shared_ptr<vector<CInsiderSentiment>>& pvInsiderSentiment);
		void ClearInsiderSentiment();
		bool IsUpdateInsiderSentiment() const noexcept { return m_fUpdateFinnhubInsiderSentiment; }
		void SetUpdateInsiderSentiment(const bool fFlag) noexcept { m_fUpdateFinnhubInsiderSentiment = fFlag; }
		bool CheckInsiderSentimentStatus(local_days lCurrentDate);
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
		nlohmannJson GetPeer() { return m_jsonPeer; }
		void SetPeer(const nlohmannJson& jsonPeer) { m_jsonPeer = jsonPeer; }
		void SetSECFilings(const shared_ptr<vector<CSECFiling>>& pv);
		void ClearSECFilings() const;
		local_days GetProfileUpdateDate();
		void SetProfileUpdateDate(local_days profileUpdateDate) noexcept;
		local_days GetCompanyNewsUpdateDate();
		void SetCompanyNewsUpdateDate(local_days companyNewsUpdateDate) noexcept;
		local_days GetBasicFinancialUpdateDate();
		void SetBasicFinancialUpdateDate(local_days basicFinancialUpdateDate) noexcept;
		local_days GetLastRTDataUpdateDate();
		void SetLastRTDataUpdateDate(local_days lastRTDataUpdateDate) noexcept;
		local_days GetPeerUpdateDate();
		void SetPeerUpdateDate(local_days peerUpdateDate) noexcept;
		local_days GetInsiderTransactionUpdateDate();
		void SetInsiderTransactionUpdateDate(local_days insiderTransactionUpdateDate) noexcept;
		local_days GetInsiderSentimentUpdateDate();
		void SetInsiderSentimentUpdateDate(local_days insiderSentimentUpdateDate) noexcept;
		local_days GetLastEPSSurpriseUpdateDate();
		void SetLastEPSSurpriseUpdateDate(local_days lastEPSSurpriseUpdateDate) noexcept;
		void SetSECFilingsUpdateDate(local_days secFilingsUpdateDate) noexcept;
		local_days GetSECFilingsUpdateDate();
		nlohmannJson GetJsonPeer() const noexcept { return m_jsonPeer; }

		string GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const;
		string GetTiingoDayLineInquiryParam(local_days lStartDate, local_days lCurrentDate) const;
		string GetFinnhubInsiderTransactionInquiryParam(time_t tCurrentTime);

		bool IsUSMarket() const;

	public:
		vector<CEPSSurprise> m_vEPSSurprise;
		bool m_fUpdateEPSSurprise{ true };
		atomic_bool m_fUpdateEPSSurpriseDB{ false };

		vector<CInsiderTransaction> m_vInsiderTransaction;
		long m_lInsiderTransactionEndDate{ 19800101 };

		shared_ptr<vector<CInsiderSentiment>> m_pvInsiderSentiment{ nullptr };
		long m_lInsiderSentimentStartDate{ 19800101 };

		shared_ptr<vector<CSECFiling>> m_pvSECFilings;
		bool m_fUpdateSECFilings{ true };
		atomic_bool m_fUpdateSECFilingsDB{ false };

	protected:
		// Finnhub symbol信息
		string m_strType{};
		string m_strSymbol2{}; // Alternative ticker for exchanges with multiple tickers for 1 stock such as BSE
		string m_strMic{};
		string m_strFigi{}; // FIGI identifier
		string m_strShareClassFIGI{}; // Global Share Class FIGI
		string m_strCurrency{};

		// Finnhub profile信息
		string m_strAddress{};
		string m_strCity{};
		string m_strCountry{};
		string m_strCusip{};
		string m_strSedol{};
		long m_lEmployeeTotal{ 0 };
		//string m_strListedExchange{ "US") }; // 使用virtualStock中的ExchangeCode
		string m_strGgroup{};
		string m_strGind{};
		string m_strGsector{};
		string m_strGsubind{};
		string m_strIPODate{};
		string m_strIsin{};
		double m_dMarketCapitalization{ 0 };
		string m_strNaics{};
		string m_strNaicsNationalIndustry{};
		string m_strNaicsSector{};
		string m_strNaicsSubsector{};
		string m_strName{};
		string m_strPhone{};
		double m_dShareOutstanding{ 0 };
		string m_strState{};
		string m_strTicker{};
		string m_strWebURL{};
		string m_strLogo{};
		string m_strFinnhubIndustry{};
		nlohmannJson m_jsonPeer{ nlohmannJson({}) }; // 读取到的Peer数据为json制式

		// 系统生成信息
		CContainerFinnhubStockDayLine m_dataDayLine;

		vector<CFinnhubCompanyNews> m_vCompanyNews;

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
}