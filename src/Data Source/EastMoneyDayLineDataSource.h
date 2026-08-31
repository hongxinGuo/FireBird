#pragma once

#include <cpr/cprtypes.h>

#include"VirtualDataSource.h"

class CVirtualWebProduct;

inline vector<string> UAList{
	"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/121.0.0.0 Safari/537.36",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.2.1 Safari/605.1.15",
	"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:122.0) Gecko/20100101 Firefox/122.0",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36"
};

class CEastmoneyDayLineDataSource : public CVirtualDataSource {
public:
	CEastmoneyDayLineDataSource();
	// 只能有一个实例,不允许赋值、拷贝
	CEastmoneyDayLineDataSource(const CEastmoneyDayLineDataSource&) = delete;
	CEastmoneyDayLineDataSource& operator=(const CEastmoneyDayLineDataSource&) = delete;
	CEastmoneyDayLineDataSource(const CEastmoneyDayLineDataSource&&) noexcept = delete;
	CEastmoneyDayLineDataSource& operator=(const CEastmoneyDayLineDataSource&&) noexcept = delete;
	~CEastmoneyDayLineDataSource() override = default;
	bool Reset() override;

	bool GenerateInquiryMessage(const local_seconds& currentTime) override;

	void ConfigureInternetOption() override; // 在读取网络数据前的准备工作，默认为设置Eastmoney DayLine的m_pSession状态。
	void CheckWebData(const string& text) override;

	void CreateCurrentInquireString() override;

	bool GenerateDayLine();
	shared_ptr<CVirtualWebProduct> CreateProduct(const CChinaStockPtr& pStock) const;

	bool IsUpdateDayLine() const noexcept { return m_fUpdateDayLine; }
	void SetUpdateDayLine(bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }

	void SetDownLoadingStockCode(const string& strStockCode);
	string GetDownLoadingStockCode() { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode() { m_strDownLoadingStockCode = ""; }

	static string GetUAList(int index) { return UAList.at(index); }
	cpr::Header GetHeader(int index) {
		return cpr::Header{
			{ "User-Agent", GetUAList(index) },
			{ "Referer", "https://quote.eastmoney.com/" },
			{ "accept", "*/*" },
			{ "Connection", "keep-alive" }
		};
	}

protected:
	bool m_fUpdateDayLine{ true }; // 每日更新公司日线数据
	string m_strDownLoadingStockCode{};

	time_point<steady_clock> m_PrevInquireTimePoint{};
	int m_InquiringTime{ 10000 }; // 每次查询间隔，单位ms。
};

using CEastmoneyDayLineDataSourcePtr = shared_ptr<CEastmoneyDayLineDataSource>;
extern CEastmoneyDayLineDataSourcePtr gl_pEastmoneyDayLineDataSource;
