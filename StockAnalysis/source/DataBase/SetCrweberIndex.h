#pragma once

#include"stdafx.h"

class CSetCrweberIndex : public CRecordset
{
public:
  CSetCrweberIndex(CDatabase* pDatabase = NULL);
  DECLARE_DYNAMIC(CSetCrweberIndex)

  // 字段/参数数据

  // 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
  // 数据类型的 CStringW)的实际数据类型。
  //  这是为防止 ODBC 驱动程序执行可能
  // 不必要的转换。如果希望，可以将这些成员更改为
  // CString 类型，ODBC 驱动程序将执行所有必要的转换。
  // (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
  // 以同时支持 Unicode 和这些转换)。
  long m_ID;
  long m_Date;
  CString m_TD1;
  CString m_TD2;
  CString m_TD3C;
  CString m_TD5;
  CString m_TD6;
  CString m_TD7;
  CString m_TD8;
  CString m_TD9;
  CString m_TD12;
  CString m_TD15;
  CString m_TD19;
  CString m_TD20;
  CString m_TD21;
  CString m_VLCC_USGSPORE;
  CString m_SUEZMAX_CBSUSG;
  CString m_TC1;
  CString m_TC2;
  CString m_TC3;
  CString m_TC4;
  CString m_TC5;
  CString m_TC14;
  CString m_CPP_USGCBS;
  CString m_VLCC_TC_1YEAR;
  CString m_SUEZMAX_TC_1YEAR;
  CString m_AFRAMAX_TC_1YEAR;
  CString m_PANAMAX_TC_1YEAR;
  CString m_MR_TC_1YEAR;
  CString m_HANDY_TC_1YEAR;
  CString m_VLCC_TC_3YEAR;
  CString m_SUEZMAX_TC_3YEAR;
  CString m_AFRAMAX_TC_3YEAR;
  CString m_PANAMAX_TC_3YEAR;
  CString m_MR_TC_3YEAR;
  CString m_HANDY_TC_3YEAR;

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
