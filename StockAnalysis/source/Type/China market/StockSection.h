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
  // ��洢����
  bool m_fActive; // ���δ����Ƿ��Ѿ�ʹ��
  long m_lIndexNumber; // ���δ���3����λ������
  WORD m_wMarket; // �г�
  CString m_strComment; // ע��

  // ����洢����
  bool m_fBuildStockPtr; // ����֤ȯ�Ƿ��ѽ���֤ȯָ�루��m_vChinaMarketStock�У���
};

typedef shared_ptr<CStockSection> CStockSectionPtr;
