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
// 计算lDay的日线相对强度, lDay的格式为：YYYYMMDD,如 19990605.
// 将日线按涨跌排列后,其相对强弱即其在队列中的位置
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
  if (setDayKLine.IsEOF()) { // 数据集为空，表明此日没有交易
    setDayKLine.Close();
    return false;
  }
  setDayKLine.m_pDatabase->BeginTrans();
  iStockNumber = 0;
  while (!setDayKLine.IsEOF()) {
    if (gl_ChinaStockMarket.IsAStock(setDayKLine.m_StockCode)) {
      long lIndex = gl_mapTotalStockToIndex.at(setDayKLine.m_StockCode);
      vStockID.push_back(gl_vTotalStock.at(lIndex));
      vIndex.push_back(iStockNumber); // 将A股的索引记录在容器中。
      iTotalAShare++;
    }
    iStockNumber++;
    setDayKLine.MoveNext();
  }

  setDayKLine.MoveFirst();
  int iCount = 0;
  int iBefore = 0;
  while (iCount < vIndex.size()) { // 只计算活跃股票的相对强度
    if (gl_fExiting) break; // 数据库有时操作很费时间，当系统退出时数据库操作要主动中断。
    for (int i = 0; i < vIndex.at(iCount) - iBefore; i++) { // 根据索引去更改数据库。
      setDayKLine.MoveNext();
    }
    setDayKLine.Edit();
    if ((((double)setDayKLine.m_Low / setDayKLine.m_LastClose) < 0.88) || (((double)setDayKLine.m_High / setDayKLine.m_LastClose) > 1.12)) { // 除权、新股上市等
      setDayKLine.m_RelativeStrong = 50; // 新股上市或者除权除息，不计算此股
    }
    else if ((setDayKLine.m_High == setDayKLine.m_Close)
      && (((double)setDayKLine.m_Close / setDayKLine.m_LastClose) > 1.095)) { // 涨停板
      setDayKLine.m_RelativeStrong = 100;
    }
    else if ((setDayKLine.m_Close == setDayKLine.m_Low)
      && (((double)setDayKLine.m_Close / setDayKLine.m_LastClose) < 0.905)) { // 跌停板
      setDayKLine.m_RelativeStrong = 0;
    }
    else {
      setDayKLine.m_RelativeStrong = ((double)iCount * 100) / iTotalAShare;
    }
    setDayKLine.Update();
    iBefore = vIndex.at(iCount++);
    setDayKLine.MoveNext(); // 移到下一个数据。
    iBefore++; // 计数器也同时加一。
  }
  setDayKLine.m_pDatabase->CommitTrans();
  setDayKLine.Close();

  vStockID.clear();
  vIndex.clear();
  vRelativeStrong.clear();
  
  sprintf_s(buffer, "%4d年%2d月%2d日的股票相对强度计算完成", lYear, lMonth, lDayOfMonth);
  CString strTemp;
  strTemp = buffer;
  gl_systemMessage.PushFindMessage(strTemp);    // 采用同步机制报告信息

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
