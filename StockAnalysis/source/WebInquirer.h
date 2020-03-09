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

class CWebInquirer : public CObject {
public:
  CWebInquirer(void);
  virtual ~CWebInquirer(void);

public:
  bool GetSinaRTData(void) { return m_pSinaRTWebInquiry->GetWebData(); }
  bool GetTengxunRTData(void) { return m_pTengxunRTWebInquiry->GetWebData(); }
  bool GetNeteaseRTData(void) { return m_pNeteaseRTWebInquiry->GetWebData(); }
  bool GetNeteaseDayLineData(void);
  bool GetCrweberIndexData(void) { return m_pCrweberIndexWebInquiry->GetWebData(); }
  bool IsReadingCrweberIndex(void) { return m_pCrweberIndexWebInquiry->IsReadingWebData(); }
  bool GetPotenDailyBriefingData(void) { return m_pPotenDailyBriefingWebInquiry->GetWebData(); }
  bool IsReadingPotenDailyBriefing(void) { return m_pPotenDailyBriefingWebInquiry->IsReadingWebData(); }

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

  bool IsReadingWebThreadRunning(void) noexcept { return m_pSinaRTWebInquiry->IsReadingWebThreadRunning(); }

protected:
  CSinaRTWebInquiryPtr m_pSinaRTWebInquiry; // ����ʵʱ���ݲɼ�
  CTengxunRTWebInquiryPtr m_pTengxunRTWebInquiry; // ��Ѷʵʱ���ݲɼ�
  CNeteaseRTWebInquiryPtr m_pNeteaseRTWebInquiry; // ����ʵʱ���ݲɼ�
  CNeteaseDayLineWebInquiryPtr m_pNeteaseDayLineWebInquiry; // ����������ʷ����
  CNeteaseDayLineWebInquiryPtr m_pNeteaseDayLineWebInquirySecond; // ����������ʷ����
  CNeteaseDayLineWebInquiryPtr m_pNeteaseDayLineWebInquiryThird; // ����������ʷ����
  CNeteaseDayLineWebInquiryPtr m_pNeteaseDayLineWebInquiryFourth; // ����������ʷ����
  CNeteaseDayLineWebInquiryPtr m_pNeteaseDayLineWebInquiryFifth; // ����������ʷ����
  CNeteaseDayLineWebInquiryPtr m_pNeteaseDayLineWebInquirySixth; // ����������ʷ����
  CCrweberIndexWebInquiryPtr m_pCrweberIndexWebInquiry; // crweber.com�ϵ�ÿ������ָ��
  CPotenDailyBriefingWebInquiryPtr m_pPotenDailyBriefingWebInquiry; // Poten.com�ϵ��������ݡ�

  CQueueWebData m_qSinaRTWebData; // �������������ݴ����
  CQueueWebData m_qTengxunRTWebData; // ��Ѷ���������ݴ����
  CQueueWebData m_qNeteaseRTWebData; // �������������ݴ����
  CQueueWebData m_qCrweberWebData; // crweber.com���������ݴ����
  CQueueWebData m_qPotenDailyBriefingWebData; // Poten.com���������ݴ����
};

extern CWebInquirer gl_WebInquirer; //
