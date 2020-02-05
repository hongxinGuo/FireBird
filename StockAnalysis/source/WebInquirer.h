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
  void PushSinaRTData(CWebDataReceivedPtr pData) { m_qSinaRTWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopSinaRTData(void) { return m_qSinaRTWebData.PopWebData(); }
  long GetTengxunRTDataSize(void) { return m_qTengxunRTWebData.GetWebDataSize(); }
  void PushTengxunRTData(CWebDataReceivedPtr pData) { m_qTengxunRTWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopTengxunRTData(void) { return m_qTengxunRTWebData.PopWebData(); }
  long GetNeteaseRTDataSize(void) { return m_qNeteaseRTWebData.GetWebDataSize(); }
  void PushNeteaseRTData(CWebDataReceivedPtr pData) { m_qNeteaseRTWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopNeteaseRTData(void) { return m_qNeteaseRTWebData.PopWebData(); }
  long GetCrweberDataSize(void) { return m_qCrweberWebData.GetWebDataSize(); }
  void PushCrweberData(CWebDataReceivedPtr pData) { m_qCrweberWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopCrweberData(void) { return m_qCrweberWebData.PopWebData(); }

  long GetPotenDailyBriefingDataSize(void) { return m_qPotenDailyBriefingWebData.GetWebDataSize(); }
  void PushPotenDailyBriefingData(CWebDataReceivedPtr pData) { m_qPotenDailyBriefingWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopPotenDailyBriefingData(void) { return m_qPotenDailyBriefingWebData.PopWebData(); }

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

  CQueueWebData m_qSinaRTWebData; // �������������ݴ����
  CQueueWebData m_qTengxunRTWebData; // ��Ѷ���������ݴ����
  CQueueWebData m_qNeteaseRTWebData; // �������������ݴ����
  CQueueWebData m_qCrweberWebData; // crweber.com���������ݴ����
  CQueueWebData m_qPotenDailyBriefingWebData; // Poten.com���������ݴ����
};

extern CWebInquirer gl_WebInquirer; //
