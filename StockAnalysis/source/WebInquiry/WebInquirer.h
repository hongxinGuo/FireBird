///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网络数据查询器。所有的网络查询皆位于此处。
// 只有头文件。
// WebData意指从网络上得到的原始数据。
//
// 由于采用独立线程模式读取网络数据，故而需要保证线程安全。
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

#include"TemplateMutexAccessQueue.h"

#include"NeteaseDayLineWebData.h"

class CWebInquirer : public CObject {
public:
	CWebInquirer(void);
	// 不允许赋值。
	CWebInquirer(const CWebInquirer&) = delete;
	CWebInquirer& operator=(const CWebInquirer&) = delete;
	CWebInquirer(const CWebInquirer&&) noexcept = delete;
	CWebInquirer& operator=(const CWebInquirer&&) noexcept = delete;
	virtual ~CWebInquirer(void);

	void Initialize(void);

public:
	// 网易日线数据
	size_t GetParsedNeteaseDayLineDataSize(void) { return(m_qParsedNeteaseDayLine.Size()); }
	void PushParsedNeteaseDayLineData(CNeteaseDayLineWebDataPtr pData) { m_qParsedNeteaseDayLine.PushData(pData); }
	CNeteaseDayLineWebDataPtr PopParsedNeteaseDayLineData(void) { return m_qParsedNeteaseDayLine.PopData(); }

protected:
	CTemplateMutexAccessQueue<CNeteaseDayLineWebData> m_qParsedNeteaseDayLine;
};

extern CWebInquirer gl_WebInquirer; //
