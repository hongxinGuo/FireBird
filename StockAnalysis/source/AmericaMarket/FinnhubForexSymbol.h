#pragma once
#include"VirtualStock.h"

#include"SetFinnhubForexSymbol.h"
#include"SetForexDayLine.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>
#include <boost/iterator/detail/config_def.hpp>

class CFinnhubForexSymbol : public CVirtualStock {
public:
  CFinnhubForexSymbol();
  virtual void Reset(void);
  virtual int GetRatio(void) { return s_iRatio; }// 此函数应该声明为纯虚函数，但由于需要测试此基类，故而有执行体。感觉还是声明为纯虚函数为佳。

  void Load(CSetFinnhubForexSymbol& setForexSymbol);
  void Append(CSetFinnhubForexSymbol& setForexSymbol);
  void Update(CSetFinnhubForexSymbol& setForexSymbol);
  void Save(CSetFinnhubForexSymbol& setForexSymbol);

  bool SaveDayLine(void);

  CString GetDescription(void) { return m_strDescription; }
  void SetDescription(CString strDescription) { m_strDescription = strDescription; }
  CString GetDisplaySymbol(void) { return m_strDisplaySymbol; }
  void SetDisplaySymbol(CString strSymbol) { m_strDisplaySymbol = strSymbol; }

  void SetCheckingDayLineStatus(void);

  CString GetFinnhubDayLineInquiryString(time_t tCurrentTime);

  void UpdateDayLine(vector<CDayLinePtr>& vDayLine);
  void UnloadDayLine(void) { m_vDayLine.resize(0); }
  size_t GetDayLineSize(void) { return m_vDayLine.size(); }
  void UpdateDayLineStartEndDate(void);
  bool HaveNewDayLineData(void);

public:

  vector<CDayLinePtr> m_vDayLine;

protected:
  CString m_strDescription;
  CString m_strDisplaySymbol;

private:
  static int s_iRatio;
};

typedef shared_ptr<CFinnhubForexSymbol> CForexSymbolPtr;
