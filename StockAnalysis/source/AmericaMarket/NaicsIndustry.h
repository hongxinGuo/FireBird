#pragma once
#include"SetNaicsIndustry.h"

using namespace std;
#include<memory>

class CNaicsIndustry : public CObject {
public:
  CNaicsIndustry();

  void Append(CSetNaicsIndustry& setNaicsIndustry);

  void Load(CSetNaicsIndustry& setNaicsIndustry);

  void SetUpdated(bool fFlag) noexcept { m_fUpdated = fFlag; }
  bool IsUpdated(void) noexcept { return m_fUpdated; }

public:
  CString m_strNaics;
  CString m_strNationalIndustry;
  CString m_strSector;
  CString m_strSubSector;

  bool m_fUpdated;
};

typedef shared_ptr<CNaicsIndustry> CNaicsIndustryPtr;
