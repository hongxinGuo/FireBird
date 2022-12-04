///////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ݲ�ѯ�������е������ѯ��λ�ڴ˴���
// ֻ��ͷ�ļ���
// WebData��ָ�������ϵõ���ԭʼ���ݡ�
//
// ���ڲ��ö����߳�ģʽ��ȡ�������ݣ��ʶ���Ҫ��֤�̰߳�ȫ��
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
	// ������ֵ��
	CWebInquirer(const CWebInquirer&) = delete;
	CWebInquirer& operator=(const CWebInquirer&) = delete;
	CWebInquirer(const CWebInquirer&&) noexcept = delete;
	CWebInquirer& operator=(const CWebInquirer&&) noexcept = delete;
	virtual ~CWebInquirer(void);

	void Initialize(void);

public:
	// ������������
	size_t GetParsedNeteaseDayLineDataSize(void) { return(m_qParsedNeteaseDayLine.Size()); }
	void PushParsedNeteaseDayLineData(CNeteaseDayLineWebDataPtr pData) { m_qParsedNeteaseDayLine.PushData(pData); }
	CNeteaseDayLineWebDataPtr PopParsedNeteaseDayLineData(void) { return m_qParsedNeteaseDayLine.PopData(); }

protected:
	CTemplateMutexAccessQueue<CNeteaseDayLineWebData> m_qParsedNeteaseDayLine;
};

extern CWebInquirer gl_WebInquirer; //
