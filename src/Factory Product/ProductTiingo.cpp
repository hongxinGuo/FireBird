#include"pch.h"

#include"ProductTiingo.h"

#include "FinnhubInquiryType.h"
#include "TiingoDataSource.h"
#include"TiingoInaccessibleStock.h"

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 目前finnhub对其免费账户只提供部分内容数据。为了加速轮询速度，受限制的数据(交易所）将不再申请。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CProductTiingo::AddInaccessibleSymbol() {
	gl_tiingoInaccessibleStock.SetUpdateDB(true);
	try { // 存在此申请类型
		const auto pStock = gl_tiingoInaccessibleStock.GetStock(m_iInquireType);
		if (!pStock->HaveSymbol(m_strInquiringSymbol)) {	// 新的证券代码？
			pStock->AddSymbol(m_strInquiringSymbol);
		}
	} catch (out_of_range&) {	// 不存在此申请类型？则存储此新类型和该证券名称
		const auto pNewStock = make_shared<CInaccessible>();
		pNewStock->SetFunction(m_iInquireType);
		pNewStock->SetFunctionString(gl_FinnhubInquiryType.GetInquiryString(m_iInquireType));
		pNewStock->AddSymbol(m_strInquiringSymbol);
		gl_tiingoInaccessibleStock.SetStock(m_iInquireType, pNewStock);
	}
}

bool CProductTiingo::__Test_checkAccessRight(CWebDataPtr pWebData) {
	if (gl_pTiingoDataSource->GetWebErrorCode() == 404) { // 404 error
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return false;
	}
	if (m_iReceivedDataStatus == NO_ACCESS_RIGHT_) m_iReceivedDataStatus = GOOD_DATA__;
	return true;
}
