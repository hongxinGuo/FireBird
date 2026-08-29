#pragma once

#include"VirtualDataSource.h"

class CTengxunRTDataSource : public CVirtualDataSource {
public:
	CTengxunRTDataSource();
	CTengxunRTDataSource(const CTengxunRTDataSource&) = delete;
	CTengxunRTDataSource& operator=(const CTengxunRTDataSource&) = delete;
	CTengxunRTDataSource(const CTengxunRTDataSource&&) noexcept = delete;
	CTengxunRTDataSource& operator=(const CTengxunRTDataSource&&) noexcept = delete;
	~CTengxunRTDataSource() override = default;

	bool Reset() override;

	bool GenerateInquiryMessage(const local_seconds& currentTime) override;

	void ConfigureInternetOption() override; // 在读取网络数据前的准备工作，默认为设置Tengxun RT的m_pSession状态。
	void CreateCurrentInquireString() override;

	// 当所有被查询的股票皆为非上市股票时，腾讯实时股票服务器会返回一个21个字符长的字符串：v_pv_none_match=\"1\";\n
	// Note: 该字符串长度为21个字符,包括结尾的换行符。改成Raw string时出现错误，最后的换行符无法识别，故而仍然使用普通字符串。
	static bool IsInvalidTengxunRTData(const string& text) { return text == "v_pv_none_match=\"1\";\n"; }
};

using CTengxunRTDataSourcePtr = shared_ptr<CTengxunRTDataSource>;

extern CTengxunRTDataSourcePtr gl_pTengxunRTDataSource;
