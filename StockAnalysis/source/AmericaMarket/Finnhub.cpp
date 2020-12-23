#include"WebInquirer.h"

#include"ProcessFinnhubWebData.h"

#include"Finnhub.h"

/// <summary>
/// //////////////////////////////////////////////////////////////////////////////////////////
/// </summary>
/// finnhub��ȡ������������ͽ����ֻ�ִ�з�ʽ���ʶ����ٵ������β����һ���ֻء�
/// <param name=""></param>
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhub::ProcessFinnhubInquiringMessage(CAmericaMarketPtr pMarket) {
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  CAmericaStakePtr pStake = nullptr;
  CForexSymbolPtr pSymbol = nullptr;

  if (pMarket->m_qFinnhubWebInquiry.size() > 0) { // ������ȴ���
    ASSERT(pMarket->m_fFinnhubInquiring);
    if (pMarket->IsFinnhubDataReceived()) { //�Ѿ�����������������Finnhub�����Ѿ����յ��ˣ�
      pMarket->m_CurrentFinnhubInquiry = pMarket->m_qFinnhubWebInquiry.top();
      pMarket->m_qFinnhubWebInquiry.pop();
      gl_pFinnhubWebInquiry->SetInquiryingStrPrefix(pMarket->m_vFinnhubInquiringStr.at(pMarket->m_CurrentFinnhubInquiry.m_lInquiryIndex)); // ����ǰ׺
      switch (pMarket->m_CurrentFinnhubInquiry.m_lInquiryIndex) { // ���ݲ�ͬ��Ҫ��������׺�ַ���
      case __COMPANY_PROFILE__: // Premium ����˻��޷���ȡ����Ϣ��sandboxģʽ�ܶ�ȡ�����Ǵ���ģ�ֻ�����ڲ��ԡ�
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(pMarket->m_vAmericaStake.at(pMarket->m_lCurrentProfilePos)->m_strSymbol);
      pMarket->m_vAmericaStake.at(pMarket->m_lCurrentProfilePos)->m_fInquiryAmericaStake = false;
      break;
      case __COMPANY_PROFILE2__:
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(pMarket->m_vAmericaStake.at(pMarket->m_lCurrentProfilePos)->m_strSymbol);
      pMarket->m_vAmericaStake.at(pMarket->m_lCurrentProfilePos)->m_fInquiryAmericaStake = false;
      break;
      case  __COMPANY_SYMBOLS__:
      // do nothing
      break;
      case __COMPANY_EXECTIVE__: // Premium
      break;
      case __MARKET_NEWS__:
      break;
      case __COMPANY_NEWS__:
      break;
      case __PRESS_RELEASE__: // Premium
      break;
      case __NEWS_SENTIMENT__:
      break;
      case __PEERS__:
      break;
      case __BASIC_FINANCIALS__:
      break;
      case __OWNERSHIP__: // Premium
      break;
      case __FUND_OWNERSHIP__: // jPremium
      break;
      case __FINANCIAL__: // Premium
      break;
      case __FINAICIAL_AS_REPORT__:
      break;
      case __SEC_FILINGS__:
      break;
      case __INTERNATIONAL_FILINGS__: // Premium
      break;
      case __SEC_SENTIMENT_ANALYSIS__: // Premium
      break;
      case __SIMILARITY_INDEX__: // Premium
      break;
      case __IPO_CALENDER__:
      break;
      case __DIVIDENDS__: // Premium
      break;
      case __STOCK_RECOMMENDATION_TRENDS__:
      break;
      case __STOCK_PRICE_TARGET__:
      break;
      case __STOCK_UPGRADE_DOWNGRADE__: // Premium
      break;
      case __STOCK_REVENUE_EXTIMATES__: // Premium
      break;
      case __STOCK_EPS_EXTIMATES__:// Premium
      break;
      case __STOCK_EPS_SURPRISE__:
      break;
      case __STOCK_EARNING_CALENDER__:
      break;
      case __STOCK_QUOTE__:
      pStake = pMarket->m_vAmericaStake.at(pMarket->m_lCurrentRTDataQuotePos);
      strMiddle = pStake->GetSymbol();
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      break;
      case __STOCK_CANDLES__:
      ASSERT(pMarket->m_CurrentFinnhubInquiry.m_lStakeIndex == pMarket->m_lCurrentUpdateDayLinePos);
      pStake = pMarket->m_vAmericaStake.at(pMarket->m_lCurrentUpdateDayLinePos);
      strMiddle = pStake->GetDayLineInquiryString(pMarket->GetMarketTime());
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      pStake->m_fDayLineNeedUpdate = false;
      break;
      case __STOCK_TICK_DATA__: // Premium
      break;
      case __STOCK_LAST_BID_ASK__: // Premium
      break;
      case __STOCK_SPLITS__:
      break;
      case __FOREX_EXCHANGE__:
      // do nothing
      break;
      case __FOREX_SYMBOLS__:
      ASSERT(pMarket->m_CurrentFinnhubInquiry.m_lStakeIndex == (pMarket->m_lCurrentForexExchangePos - 1));
      strMiddle = pMarket->m_vForexExchange.at(pMarket->m_CurrentFinnhubInquiry.m_lStakeIndex);
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      break;
      case __FOREX_CANDLES__:
      ASSERT(pMarket->m_CurrentFinnhubInquiry.m_lStakeIndex == pMarket->m_lCurrentUpdateForexDayLinePos);
      pSymbol = pMarket->m_vForexSymbol.at(pMarket->m_lCurrentUpdateForexDayLinePos);
      strMiddle = pSymbol->GetDayLineInquiryString(pMarket->GetMarketTime());
      gl_pFinnhubWebInquiry->SetInquiryingStringMiddle(strMiddle);
      pSymbol->m_fDayLineNeedUpdate = false;
      break;
      case __FOREX_ALL_RATES__:
      break;
      default:
      TRACE("δ����ָ��%d\n", pMarket->m_CurrentFinnhubInquiry.m_lInquiryIndex);
      break;
      }
      pMarket->SetFinnhubDataReceived(false); // ���ô˱�ʶ��Ҫ�������������̣߳�GetWebData��֮ǰ���������߳��еĶ������׳���
      gl_pFinnhubWebInquiry->GetWebData();
    }
  }

  return true;
}

//////////////////////////////////////////////
//
// �������߳̽��յ���Finnhub������Ϣ��
//
// Ŀǰֻ����ͬʱ����һ����Ϣ������Ϣ��������������֮ǰ������������һ����Ϣ�������ܹ���֤ͬһ�ԡ�������Finnhub��վ���ٶ����ƣ�
// ÿ����ֻ����60�����룬�ʶ�û�б�Ҫǿ�������ٶȡ�
//
//////////////////////////////////////////////
bool CFinnhub::ProcessFinnhubWebDataReceived(CAmericaMarketPtr pMarket, WebInquiry finnhubInquiry) {
  CWebDataPtr pWebData = nullptr;
  CString str = _T("");
  vector<CString> vExchange;
  vector<CForexSymbolPtr> vForexSymbol;
  vector<CAmericaStakePtr> vStake;
  long lTemp = 0;

  ASSERT(gl_WebInquirer.GetFinnhubDataSize() <= 1);
  if (pMarket->IsFinnhubDataReceived()) { // ����������ݽ������
    if (gl_WebInquirer.GetFinnhubDataSize() > 0) {  // ����ǰ��������
      ASSERT(pMarket->m_fFinnhubInquiring);
      pWebData = gl_WebInquirer.PopFinnhubData();
      switch (finnhubInquiry.m_lInquiryIndex) {
      case __COMPANY_PROFILE__: // Ŀǰ����˻��޷�ʹ�ô˹��ܡ�
      ASSERT(finnhubInquiry.m_lStakeIndex == pMarket->m_lCurrentProfilePos);
      ProcessFinnhubStockProfile(pWebData, pMarket->m_vAmericaStake.at(pMarket->m_lCurrentProfilePos));
      break;
      case __COMPANY_PROFILE2__:
      ASSERT(finnhubInquiry.m_lStakeIndex == pMarket->m_lCurrentProfilePos);
      ProcessFinnhubStockProfile2(pWebData, pMarket->m_vAmericaStake.at(pMarket->m_lCurrentProfilePos));
      break;
      case  __COMPANY_SYMBOLS__:
      ProcessFinnhubStockSymbol(pWebData, vStake);
      pMarket->m_fSymbolUpdated = true;
      break;
      case  __MARKET_NEWS__:
      break;
      case __COMPANY_NEWS__:
      break;
      case __NEWS_SENTIMENT__:
      break;
      case __PEERS__:
      break;
      case __BASIC_FINANCIALS__:
      break;
      case __STOCK_QUOTE__:
      ASSERT(finnhubInquiry.m_lStakeIndex == pMarket->m_lCurrentRTDataQuotePos);
      ProcessFinnhubStockQuote(pWebData, pMarket->m_vAmericaStake.at(pMarket->m_lCurrentRTDataQuotePos));
      break;
      case __STOCK_CANDLES__:
      ASSERT(finnhubInquiry.m_lStakeIndex == pMarket->m_lCurrentUpdateDayLinePos);
      ProcessFinnhubStockCandle(pWebData, pMarket->m_vAmericaStake.at(pMarket->m_lCurrentUpdateDayLinePos));
      TRACE("����%s��������\n", pMarket->m_vAmericaStake.at(pMarket->m_lCurrentUpdateDayLinePos)->m_strSymbol.GetBuffer());
      break;
      case __FOREX_EXCHANGE__:
      ProcessFinnhubForexExchange(pWebData, vExchange);
      for (int i = 0; i < vExchange.size(); i++) {
        if (pMarket->m_mapForexExchange.find(vExchange.at(i)) == pMarket->m_mapForexExchange.end()) {
          lTemp = pMarket->m_vForexExchange.size();
          pMarket->m_vForexExchange.push_back(vExchange.at(i));
          pMarket->m_mapForexExchange[vExchange.at(i)] = lTemp;
        }
      }
      pMarket->m_lTotalForexExchange = pMarket->m_vForexExchange.size();
      pMarket->m_fForexExhangeUpdated = true;
      break;
      case __FOREX_SYMBOLS__:
      ProcessFinnhubForexSymbol(pWebData, vForexSymbol);
      for (auto& pSymbol : vForexSymbol) {
        if (pMarket->m_mapForexSymbol.find(pSymbol->m_strSymbol) == pMarket->m_mapForexSymbol.end()) {
          pSymbol->m_strExchange = pMarket->m_vForexExchange.at(pMarket->m_CurrentFinnhubInquiry.m_lStakeIndex);
          lTemp = pMarket->m_mapForexSymbol.size();
          pMarket->m_mapForexSymbol[pSymbol->m_strSymbol] = lTemp;
          pMarket->m_vForexSymbol.push_back(pSymbol);
        }
      }
      pMarket->m_lTotalForexSymbol = pMarket->m_vForexSymbol.size();
      break;
      case __FOREX_CANDLES__:
      ASSERT(finnhubInquiry.m_lStakeIndex == pMarket->m_lCurrentUpdateForexDayLinePos);
      ProcessFinnhubForexCandle(pWebData, pMarket->m_vForexSymbol.at(pMarket->m_lCurrentUpdateForexDayLinePos));
      TRACE("����%s��������\n", pMarket->m_vForexSymbol.at(pMarket->m_lCurrentUpdateForexDayLinePos)->m_strSymbol.GetBuffer());
      break;
      case __FOREX_ALL_RATES__:
      break;
      default:
      break;
      }
      pMarket->m_fFinnhubInquiring = false;
    }
  }

  return true;
}