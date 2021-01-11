#pragma once

#include"stdafx.h"

#include"SetFinnhubExchanges.h"

using namespace std;
#include<memory>

class CFinnhubExchanges : public CObject {
public:
  CFinnhubExchanges();

  void Append(CSetFinnhubExchanges& setFinnhubExchanges);

  void Load(CSetFinnhubExchanges& setFinnhubExchanges);

public:
  CString m_strCode;
  CString m_strName;
  CString m_strMic;
  CString m_strTimeZone;
  CString m_strHour;
  CString m_strCloseDate;
  CString m_strCountry;
  CString m_strSource;
};

typedef shared_ptr<CFinnhubExchanges> CFinnhubExchangesPtr;
