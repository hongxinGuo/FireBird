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
protected:
  CQueueWebData m_queueSinaRTWebData; // �������������ݴ����
  CQueueWebData m_queueTengxunRTWebData; // ��Ѷ���������ݴ����
  CQueueWebData m_queueNeteaseRTWebData; // �������������ݴ����
  CQueueWebData m_queueCrweberWebData; // crweber.com���������ݴ����
};
