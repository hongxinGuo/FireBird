#include "stdafx.h"

#include"globedef.h"
#include"ChinaStockHistoryDataContainer.h"

CChinaStockHistoryDataContainer::CChinaStockHistoryDataContainer() : CObject() {
  Reset();
}

CChinaStockHistoryDataContainer::~CChinaStockHistoryDataContainer() {
}

void CChinaStockHistoryDataContainer::Reset(void) {
  m_vHistoryData.clear();

  m_fDataLoaded = false;
  m_fDatabaseTodayUpdated = false;

  m_fLoadDataFirst = false;
}

bool CChinaStockHistoryDataContainer::CalculateRelativeStrong(void) {
  CalculateRS(3);
  CalculateRS(5);
  CalculateRS(10);
  CalculateRS(30);
  CalculateRS(60);
  CalculateRS(120);
  return true;
}

bool CChinaStockHistoryDataContainer::CalculateRelativeStrongLogarithm(void) {
  CalculateRSLogarithm(3);
  CalculateRSLogarithm(5);
  CalculateRSLogarithm(10);
  CalculateRSLogarithm(30);
  CalculateRSLogarithm(60);
  CalculateRSLogarithm(120);
  return true;
}

bool CChinaStockHistoryDataContainer::CalculateRelativeStrongIndex(void) {
  CalculateRSIndex(3);
  CalculateRSIndex(5);
  CalculateRSIndex(10);
  CalculateRSIndex(30);
  CalculateRSIndex(60);
  CalculateRSIndex(120);
  return true;
}

bool CChinaStockHistoryDataContainer::CalculateRSLogarithm(INT64 lNumber) {
  double dTempRS = 0;
  const INT64 lTotalNumber = m_vHistoryData.size();
  for (INT64 i = lNumber; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (INT64 j = i - lNumber; j < i; j++) {
      dTempRS += m_vHistoryData.at(j)->GetRelativeStrongLogarithm();
    }
    switch (lNumber) {
    case 3:
    m_vHistoryData.at(i)->m_d3RS = dTempRS / lNumber;
    break;
    case 5:
    m_vHistoryData.at(i)->m_d5RS = dTempRS / lNumber;
    break;
    case 10:
    m_vHistoryData.at(i)->m_d10RS = dTempRS / lNumber;
    break;
    case 30:
    m_vHistoryData.at(i)->m_d30RS = dTempRS / lNumber;
    break;
    case 60:
    m_vHistoryData.at(i)->m_d60RS = dTempRS / lNumber;
    break;
    case 120:
    m_vHistoryData.at(i)->m_d120RS = dTempRS / lNumber;
    break;
    default:
    ASSERT(0);
    }
  }
  return true;
}

bool CChinaStockHistoryDataContainer::CalculateRS(INT64 lNumber) {
  double dTempRS = 0;
  const INT64 lTotalNumber = m_vHistoryData.size();
  for (INT64 i = lNumber; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (INT64 j = i - lNumber; j < i; j++) {
      dTempRS += m_vHistoryData.at(j)->GetRelativeStrong();
    }
    switch (lNumber) {
    case 3:
    m_vHistoryData.at(i)->m_d3RS = dTempRS / lNumber;
    break;
    case 5:
    m_vHistoryData.at(i)->m_d5RS = dTempRS / lNumber;
    break;
    case 10:
    m_vHistoryData.at(i)->m_d10RS = dTempRS / lNumber;
    break;
    case 30:
    m_vHistoryData.at(i)->m_d30RS = dTempRS / lNumber;
    break;
    case 60:
    m_vHistoryData.at(i)->m_d60RS = dTempRS / lNumber;
    break;
    case 120:
    m_vHistoryData.at(i)->m_d120RS = dTempRS / lNumber;
    break;
    default:
    ASSERT(0);
    }
  }
  return true;
}

bool CChinaStockHistoryDataContainer::CalculateRSIndex(INT64 lNumber) {
  double dTempRS = 0;
  const INT64 lTotalNumber = m_vHistoryData.size();
  for (INT64 i = lNumber; i < lTotalNumber; i++) {
    dTempRS = 0;
    for (INT64 j = i - lNumber; j < i; j++) {
      dTempRS += m_vHistoryData.at(j)->GetRelativeStrongIndex();
    }
    switch (lNumber) {
    case 3:
    m_vHistoryData.at(i)->m_d3RS = dTempRS / lNumber;
    break;
    case 5:
    m_vHistoryData.at(i)->m_d5RS = dTempRS / lNumber;
    break;
    case 10:
    m_vHistoryData.at(i)->m_d10RS = dTempRS / lNumber;
    break;
    case 30:
    m_vHistoryData.at(i)->m_d30RS = dTempRS / lNumber;
    break;
    case 60:
    m_vHistoryData.at(i)->m_d60RS = dTempRS / lNumber;
    break;
    case 120:
    m_vHistoryData.at(i)->m_d120RS = dTempRS / lNumber;
    break;
    default:
    ASSERT(0);
    }
  }
  return true;
}