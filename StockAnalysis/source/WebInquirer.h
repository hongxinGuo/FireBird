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

extern CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry; // ����ʵʱ���ݲɼ�
extern CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry; // ��Ѷʵʱ���ݲɼ�
extern CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry; // ����ʵʱ���ݲɼ�
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry; // ����������ʷ����
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySecond; // ����������ʷ����
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryThird; // ����������ʷ����
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFourth; // ����������ʷ����
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiryFifth; // ����������ʷ����
extern CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquirySixth; // ����������ʷ����
extern CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry; // crweber.com�ϵ�ÿ������ָ��
extern CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry; // Poten.com�ϵ��������ݡ�

class CWebInquirer : public CObject {
public:
  CWebInquirer(void);
  virtual ~CWebInquirer(void);

  void Initialize(void);
public:
  bool GetSinaRTData(void) { return gl_pSinaRTWebInquiry->GetWebData(); }
  bool GetTengxunRTData(void) { return gl_pTengxunRTWebInquiry->GetWebData(); }
  bool GetNeteaseRTData(void) { return gl_pNeteaseRTWebInquiry->GetWebData(); }
  bool GetNeteaseDayLineData(void);
  bool GetCrweberIndexData(void) { return gl_pCrweberIndexWebInquiry->GetWebData(); }
  bool IsReadingCrweberIndex(void) { return gl_pCrweberIndexWebInquiry->IsReadingWebData(); }
  bool GetPotenDailyBriefingData(void) { return gl_pPotenDailyBriefingWebInquiry->GetWebData(); }
  bool IsReadingPotenDailyBriefing(void) { return gl_pPotenDailyBriefingWebInquiry->IsReadingWebData(); }

  bool IsReadingWebThreadRunning(void) noexcept { return gl_pSinaRTWebInquiry->IsReadingWebThreadRunning(); }

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

protected:
  CQueueWebData m_qSinaRTWebData; // �������������ݴ����
  CQueueWebData m_qTengxunRTWebData; // ��Ѷ���������ݴ����
  CQueueWebData m_qNeteaseRTWebData; // �������������ݴ����
  CQueueWebData m_qCrweberWebData; // crweber.com���������ݴ����
  CQueueWebData m_qPotenDailyBriefingWebData; // Poten.com���������ݴ����
};

extern CWebInquirer gl_WebInquirer; //
