#include"pch.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"
#include"WebInquirer.h"

#include"Accessory.h"

#include "PotenDailyBriefingWebInquiry.h"

using namespace std;
#include<thread>

CPotenDailyBriefingWebInquiry::CPotenDailyBriefingWebInquiry() : CVirtualWebInquiry() {
	m_strWebDataInquirePrefix = _T("http://energy.poten.com/poten-daily-briefing-webpage-");
	m_strWebDataInquireSuffix = _T("");
	m_strConnectionName = _T("PotenDailyBriefing");

	m_lInquiringDate = 20180411; //poten.com��վ�����¸�ʽ�Ӵ�����֮��ʼ��֮ǰ�ĸ�ʽ��ʱ��ȥ��ȡ��
}

CPotenDailyBriefingWebInquiry::~CPotenDailyBriefingWebInquiry() {
}

bool CPotenDailyBriefingWebInquiry::PrepareNextInquiringStr(void) {
	m_lInquiringDate = gl_pPotenDailyBriefingMarket->GetCurrentInquiringDate();
	CString strMiddle = _T("");
	char buffer[50];
	const long year = m_lInquiringDate / 10000;
	const long month = m_lInquiringDate / 100 - year * 100;
	const long day = m_lInquiringDate - year * 10000 - month * 100;

	sprintf_s(buffer, _T("%02d/%02d/%04d"), month, day, year);
	strMiddle = buffer;
	CreateTotalInquiringString(strMiddle);
	TRACE(_T("��ȡ%08d�յ�poten����\n"), m_lInquiringDate);
	return true;
}

void CPotenDailyBriefingWebInquiry::SetTime(CWebDataPtr pDataTimeBeSet) {
	pDataTimeBeSet->SetTime(gl_pPotenDailyBriefingMarket->TransferToUTCTime(GetInquiringDate()));
}

void CPotenDailyBriefingWebInquiry::StoreWebData(CWebDataPtr pWebData) {
	gl_WebInquirer.PushPotenDailyBriefingData(pWebData);
}