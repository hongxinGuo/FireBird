#pragma once
#include"SetSICIndustry.h"

using namespace std;
#include<memory>

class CSICIndustry : public CObject {
public:
  CSICIndustry();

  void Append(CSetSICIndustry& setSICIndustry);
  void Load(CSetSICIndustry& setSICIndustry);

  void SetUpdated(bool fFlag) noexcept { m_fUpdated = fFlag; }
  bool IsUpdated(void) const noexcept { return m_fUpdated; }

public:
  long m_lCode;
  CString m_strIndustry;
  CString m_strSector;

  bool m_fUpdated;
};

typedef shared_ptr<CSICIndustry> CSICIndustryPtr;
