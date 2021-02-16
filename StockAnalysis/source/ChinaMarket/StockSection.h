#pragma once
#include"stdafx.h"

using namespace std;
#include<memory>

class CStockSection : public CObject {
public:
  CStockSection();
  ~CStockSection(void);

  bool IsActive(void) noexcept { return m_fActive; }
  void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }
  long GetIndexNumber(void) noexcept { return m_lIndexNumber; }
  void SetIndexNumber(long lNumber) noexcept { m_lIndexNumber = lNumber; }
  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wMarket) noexcept { m_wMarket = wMarket; }
  CString GetComment(void) { return m_strComment; }
  void SetComment(CString strComment) { m_strComment = strComment; }
  bool IsBuildStakePtr(void) noexcept { return m_fBuildStakePtr; }
  void SetBuildStakePtr(bool fFlag) noexcept { m_fBuildStakePtr = fFlag; }

protected:
  // 需存储数据
  bool m_fActive; // 本段代码是否已经使用
  long m_lIndexNumber; // 本段代码3个高位的数字
  WORD m_wMarket; // 市场
  CString m_strComment; // 注释

  // 无需存储数据
  bool m_fBuildStakePtr; // 本段证券是否已建立证券指针（在m_vChinaMarketStock中）。
};

typedef shared_ptr<CStockSection> CStockSectionPtr;
