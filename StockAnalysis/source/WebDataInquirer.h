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

class CWebInquirer final : public CObject {
public:
  bool GetSinaRTData(void) { return m_SinaRTWebData.GetWebData(); }
  bool GetTengxunRTData(void) { return m_TengxunRTWebData.GetWebData(); }
  bool GetNeteaseRTData(void) { return m_NeteaseRTWebData.GetWebData(); }
  bool GetNeteaseDayLineData(void);
  bool GetCrweberIndexData(void) { return m_CrweberIndexWebData.GetWebData(); }
  bool IsReadingCrweberIndex(void) { return m_CrweberIndexWebData.IsReadingWebData(); }
  bool GetPotenDailyBriefingData(void) { return m_PotenDailyBriefingWebData.GetWebData(); }
  bool IsReadingPotenDailyBriefing(void) { return m_PotenDailyBriefingWebData.IsReadingWebData(); }

public:
  long GetSinaRTDataSize(void) { return m_queueSinaRTWebData.GetWebDataSize(); }
  void PushSinaRTData(CWebDataReceivedPtr pData) { m_queueSinaRTWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopSinaRTData(void) { return m_queueSinaRTWebData.PopWebData(); }
  long GetTengxunRTDataSize(void) { return m_queueTengxunRTWebData.GetWebDataSize(); }
  void PushTengxunRTData(CWebDataReceivedPtr pData) { m_queueTengxunRTWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopTengxunRTData(void) { return m_queueTengxunRTWebData.PopWebData(); }
  long GetNeteaseRTDataSize(void) { return m_queueNeteaseRTWebData.GetWebDataSize(); }
  void PushNeteaseRTData(CWebDataReceivedPtr pData) { m_queueNeteaseRTWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopNeteaseRTData(void) { return m_queueNeteaseRTWebData.PopWebData(); }
  long GetCrweberDataSize(void) { return m_queueCrweberWebData.GetWebDataSize(); }
  void PushCrweberData(CWebDataReceivedPtr pData) { m_queueCrweberWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopCrweberData(void) { return m_queueCrweberWebData.PopWebData(); }

  long GetPotenDailyBriefingDataSize(void) { return m_queuePotenDailyBriefingWebData.GetWebDataSize(); }
  void PushPotenDailyBriefingData(CWebDataReceivedPtr pData) { m_queuePotenDailyBriefingWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopPotenDailyBriefingData(void) { return m_queuePotenDailyBriefingWebData.PopWebData(); }

protected:
  CSinaRTWebInquiry m_SinaRTWebData; // 新浪实时数据采集
  CTengxunRTWebInquiry m_TengxunRTWebData; // 腾讯实时数据采集
  CNeteaseRTWebInquiry m_NeteaseRTWebData; // 网易实时数据采集
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebData; // 网易日线历史数据
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebDataSecond; // 网易日线历史数据
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebDataThird; // 网易日线历史数据
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebDataFourth; // 网易日线历史数据
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebDataFifth; // 网易日线历史数据
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebDataSixth; // 网易日线历史数据
  CCrweberIndexWebInquiry m_CrweberIndexWebData; // crweber.com上的每日油运指数
  CPotenDailyBriefingWebInquiry m_PotenDailyBriefingWebData; // Poten.com上的油运数据。

  CQueueWebData m_queueSinaRTWebData; // 新浪网络数据暂存队列
  CQueueWebData m_queueTengxunRTWebData; // 腾讯网络数据暂存队列
  CQueueWebData m_queueNeteaseRTWebData; // 网易网络数据暂存队列
  CQueueWebData m_queueCrweberWebData; // crweber.com网络数据暂存队列
  CQueueWebData m_queuePotenDailyBriefingWebData; // Poten.com网络数据暂存队列
};

extern CWebInquirer gl_WebDataInquirer; //
