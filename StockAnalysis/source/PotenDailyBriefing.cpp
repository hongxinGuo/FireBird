#include"stdafx.h"

#include"Accessory.h"
#include"ChinaMarket.h"
#include "PotenDailyBriefing.h"

CPotenDailyBriefing::CPotenDailyBriefing() {
  m_lID = 0;
  m_lDay = 0;
  m_dTD3C = m_dTD9 = m_dTD20 = m_dTD21 = 0;
  m_dTC1 = m_dTC2 = m_dTC5 = m_dTC14 = 0;
  m_dVLCC_TC_1YEAR = m_dSUEZMAX_TC_1YEAR = m_dAFRAMAX_TC_1YEAR = m_dLR2_TC_1YEAR = m_dLR1_TC_1YEAR = m_dMR_TC_1YEAR = m_dHANDY_TC_1YEAR = 0;

  m_fTodayUpdated = false;
  m_lLastUpdateDay = 0;
}

CPotenDailyBriefing::~CPotenDailyBriefing() {
}

void CPotenDailyBriefing::Reset(void) {
  m_fTodayUpdated = false;
}

void CPotenDailyBriefing::LoadData(CSetPotenDailyBriefing& setPotenDailyBriefing) {
  m_lDay = setPotenDailyBriefing.m_Day;
  m_dTD3C = atof(setPotenDailyBriefing.m_TD3C);
  m_dTD9 = atof(setPotenDailyBriefing.m_TD9);
  m_dTD20 = atof(setPotenDailyBriefing.m_TD20);
  m_dTD21 = atof(setPotenDailyBriefing.m_TD21);
  m_dTC1 = atof(setPotenDailyBriefing.m_TC1);
  m_dTC2 = atof(setPotenDailyBriefing.m_TC2);
  m_dTC5 = atof(setPotenDailyBriefing.m_TC5);
  m_dTC14 = atof(setPotenDailyBriefing.m_TC14);
  m_dVLCC_TC_1YEAR = atof(setPotenDailyBriefing.m_VLCC_TC_1YEAR);
  m_dSUEZMAX_TC_1YEAR = atof(setPotenDailyBriefing.m_SUEZMAX_TC_1YEAR);
  m_dAFRAMAX_TC_1YEAR = atof(setPotenDailyBriefing.m_AFRAMAX_TC_1YEAR);
  m_dLR2_TC_1YEAR = atof(setPotenDailyBriefing.m_LR2_TC_1YEAR);
  m_dLR1_TC_1YEAR = atof(setPotenDailyBriefing.m_LR1_TC_1YEAR);
  m_dMR_TC_1YEAR = atof(setPotenDailyBriefing.m_MR_TC_1YEAR);
  m_dHANDY_TC_1YEAR = atof(setPotenDailyBriefing.m_HANDY_TC_1YEAR);
}

void CPotenDailyBriefing::SaveData(CSetPotenDailyBriefing& setPotenDailyBriefing) {
  ASSERT(setPotenDailyBriefing.IsOpen());

  setPotenDailyBriefing.m_Day = m_lDay;
  setPotenDailyBriefing.m_TD3C = ConvertValueToString(m_dTD3C);
  setPotenDailyBriefing.m_TD9 = ConvertValueToString(m_dTD9);
  setPotenDailyBriefing.m_TD20 = ConvertValueToString(m_dTD20);
  setPotenDailyBriefing.m_TD21 = ConvertValueToString(m_dTD21);
  setPotenDailyBriefing.m_TC1 = ConvertValueToString(m_dTC1);
  setPotenDailyBriefing.m_TC2 = ConvertValueToString(m_dTC2);
  setPotenDailyBriefing.m_TC5 = ConvertValueToString(m_dTC5);
  setPotenDailyBriefing.m_TC14 = ConvertValueToString(m_dTC14);

  setPotenDailyBriefing.m_VLCC_TC_1YEAR = ConvertValueToString(m_dVLCC_TC_1YEAR);
  setPotenDailyBriefing.m_SUEZMAX_TC_1YEAR = ConvertValueToString(m_dSUEZMAX_TC_1YEAR);
  setPotenDailyBriefing.m_AFRAMAX_TC_1YEAR = ConvertValueToString(m_dAFRAMAX_TC_1YEAR);
  setPotenDailyBriefing.m_LR2_TC_1YEAR = ConvertValueToString(m_dLR2_TC_1YEAR);
  setPotenDailyBriefing.m_LR1_TC_1YEAR = ConvertValueToString(m_dLR1_TC_1YEAR);
  setPotenDailyBriefing.m_MR_TC_1YEAR = ConvertValueToString(m_dMR_TC_1YEAR);
  setPotenDailyBriefing.m_HANDY_TC_1YEAR = ConvertValueToString(m_dHANDY_TC_1YEAR);
}

void CPotenDailyBriefing::AppendData(CSetPotenDailyBriefing& setPotenDailyBriefing) {
  ASSERT(setPotenDailyBriefing.IsOpen());
  setPotenDailyBriefing.AddNew();
  SaveData(setPotenDailyBriefing);
  setPotenDailyBriefing.Update();
}

bool CPotenDailyBriefing::ReadData(CWebDataReceivedPtr pWebDataReceived) {
  pWebDataReceived->m_lCurrentPos = 0;
  CString str, str1, strHead = _T("");
  CString strValue, strTime;
  CString strNoUse;

  bool fFoundTime = false;
  while (!fFoundTime) {
    str = GetNextString(pWebDataReceived);
    strHead = str.Left(20);
    if (strHead.Compare(_T("POTEN DAILY BRIEFING")) == 0) {
      strTime = GetNextString(pWebDataReceived); // 当前时间
      m_lDay = ConvertStringToTime(strTime);
      fFoundTime = true;
    }
  }

  while (pWebDataReceived->GetCurrentPos() < pWebDataReceived->GetBufferLength()) {
    str = GetNextString(pWebDataReceived);
    strHead = str.Left(12);
    if (strHead.Compare(_T("DIRTY TANKER")) == 0) {
      for (int i = 0; i < 5; i++) strNoUse = GetNextString(pWebDataReceived); // 抛掉4个没用字符串
      str1 = GetNextString(pWebDataReceived); // "VLCC"
      m_dTD3C = GetOneValue(str1);

      for (int i = 0; i < 3; i++) strNoUse = GetNextString(pWebDataReceived); // 抛掉4个没用字符串
      str1 = GetNextString(pWebDataReceived); // "SUEZMAX"
      m_dTD20 = GetOneValue(str1);

      for (int i = 0; i < 3; i++) strNoUse = GetNextString(pWebDataReceived); // 抛掉4个没用字符串
      str1 = GetNextString(pWebDataReceived); // "AFRAMAX"
      m_dTD9 = GetOneValue(str1);

      for (int i = 0; i < 3; i++) strNoUse = GetNextString(pWebDataReceived); // 抛掉3个没用字符串
      str1 = GetNextString(pWebDataReceived); // "PANAMAX"
      m_dTD21 = GetOneValue(str1);

      for (int i = 0; i < 4; i++) {
        strNoUse = GetNextString(pWebDataReceived);
        strHead = strNoUse.Left(5);
        strNoUse = GetNextString(pWebDataReceived);
        str1 = GetNextString(pWebDataReceived); // "CPP"
        strNoUse = GetNextString(pWebDataReceived);
        if (strHead.Compare(_T("MR,38")) == 0) {
          m_dTC14 = GetOneValue(str1);
        }
        else if (strHead.Compare(_T("MR,37"))) {
          m_dTC2 = GetOneValue(str1);
        }
        if (strHead.Compare(_T("LR2,7")) == 0) {
          m_dTC1 = GetOneValue(str1);
        }
        if (strHead.Compare(_T("LR1,5")) == 0) {
          m_dTC5 = GetOneValue(str1);
        }
        if (strHead.Compare(_T("MR,38")) == 0) {
          m_dTC14 = GetOneValue(str1);
        }
        if (strHead.Compare(_T("TIME ")) == 0) {
          i = 4;
        }
      }
    }

    strHead = str.Left(6);
    if (strHead.Compare(_T("Tanker")) == 0) {
      for (int i = 0; i < 7; i++) strNoUse = GetNextString(pWebDataReceived); // "CPP"
      str1 = GetNextString(pWebDataReceived);
      gl_CrweberIndex.m_dVLCC_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString(pWebDataReceived);
      gl_CrweberIndex.m_dVLCC_TC_3YEAR = ConvertStringToTC(str1);

      strNoUse = GetNextString(pWebDataReceived);
      strNoUse = GetNextString(pWebDataReceived);
      str1 = GetNextString(pWebDataReceived);
      gl_CrweberIndex.m_dSUEZMAX_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString(pWebDataReceived);
      gl_CrweberIndex.m_dSUEZMAX_TC_3YEAR = ConvertStringToTC(str1);

      strNoUse = GetNextString(pWebDataReceived);
      strNoUse = GetNextString(pWebDataReceived);
      str1 = GetNextString(pWebDataReceived);
      gl_CrweberIndex.m_dAFRAMAX_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString(pWebDataReceived);
      gl_CrweberIndex.m_dAFRAMAX_TC_3YEAR = ConvertStringToTC(str1);

      strNoUse = GetNextString(pWebDataReceived);
      strNoUse = GetNextString(pWebDataReceived);
      str1 = GetNextString(pWebDataReceived);
      gl_CrweberIndex.m_dPANAMAX_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString(pWebDataReceived);
      gl_CrweberIndex.m_dPANAMAX_TC_3YEAR = ConvertStringToTC(str1);

      strNoUse = GetNextString(pWebDataReceived);
      strNoUse = GetNextString(pWebDataReceived);
      str1 = GetNextString(pWebDataReceived);
      gl_CrweberIndex.m_dMR_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString(pWebDataReceived);
      gl_CrweberIndex.m_dMR_TC_3YEAR = ConvertStringToTC(str1);

      strNoUse = GetNextString(pWebDataReceived);
      strNoUse = GetNextString(pWebDataReceived);
      str1 = GetNextString(pWebDataReceived);
      gl_CrweberIndex.m_dHANDY_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString(pWebDataReceived);
      gl_CrweberIndex.m_dHANDY_TC_3YEAR = ConvertStringToTC(str1);

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

double CPotenDailyBriefing::ConvertStringToTC(CString str) {
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

/////////////////////////////////////////////////////////////////////////////
//
// 此时间串的格式为：mm/dd/yyyy
//
/////////////////////////////////////////////////////////////////////////////
long CPotenDailyBriefing::ConvertStringToTime(CString str) {
  int month, day, year;

  sscanf_s(str.GetBuffer(), "%02d/%02d/%04d", &month, &day, &year);
  return year * 10000 + month * 100 + day;
}

double CPotenDailyBriefing::GetOneValue(CString strValue) {
  double dValue = 0;
  char buffer[30];
  bool fMinus = false;
  char* p = strValue.GetBuffer();
  int i = 0;

  while (*p != 0x000) {
    if (*p == '(') {
      fMinus = true; p++;
    }
    if (*p == ')') {
      fMinus = true;
      p++;
    }
    if (*p == ',') p++;
    buffer[i++] = *p++;
  }
  buffer[i] = 0x000;
  dValue = atof(buffer);
  if (fMinus) dValue = -dValue;

  return dValue;
}

CString CPotenDailyBriefing::GetNextString(CWebDataReceivedPtr pWebDataReceived) {
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

bool CPotenDailyBriefing::IsDataChanged(void) {
  if ((gl_CrweberIndex.GetTD1() != gl_CrweberIndexLast.GetTD1()) ||
    (gl_CrweberIndex.GetTD2() != gl_CrweberIndexLast.GetTD2()) ||
      (gl_CrweberIndex.GetTD3C() != gl_CrweberIndexLast.GetTD3C()) ||
      (gl_CrweberIndex.GetTD5() != gl_CrweberIndexLast.GetTD5()) ||
      (gl_CrweberIndex.GetTD6() != gl_CrweberIndexLast.GetTD6()) ||
      (gl_CrweberIndex.GetTD7() != gl_CrweberIndexLast.GetTD7()) ||
      (gl_CrweberIndex.GetTD8() != gl_CrweberIndexLast.GetTD8()) ||
      (gl_CrweberIndex.GetTD9() != gl_CrweberIndexLast.GetTD9()) ||
      (gl_CrweberIndex.GetTD12() != gl_CrweberIndexLast.GetTD12()) ||
      (gl_CrweberIndex.GetTD15() != gl_CrweberIndexLast.GetTD15()) ||
      (gl_CrweberIndex.GetTD19() != gl_CrweberIndexLast.GetTD19()) ||
      (gl_CrweberIndex.GetTD20() != gl_CrweberIndexLast.GetTD20()) ||
      (gl_CrweberIndex.GetTD21() != gl_CrweberIndexLast.GetTD21()) ||
      (gl_CrweberIndex.GetVLCC_USGSPORE() != gl_CrweberIndexLast.GetVLCC_USGSPORE()) ||
      (gl_CrweberIndex.GetTC1() != gl_CrweberIndexLast.GetTC1()) ||
      (gl_CrweberIndex.GetTC2() != gl_CrweberIndexLast.GetTC2()) ||
      (gl_CrweberIndex.GetTC3() != gl_CrweberIndexLast.GetTC3()) ||
      (gl_CrweberIndex.GetTC4() != gl_CrweberIndexLast.GetTC4()) ||
      (gl_CrweberIndex.GetTC5() != gl_CrweberIndexLast.GetTC5()) ||
      (gl_CrweberIndex.GetTC14() != gl_CrweberIndexLast.GetTC14())) {
    return true;
  }
  return false;
}