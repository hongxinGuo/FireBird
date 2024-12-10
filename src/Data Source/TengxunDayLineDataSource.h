#pragma once

#include"ChinaStock.h"
#include"VirtualDataSource.h"
import FireBird.WebProduct.Virtual;

class CTengxunDayLineDataSource : public CVirtualDataSource {
public:
	CTengxunDayLineDataSource();
	// 只能有一个实例,不允许赋值、拷贝
	CTengxunDayLineDataSource(const CTengxunDayLineDataSource&) = delete;
	CTengxunDayLineDataSource& operator=(const CTengxunDayLineDataSource&) = delete;
	CTengxunDayLineDataSource(const CTengxunDayLineDataSource&&) noexcept = delete;
	CTengxunDayLineDataSource& operator=(const CTengxunDayLineDataSource&&) noexcept = delete;
	~CTengxunDayLineDataSource() override = default;
	bool Reset() override;

	bool GenerateInquiryMessage(long lCurrentTime) override;

	void ConfigureInternetOption() override; // 在读取网络数据前的准备工作，默认为设置Netease DayLine的m_pSession状态。
	void CreateCurrentInquireString() override;
	void UpdateStatus(CWebDataPtr pData) override; // 成功接收后更新系统状态, 此处更新其股票代码

	virtual bool Inquire();
	vector<CVirtualWebProductPtr> CreateProduct(const CChinaStockPtr& pStock) const;

	bool IsUpdateDayLine() const noexcept { return m_fUpdateDayLine; }
	void SetUpdateDayLine(bool fFlag) noexcept { m_fUpdateDayLine = fFlag; }

	void SetDownLoadingStockCode(const CString& strStockCode);
	CString GetDownLoadingStockCode() { return m_strDownLoadingStockCode; }
	void ResetDownLoadingStockCode() { m_strDownLoadingStockCode = _T(""); }

protected:
	bool m_fUpdateDayLine{ true }; // 每日更新公司日线数据
	CString m_strDownLoadingStockCode{ _T("") };
};

using CTengxunDayLineDataSourcePtr = shared_ptr<CTengxunDayLineDataSource>;
extern CTengxunDayLineDataSourcePtr gl_pTengxunDayLineDataSource;
