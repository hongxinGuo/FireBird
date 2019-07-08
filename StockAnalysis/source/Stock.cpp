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
	
  m_wMarket = 0;
  m_strStockCode = "";
	m_strStockName = "";
	m_iStockCode = 0;
	m_fActive = false;
	m_lLastClose = m_lOpen = 0;
  m_lHigh = m_lLow = m_lNew = 0;
  for (int i = 0; i < 5; i++) {
    m_lPBuy[i] = m_lPSell[i] = 0;
    m_lVBuy[i] = m_lVSell[i] = 0;
  }
  m_lVolume = 0;
  m_lAmount = 0;
   m_dRelativeStrong = 0;
  m_nHand = 100;
  m_lTransactionNumber = 0;
  m_lTransactionNumberBelow5000 = 0;
  m_lTransactionNumberBelow50000 = 0;
  m_lTransactionNumberBelow200000 = 0;
  m_lTransactionNumberAbove200000 = 0;
  Reset();

  m_fDayLineLoaded = false;
  m_fDayLineNeededSaving = false;

  m_fChoiced = false;
  m_fMinLineUpdated = false;
  m_fDayKLineUpdated = false;

  m_fStartCalculating = false;  // ʵʱ���ݿ�ʼ�����ʶ����һ��ʵʱ����ֻ��������ʼ��ϵͳ���������ڼ��㡣�ӵڶ������ݿ�ʼ�������Ч��

  m_lAttackBuyAbove200000 = m_lAttackBuyBelow200000 = m_lAttackBuyBelow50000 = 0;
  m_lAttackSellAbove200000 = m_lAttackSellBelow200000 = m_lAttackSellBelow50000 = 0;
  m_lCancelBuyVolume = m_lCancelSellVolume = 0;
  m_lStrongBuyVolume = m_lStrongSellVolume = 0;
  m_lOrdinaryBuyVolume = m_lOrdinarySellVolume = 0;
  m_lUnknownVolume = 0;
  m_pLastRTData = nullptr;
}

CStock::~CStock(void) {

}

void CStock::Reset(void) {
  m_lHigh = m_lNew = m_lLow = 0;
  m_lVolume = 0;
  m_lAmount = 0;

  for (int i = 0; i < 5; i++) {
    m_lPBuy[i] = m_lPSell[i] = 0;
    m_lVBuy[i] = m_lVSell[i] = 0;
  }
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
  m_lNew = pRTData->m_lNew;
  m_lHigh = pRTData->m_lHigh;
  m_lLow = pRTData->m_lLow;
  m_lVolume = pRTData->m_lVolume;
  m_lAmount = pRTData->m_lAmount;
  for (int i = 0; i < 5; i++) {
    m_lPBuy[i] = pRTData->m_lPBuy[i];
    m_lPSell[i] = pRTData->m_lPSell[i];
    m_lVBuy[i] = pRTData->m_lVBuy[i];
    m_lVSell[i] = pRTData->m_lVSell[i];
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
  CString str;
  char buffer[100];
  CStockRTDataPtr pRTData;
  long lCurrentGuaDanTransactionPrice = 0;

  long lTotalNumber = GetRTDataDequeSize(); //  ������еĳ��ȡ�����ͬ�����ƻ�ȡ����ֵ.
  // ����Ϊ����ҵ��仯����Ʊ��Ծ�ȡ����������
  for (int i = 0; i < lTotalNumber; i++) {
    pRTData = PopRTStockData(); // ����ͬ�����ƻ�ȡ����
    if ((pRTData->m_lNew != 0) && (pRTData->m_lOpen != 0)) { // ������Ч
      if (m_fStartCalculating) {
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
          m_dCurrentGuaDanTransactionPrice = ((double)(pRTData->m_lAmount - m_pLastRTData->m_lAmount)) / m_lCurrentGuadanTransactionVolume;
          lCurrentGuaDanTransactionPrice = m_dCurrentGuaDanTransactionPrice * 1000;
          if (lCurrentGuaDanTransactionPrice >= m_pLastRTData->m_lPBuy[0]) { // ����ǰ����1
            if (lCurrentGuaDanTransactionPrice <= m_pLastRTData->m_lPSell[0]) { // ����ǰ����1
              if ((m_pLastRTData->m_lPSell[0] - lCurrentGuaDanTransactionPrice) <= 2) { //һ��������
                m_lOrdinaryBuyVolume += m_lCurrentGuadanTransactionVolume;
                m_nCurrentTransactionType = __ORDINARY_BUY__;
              }
              else if ((lCurrentGuaDanTransactionPrice - m_pLastRTData->m_lPBuy[0]) <= 2) { // һ��������
                m_nCurrentTransactionType = __ORDINARY_SELL__;
                m_lOrdinarySellVolume += m_lCurrentGuadanTransactionVolume;
              }
              else { // �������ӣ���������
                m_nCurrentTransactionType = __UNKNOWN_BUYSELL__;
                m_lUnknownVolume += m_lCurrentGuadanTransactionVolume;
              }
            }
            else if (lCurrentGuaDanTransactionPrice < m_pLastRTData->m_lPSell[1]) { // ��������һ���������̶������������롣AttackBuy
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
              m_lAttackBuyVolume += m_lCurrentGuadanTransactionVolume;
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

          }
          else if (lCurrentGuaDanTransactionPrice > m_pLastRTData->m_lPBuy[1]) { // ��������һ���������̶���������������AttackSell
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
            m_lAttackBuyVolume += m_lCurrentGuadanTransactionVolume;
            m_lStrongBuyVolume += m_lCurrentGuadanTransactionVolume;
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
          if( gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
            if (gl_ChinaStockMarket.m_pCurrentStock->m_strStockCode.Compare(m_strStockCode) == 0) {
              CTime ctime(pRTData->m_time);
              sprintf_s(buffer, "%02d:%02d:%02d %s %d�ɳɽ���%10.3f", ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond(), m_strStockCode.GetBuffer(),
                m_lCurrentGuadanTransactionVolume, m_dCurrentGuaDanTransactionPrice);
              str = buffer;
              CString str1;
              switch (m_nCurrentTransactionType) {
              case __STRONG_BUY__:
                str1 = _T(" STRONG BUY");
                break;
              case __STRONG_SELL__:
                str1 = _T(" STRONG SELL");
                break;
              case __ATTACK_BUY__:
                str1 = _T(" ATTACK BUY");
                break;
              case __ATTACK_SELL__:
                str1 = _T(" ATTACK SELL");
                break;
              case __ORDINARY_BUY__:
                str1 = _T(" ORDINARY BUY");
                break;
              case __ORDINARY_SELL__:
                str1 = _T(" ORDINARY SELL");
                break;
              case __UNKNOWN_BUYSELL__:
                str1 = _T(" UNKNOWN BUYSELL");
                break;
              default:
                break;
              }
              str += str1;
              gl_systemMessage.PushWarningMessage(str); // ����ͬ�����ƴ�����Ϣ
            }
          }
        }

        // ���濪ʼ�����ҵ����
        AnalysisingGuaDan(pRTData, m_pLastRTData, m_nCurrentTransactionType, m_dCurrentGuaDanTransactionPrice);

        // ����ǰ����״̬
        m_pLastRTData = pRTData;
      }
      else { // ��һ�����ݣ���ʼ��ϵͳ
        m_pLastRTData = pRTData;
        m_fStartCalculating = true;
        // ���õ�һ�εĹҵ�ӳ�䡣
        for (int j = 0; j < 5; j++) {
          m_mapGuaDan[pRTData->m_lPBuy[j]] = pRTData->m_lVBuy[i];
          m_mapGuaDan[pRTData->m_lPSell[j]] = pRTData->m_lVSell[i];
        }
      }
    }
  }

  if (lTotalNumber == 0) return false;
  else return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ҵ����ı仯���ɹ����̵߳���
//
// ����map�ṹ�洢����������������⣬Ψһ�������Ѱַ����ʱ�䳤����Ҫ���Ժ󿴿��ٶ���Ρ�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CStock::AnalysisingGuaDan(CStockRTDataPtr pCurrentRTData, CStockRTDataPtr pLastRTData, int nTRanscationType, double dCurrentTransactionPrice) {
  bool fNeedCheck[10]; // ��Ҫ���Ĺҵ�λ�á�˳��Ϊ��Sell4, Sell3, ... Sell0, Buy0, .... Buy3, Buy4
  m_lCurrentCanselSellVolume = 0;
  m_lCurrentCanselBuyVolume = 0;

  for (int i = 0; i < 10; i++) fNeedCheck[i] = true; // Ԥ��Ϊ��Ҫ���㡣

  // ȷ����Ҫ������Щ�ҵ���һ����ʮ����û���յ�����Ӱ��Ķ�Ҫ���㡣
  switch (nTRanscationType) {
  case __NO_TRANSACTION__: // û�гɽ�������ٵ���������Ӧ��λ�ϵĳ�����
    break;
  case __ATTACK_BUY__: // ����һ�Ѿ���ʧ����������Ӱ�졣���������߸��ҵ���
    fNeedCheck[3] = false;
    fNeedCheck[4] = fNeedCheck[5] = false;
    break;
  case __STRONG_BUY__: // ����һ����������ʧ������������δ������Ҫ���㡣
    if (dCurrentTransactionPrice < pLastRTData->m_lPSell[2]) { // ����4������5�д�
      fNeedCheck[2] = false;
    }
    else if (dCurrentTransactionPrice < pLastRTData->m_lPSell[3]) { // ����5�д�
      fNeedCheck[1] = false;
      fNeedCheck[2] = false;
    }
    else { // ���е���������Ӱ��
      fNeedCheck[0] = false;
      fNeedCheck[2] = false;
      fNeedCheck[1] = false;
    }
    fNeedCheck[3] = false;
    fNeedCheck[4] = fNeedCheck[5] = false;
    break;
  case __ORDINARY_BUY__: // ����һ��Ӱ�졣���������˸��ҵ���
  case __UNKNOWN_BUYSELL__: // ����һ����һ��Ӱ�졣���������˸��ҵ���
  case __ORDINARY_SELL__: // ��һ��Ӱ�졣���������˸��ҵ���
    fNeedCheck[4] = fNeedCheck[5] = false;
    break;
  case __ATTACK_SELL__: // ��һ��ʧ���򵥶���Ӱ�졣���������߸��ҵ���
    fNeedCheck[4] = fNeedCheck[5] = fNeedCheck[6] = false;
    break;
  case __STRONG_SELL__: // ��һ���򵥶���ʧ����������Ҫ���㡣
    if (pLastRTData->m_lPBuy[3] > dCurrentTransactionPrice) { // �����򵥶���Ӱ��
      fNeedCheck[7] = false;
      fNeedCheck[8] = false;
      fNeedCheck[9] = false;
    }
    else if (pLastRTData->m_lPBuy[2] > dCurrentTransactionPrice) { // ��5�д�
      fNeedCheck[7] = false;
      fNeedCheck[8] = false;
    }
    else { // ��4����5û�仯
      fNeedCheck[7] = false;
    }
    fNeedCheck[6] = false;
    fNeedCheck[4] = fNeedCheck[5] = false;
    break;
  default:
    ASSERT(0); // ������ִ�е��˷�֧��
    break;
  }
  
  for (int i = 0; i < 5; i++) { // ��Ŀǰ��ʮ���ҵ�״̬����ӳ���У��ҵ�״̬����Ϊ����̬
    m_mapGuaDan[pCurrentRTData->m_lPSell[i]] = pCurrentRTData->m_lVSell[i];
    m_mapGuaDan[pCurrentRTData->m_lPBuy[i]] = pCurrentRTData->m_lVBuy[i];
  }

  // �����ʮ���ҵ���λ�Ϲ����ı仯���.��Ŀǰ�ҵ�״̬��֮ǰ��ʮ����λ��pLastRTData�����ģ���ȣ��鿴�仯
  for (int i = 0; i < 10; i++) {
    switch (i) {
    case 0: // ������
      if (fNeedCheck[0]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPSell[4]) == m_mapGuaDan.end()) { // ����˼�λ��û�йҵ�
          m_mapGuaDan[pLastRTData->m_lPSell[4]] = pLastRTData->m_lVSell[4]; // ���ڴ˼�λ�ϴ洢Ŀǰ�Ĺ����������µ�ӳ�䣩��
        }
        else {  // �йҵ�
          if (m_mapGuaDan.at(pLastRTData->m_lPSell[4]) < pLastRTData->m_lVSell[4]) { // �����˵Ļ�
            m_lCurrentCanselSellVolume += pLastRTData->m_lVSell[4] - m_mapGuaDan.at(pLastRTData->m_lPSell[4]);
            m_lCancelSellVolume += pLastRTData->m_lVSell[4] - m_mapGuaDan.at(pLastRTData->m_lPSell[4]);
          }
          else { // 
            m_mapGuaDan.at(pLastRTData->m_lPSell[4]) = pLastRTData->m_lVSell[4];
          }
        }
      }
      break;
    case 1: // ������
      if (fNeedCheck[1]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPSell[3]) == m_mapGuaDan.end()) { // �˼�λû�йҵ�
          m_mapGuaDan[pLastRTData->m_lPSell[3]] = pLastRTData->m_lVSell[3];
        }
        else {  // �йҵ�
          if (m_mapGuaDan.at(pLastRTData->m_lPSell[3]) < pLastRTData->m_lVSell[3]) { // �����˵Ļ�
            m_lCurrentCanselSellVolume += pLastRTData->m_lVSell[3] - m_mapGuaDan.at(pLastRTData->m_lPSell[3]);
            m_lCancelSellVolume += pLastRTData->m_lVSell[3] - m_mapGuaDan.at(pLastRTData->m_lPSell[3]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPSell[3]) = pLastRTData->m_lVSell[3];
          }
        }
      }
      break;
    case 2: // ������
      if (fNeedCheck[2]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPSell[2]) == m_mapGuaDan.end()) { // �˼�λû�йҵ�
          m_mapGuaDan[pLastRTData->m_lPSell[2]] = pLastRTData->m_lVSell[2];
        }
        else {  // �йҵ�
          if (m_mapGuaDan.at(pLastRTData->m_lPSell[2]) < pLastRTData->m_lVSell[2]) { // �����˵Ļ�
            m_lCurrentCanselSellVolume += pLastRTData->m_lVSell[2] - m_mapGuaDan.at(pLastRTData->m_lPSell[2]);
            m_lCancelSellVolume += pLastRTData->m_lVSell[2] - m_mapGuaDan.at(pLastRTData->m_lPSell[2]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPSell[2]) = pLastRTData->m_lVSell[2];
          }
        }
      }
      break;
    case 3: // ������
      if (fNeedCheck[3]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPSell[1]) == m_mapGuaDan.end()) { // �˼�λû�йҵ�
          m_mapGuaDan[pLastRTData->m_lPSell[1]] = pLastRTData->m_lVSell[1];
        }
        else {  // �йҵ�
          if (m_mapGuaDan.at(pLastRTData->m_lPSell[1]) < pLastRTData->m_lVSell[1]) { // �����˵Ļ�
            m_lCurrentCanselSellVolume += pLastRTData->m_lVSell[1] - m_mapGuaDan.at(pLastRTData->m_lPSell[1]);
            m_lCancelSellVolume += pLastRTData->m_lVSell[1] - m_mapGuaDan.at(pLastRTData->m_lPSell[1]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPSell[1]) = pLastRTData->m_lVSell[1];
          }
        }
      }
      break;
    case 4: // ����һ
      if (fNeedCheck[4]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPSell[0]) == m_mapGuaDan.end()) { // �˼�λû�йҵ�
          m_mapGuaDan[pLastRTData->m_lPSell[0]] = pLastRTData->m_lVSell[0];
        }
        else {  // �йҵ�
          if (m_mapGuaDan.at(pLastRTData->m_lPSell[0]) < pLastRTData->m_lVSell[0]) { // �����˵Ļ�
            m_lCurrentCanselSellVolume += pLastRTData->m_lVSell[0] - m_mapGuaDan.at(pLastRTData->m_lPSell[0]);
            m_lCancelSellVolume += pLastRTData->m_lVSell[0] - m_mapGuaDan.at(pLastRTData->m_lPSell[0]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPSell[0]) = pLastRTData->m_lVSell[0];
          }
        }
      }
      break;
    case 5:   // ����Ϊ�򵥱仯�������һ
      if (fNeedCheck[5]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPBuy[0]) == m_mapGuaDan.end()) { // �˼�λû�йҵ�
          m_mapGuaDan[pLastRTData->m_lPBuy[0]] = pLastRTData->m_lVBuy[0];
        }
        else {  // �йҵ�
          if (m_mapGuaDan.at(pLastRTData->m_lPBuy[0]) < pLastRTData->m_lVBuy[0]) { // �����˵Ļ�
            m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy[0] - m_mapGuaDan.at(pLastRTData->m_lPBuy[0]);
            m_lCancelBuyVolume += pLastRTData->m_lVBuy[0] - m_mapGuaDan.at(pLastRTData->m_lPBuy[0]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPBuy[0]) = pLastRTData->m_lVBuy[0];
          }
        }
      }
      break;
    case 6: // �򵥶�
      if (fNeedCheck[6]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPBuy[1]) == m_mapGuaDan.end()) { // �˼�λû�йҵ�
          m_mapGuaDan[pLastRTData->m_lPBuy[1]] = pLastRTData->m_lVBuy[1];
        }
        else {  // �йҵ�
          if (m_mapGuaDan.at(pLastRTData->m_lPBuy[1]) < pLastRTData->m_lVBuy[1]) { // �����˵Ļ�
            m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy[1] - m_mapGuaDan.at(pLastRTData->m_lPBuy[1]);
            m_lCancelBuyVolume += pLastRTData->m_lVBuy[1] - m_mapGuaDan.at(pLastRTData->m_lPBuy[1]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPBuy[1]) = pLastRTData->m_lVBuy[1];
          }
        }
      }
      break;
    case 7: // ����
      if (fNeedCheck[7]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPBuy[2]) == m_mapGuaDan.end()) { // �˼�λû�йҵ�
          m_mapGuaDan[pLastRTData->m_lPBuy[2]] = pLastRTData->m_lVBuy[2];
        }
        else {  // �йҵ�
          if (m_mapGuaDan.at(pLastRTData->m_lPBuy[2]) < pLastRTData->m_lVBuy[2]) { // �����˵Ļ�
            m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy[2] - m_mapGuaDan.at(pLastRTData->m_lPBuy[2]);
            m_lCancelBuyVolume += pLastRTData->m_lVBuy[2] - m_mapGuaDan.at(pLastRTData->m_lPBuy[2]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPBuy[2]) = pLastRTData->m_lVBuy[2];
          }
        }
      }
      break;
    case 8: // ����
      if (fNeedCheck[8]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPBuy[3]) == m_mapGuaDan.end()) { // �˼�λû�йҵ�
          m_mapGuaDan[pLastRTData->m_lPBuy[3]] = pLastRTData->m_lVBuy[3];
        }
        else {  // �йҵ�
          if (m_mapGuaDan.at(pLastRTData->m_lPBuy[3]) < pLastRTData->m_lVBuy[3]) { // �����˵Ļ�
            m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy[3] - m_mapGuaDan.at(pLastRTData->m_lPBuy[3]);
            m_lCancelBuyVolume += pLastRTData->m_lVBuy[3] - m_mapGuaDan.at(pLastRTData->m_lPBuy[3]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPBuy[3]) = pLastRTData->m_lVBuy[3];
          }
        }
      }
      break;
    case 9: // ����
      if (fNeedCheck[9]) {
        if (m_mapGuaDan.find(pLastRTData->m_lPBuy[4]) == m_mapGuaDan.end()) { // �˼�λû�йҵ�
          m_mapGuaDan[pLastRTData->m_lPBuy[4]] = pLastRTData->m_lVBuy[4];
        }
        else {  // �йҵ�
          if (m_mapGuaDan.at(pLastRTData->m_lPBuy[4]) < pLastRTData->m_lVBuy[4]) { // �����˵Ļ�
            m_lCurrentCanselBuyVolume += pLastRTData->m_lVBuy[4] - m_mapGuaDan.at(pLastRTData->m_lPBuy[4]);
            m_lCancelBuyVolume += pLastRTData->m_lVBuy[4] - m_mapGuaDan.at(pLastRTData->m_lPBuy[4]);
          }
          else {
            m_mapGuaDan.at(pLastRTData->m_lPBuy[4]) = pLastRTData->m_lVBuy[4];
          }
        }
      }
      break;
    default : // û�����ѡ��
      ASSERT(0);
    }
  }

  // ��ʾ��ǰȡ���ҵ������
  if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
    if (gl_ChinaStockMarket.m_pCurrentStock->m_strStockCode.Compare(m_strStockCode) == 0) {
      CString str1;
      char buffer[30];
      if (m_lCurrentCanselSellVolume > 0) {
        sprintf_s(buffer, "��ǰȡ����������%d", m_lCurrentCanselSellVolume);
        str1 = buffer;
        sprintf_s(buffer, "  ��ȡ����������%d", m_lCancelSellVolume);
        str1 += buffer;
        gl_systemMessage.PushDataBaseMessage(str1);   // ����ͬ�����ƴ�����Ϣ

      }
      if (m_lCurrentCanselBuyVolume > 0) {
        sprintf_s(buffer, "��ǰȡ��������%d", m_lCurrentCanselBuyVolume);
        str1 = buffer;
        sprintf_s(buffer, "  ��ȡ��������%d", m_lCancelBuyVolume);
        str1 += buffer;
        gl_systemMessage.PushTrace1Message(str1); // ����ͬ�����ƴ�����Ϣ

      }
    }
  }
  return(true);
}

bool CStock::SaveRealTimeData(CSetRealTimeData * psetRTData) {

  ASSERT(psetRTData->IsOpen());
  for ( auto pRTData : m_dequeRTStockData) {
    psetRTData->AddNew();
    psetRTData->m_Time = pRTData->m_time;
    psetRTData->m_lMarket = m_wMarket;
    psetRTData->m_StockCode = m_strStockCode;
    psetRTData->m_StockName = m_strStockName;
    psetRTData->m_CurrentPrice = (double)pRTData->m_lNew / 1000;
    psetRTData->m_High = (double)pRTData->m_lHigh / 1000;
    psetRTData->m_Low = (double)pRTData->m_lLow / 1000;
    psetRTData->m_LastClose = (double)pRTData->m_lLastClose / 1000;
    psetRTData->m_Open = (double)pRTData->m_lOpen / 1000;
    psetRTData->m_Volume = pRTData->m_lVolume;
    psetRTData->m_Amount = (double)pRTData->m_lAmount;
    psetRTData->m_Stroke = 0;
    psetRTData->m_PBuy1 = (double)pRTData->m_lPBuy[0] / 1000;
    psetRTData->m_VBuy1 = pRTData->m_lVBuy[0];
    psetRTData->m_PSell1 = (double)pRTData->m_lPSell[0] / 1000;
    psetRTData->m_VSell1 = pRTData->m_lVSell[0];

    psetRTData->m_PBuy2 = (double)pRTData->m_lPBuy[1] / 1000;
    psetRTData->m_VBuy2 = pRTData->m_lVBuy[1];
    psetRTData->m_PSell2 = (double)pRTData->m_lPSell[1] / 1000;
    psetRTData->m_VSell2 = pRTData->m_lVSell[1];

    psetRTData->m_PBuy3 = (double)pRTData->m_lPBuy[2] / 1000;
    psetRTData->m_VBuy3 = pRTData->m_lVBuy[2];
    psetRTData->m_PSell3 = (double)pRTData->m_lPSell[2] / 1000;
    psetRTData->m_VSell3 = pRTData->m_lVSell[2];

    psetRTData->m_PBuy4 = (double)pRTData->m_lPBuy[3] / 1000;
    psetRTData->m_VBuy4 = pRTData->m_lVBuy[3];
    psetRTData->m_PSell4 = (double)pRTData->m_lPSell[3] / 1000;
    psetRTData->m_VSell4 = pRTData->m_lVSell[3];

    psetRTData->m_PBuy5 = (double)pRTData->m_lPBuy[4] / 1000;
    psetRTData->m_VBuy5 = pRTData->m_lVBuy[4];
    psetRTData->m_PSell5 = (double)pRTData->m_lPSell[4] / 1000;
    psetRTData->m_VSell5 = pRTData->m_lVSell[4];
    psetRTData->Update();

  }

  return true;
}

void CStock::PushRTStockData(CStockRTDataPtr pData)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeRTStockData.push_back(pData);
    singleLock.Unlock();
  }
}

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

void CStock::SetDayLineNeedSavingFlag(bool fFlag)
{
  CSingleLock singleLock(&m_DayLineNeedSacingLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fDayLineNeededSaving = fFlag;
    singleLock.Unlock();
  }
}

bool CStock::IsDayLineNeedSaving(void)
{
  CSingleLock singleLock(&m_DayLineNeedSacingLock);
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


///////////////////////////////////////////////////////////////////////////////////
//
//	������Ʊ����ʷ���ݴ����������ݿ�(DayKLine.mdb)
//
//	һ�δ�����̫�࣬���Խ����ݿ�򿪺͹رշ���������������һ��Ļ���
// ������Ĺ�Ʊ̫��ʱ(>400)�����ݿ�������ֹ��ϡ�
//
// ���ñ��������ʹ��On_Timer, On_Idle.��ϵͳ����ʱһ�δ���һֻ��Ʊ��
//
// �洢ʱҪ���Ŵ棬ʹ��m_lTotalDLDayKLine,��Ϊ�п��ܹ�Ʊ�����������������ص�����������
// �˴洢����ֻ�����ص����ߴ������ݿ⡣�������ܡ�
//
////////////////////////////////////////////////////////////////////////////////////



