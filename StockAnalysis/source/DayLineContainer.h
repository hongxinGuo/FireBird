#pragma once
#include"stdafx.h"

#include"DayLine.h"
#include "ChinaStockHistoryDataContainer.h"

#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"

using namespace std;

class CDayLineContainer final : public CChinaStockHistoryDataContainer {
public:
  virtual bool SaveData(void) override;
  virtual bool LoadData(void) override;
  virtual bool CalculatingData(void) override;

  bool StoreData(CDayLinePtr pData) noexcept { m_vHistoryData.push_back(dynamic_pointer_cast<CChinaStockHistoryData>(pData)); return true; }
  CDayLinePtr GetData(long lIndex) { return dynamic_pointer_cast<CDayLine>(m_vHistoryData.at(lIndex)); }
};
