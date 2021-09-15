#pragma once

#include"Accessory.h"

#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"
#include"SetWorldStockDayLine.h"
#include"SetForexDayLine.h"
#include"SetCryptoDayLine.h"

#include"ChinaStockHistoryCandle.h"

#include<gsl/gsl>
using namespace gsl;

using namespace std;
#include<memory>

class CDayLine;
typedef shared_ptr<CDayLine> CDayLinePtr;

class CDayLine : public CChinaStockHistoryCandle {
public:
	CDayLine();
	void Reset(void);       // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

	bool LoadChinaMarketExtendData(not_null<CSetDayLineExtendInfo*> psetDayLineExtendInfo);

	bool ProcessNeteaseData(CString strStockCode, vector<char>& pBuffer, INT64& lCurrentPos);
	bool IsActive(void) const;

private:
};
