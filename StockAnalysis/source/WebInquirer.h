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
  bool GetSinaRTData(void) { return m_SinaRTWebInquiry.GetWebData(); }
  bool GetTengxunRTData(void) { return m_TengxunRTWebInquiry.GetWebData(); }
  bool GetNeteaseRTData(void) { return m_NeteaseRTWebInquiry.GetWebData(); }
  bool GetNeteaseDayLineData(void);
  bool GetCrweberIndexData(void) { return m_CrweberIndexWebInquiry.GetWebData(); }
  bool IsReadingCrweberIndex(void) { return m_CrweberIndexWebInquiry.IsReadingWebData(); }
  bool GetPotenDailyBriefingData(void) { return m_PotenDailyBriefingWebInquiry.GetWebData(); }
  bool IsReadingPotenDailyBriefing(void) { return m_PotenDailyBriefingWebInquiry.IsReadingWebData(); }

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
  CSinaRTWebInquiry m_SinaRTWebInquiry; // ����ʵʱ���ݲɼ�
  CTengxunRTWebInquiry m_TengxunRTWebInquiry; // ��Ѷʵʱ���ݲɼ�
  CNeteaseRTWebInquiry m_NeteaseRTWebInquiry; // ����ʵʱ���ݲɼ�
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiry; // ����������ʷ����
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquirySecond; // ����������ʷ����
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiryThird; // ����������ʷ����
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiryFourth; // ����������ʷ����
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiryFifth; // ����������ʷ����
  CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquirySixth; // ����������ʷ����
  CCrweberIndexWebInquiry m_CrweberIndexWebInquiry; // crweber.com�ϵ�ÿ������ָ��
  CPotenDailyBriefingWebInquiry m_PotenDailyBriefingWebInquiry; // Poten.com�ϵ��������ݡ�

  CQueueWebData m_queueSinaRTWebData; // �������������ݴ����
  CQueueWebData m_queueTengxunRTWebData; // ��Ѷ���������ݴ����
  CQueueWebData m_queueNeteaseRTWebData; // �������������ݴ����
  CQueueWebData m_queueCrweberWebData; // crweber.com���������ݴ����
  CQueueWebData m_queuePotenDailyBriefingWebData; // Poten.com���������ݴ����
};

extern CWebInquirer gl_WebInquirer; //
