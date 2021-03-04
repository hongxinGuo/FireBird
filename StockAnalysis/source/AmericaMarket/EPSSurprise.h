#pragma once
#include"SetEPSSurprise.h"

using namespace std;
#include<memory>

class CEPSSurprise : public CObject {
public:
  CEPSSurprise();

  void Append(CSetEPSSurprise& setEPSSukrprise);
  void Load(CSetEPSSurprise& setEPSSukrprise);

public:
  CString m_strSymbol;
  long m_lDate;
  double m_dActual;
  double m_dEstimate;
};

typedef shared_ptr<CEPSSurprise> CEPSSurprisePtr;
