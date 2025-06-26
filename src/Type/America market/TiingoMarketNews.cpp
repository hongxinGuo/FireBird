#include"pch.h"

#include"TiingoMarketNews.h"

CTiingoMarketNews::CTiingoMarketNews() {
	Reset();
}

void CTiingoMarketNews::Reset() {
}

void CTiingoMarketNews::Load(const CSetTiingoMarketNews& setTiingoMarketNews) {
}

void CTiingoMarketNews::Append(CSetTiingoMarketNews& setTiingoMarketNews) {
	setTiingoMarketNews.AddNew();
	Save(setTiingoMarketNews);
	setTiingoMarketNews.Update();
}

void CTiingoMarketNews::Save(CSetTiingoMarketNews& setTiingoMarketNews) {
	if ((m_strDescription.length() > 500)
		|| (m_strSource.length() > 100)
		|| (m_strTags.length() > 400)
		|| (m_strTickers.length() > 200)
		|| (m_strTitle.length() > 100)
		|| (m_strUrl.length() > 100)) {
		string s = _T("Tiingo market news ");
		s += m_strTickers;
		s += _T(" ×Ö·û´®Ì«³¤");
		gl_systemMessage.PushErrorMessage(s);
	}
}
