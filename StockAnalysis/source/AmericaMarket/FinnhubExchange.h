#pragma once

#include"stdafx.h"

#include"SetFinnhubExchange.h"

using namespace std;
#include<memory>

class CFinnhubExchange : public CObject {
public:
  CFinnhubExchange();

  void Append(CSetFinnhubExchange& setFinnhubExchange);

  void Load(CSetFinnhubExchange& setFinnhubExchange);

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

typedef shared_ptr<CFinnhubExchange> CFinnhubExchangePtr;
