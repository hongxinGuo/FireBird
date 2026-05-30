#pragma once

#include"VirtualStock.h"

#include"ContainerCryptoDayLine.h"

#include<memory>
#include<vector>

class CFinnhubCrypto : public CVirtualStock {
public:
	CFinnhubCrypto() = default;
	// 不允许赋值。
	CFinnhubCrypto(const CFinnhubCrypto&) = delete;
	CFinnhubCrypto& operator=(const CFinnhubCrypto&) = delete;
	CFinnhubCrypto(const CFinnhubCrypto&&) noexcept = delete;
	CFinnhubCrypto& operator=(const CFinnhubCrypto&&) noexcept = delete;

	int GetRatio() const override { return 1000; } // 比例为1000

	void SetCheckingDayLineStatus();

	string GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const;

	void UpdateDayLine(const CDayLinesPtr& vDayLine) { m_dataDayLine.UpdateData(vDayLine); }
	void UnloadDayLine() { m_dataDayLine.Unload(); }
	size_t GetDayLineSize() const noexcept { return m_dataDayLine.Size(); }
	void UpdateDayLineStartEndDate();
	bool HaveNewDayLineData(); //Todo: 移至VirtualStock中，合并其他股票类型的同名函数

	void UpdateDayLineDB();
	virtual void SaveDayLineDB() { m_dataDayLine.SaveDB(GetSymbol()); }
	bool IsDayLineDuplicated() noexcept final;
	void DeleteDuplicatedDayLine() noexcept final;

public:
	CContainerCryptoDayLine m_dataDayLine;
};

typedef shared_ptr<CFinnhubCrypto> CFinnhubCryptoPtr;
typedef shared_ptr<vector<CFinnhubCryptoPtr>> CFinnhubCryptosPtr;
