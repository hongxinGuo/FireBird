///////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ݲ�ѯ�������е������ѯ��λ�ڴ˴���
// ֻ��ͷ�ļ���
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
  CSinaRTWebInquiry m_SinaRTWebData; // ����ʵʱ���ݲɼ�
  CTengxunRTWebInquiry m_TengxunRTWebData; // ��Ѷʵʱ���ݲɼ�
  CNeteaseRTWebInquiry m_NeteaseRTWebData; // ����ʵʱ���ݲɼ�
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebData; // ����������ʷ����
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebDataSecond; // ����������ʷ����
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebDataThird; // ����������ʷ����
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebDataFourth; // ����������ʷ����
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebDataFifth; // ����������ʷ����
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebDataSixth; // ����������ʷ����
  CCrweberIndexWebInquiry m_CrweberIndexWebData; // crweber.com�ϵ�ÿ������ָ��
  CPotenDailyBriefingWebInquiry m_PotenDailyBriefingWebData; // Poten.com�ϵ��������ݡ�

  CQueueWebData m_queueSinaRTWebData; // �������������ݴ����
  CQueueWebData m_queueTengxunRTWebData; // ��Ѷ���������ݴ����
  CQueueWebData m_queueNeteaseRTWebData; // �������������ݴ����
  CQueueWebData m_queueCrweberWebData; // crweber.com���������ݴ����
  CQueueWebData m_queuePotenDailyBriefingWebData; // Poten.com���������ݴ����
};

extern CWebInquirer gl_WebDataInquirer; //
