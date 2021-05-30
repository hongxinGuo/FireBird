#pragma once

#include"SetInsiderTransaction.h"

using namespace std;
#include<memory>

class CInsiderTransaction : public CObject {
public:
  CInsiderTransaction();

  void Append(CSetInsiderTransaction& setInsiderTransaction);
  void Update(CSetInsiderTransaction& setInsiderTransaction);

  void Save(CSetInsiderTransaction& setInsiderTransaction);
  void Load(CSetInsiderTransaction& setInsiderTransaction);

public:
  CString m_strSymbol;
  CString m_strPersonName;
  long m_lShare;
  long m_lChange;
  long m_lFilingDate;
  long m_lTransactionDate;
  CString m_strTransactionCode;
  double m_dTransactionPrice;
};

typedef shared_ptr<CInsiderTransaction> CInsiderTransactionPtr;
