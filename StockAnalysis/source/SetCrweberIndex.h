#pragma once

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
  long m_Day;
  long m_TD1;
  long m_TD2;
  long m_TD3C;
  long m_TD5;
  long m_TD6;
  long m_TD7;
  long m_TD8;
  long m_TD9;
  long m_TD12;
  long m_TD15;
  long m_TD19;
  long m_TD20;
  long m_TD21;
  long m_VLCC_USGSPORE;
  long m_SUEZMAX_CBSUSG;
  long m_TC1;
  long m_TC2;
  long m_TC3;
  long m_TC4;
  long m_TC5;
  long m_TC14;
  long m_CPP_USGCBS;
  long m_VLCC_TC_1YEAR;
  long m_SUEZMAX_TC_1YEAR;
  long m_AFRAMAX_TC_1YEAR;
  long m_PANAMAX_TC_1YEAR;
  long m_MR_TC_1YEAR;
  long m_HANDY_TC_1YEAR;
  long m_VLCC_TC_3YEAR;
  long m_SUEZMAX_TC_3YEAR;
  long m_AFRAMAX_TC_3YEAR;
  long m_PANAMAX_TC_3YEAR;
  long m_MR_TC_3YEAR;
  long m_HANDY_TC_3YEAR;

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
