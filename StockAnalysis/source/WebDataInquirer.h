///////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ݲ�ѯ�������е������ѯ��λ�ڴ˴���
// ֻ��ͷ�ļ���
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
    // ץȡ��������.
    // ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
    switch (gl_cMaxSavingOneDayLineThreads) {
    case 8: case 7: case 6:
    m_NeteaseDayLineWebDataSixth.GetWebData(); // ����������ʷ����
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
  CSinaRTWebData m_SinaRTWebData; // ����ʵʱ���ݲɼ�
  CTengxunRTWebData m_TengxunRTWebData; // ��Ѷʵʱ���ݲɼ�
  CNeteaseRTWebData m_NeteaseRTWebData; // ����ʵʱ���ݲɼ�
  CNeteaseDayLineWebData m_NeteaseDayLineWebData; // ����������ʷ����
  CNeteaseDayLineWebData m_NeteaseDayLineWebDataSecond; // ����������ʷ����
  CNeteaseDayLineWebData m_NeteaseDayLineWebDataThird; // ����������ʷ����
  CNeteaseDayLineWebData m_NeteaseDayLineWebDataFourth; // ����������ʷ����
  CNeteaseDayLineWebData m_NeteaseDayLineWebDataFifth; // ����������ʷ����
  CNeteaseDayLineWebData m_NeteaseDayLineWebDataSixth; // ����������ʷ����
  CCrweberIndexWebData m_CrweberIndexWebData; // crweber.com�ϵ�ÿ������ָ��
};
