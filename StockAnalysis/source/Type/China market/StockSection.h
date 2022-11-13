#pragma once

using namespace std;
#include<memory>

class CStockSection : public CObject {
public:
  CStockSection();
  ~CStockSection(void);

  bool IsActive(void) const noexcept { return m_fActive; }
  void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }
  long GetIndexNumber(void) const noexcept { return m_lIndexNumber; }
  void SetIndexNumber(long lNumber) noexcept { m_lIndexNumber = lNumber; }
  WORD GetMarket(void) const noexcept { return m_wMarket; }
  void SetMarket(WORD wMarket) noexcept { m_wMarket = wMarket; }
  CString GetComment(void) const { return m_strComment; }
  void SetComment(CString strComment) { m_strComment = strComment; }
  bool IsBuildStockPtr(void) const noexcept { return m_fBuildStockPtr; }
  void SetBuildStockPtr(bool fFlag) noexcept { m_fBuildStockPtr = fFlag; }

protected:
  // 需存储数据
  bool m_fActive; // 本段代码是否已经使用
  long m_lIndexNumber; // 本段代码3个高位的数字
  WORD m_wMarket; // 市场
  CString m_strComment; // 注释

  // 无需存储数据
  bool m_fBuildStockPtr; // 本段证券是否已建立证券指针（在m_vChinaMarketStock中）。
};

typedef shared_ptr<CStockSection> CStockSectionPtr;
