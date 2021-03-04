#pragma once
#include"SetFinnhubExchange.h"

using namespace std;
#include<memory>

class CFinnhubExchange : public CObject {
public:
  CFinnhubExchange();

  void Append(CSetFinnhubExchange& setFinnhubExchange);

  void Load(CSetFinnhubExchange& setFinnhubExchange);

  void SetUpdated(bool fFlag) noexcept { m_fUpdated = fFlag; }
  bool IsUpdated(void) noexcept { return m_fUpdated; }

public:
  CString m_strCode;
  CString m_strName;
  CString m_strMic;
  CString m_strTimeZone;
  CString m_strHour;
  CString m_strCloseDate;
  CString m_strCountry;
  CString m_strSource;
  bool m_fIsActive;

  bool m_fUpdated;
};

typedef shared_ptr<CFinnhubExchange> CFinnhubExchangePtr;
