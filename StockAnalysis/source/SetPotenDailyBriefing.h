#pragma once

#include"stdafx.h"

class CSetPotenDailyBriefing : public CRecordset
{
public:
  CSetPotenDailyBriefing(CDatabase* pDatabase = NULL);
  DECLARE_DYNAMIC(CSetPotenDailyBriefing)

  // 字段/参数数据

  // 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
  // 数据类型的 CStringW)的实际数据类型。
  //  这是为防止 ODBC 驱动程序执行可能
  // 不必要的转换。如果希望，可以将这些成员更改为
  // CString 类型，ODBC 驱动程序将执行所有必要的转换。
  // (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
  // 以同时支持 Unicode 和这些转换)。
  long m_ID;
  long m_Day;
  CString m_TD3C;
  CString m_TD9;
  CString m_TD20;
  CString m_TD21;
  CString m_TC1;
  CString m_TC2;
  CString m_TC5;
  CString m_TC14;
  CString m_VLCC_TC_1YEAR;
  CString m_SUEZMAX_TC_1YEAR;
  CString m_AFRAMAX_TC_1YEAR;
  CString m_PANAMAX_TC_1YEAR;
  CString m_MR_TC_1YEAR;
  CString m_HANDY_TC_1YEAR;
  CString m_VLCC_NEW_BUILD;
  CString m_SUEZMAX_NEW_BUILD;
  CString m_AFRAMAX_NEW_BUILD;
  CString m_LR2_NEW_BUILD;
  CString m_LR1_NEW_BUILD;
  CString m_MR_NEW_BUILD;
  CString m_HANDYMAX_NEW_BUILD;
  CString m_VLCC_5YEARS_OLD;
  CString m_SUEZMAX_5YEARS_OLD;
  CString m_AFRAMAX_5YEARS_OLD;
  CString m_LR2_5YEARS_OLD;
  CString m_LR1_5YEARS_OLD;
  CString m_MR_5YEARS_OLD;
  CString m_HANDYMAX_5YEARS_OLD;

  // 重写
    // 向导生成的虚函数重写
public:
  virtual CString GetDefaultConnect();	// 默认连接字符串

  virtual CString GetDefaultSQL(); 	// 记录集的默认 SQL
  virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX 支持

// 实现
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
};
