///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网络数据查询器。所有的网络查询皆位于此处。
// 只有头文件。
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"SinaWebRTData.h"
#include"TengxunWebRTData.h"
#include"NeteaseWebDayLineData.h"
#include"NeteaseWebRTData.h"
#include"CrweberIndexWebData.h"
#include"CrweberIndex.h"

class CWebDataInquirer final : public CObject {
public:
  bool GetSinaRTData(void) { return m_SinaRTWebData.GetWebData(); }
  bool GetTengxunRTData(void) { return m_TengxunRTWebData.GetWebData(); }
  bool GetNeteaseRTData(void) { return m_NeteaseRTWebData.GetWebData(); }
  bool GetNeteaseDayLineData(void) {
    // 抓取日线数据.
    // 最多使用四个引擎，否则容易被网易服务器拒绝服务。一般还是用两个为好。
    switch (gl_cMaxSavingOneDayLineThreads) {
    case 8: case 7: case 6:
    m_NeteaseDayLineWebDataSixth.GetWebData(); // 网易日线历史数据
    case 5:
    m_NeteaseDayLineWebDataFifth.GetWebData();
    case 4:
    m_NeteaseDayLineWebDataFourth.GetWebData();
    case 3:
    m_NeteaseDayLineWebDataThird.GetWebData();
    case 2:
    m_NeteaseDayLineWebDataSecond.GetWebData();
    case 1: case 0:
    m_NeteaseDayLineWebData.GetWebData();
    break;
    default:
    m_NeteaseDayLineWebData.GetWebData();
    TRACE(_T("Out of range in Get Newease DayLine Web Data\n"));
    break;
    }
    return true;
  }
  bool GetCrweberIndexData(void) { return m_CrweberIndexWebData.GetWebData(); }
public:
  CSinaRTWebData m_SinaRTWebData; // 新浪实时数据采集
  CTengxunRTWebData m_TengxunRTWebData; // 腾讯实时数据采集
  CNeteaseRTWebData m_NeteaseRTWebData; // 网易实时数据采集
  CNeteaseDayLineWebData m_NeteaseDayLineWebData; // 网易日线历史数据
  CNeteaseDayLineWebData m_NeteaseDayLineWebDataSecond; // 网易日线历史数据
  CNeteaseDayLineWebData m_NeteaseDayLineWebDataThird; // 网易日线历史数据
  CNeteaseDayLineWebData m_NeteaseDayLineWebDataFourth; // 网易日线历史数据
  CNeteaseDayLineWebData m_NeteaseDayLineWebDataFifth; // 网易日线历史数据
  CNeteaseDayLineWebData m_NeteaseDayLineWebDataSixth; // 网易日线历史数据
  CCrweberIndexWebData m_CrweberIndexWebData; // crweber.com上的每日油运指数
};
