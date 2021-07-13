#include"pch.h"
#include"globedef.h"

#include "ChinaStock.h"

#include"WebInquirer.h"

int gl_iMaxSavingOneDayLineThreads = 4; // �˱���ȡֵ��ΧΪ1-4

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"CrweberIndexWebInquiry.h"
#include"PotenDailyBriefingWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // ��Ѷʵʱ���ݲɼ�
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // ����ʵʱ���ݲɼ�
CCrweberIndexWebInquiryPtr gl_pCrweberIndexWebInquiry = nullptr; // crweber.com�ϵ�ÿ������ָ��
CPotenDailyBriefingWebInquiryPtr gl_pPotenDailyBriefingWebInquiry = nullptr; // Poten.com�ϵ��������ݡ�
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // ����������ʷ����
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2 = nullptr; // ����������ʷ����
CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry; // Finnhub.io֤ȯ��Ϣ
CQuandlWebInquiryPtr gl_pQuandlWebInquiry;
CTiingoWebInquiryPtr gl_pTiingoWebInquiry;

CWebInquirer::CWebInquirer(void) : CObject() {
}

CWebInquirer::~CWebInquirer(void) {
}

void CWebInquirer::Initialize(void) {
	gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
	gl_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
	gl_pNeteaseRTWebInquiry = make_shared<CNeteaseRTWebInquiry>();
	gl_pPotenDailyBriefingWebInquiry = make_shared<CPotenDailyBriefingWebInquiry>();
	gl_pCrweberIndexWebInquiry = make_shared<CCrweberIndexWebInquiry>();
	gl_pNeteaseDayLineWebInquiry = make_shared<CNeteaseDayLineWebInquiry>();
	gl_pNeteaseDayLineWebInquiry2 = make_shared<CNeteaseDayLineWebInquiry>();
	gl_pFinnhubWebInquiry = make_shared<CFinnhubWebInquiry>();
	gl_pTiingoWebInquiry = make_shared<CTiingoWebInquiry>();
	gl_pQuandlWebInquiry = make_shared<CQuandlWebInquiry>();
}

bool CWebInquirer::GetNeteaseDayLineData(void) {
	// ץȡ��������.
	// ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�Ŀǰʹ��������
	ASSERT((gl_iMaxSavingOneDayLineThreads <= 32) && (gl_iMaxSavingOneDayLineThreads > 0));
	switch (gl_iMaxSavingOneDayLineThreads) {
	case 6:
	case 5:
	case 4:
	case 3:
	case 2:
		if (!gl_pNeteaseDayLineWebInquiry2->IsReadingWebData()) {
			gl_pNeteaseDayLineWebInquiry2->GetWebData();
		}
	case 1:
		if (!gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) {
			gl_pNeteaseDayLineWebInquiry->GetWebData();
		}
		break;
	default:
		if (!gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) {
			gl_pNeteaseDayLineWebInquiry->GetWebData();
		}
		break;
	}
	return true;
}