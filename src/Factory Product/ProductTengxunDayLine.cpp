#include "pch.h"

#include "ProductTengxunDayLine.h"
#include"ChinaMarket.h"

#include"JsonParse.h"

CProductTengxunDayLine::CProductTengxunDayLine() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=");
}

CString CProductTengxunDayLine::CreateMessage(void) {
	return m_strInquiry; // 腾讯日线数据的申请字符串目前由CNeteaseDayLineWebInquiry类完成，本Product无需动作。
}

//
// {
// "code":0,
// "msg":"",
// "data":
//   {
//   "sh600601":
//      { "day":
//				[ ["2023-01-19","2.550","2.600","2.610","2.550","86162.000"],
//					["2023-01-20","2.600","2.620","2.620","2.590","100735.000"]],
//				"qt":{},
//				"mx_price":{"mx":[],"price":[]},
//				"prec":"2.560",
//				"version":"16"
//		  }
//	 }
// }
// 
bool CProductTengxunDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pDayLineWebData = ParseTengxunDayLine(pWebData);
	pDayLineWebData->SetStockCode(pWebData->GetStockCode());
	gl_pChinaMarket->PushDayLine(pDayLineWebData);

	return true;
}
