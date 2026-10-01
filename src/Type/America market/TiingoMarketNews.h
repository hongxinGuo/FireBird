#pragma once

using std::string;
using std::vector;
using std::shared_ptr;
using std::chrono::system_clock;

class CTiingoMarketNews final {
public:
	CTiingoMarketNews();
	// 不允许复制和赋值。
	CTiingoMarketNews(const CTiingoMarketNews&) = default;
	CTiingoMarketNews& operator=(const CTiingoMarketNews&) = default;
	CTiingoMarketNews(CTiingoMarketNews&&) noexcept = default;
	CTiingoMarketNews& operator=(CTiingoMarketNews&&) noexcept = default;
	~CTiingoMarketNews() = default;

	void Reset();

public:
	long m_lId;
	string m_strTickers;
	string m_strTitle;
	string m_strTags;
	string m_strSource;
	string m_strDescription;
	string m_strUrl;
	system_clock::time_point m_tpCrawlDate;
	system_clock::time_point m_tpPublishDate;
	// 无需存储数据区
};

using CTiingoMarketNewsPtr = shared_ptr<CTiingoMarketNews>;
using CTiingoMarketNewssPtr = shared_ptr<vector<CTiingoMarketNews>>;
