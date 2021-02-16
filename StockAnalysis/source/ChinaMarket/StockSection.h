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
  // ��洢����
  bool m_fActive; // ���δ����Ƿ��Ѿ�ʹ��
  long m_lIndexNumber; // ���δ���3����λ������
  WORD m_wMarket; // �г�
  CString m_strComment; // ע��

  // ����洢����
  bool m_fBuildStakePtr; // ����֤ȯ�Ƿ��ѽ���֤ȯָ�루��m_vChinaMarketStock�У���
};

typedef shared_ptr<CStockSection> CStockSectionPtr;
