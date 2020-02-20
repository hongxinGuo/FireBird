///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网络数据查询器。所有的网络查询皆位于此处。
// 只有头文件。
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"globedef.h"

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"CrweberIndexWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"

#include"CrweberIndex.h"

extern int gl_cMaxSavingOneDayLineThreads;

class CWebInquirer final : public CObject {
public:
  bool GetSinaRTData(void) { return m_SinaRTWebInquiry.GetWebData(); }
  bool GetTengxunRTData(void) { return m_TengxunRTWebInquiry.GetWebData(); }
  bool GetNeteaseRTData(void) { return m_NeteaseRTWebInquiry.GetWebData(); }
  bool GetNeteaseDayLineData(void);
  bool GetCrweberIndexData(void) { return m_CrweberIndexWebInquiry.GetWebData(); }
  bool IsReadingCrweberIndex(void) { return m_CrweberIndexWebInquiry.IsReadingWebData(); }
  bool GetPotenDailyBriefingData(void) { return m_PotenDailyBriefingWebInquiry.GetWebData(); }
  bool IsReadingPotenDailyBriefing(void) { return m_PotenDailyBriefingWebInquiry.IsReadingWebData(); }

public:
  long GetSinaRTDataSize(void) { return m_qSinaRTWebData.GetWebDataSize(); }
  void PushSinaRTData(CWebDataPtr pData) { m_qSinaRTWebData.PushWebData(pData); }
  CWebDataPtr PopSinaRTData(void) { return m_qSinaRTWebData.PopWebData(); }
  long GetTengxunRTDataSize(void) { return m_qTengxunRTWebData.GetWebDataSize(); }
  void PushTengxunRTData(CWebDataPtr pData) { m_qTengxunRTWebData.PushWebData(pData); }
  CWebDataPtr PopTengxunRTData(void) { return m_qTengxunRTWebData.PopWebData(); }
  long GetNeteaseRTDataSize(void) { return m_qNeteaseRTWebData.GetWebDataSize(); }
  void PushNeteaseRTData(CWebDataPtr pData) { m_qNeteaseRTWebData.PushWebData(pData); }
  CWebDataPtr PopNeteaseRTData(void) { return m_qNeteaseRTWebData.PopWebData(); }
  long GetCrweberDataSize(void) { return m_qCrweberWebData.GetWebDataSize(); }
  void PushCrweberData(CWebDataPtr pData) { m_qCrweberWebData.PushWebData(pData); }
  CWebDataPtr PopCrweberData(void) { return m_qCrweberWebData.PopWebData(); }

  long GetPotenDailyBriefingDataSize(void) { return m_qPotenDailyBriefingWebData.GetWebDataSize(); }
  void PushPotenDailyBriefingData(CWebDataPtr pData) { m_qPotenDailyBriefingWebData.PushWebData(pData); }
  CWebDataPtr PopPotenDailyBriefingData(void) { return m_qPotenDailyBriefingWebData.PopWebData(); }

  bool IsReadingWebThreadRunning(void) noexcept { return m_SinaRTWebInquiry.IsReadingWebThreadRunning(); }

protected:
  CSinaRTWebInquiry m_SinaRTWebInquiry; // 新浪实时数据采集
  CTengxunRTWebInquiry m_TengxunRTWebInquiry; // 腾讯实时数据采集
  CNeteaseRTWebInquiry m_NeteaseRTWebInquiry; // 网易实时数据采集
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiry; // 网易日线历史数据
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquirySecond; // 网易日线历史数据
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiryThird; // 网易日线历史数据
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiryFourth; // 网易日线历史数据
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiryFifth; // 网易日线历史数据
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquirySixth; // 网易日线历史数据
  CCrweberIndexWebInquiry m_CrweberIndexWebInquiry; // crweber.com上的每日油运指数
  CPotenDailyBriefingWebInquiry m_PotenDailyBriefingWebInquiry; // Poten.com上的油运数据。

  CQueueWebData m_qSinaRTWebData; // 新浪网络数据暂存队列
  CQueueWebData m_qTengxunRTWebData; // 腾讯网络数据暂存队列
  CQueueWebData m_qNeteaseRTWebData; // 网易网络数据暂存队列
  CQueueWebData m_qCrweberWebData; // crweber.com网络数据暂存队列
  CQueueWebData m_qPotenDailyBriefingWebData; // Poten.com网络数据暂存队列
};

extern CWebInquirer gl_WebInquirer; //
