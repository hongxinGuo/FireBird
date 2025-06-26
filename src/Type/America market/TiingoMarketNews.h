#pragma once

#include"SetTiingoMarketNews.h"

#include<memory>
#include<vector>

class CTiingoMarketNews final {
public:
	CTiingoMarketNews();
	// 不允许复制和赋值。
	CTiingoMarketNews(const CTiingoMarketNews&) = delete;
	CTiingoMarketNews& operator=(const CTiingoMarketNews&) = delete;
	CTiingoMarketNews(const CTiingoMarketNews&&) noexcept = delete;
	CTiingoMarketNews& operator=(const CTiingoMarketNews&&) noexcept = delete;
	~CTiingoMarketNews() = default;

	void Reset();

	void Load(const CSetTiingoMarketNews& setTiingoMarketNews);
	void Append(CSetTiingoMarketNews& setTiingoMarketNews);
	void Save(CSetTiingoMarketNews& setTiingoMarketNews);

public:
	long m_lId;
	string m_strTickers;
	string m_strTitle;
	string m_strTags;
	string m_strSource;
	string m_strDescription;
	string m_strUrl;
	long long m_llCrawlDate;
	long long m_LLPublishDate;
	// 无需存储数据区
};

using CTiingoMarketNewsPtr = shared_ptr<CTiingoMarketNews>;
using CVectorTiingoMarketNewsPtr = shared_ptr<vector<CTiingoMarketNewsPtr>>;
