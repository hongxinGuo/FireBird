#include"pch.h"
#include"globedef.h"

#include"accessory.h"
#include "VirtualStock.h"

CVirtualStock::CVirtualStock() : CObject() {
  Reset();
}

CVirtualStock::~CVirtualStock() {
}

void CVirtualStock::Reset(void) {
  m_strSymbol = _T("");

  m_TransactionTime = 0;
  m_lLastClose = m_lOpen = 0;
  m_lHigh = m_lLow = m_lNew = 0;
  m_llVolume = 0;
  m_llAmount = 0;
  m_lUpDown = 0;
  m_dUpDownRate = 0;
  m_dChangeHandRate = 0;
  m_llTotalValue = m_llCurrentValue = 0;

  m_fTodayNewStock = false;
  m_fUpdateProfileDB = false;
  m_fActive = false;
  m_lDayLineStartDate = 29900101;
  m_lDayLineEndDate = 19000101;
  m_lIPOStatus = __STOCK_NOT_CHECKED__;   // Ĭ��״̬Ϊ��Ч��Ʊ���롣

  m_fDayLineNeedUpdate = true; // Ĭ��״̬��������Ҫ����
  m_fDayLineNeedProcess = false; // ��ʼ״̬Ϊ��δ��ȡ������ʷ���ݣ����账��
  m_fDayLineNeedSaving = false;
}