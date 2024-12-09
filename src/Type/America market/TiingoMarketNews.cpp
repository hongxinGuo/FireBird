#include"pch.h"

import FireBird.System.Message;

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
	if ((m_strDescription.GetLength() > 500)
		|| (m_strSource.GetLength() > 100)
		|| (m_strTags.GetLength() > 400)
		|| (m_strTickers.GetLength() > 200)
		|| (m_strTitle.GetLength() > 100)
		|| (m_strUrl.GetLength() > 100)) {
		gl_systemMessage.PushErrorMessage(_T("Tiingo market news ") + m_strTickers + _T(" ×Ö·û´®Ì«³¤"));
	}
}
