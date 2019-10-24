#pragma once

#include"stdafx.h"

#include"SetCrweberIndex.h"

using namespace std;
#include<vector>
#include<memory>

class CCrweberIndex final {
public:
  CCrweberIndex();
  ~CCrweberIndex();
  void SetData(CSetCrweberIndex* pSetCrwwberIndex);

protected:
  long m_lID;
  long m_lDay;
  long m_lTD1;
  long m_lTD2;
  long m_lTD3C;
  long m_lTD5;
  long m_lTD6;
  long m_lTD7;
  long m_lTD8;
  long m_lTD9;
  long m_lTD12;
  long m_lTD15;
  long m_lTD19;
  long m_lTD20;
  long m_lTD21;
  long m_lVLCC_USGSPORE;
  long m_lSUEZMAX_CBSUSG;
  long m_lTC1;
  long m_lTC2;
  long m_lTC3;
  long m_lTC4;
  long m_lTC5;
  long m_lTC14;
};

typedef shared_ptr<CCrweberIndex> CrweberIndexPtr;
