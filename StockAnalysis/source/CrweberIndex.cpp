#include"stdafx.h"

#include"Accessory.h"
#include"ChinaMarket.h"
#include "CrweberIndex.h"

CCrweberIndex::CCrweberIndex() {
  m_lID = 0;
  m_lDay = 0;
  m_dTD1 = m_dTD2 = m_dTD3C = m_dTD5 = m_dTD6 = m_dTD7 = m_dTD8 = m_dTD9 = m_dTD12 = m_dTD15 = m_dTD19 = m_dTD20
    = m_dTD21 = m_dVLCC_USGSPORE = m_dSUEZMAX_CBSUSG = 0;
  m_dTC1 = m_dTC2 = m_dTC3 = m_dTC4 = m_dTC5 = m_dTC14 = m_dCPP_USGCBS = 0;
  m_dVLCC_TC_1YEAR = m_dSUEZMAX_TC_1YEAR = m_dAFRAMAX_TC_1YEAR = m_dPANAMAX_TC_1YEAR = m_dMR_TC_1YEAR = m_dHANDY_TC_1YEAR = 0;
  m_dVLCC_TC_3YEAR = m_dSUEZMAX_TC_3YEAR = m_dAFRAMAX_TC_3YEAR = m_dPANAMAX_TC_3YEAR = m_dMR_TC_3YEAR = m_dHANDY_TC_3YEAR = 0;

  m_fTodayUpdated = false;
  m_lLastUpdateDay = 0;

  m_mapMonth[_T("January")] = 1;
  m_mapMonth[_T("February")] = 2;
  m_mapMonth[_T("March")] = 3;
  m_mapMonth[_T("April")] = 4;
  m_mapMonth[_T("May")] = 5;
  m_mapMonth[_T("June")] = 6;
  m_mapMonth[_T("July")] = 7;
  m_mapMonth[_T("August")] = 8;
  m_mapMonth[_T("September")] = 9;
  m_mapMonth[_T("October")] = 10;
  m_mapMonth[_T("November")] = 11;
  m_mapMonth[_T("December")] = 12;
}

CCrweberIndex::~CCrweberIndex() {
}

void CCrweberIndex::Reset(void) {
  m_fTodayUpdated = false;
}

void CCrweberIndex::LoadData(CSetCrweberIndex& setCrweberIndex) {
  m_lDay = setCrweberIndex.m_Day;
  m_dTD1 = atof(setCrweberIndex.m_TD1);
  m_dTD2 = atof(setCrweberIndex.m_TD2);
  m_dTD3C = atof(setCrweberIndex.m_TD3C);
  m_dTD5 = atof(setCrweberIndex.m_TD5);
  m_dTD6 = atof(setCrweberIndex.m_TD6);
  m_dTD7 = atof(setCrweberIndex.m_TD7);
  m_dTD8 = atof(setCrweberIndex.m_TD8);
  m_dTD9 = atof(setCrweberIndex.m_TD9);
  m_dTD12 = atof(setCrweberIndex.m_TD12);
  m_dTD15 = atof(setCrweberIndex.m_TD15);
  m_dTD19 = atof(setCrweberIndex.m_TD19);
  m_dTD20 = atof(setCrweberIndex.m_TD20);
  m_dTD21 = atof(setCrweberIndex.m_TD21);
  m_dVLCC_USGSPORE = atof(setCrweberIndex.m_VLCC_USGSPORE);
  m_dSUEZMAX_CBSUSG = atof(setCrweberIndex.m_SUEZMAX_CBSUSG);
  m_dTC1 = atof(setCrweberIndex.m_TC1);
  m_dTC2 = atof(setCrweberIndex.m_TC2);
  m_dTC3 = atof(setCrweberIndex.m_TC3);
  m_dTC4 = atof(setCrweberIndex.m_TC4);
  m_dTC5 = atof(setCrweberIndex.m_TC5);
  m_dTC14 = atof(setCrweberIndex.m_TC14);
  m_dVLCC_TC_1YEAR = atof(setCrweberIndex.m_VLCC_TC_1YEAR);
  m_dCPP_USGCBS = atof(setCrweberIndex.m_CPP_USGCBS);
  m_dSUEZMAX_TC_1YEAR = atof(setCrweberIndex.m_SUEZMAX_TC_1YEAR);
  m_dAFRAMAX_TC_1YEAR = atof(setCrweberIndex.m_AFRAMAX_TC_1YEAR);
  m_dPANAMAX_TC_1YEAR = atof(setCrweberIndex.m_PANAMAX_TC_1YEAR);
  m_dMR_TC_1YEAR = atof(setCrweberIndex.m_MR_TC_1YEAR);
  m_dHANDY_TC_1YEAR = atof(setCrweberIndex.m_HANDY_TC_1YEAR);
  m_dVLCC_TC_3YEAR = atof(setCrweberIndex.m_VLCC_TC_3YEAR);
  m_dSUEZMAX_TC_3YEAR = atof(setCrweberIndex.m_SUEZMAX_TC_3YEAR);
  m_dAFRAMAX_TC_3YEAR = atof(setCrweberIndex.m_AFRAMAX_TC_3YEAR);
  m_dPANAMAX_TC_3YEAR = atof(setCrweberIndex.m_PANAMAX_TC_3YEAR);
  m_dMR_TC_3YEAR = atof(setCrweberIndex.m_MR_TC_3YEAR);
  m_dHANDY_TC_3YEAR = atof(setCrweberIndex.m_HANDY_TC_3YEAR);
}

void CCrweberIndex::SaveData(CSetCrweberIndex& setCrweberIndex) {
  ASSERT(setCrweberIndex.IsOpen());

  setCrweberIndex.m_Day = m_lDay;
  setCrweberIndex.m_TD1 = ConvertValueToString(m_dTD1);
  setCrweberIndex.m_TD2 = ConvertValueToString(m_dTD2);
  setCrweberIndex.m_TD3C = ConvertValueToString(m_dTD3C);
  setCrweberIndex.m_TD5 = ConvertValueToString(m_dTD5);
  setCrweberIndex.m_TD6 = ConvertValueToString(m_dTD6);
  setCrweberIndex.m_TD7 = ConvertValueToString(m_dTD7);
  setCrweberIndex.m_TD8 = ConvertValueToString(m_dTD8);
  setCrweberIndex.m_TD9 = ConvertValueToString(m_dTD9);
  setCrweberIndex.m_TD12 = ConvertValueToString(m_dTD12);
  setCrweberIndex.m_TD15 = ConvertValueToString(m_dTD15);
  setCrweberIndex.m_TD19 = ConvertValueToString(m_dTD19);
  setCrweberIndex.m_TD20 = ConvertValueToString(m_dTD20);
  setCrweberIndex.m_TD21 = ConvertValueToString(m_dTD21);
  setCrweberIndex.m_VLCC_USGSPORE = ConvertValueToString(m_dVLCC_USGSPORE); // 从百万转成以万为单位。
  setCrweberIndex.m_SUEZMAX_CBSUSG = ConvertValueToString(m_dSUEZMAX_CBSUSG);
  setCrweberIndex.m_TC1 = ConvertValueToString(m_dTC1);
  setCrweberIndex.m_TC2 = ConvertValueToString(m_dTC2);
  setCrweberIndex.m_TC3 = ConvertValueToString(m_dTC3);
  setCrweberIndex.m_TC4 = ConvertValueToString(m_dTC4);
  setCrweberIndex.m_TC5 = ConvertValueToString(m_dTC5);
  setCrweberIndex.m_TC14 = ConvertValueToString(m_dTC14);
  setCrweberIndex.m_CPP_USGCBS = ConvertValueToString(m_dCPP_USGCBS);

  setCrweberIndex.m_VLCC_TC_1YEAR = ConvertValueToString(m_dVLCC_TC_1YEAR);
  setCrweberIndex.m_SUEZMAX_TC_1YEAR = ConvertValueToString(m_dSUEZMAX_TC_1YEAR);
  setCrweberIndex.m_AFRAMAX_TC_1YEAR = ConvertValueToString(m_dAFRAMAX_TC_1YEAR);
  setCrweberIndex.m_PANAMAX_TC_1YEAR = ConvertValueToString(m_dPANAMAX_TC_1YEAR);
  setCrweberIndex.m_MR_TC_1YEAR = ConvertValueToString(m_dMR_TC_1YEAR);
  setCrweberIndex.m_HANDY_TC_1YEAR = ConvertValueToString(m_dHANDY_TC_1YEAR);
  setCrweberIndex.m_VLCC_TC_3YEAR = ConvertValueToString(m_dVLCC_TC_3YEAR);
  setCrweberIndex.m_SUEZMAX_TC_3YEAR = ConvertValueToString(m_dSUEZMAX_TC_3YEAR);
  setCrweberIndex.m_AFRAMAX_TC_3YEAR = ConvertValueToString(m_dAFRAMAX_TC_3YEAR);
  setCrweberIndex.m_PANAMAX_TC_3YEAR = ConvertValueToString(m_dPANAMAX_TC_3YEAR);
  setCrweberIndex.m_MR_TC_3YEAR = ConvertValueToString(m_dMR_TC_3YEAR);
  setCrweberIndex.m_HANDY_TC_3YEAR = ConvertValueToString(m_dHANDY_TC_3YEAR);
}

void CCrweberIndex::AppendData(CSetCrweberIndex& setCrweberIndex) {
  ASSERT(setCrweberIndex.IsOpen());
  setCrweberIndex.AddNew();
  SaveData(setCrweberIndex);
  setCrweberIndex.Update();
}

bool CCrweberIndex::ReadData(CWebDataReceivedPtr pWebDataReceived) {
  pWebDataReceived->m_lCurrentPos = 0;
  CString str, str1, strHead = _T("");
  CString strValue, strTime;
  CString strNoUse;
  long lUpdateDay = 0;

  while (pWebDataReceived->GetCurrentPos() < pWebDataReceived->GetBufferLength()) {
    str = GetNextString(pWebDataReceived);
    strHead = str.Left(10);
    if (strHead.Compare(_T("Updated by")) == 0) {
      strTime = GetNextString(pWebDataReceived); // 当前时间
      lUpdateDay = ConvertStringToTime(strTime);

      for (int i = 0; i < 4; i++) strNoUse = GetNextString(pWebDataReceived); // 抛掉4个没用字符串

      str1 = GetNextString(pWebDataReceived); // "VLCC"
      m_dTD1 = GetOneValue(pWebDataReceived);
      m_dTD2 = GetOneValue(pWebDataReceived);
      m_dTD3C = GetOneValue(pWebDataReceived);
      m_dTD15 = GetOneValue(pWebDataReceived);
      m_dVLCC_USGSPORE = GetOneValue(pWebDataReceived) * 100;

      str1 = GetNextString(pWebDataReceived); // "SUEZMAX"
      m_dTD5 = GetOneValue(pWebDataReceived);
      m_dTD20 = GetOneValue(pWebDataReceived);
      m_dTD6 = GetOneValue(pWebDataReceived);
      m_dSUEZMAX_CBSUSG = GetOneValue(pWebDataReceived);

      str1 = GetNextString(pWebDataReceived); // "AFRAMAX"
      m_dTD7 = GetOneValue(pWebDataReceived);
      m_dTD9 = GetOneValue(pWebDataReceived);
      m_dTD19 = GetOneValue(pWebDataReceived);
      m_dTD8 = GetOneValue(pWebDataReceived);

      str1 = GetNextString(pWebDataReceived); // "PANAMAX"
      m_dTD21 = GetOneValue(pWebDataReceived);
      m_dTD12 = GetOneValue(pWebDataReceived);

      str1 = GetNextString(pWebDataReceived); // "CPP"
      m_dTC2 = GetOneValue(pWebDataReceived);
      m_dTC3 = GetOneValue(pWebDataReceived);
      m_dTC14 = GetOneValue(pWebDataReceived);
      m_dCPP_USGCBS = GetOneValue(pWebDataReceived) * 1000;
      m_dTC1 = GetOneValue(pWebDataReceived);
      m_dTC5 = GetOneValue(pWebDataReceived);
      m_dTC4 = GetOneValue(pWebDataReceived);

      CString strDay = ConvertValueToString(lUpdateDay, 1);
      if (lUpdateDay > m_lLastUpdateDay) {
        m_lDay = lUpdateDay;
        m_lLastUpdateDay = lUpdateDay;
        m_fTodayUpdated = true;
      }
    }

    strHead = str.Left(6);
    if (strHead.Compare(_T("Tanker")) == 0) {
      for (int i = 0; i < 7; i++) strNoUse = GetNextString(pWebDataReceived); // "CPP"
      str1 = GetNextString(pWebDataReceived);
      m_dVLCC_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString(pWebDataReceived);
      m_dVLCC_TC_3YEAR = ConvertStringToTC(str1);

      strNoUse = GetNextString(pWebDataReceived);
      strNoUse = GetNextString(pWebDataReceived);
      str1 = GetNextString(pWebDataReceived);
      m_dSUEZMAX_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString(pWebDataReceived);
      m_dSUEZMAX_TC_3YEAR = ConvertStringToTC(str1);

      strNoUse = GetNextString(pWebDataReceived);
      strNoUse = GetNextString(pWebDataReceived);
      str1 = GetNextString(pWebDataReceived);
      m_dAFRAMAX_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString(pWebDataReceived);
      m_dAFRAMAX_TC_3YEAR = ConvertStringToTC(str1);

      strNoUse = GetNextString(pWebDataReceived);
      strNoUse = GetNextString(pWebDataReceived);
      str1 = GetNextString(pWebDataReceived);
      m_dPANAMAX_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString(pWebDataReceived);
      m_dPANAMAX_TC_3YEAR = ConvertStringToTC(str1);

      strNoUse = GetNextString(pWebDataReceived);
      strNoUse = GetNextString(pWebDataReceived);
      str1 = GetNextString(pWebDataReceived);
      m_dMR_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString(pWebDataReceived);
      m_dMR_TC_3YEAR = ConvertStringToTC(str1);

      strNoUse = GetNextString(pWebDataReceived);
      strNoUse = GetNextString(pWebDataReceived);
      str1 = GetNextString(pWebDataReceived);
      m_dHANDY_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString(pWebDataReceived);
      m_dHANDY_TC_3YEAR = ConvertStringToTC(str1);

      pWebDataReceived->m_lCurrentPos = pWebDataReceived->m_lBufferLength; //
    }
  }
  pWebDataReceived->m_pCurrentPos = pWebDataReceived->GetBufferAddr();
  for (int i = 0; i < pWebDataReceived->m_lBufferLength; i++) {
    *pWebDataReceived->m_pCurrentPos++ = 0x000;
  }
  pWebDataReceived->m_lCurrentPos = pWebDataReceived->m_lBufferLength; //

  return true;
}

double CCrweberIndex::ConvertStringToTC(CString str) {
  char buffer[200];
  int iTotal = str.GetLength();
  int i = 0, j = 0;

  while (i < iTotal) {
    if ((str[i] != ',') && (str[i] != ' ')) buffer[j++] = str[i];
    i++;
  }
  buffer[j] = 0x000;
  double abc = atof(buffer);

  return abc;
}

long CCrweberIndex::ConvertStringToTime(CString str) {
  char buffer1[20];
  char* pChar = str.GetBuffer();
  while (*pChar != ' ') pChar++;
  pChar++;
  int i = 0;
  while (*pChar != ' ') buffer1[i++] = *pChar++;
  pChar++;
  buffer1[i] = 0x000;
  CString strTime = buffer1;
  int month = 1, day, year;
  month = GetMonthOfTheYear(strTime);

  i = 0;
  while (*pChar != ' ') buffer1[i++] = *pChar++;
  pChar++;
  buffer1[i] = 0x000;
  strTime = buffer1;
  day = atol(strTime);
  i = 0;
  while (*pChar != ' ') buffer1[i++] = *pChar++;
  buffer1[i] = 0x000;
  strTime = buffer1;
  year = atol(strTime);

  return year * 10000 + month * 100 + day;
}

long CCrweberIndex::GetMonthOfTheYear(CString strMonth) {
  try {
    return m_mapMonth.at(strMonth);
  }
  catch (exception&) {
    TRACE(_T("不正确的月份名称"));
    return gl_CrweberIndexMarket.GetMonthOfYear();
  }
}

double CCrweberIndex::GetOneValue(CWebDataReceivedPtr pWebDataReceived) {
  CString str, strValue;
  double dValue = 0.0;

  str = GetNextString(pWebDataReceived); // "TD1\r\n   "
  str = GetNextString(pWebDataReceived); // 无用数据
  str = GetNextString(pWebDataReceived); // 无用数据
  strValue = GetNextString(pWebDataReceived); // TD1指数的当前值
  dValue = atof(strValue.GetBuffer());
  return dValue;
}

CString CCrweberIndex::GetNextString(CWebDataReceivedPtr pWebDataReceived) {
  bool fFound = false;
  char buffer[10000];
  long iBufferCount = 0;

  while ((*pWebDataReceived->m_pCurrentPos != 0x000) && !fFound) {
    if (*pWebDataReceived->m_pCurrentPos == '<') { // 无用配置字符
      while (*pWebDataReceived->m_pCurrentPos != '>') {
        pWebDataReceived->IncreaseCurrentPos();
      }
      pWebDataReceived->IncreaseCurrentPos();
      while ((*pWebDataReceived->m_pCurrentPos == 0x00a) || (*pWebDataReceived->m_pCurrentPos == 0x00d)
             || (*pWebDataReceived->m_pCurrentPos == ' ')) { // 跨过回车、换行和空格符
        pWebDataReceived->IncreaseCurrentPos();
      }
    }
    else fFound = true;
  }
  if (*pWebDataReceived->m_pCurrentPos == 0x000) { // 读到结尾处了
    ASSERT(pWebDataReceived->m_lCurrentPos >= pWebDataReceived->m_lBufferLength);
    return _T("");
  }
  while (*pWebDataReceived->m_pCurrentPos != '<') {
    if (*pWebDataReceived->m_pCurrentPos != ',') buffer[iBufferCount++] = *pWebDataReceived->m_pCurrentPos; // 抛掉逗号，逗号导致atof函数无法顺利转化字符串
    pWebDataReceived->IncreaseCurrentPos();
  }
  buffer[iBufferCount] = 0x000;
  CString str;
  str = buffer;
  return str;
}

bool CCrweberIndex::IsDataChanged(CCrweberIndex& CrweberIndexLast) {
  if ((GetTD1() != CrweberIndexLast.GetTD1()) ||
    (GetTD2() != CrweberIndexLast.GetTD2()) ||
      (GetTD3C() != CrweberIndexLast.GetTD3C()) ||
      (GetTD5() != CrweberIndexLast.GetTD5()) ||
      (GetTD6() != CrweberIndexLast.GetTD6()) ||
      (GetTD7() != CrweberIndexLast.GetTD7()) ||
      (GetTD8() != CrweberIndexLast.GetTD8()) ||
      (GetTD9() != CrweberIndexLast.GetTD9()) ||
      (GetTD12() != CrweberIndexLast.GetTD12()) ||
      (GetTD15() != CrweberIndexLast.GetTD15()) ||
      (GetTD19() != CrweberIndexLast.GetTD19()) ||
      (GetTD20() != CrweberIndexLast.GetTD20()) ||
      (GetTD21() != CrweberIndexLast.GetTD21()) ||
      (GetVLCC_USGSPORE() != CrweberIndexLast.GetVLCC_USGSPORE()) ||
      (GetTC1() != CrweberIndexLast.GetTC1()) ||
      (GetTC2() != CrweberIndexLast.GetTC2()) ||
      (GetTC3() != CrweberIndexLast.GetTC3()) ||
      (GetTC4() != CrweberIndexLast.GetTC4()) ||
      (GetTC5() != CrweberIndexLast.GetTC5()) ||
      (GetTC14() != CrweberIndexLast.GetTC14())) {
    return true;
  }
  return false;
}