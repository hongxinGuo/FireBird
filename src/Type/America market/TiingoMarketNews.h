#pragma once

import FireBird.Set.TiingoMarketNews;

#include<memory>
#include<vector>

class CTiingoMarketNews final {
public:
	CTiingoMarketNews();
	// �������ƺ͸�ֵ��
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
	CString m_strTickers;
	CString m_strTitle;
	CString m_strTags;
	CString m_strSource;
	CString m_strDescription;
	CString m_strUrl;
	long long m_llCrawlDate;
	long long m_LLPublishDate;
	// ����洢������
};

using CTiingoMarketNewsPtr = shared_ptr<CTiingoMarketNews>;
using CVectorTiingoMarketNewsPtr = shared_ptr<vector<CTiingoMarketNewsPtr>>;
