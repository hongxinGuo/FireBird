#include "stdafx.h"

#include "globedef.h"
//#include "timer.h"

#include "Stock.h"
#include"Market.h"

using namespace std;
#include"string"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CStock::CStock() : CObject() {
	
  Reset();

}

CStock::~CStock(void) {

}

void CStock::Reset(void) {
  m_wMarket = 0;
  m_strStockCode = "";
  m_strStockName = "";
  m_iStockCode = 0;
  m_fActive = false;
  m_lLastClose = m_lOpen = 0;
  m_lHigh = m_lLow = m_lNew = 0;
  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = m_lPSell.at(i) = 0;
    m_lVBuy.at(i) = m_lVSell.at(i) = 0;
  }
  m_lVolume = 0;
  m_lAmount = 0;
  m_dRelativeStrong = 0;
  m_nHand = 100;

  m_lHigh = m_lNew = m_lLow = 0;

  m_lAttackBuyAmount = 0;
  m_lAttackSellAmount = 0;
  m_lCurrentVolume = 0;
  m_lAttackBuyVolume = 0;
  m_lCurrentAttackBuy = 0;
  m_lAttackSellVolume = 0;
  m_lCurrentAttackSell = 0;
  m_lStrongBuyVolume = 0;
  m_lCurrentStrongBuy = 0;
  m_lStrongSellVolume = 0;
  m_lCurrentStrongSell = 0;
  m_lUnknownVolume = 0;
  m_lCurrentUnknown = 0;
  m_lCancelBuyVolume = 0;
  m_lCancelSellVolume = 0;
  m_lTransactionNumber = 0;
  m_lTransactionNumberBelow5000 = 0;
  m_lTransactionNumberBelow50000 = 0;
  m_lTransactionNumberBelow200000 = 0;
  m_lTransactionNumberAbove200000 = 0;

  m_lOrdinaryBuyVolume = m_lAttackBuyBelow50000 = m_lAttackBuyBelow200000 = m_lAttackBuyAbove200000 = 0;
  m_lOrdinarySellVolume = m_lAttackSellBelow50000 = m_lAttackSellBelow200000 = m_lAttackSellAbove200000 = 0;

  m_lFirstDataVolume = 0;

  m_dCurrentGuaDanTransactionPrice = 0;

  m_fDayLineLoaded = false;
  m_fDayLineNeededSaving = false;

  m_fChoiced = false;
  m_fMinLineUpdated = false;
  m_fDayKLineUpdated = false;

  m_fStartCalculating = false;  // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��

  m_pLastRTData = nullptr;
}

void CStock::operator =(CStock &pStock) {
  m_wMarket = pStock.m_wMarket;
  m_strStockCode = pStock.m_strStockCode;
  m_strStockName = pStock.m_strStockName;
  m_nHand = pStock.m_nHand;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ���¹�Ʊ��ǰ��״̬��������ʾ��
//
//
///////////////////////////////////////////////////////////////////////////////////////////
void CStock::UpdataCurrentStatus(CStockRTDataPtr pRTData)
{
  m_Time = pRTData->m_time;
  m_lLastClose = pRTData->m_lLastClose;
  m_lNew = pRTData->m_lNew;
  m_lHigh = pRTData->m_lHigh;
  m_lLow = pRTData->m_lLow;
  m_lOpen = pRTData->m_lOpen;
  m_lVolume = pRTData->m_lVolume;
  m_lAmount = pRTData->m_lAmount;
  for (int i = 0; i < 5; i++) {
    m_lPBuy.at(i) = pRTData->m_lPBuy.at(i);
    m_lPSell.at(i) = pRTData->m_lPSell.at(i);
    m_lVBuy.at(i) = pRTData->m_lVBuy.at(i);
    m_lVSell.at(i) = pRTData->m_lVSell.at(i);
  }
}

bool CStock::LoadDayLine(CSetDayLine * psetDayLine)
{

  return false;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// �˺����������߳�ClientThreadCalculatingRTDataProc���ã��������ٶ�ȫ�ֱ����Ĳ�����
//
// ��������͹ҵ�����ƺ��ֿ�����ȽϺã�˼·����������Ŀǰ�������������й�����Ҫ���Ǿɻ������ٶ��ˡ�
//
//
////////////////////////////////////////////////////////////////////////////////////
bool CStock::CalculateRTData(void) {
  CStockRTDataPtr pRTData;

  const long lTotalNumber = GetRTDataDequeSize(); //  ������еĳ��ȡ�����ͬ�����ƻ�ȡ����ֵ.
  // ����Ϊ����ҵ��仯����Ʊ��Ծ�ȡ����������
  for (int i = 0; i < lTotalNumber; i++) {
    pRTData = PopRTStockData(); // ����ͬ�����ƻ�ȡ����
    if ((pRTData->m_lNew != 0) && (pRTData->m_lOpen != 0)) { // ������Ч
      CalculateOneRTData(pRTData);
      if ((m_lOrdinaryBuyVolume < 0) || (m_lOrdinarySellVolume < 0) || (m_lAttackBuyVolume < 0)
        || (m_lAttackSellVolume < 0) || (m_lStrongBuyVolume < 0) || (m_lStrongSellVolume < 0)) {
        TRACE("%06d %s Error in volume\n", gl_systemTime.GetTime(),  m_strStockCode);
      }
      // ��ʾ��ǰ�������
      if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
        if (gl_ChinaStockMarket.m_pCurrentStock->GetStockCode().Compare(m_strStockCode) == 0) {
          if (gl_ChinaStockMarket.m_pCurrentStock->GetCurrentTransationVolume() > 0) {
            gl_ChinaStockMarket.m_pCurrentStock->ReportGuaDanTransaction();
          }
        }
      }
      // ��ʾ��ǰȡ���ҵ������
      if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
        if (gl_ChinaStockMarket.m_pCurrentStock->GetStockCode().Compare(m_strStockCode) == 0) {
          gl_ChinaStockMarket.m_pCurrentStock->ReportGuaDan();
        }
      }

    }
  }

  if (lTotalNumber == 0) return false;
  else return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ���ҵ���
// ����ǵ�һ�μ��㣬��ֻ���ó�ʼ״̬��
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::CalculateOneRTData(CStockRTDataPtr pRTData) {
  long lCurrentGuaDanTransactionPrice = 0;

  if (IsStartCalculating()) {
    m_lCurrentGuadanTransactionVolume = pRTData->m_lVolume - m_pLastRTData->m_lVolume;
    if (m_lCurrentGuadanTransactionVolume == 0) { // �޳ɽ���
      // ���ҵ����
      m_nCurrentTransactionType = __NO_TRANSACTION__;
    }
    else { // �гɽ���
      m_lTransactionNumber++; // �ɽ�����һ��
      if (m_lCurrentGuadanTransactionVolume < 5000) {
        m_lTransactionNumberBelow5000++;
      }
      else if (m_lCurrentGuadanTransactionVolume < 50000) {
        m_lTransactionNumberBelow50000++;
      }
      else if (m_lCurrentGuadanTransactionVolume < 200000) {
        m_lTransactionNumberBelow200000++;
      }
      else {
        m_lTransactionNumberAbove200000++;
      }
      m_dCurrentGuaDanTransactionPrice = (static_cast<double>(pRTData->m_lAmount - m_pLastRTData->m_lAmount)) / m_lCurrentGuadanTransactionVolume;
      lCurrentGuaDanTransactionPrice = m_dCurrentGuaDanTransactionPrice * 1000; // ���ɱȽ��õļ۸񣨷Ŵ�һǧ������ó����ͣ�
      if (lCurrentGuaDanTransactionPrice >= m_pLastRTData->m_lPBuy.at(0)) { // ����ǰ����1
        if (lCurrentGuaDanTransactionPrice <= m_pLastRTData->m_lPSell.at(0)) { // ����ǰ����1
          if ((m_pLastRTData->m_lPSell.at(0) - lCurrentGuaDanTransactionPrice) <= 2) { //һ��������
            m_lOrdinaryBuyVolume += m_lCurrentGuadanTransactionVolume;
            m_nCurrentTransactionType = __ORDINARY_BUY__;
          }
          else if ((lCurrentGuaDanTransactionPrice - m_pLastRTData->m_lPBuy.at(0)) <= 2) { // һ��������
            m_nCurrentTransactionType = __ORDINARY_SELL__;
            m_lOrdinarySellVolume += m_lCurrentGuadanTransactionVolume;
          }
          else { // �������ӣ���������
            m_nCurrentTransactionType = __UNKNOWN_BUYSELL__;
            m_lUnknownVolume += m_lCurrentGuadanTransactionVolume;
          }
        }
        else if (lCurrentGuaDanTransactionPrice < m_pLastRTData->m_lPSell.at(1)) { // ��������һ���������̶������������롣AttackBuy
          m_nCurrentTransactionType = __ATTACK_BUY__;
          m_lAttackBuyVolume += m_lCurrentGuadanTransactionVolume;
          if (m_lCurrentGuadanTransactionVolume < 50000) {
            m_lAttackBuyBelow50000 += m_lCurrentGuadanTransactionVolume;
          }
          else if (m_lCurrentGuadanTransactionVolume < 200000) {
            m_lAttackBuyBelow200000 += m_lCurrentGuadanTransactionVolume;
          }
          else {
            m_lAttackBuyAbove200000 += m_lCurrentGuadanTransactionVolume;
          }
        }
        else { // �������̶���ǿ�����롣StrongBuy��
          m_nCurrentTransactionType = __STRONG_BUY__;
          m_lStrongBuyVolume += m_lCurrentGuadanTransactionVolume;
          if (m_lCurrentGuadanTransactionVolume < 50000) {
            m_lAttackBuyBelow50000 += m_lCurrentGuadanTransactionVolume;
          }
          else if (m_lCurrentGuadanTransactionVolume < 200000) {
            m_lAttackBuyBelow200000 += m_lCurrentGuadanTransactionVolume;
          }
          else {
            m_lAttackBuyAbove200000 += m_lCurrentGuadanTransactionVolume;
          }
        }
      } // ����ǰ��һ
      else if (lCurrentGuaDanTransactionPrice > m_pLastRTData->m_lPBuy.at(1)) { // ��������һ���������̶���������������AttackSell
        m_nCurrentTransactionType = __ATTACK_SELL__;
        m_lAttackSellVolume += m_lCurrentGuadanTransactionVolume;
        if (m_lCurrentGuadanTransactionVolume < 50000) {
          m_lAttackSellBelow50000 += m_lCurrentGuadanTransactionVolume;
        }
        else if (m_lCurrentGuadanTransactionVolume < 200000) {
          m_lAttackSellBelow200000 += m_lCurrentGuadanTransactionVolume;
        }
        else {
          m_lAttackSellAbove200000 += m_lCurrentGuadanTransactionVolume;
        }
      }
      else { // �������̶���ǿ��������StrongSell
        m_nCurrentTransactionType = __STRONG_SELL__;
        m_lStrongSellVolume += m_lCurrentGuadanTransactionVolume;
        if (m_lCurrentGuadanTransactionVolume < 50000) {
          m_lAttackSellBelow50000 += m_lCurrentGuadanTransactionVolume;
        }
        else if (m_lCurrentGuadanTransactionVolume < 200000) {
          m_lAttackSellBelow200000 += m_lCurrentGuadanTransactionVolume;
        }
        else {
          m_lAttackSellAbove200000 += m_lCurrentGuadanTransactionVolume;
        }
      }
      ASSERT(m_Time >= pRTData->m_time);
      ASSERT(pRTData->m_lVolume == m_lOrdinaryBuyVolume + m_lOrdinarySellVolume
        + m_lAttackBuyVolume + m_lAttackSellVolume + + m_lStrongBuyVolume + m_lStrongSellVolume + m_lUnknownVolume);
    }

    // ���濪ʼ�����ҵ����
    AnalysisingGuaDan(pRTData, m_pLastRTData, m_nCurrentTransactionType, lCurrentGuaDanTransactionPrice);

    // ����ǰ����״̬
    m_pLastRTData = pRTData;
  }
  else { // ��һ�����ݣ���ʼ��ϵͳ
    m_pLastRTData = pRTData;
    SetStartCalculating(true);
    m_lUnknownVolume = m_pLastRTData->m_lVolume; // ��һ�ιҵ����޷��ж�����״̬
    // ���õ�һ�εĹҵ�ӳ�䡣
    for (int j = 0; j < 5; j++) {
      SetGuaDan(pRTData->m_lPBuy.at(j), pRTData->m_lVBuy.at(j));
      SetGuaDan(pRTData->m_lPSell.at(j), pRTData->m_lVSell.at(j));
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ҵ����ı仯���ɹ����̵߳���
//
// ����map�ṹ�洢����������������⣬Ψһ�������Ѱַ����ʱ�䳤����Ҫ���Ժ󿴿��ٶ���Ρ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::AnalysisingGuaDan(CStockRTDataPtr pCurrentRTData, CStockRTDataPtr pLastRTData, int nTRanscationType, long lCurrentTransactionPrice) {
  array<bool, 10> fNeedCheck; // ��Ҫ���Ĺҵ�λ�á�˳��Ϊ��Sell4, Sell3, ... Sell0, Buy0, .... Buy3, Buy4
  m_lCurrentCanselSellVolume = 0;
  m_lCurrentCanselBuyVolume = 0;

  for (int i = 0; i < 10; i++) fNeedCheck.at(i) = true; // Ԥ��Ϊ��Ҫ���㡣

  // ȷ����Ҫ������Щ�ҵ���һ����ʮ����û���յ�����Ӱ��Ķ�Ҫ���㡣
  switch (nTRanscationType) {
  case __NO_TRANSACTION__: // û�гɽ�������ٵ���������Ӧ��λ�ϵĳ�����
    break;
  case __ATTACK_BUY__: // ����һ�Ѿ���ʧ����������Ӱ�졣���������߸��ҵ���
    fNeedCheck.at(3) = false;
    fNeedCheck.at(4) = fNeedCheck.at(5) = false;
    break;
  case __STRONG_BUY__: // ����һ����������ʧ������������δ������Ҫ���㡣
    if (lCurrentTransactionPrice < pLastRTData->m_lPSell.at(2)) { // ����4������5�д�
      fNeedCheck.at(2) = false;
    }
    else if (lCurrentTransactionPrice < pLastRTData->m_lPSell.at(3)) { // ����5�д�
      fNeedCheck.at(1) = false;
      fNeedCheck.at(2) = false;
    }
    else { // ���е���������Ӱ��
      fNeedCheck.at(0) = false;
      fNeedCheck.at(2) = false;
      fNeedCheck.at(1) = false;
    }
    fNeedCheck.at(3) = false;
    fNeedCheck.at(4) = fNeedCheck.at(5) = false;
    break;
  case __ORDINARY_BUY__: // ����һ��Ӱ�졣���������˸��ҵ���
  case __UNKNOWN_BUYSELL__: // ����һ����һ��Ӱ�졣���������˸��ҵ���
  case __ORDINARY_SELL__: // ��һ��Ӱ�졣���������˸��ҵ���
    fNeedCheck.at(4) = fNeedCheck.at(5) = false;
    break;
  case __ATTACK_SELL__: // ��һ��ʧ���򵥶���Ӱ�졣���������߸��ҵ���
    fNeedCheck.at(4) = fNeedCheck.at(5) = fNeedCheck.at(6) = false;
    break;
  case __STRONG_SELL__: // ��һ���򵥶���ʧ����������Ҫ���㡣
    if (pLastRTData->m_lPBuy.at(3) > lCurrentTransactionPrice) { // �����򵥶���Ӱ��
      fNeedCheck.at(7) = false;
      fNeedCheck.at(8) = false;
      fNeedCheck.at(9) = false;
    }
    else if (pLastRTData->m_lPBuy.at(2) > lCurrentTransactionPrice) { // ��5�д�
      fNeedCheck.at(7) = false;
      fNeedCheck.at(8) = false;
    }
    else { // ��4����5û�仯
      fNeedCheck.at(7) = false;
    }
    fNeedCheck.at(6) = false;
    fNeedCheck.at(4) = fNeedCheck.at(5) = false;
    break;
  default:
    ASSERT(0); // ������ִ�е��˷�֧��
    break;
  }

  if ((pCurrentRTData->m_lPSell.at(4) - pCurrentRTData->m_lPBuy.at(4)) > 90) { // ʮ����λ֮���п�λ
    // ��λ�������ǳɽ��ˣ�Ҳ�����ǳ����ˣ�Ŀǰ��������Щϸ�ڣ�ͳһ��Ϊ�ǳɽ��ˣ������㳷�������Ժ��ٷ���֮��
    // ����յ�ǰ�ҵ�֮��Ĺҵ�������Ȼ�����ϵ�ǰ��������пյ��Ļ�������Ȼ����ˡ�
    for (int i2 = pCurrentRTData->m_lPBuy.at(4); i2 <= pCurrentRTData->m_lPSell.at(4); i2 += 10) {
      m_mapGuaDan[i2] = 0; // �˴���Ҫ����ֵ���п���֮ǰΪ�գ��ʶ�����ʹ��at��������������[]�±������
    }
  }
  for (int i = 0; i < 5; i++) { // ��Ŀǰ��ʮ���ҵ�״̬����ӳ���У��ҵ�״̬����Ϊ����̬
    SetGuaDan(pCurrentRTData->m_lPSell.at(i), pCurrentRTData->m_lVSell.at(i));
    SetGuaDan(pCurrentRTData->m_lPBuy.at(i), pCurrentRTData->m_lVBuy.at(i));
  }

  // �����ʮ���ҵ���λ�Ϲ����ı仯���.��Ŀǰ�ҵ�״̬��֮ǰ��ʮ����λ��pLastRTData�����ģ���ȣ��鿴�仯
  for (int i = 0; i < 10; i++) {
    switch (i) {
    case 0: // ������
      if (fNeedCheck.at(0)) {
        if (GetGuaDan(pLastRTData->m_lPSell.at(4)) < pLastRTData->m_lVSell.at(4)) { // �����˵Ļ�
          m_lCurrentCanselSellVolume += pLastRTData->m_lVSell.at(4) - GetGuaDan(pLastRTData->m_lPSell.at(4));
          m_lCancelSellVolume += pLastRTData->m_lVSell.at(4) - GetGuaDan(pLastRTData->m_lPSell.at(4));
        }
      }
      break;
    case 1: // ������
      if (fNeedCheck.at(1)) {
        if (GetGuaDan(pLastRTData->m_lPSell.at(3)) < pLastRTData->m_lVSell.at(3)) { // �����˵Ļ�
          m_lCurrentCanselSellVolume += pLastRTData->m_lVSell.at(3) - GetGuaDan(pLastRTData->m_lPSell.at(3));
          m_lCancelSellVolume += pLastRTData->m_lVSell.at(3) - GetGuaDan(pLastRTData->m_lPSell.at(3));
        }
      }
      break;
    case 2: // ������
      if (fNeedCheck.at(2)) {
        if (GetGuaDan(pLastRTData->m_lPSell.at(2)) < pLastRTData->m_lVSell.at(2)) { // �����˵Ļ�
          m_lCurrentCanselSellVolume += pLastRTData->m_lVSell.at(2) - GetGuaDan(pLastRTData->m_lPSell.at(2));
          m_lCancelSellVolume += pLastRTData->m_lVSell.at(2) - GetGuaDan(pLastRTData->m_lPSell.at(2));
        }
      }
      break;
    case 3: // ������
      if (fNeedCheck.at(3)) {
        if (GetGuaDan(pLastRTData->m_lPSell.at(1)) < pLastRTData->m_lVSell.at(1)) { // �����˵Ļ�
          m_lCurrentCanselSellVolume += pLastRTData->m_lVSell.at(1) - GetGuaDan(pLastRTData->m_lPSell.at(1));
          m_lCancelSellVolume += pLastRTData->m_lVSell.at(1) - GetGuaDan(pLastRTData->m_lPSell.at(1));
        }
      }
      break;
    case 4: // ����һ
      if(fNeedCheck.at(4)) {
        if (GetGuaDan(pLastRTData->m_lPSell.at(0)) < pLastRTData->m_lVSell.at(0)) { // �����˵Ļ�
          m_lCurrentCanselSellVolume += pLastRTData->m_lVSell.at(0) - GetGuaDan(pLastRTData->m_lPSell.at(0));
          m_lCancelSellVolume += pLastRTData->m_lVSell.at(0) - GetGuaDan(pLastRTData->m_lPSell.at(0));
        }
      }
      break;
    case 5:   // ����Ϊ�򵥱仯�������һ
      if (fNeedCheck.at(5)) {
        if (GetGuaDan(pLastRTData->m_lPBuy.at(0)) < pLastRTData->m_lVBuy.at(0)) { // �����˵Ļ�
          m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy.at(0) - GetGuaDan(pLastRTData->m_lPBuy.at(0));
          m_lCancelBuyVolume += pLastRTData->m_lVBuy.at(0) - GetGuaDan(pLastRTData->m_lPBuy.at(0));
        }
      }
      break;
    case 6: // �򵥶�
      if (fNeedCheck.at(6)) {
        if (GetGuaDan(pLastRTData->m_lPBuy.at(1)) < pLastRTData->m_lVBuy.at(1)) { // �����˵Ļ�
          m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy.at(1) - GetGuaDan(pLastRTData->m_lPBuy.at(1));
          m_lCancelBuyVolume += pLastRTData->m_lVBuy.at(1) - GetGuaDan(pLastRTData->m_lPBuy.at(1));
        }
      }
      break;
    case 7: // ����
      if (fNeedCheck.at(7)) {
        if (GetGuaDan(pLastRTData->m_lPBuy.at(2)) < pLastRTData->m_lVBuy.at(2)) { // �����˵Ļ�
          m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy.at(2) - GetGuaDan(pLastRTData->m_lPBuy.at(2));
          m_lCancelBuyVolume += pLastRTData->m_lVBuy.at(2) - GetGuaDan(pLastRTData->m_lPBuy.at(2));
        }
      }
      break;
    case 8: // ����
      if (fNeedCheck.at(8)) {
        if (GetGuaDan(pLastRTData->m_lPBuy.at(3)) < pLastRTData->m_lVBuy.at(3)) { // �����˵Ļ�
          m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy.at(3) - GetGuaDan(pLastRTData->m_lPBuy.at(3));
          m_lCancelBuyVolume += pLastRTData->m_lVBuy.at(3) - GetGuaDan(pLastRTData->m_lPBuy.at(3));
        }
      }
      break;
    case 9: // ����
      if (fNeedCheck.at(9)) {
        if (GetGuaDan(pLastRTData->m_lPBuy.at(4)) < pLastRTData->m_lVBuy.at(4)) { // �����˵Ļ�
          m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy.at(4) - GetGuaDan(pLastRTData->m_lPBuy.at(4));
          m_lCancelBuyVolume += pLastRTData->m_lVBuy.at(4) - GetGuaDan(pLastRTData->m_lPBuy.at(4));
        }
      }
      break;
    default : // û�����ѡ��
      ASSERT(0);
    }
  }

  return(true);
}

void CStock::ReportGuaDanTransaction(void)
{
  char buffer[100];
  CString str, str2, strTime;
  const CTime ctime(m_pLastRTData->m_time);
  sprintf_s(buffer, "%02d:%02d:%02d", ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond());
  strTime = buffer;
  sprintf_s(buffer, " %s %I64d�ɳɽ���%10.3f    ", m_strStockCode.GetBuffer(),
    m_lCurrentGuadanTransactionVolume, m_dCurrentGuaDanTransactionPrice);
  str = strTime;
  str += buffer;
  CString str1;
  switch (m_nCurrentTransactionType) {
  case __STRONG_BUY__:
    str1 = _T(" STRONG BUY");
    sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_lStrongBuyVolume);
    break;
  case __STRONG_SELL__:
    str1 = _T(" STRONG SELL");
    sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_lStrongSellVolume);
    break;
  case __ATTACK_BUY__:
    str1 = _T(" ATTACK BUY");
    sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_lAttackBuyVolume);
    break;
  case __ATTACK_SELL__:
    str1 = _T(" ATTACK SELL");
    sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_lAttackSellVolume);
    break;
  case __ORDINARY_BUY__:
    str1 = _T(" ORDINARY BUY");
    sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_lOrdinaryBuyVolume);
    break;
  case __ORDINARY_SELL__:
    str1 = _T(" ORDINARY SELL");
    sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_lOrdinarySellVolume);
    break;
  case __UNKNOWN_BUYSELL__:
    str1 = _T(" UNKNOWN BUYSELL");
    sprintf_s(buffer, ": %I64d��  %I64d", m_lCurrentGuadanTransactionVolume, m_lUnknownVolume);
    break;
  default:
    break;
  }
  str += str1;
  // ��ʾ��ǰ�ɽ����
  gl_systemMessage.PushTransactionMessage(str); // ����ͬ�����ƴ�����Ϣ

  str2 = strTime;
  str2 += str1;
  str2 += buffer;
  gl_systemMessage.PushTrace2Message(str2);


}

void CStock::ReportGuaDan(void)
{
  CString str1;
  char buffer[30];
  if (m_lCurrentCanselSellVolume > 0) {
    sprintf_s(buffer, "��ǰȡ����������%I64d", m_lCurrentCanselSellVolume);
    str1 = buffer;
    sprintf_s(buffer, "  ��ȡ����������%I64d", m_lCancelSellVolume);
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1);   // ����ͬ�����ƴ�����Ϣ

  }
  if (m_lCurrentCanselBuyVolume > 0) {
    sprintf_s(buffer, "��ǰȡ��������%I64d", m_lCurrentCanselBuyVolume);
    str1 = buffer;
    sprintf_s(buffer, "  ��ȡ��������%I64d", m_lCancelBuyVolume);
    str1 += buffer;
    gl_systemMessage.PushCancelSellMessage(str1); // ����ͬ�����ƴ�����Ϣ

  }
}

bool CStock::SaveRealTimeData(CSetRealTimeData * psetRTData) {
  ASSERT(psetRTData != nullptr);
  ASSERT(psetRTData->IsOpen());
  for ( auto pRTData : m_dequeRTStockData) {
    psetRTData->AddNew();
    psetRTData->m_Time = pRTData->m_time;
    psetRTData->m_lMarket = m_wMarket;
    psetRTData->m_StockCode = m_strStockCode;
    psetRTData->m_StockName = m_strStockName;
    psetRTData->m_CurrentPrice = static_cast<double>(pRTData->m_lNew) / 1000;
    psetRTData->m_High = static_cast<double>(pRTData->m_lHigh) / 1000;
    psetRTData->m_Low = static_cast<double>(pRTData->m_lLow) / 1000;
    psetRTData->m_LastClose = static_cast<double>(pRTData->m_lLastClose) / 1000;
    psetRTData->m_Open = static_cast<double>(pRTData->m_lOpen) / 1000;
    psetRTData->m_Volume = pRTData->m_lVolume;
    psetRTData->m_Amount = static_cast<double>(pRTData->m_lAmount);
    psetRTData->m_Stroke = 0;
    psetRTData->m_PBuy1 = static_cast<double>(pRTData->m_lPBuy.at(0)) / 1000;
    psetRTData->m_VBuy1 = pRTData->m_lVBuy.at(0);
    psetRTData->m_PSell1 = static_cast<double>(pRTData->m_lPSell.at(0)) / 1000;
    psetRTData->m_VSell1 = pRTData->m_lVSell.at(0);

    psetRTData->m_PBuy2 = static_cast<double>(pRTData->m_lPBuy.at(1)) / 1000;
    psetRTData->m_VBuy2 = pRTData->m_lVBuy.at(1);
    psetRTData->m_PSell2 = static_cast<double>(pRTData->m_lPSell.at(1)) / 1000;
    psetRTData->m_VSell2 = pRTData->m_lVSell.at(1);

    psetRTData->m_PBuy3 = static_cast<double>(pRTData->m_lPBuy.at(2)) / 1000;
    psetRTData->m_VBuy3 = pRTData->m_lVBuy.at(2);
    psetRTData->m_PSell3 = static_cast<double>(pRTData->m_lPSell.at(2)) / 1000;
    psetRTData->m_VSell3 = pRTData->m_lVSell.at(2);

    psetRTData->m_PBuy4 = static_cast<double>(pRTData->m_lPBuy.at(3)) / 1000;
    psetRTData->m_VBuy4 = pRTData->m_lVBuy.at(3);
    psetRTData->m_PSell4 = static_cast<double>(pRTData->m_lPSell.at(3)) / 1000;
    psetRTData->m_VSell4 = pRTData->m_lVSell.at(3);

    psetRTData->m_PBuy5 = static_cast<double>(pRTData->m_lPBuy.at(4)) / 1000;
    psetRTData->m_VBuy5 = pRTData->m_lVBuy.at(4);
    psetRTData->m_PSell5 = static_cast<double>(pRTData->m_lPSell.at(4)) / 1000;
    psetRTData->m_VSell5 = pRTData->m_lVSell.at(4);
    psetRTData->Update();

  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ�����ƴ洢ʵʱ���ݡ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CStock::PushRTStockData(CStockRTDataPtr pData)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeRTStockData.push_back(pData);
    singleLock.Unlock();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡʵʱ���ݡ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
CStockRTDataPtr CStock::PopRTStockData(void)
{
  CStockRTDataPtr pData;
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_dequeRTStockData.front();
    m_dequeRTStockData.pop_front();
    singleLock.Unlock();
    return pData;
  }
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡջ�ڵ�ʵʱ����������
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
long CStock::GetRTDataDequeSize(void)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeRTStockData.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ���������������Ƿ���Ҫ�洢��ʶ��
//
//
////////////////////////////////////////////////////////////////////////////////////////////////

void CStock::SetDayLineNeedSavingFlag(bool fFlag)
{
  CSingleLock singleLock(&m_DayLineNeedSavingLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fDayLineNeededSaving = fFlag;
    singleLock.Unlock();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ͬ��������ȡ�����Ƿ���Ҫ�洢��ʶ��
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::IsDayLineNeedSaving(void)
{
  CSingleLock singleLock(&m_DayLineNeedSavingLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag = m_fDayLineNeededSaving;
    singleLock.Unlock();
    return fFlag;
  }
  return false;
}

#ifdef _DEBUG
void CStock::AssertValid() const
{
  CObject::AssertValid();
}

void CStock::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);
}

#endif //_DEBUG


