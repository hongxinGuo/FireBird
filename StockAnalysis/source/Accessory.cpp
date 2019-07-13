#include"stdafx.h"
#include"atltime.h"

#include"globedef.h"

#include"accessory.h"

#include"StockID.h"
#include"Stock.h"
#include"Market.h"

#include"SetDayLine.h"

///////////////////////////////////////////////////////////////////////////////////
//
// ����lDay���������ǿ��, lDay�ĸ�ʽΪ��YYYYMMDD,�� 19990605.
// �����߰��ǵ����к�,�����ǿ�������ڶ����е�λ��
//
//////////////////////////////////////////////////////////////////////////////////
bool CalculateOneDayRelativeStrong(long lDay) {
  vector<StockIDPtr> vStockID;
  vector<int> vIndex;
  vector<double> vRelativeStrong;
  int iTotalAShare = 0;
  CString strSQL;
  CString strDay;
  char  pch[30];
  int iStockNumber = 0, j = 0;
  CTime ctTime;
  CSetDayLine setDayKLine;
  long lYear = lDay / 10000;
  long lMonth = lDay / 100 - lYear * 100;
  long lDayOfMonth = lDay - lYear * 10000 - lMonth * 100;
  char buffer[100];
  gl_systemStatus.SetCalculateRSInProcess(true);

  for (j = 0; j < 30; j++) pch[j] = 0x000;

  _ltoa_s(lDay, pch, 10);
  strDay = pch;
  setDayKLine.m_strSort = _T("[UpDownRate]");
  setDayKLine.m_strFilter = _T("[Time] =");
  setDayKLine.m_strFilter += strDay;
  setDayKLine.Open();
  if (setDayKLine.IsEOF()) { // ���ݼ�Ϊ�գ���������û�н���
    setDayKLine.Close();
    return false;
  }
  setDayKLine.m_pDatabase->BeginTrans();
  iStockNumber = 0;
  while (!setDayKLine.IsEOF()) {
    if (gl_ChinaStockMarket.IsAStock(setDayKLine.m_StockCode)) {
      long lIndex = gl_mapTotalStockToIndex.at(setDayKLine.m_StockCode);
      vStockID.push_back(gl_vTotalStock.at(lIndex));
      vIndex.push_back(iStockNumber); // ��A�ɵ�������¼�������С�
      iTotalAShare++;
    }
    iStockNumber++;
    setDayKLine.MoveNext();
  }

  setDayKLine.MoveFirst();
  int iCount = 0;
  int iBefore = 0;
  while (iCount < vIndex.size()) { // ֻ�����Ծ��Ʊ�����ǿ��
    if (gl_fExiting) break; // ���ݿ���ʱ�����ܷ�ʱ�䣬��ϵͳ�˳�ʱ���ݿ����Ҫ�����жϡ�
    for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // ��������ȥ�������ݿ⡣
      setDayKLine.MoveNext();
    }
    setDayKLine.Edit();
    if ((((double)setDayKLine.m_Low / setDayKLine.m_LastClose) < 0.88) || (((double)setDayKLine.m_High / setDayKLine.m_LastClose) > 1.12)) { // ��Ȩ���¹����е�
      setDayKLine.m_RelativeStrong = 50; // �¹����л��߳�Ȩ��Ϣ��������˹�
    }
    else if ((setDayKLine.m_High == setDayKLine.m_Close)
      && (((double)setDayKLine.m_Close / setDayKLine.m_LastClose) > 1.095)) { // ��ͣ��
      setDayKLine.m_RelativeStrong = 100;
    }
    else if ((setDayKLine.m_Close == setDayKLine.m_Low)
      && (((double)setDayKLine.m_Close / setDayKLine.m_LastClose) < 0.905)) { // ��ͣ��
      setDayKLine.m_RelativeStrong = 0;
    }
    else {
      setDayKLine.m_RelativeStrong = ((double)iCount * 100) / iTotalAShare;
    }
    setDayKLine.Update();
    iBefore = vIndex.at(iCount++);
    setDayKLine.MoveNext(); // �Ƶ���һ�����ݡ�
    iBefore++; // ������Ҳͬʱ��һ��
  }
  setDayKLine.m_pDatabase->CommitTrans();
  setDayKLine.Close();

  vStockID.clear();
  vIndex.clear();
  vRelativeStrong.clear();
  
  sprintf_s(buffer, "%4d��%2d��%2d�յĹ�Ʊ���ǿ�ȼ������", lYear, lMonth, lDayOfMonth);
  CString strTemp;
  strTemp = buffer;
  gl_systemMessage.PushFindMessage(strTemp);    // ����ͬ�����Ʊ�����Ϣ

  gl_systemStatus.SetCalculateRSInProcess(false);
  return(true);
}

bool UpdateStockCodeDataBase(void)
{
  CSetStockCode setStockCode;

  setStockCode.Open();
  setStockCode.m_pDatabase->BeginTrans();
  while (!setStockCode.IsEOF()) {
    setStockCode.Delete();
    setStockCode.MoveNext();
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.m_pDatabase->BeginTrans();
  for (auto pStockID : gl_vTotalStock) {
    setStockCode.AddNew();
    setStockCode.m_Counter = pStockID->GetIndex();
    setStockCode.m_StockType = pStockID->GetMarket();
    setStockCode.m_StockCode = pStockID->GetStockCode();
    setStockCode.m_StockName = pStockID->GetStockName();
    setStockCode.m_IPOed = pStockID->GetIPOStatus();
    setStockCode.m_DayLineStartDay = pStockID->GetDayLineStartDay();
    setStockCode.m_DayLineEndDay = pStockID->GetDayLineEndDay();
    setStockCode.m_NewestDayLineDay = pStockID->GetNewestDayLineDay();
    setStockCode.Update();
  }
  setStockCode.m_pDatabase->CommitTrans();
  setStockCode.Close();

  return true;
}
