#include"pch.h"

#include"ProductTiingo.h"
import FireBird.DataSource.VirtualBase;
import FireBird.DataSource.Tiingo;

import FireBird.FinnhubInquiryType;
import FireBird.Tiingo.InaccessibleStock; 

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Ŀǰfinnhub��������˻�ֻ�ṩ�����������ݡ�Ϊ�˼�����ѯ�ٶȣ������Ƶ�����(�����������������롣
//
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CProductTiingo::AddInaccessibleSymbol() {
	gl_tiingoInaccessibleStock.SetUpdateDB(true);
	try { // ���ڴ���������
		const auto pStock = gl_tiingoInaccessibleStock.GetStock(m_iInquireType);
		if (!pStock->HaveSymbol(m_strInquiringSymbol)) {	// �µ�֤ȯ���룿
			pStock->AddSymbol(m_strInquiringSymbol);
		}
	} catch (out_of_range&) {	// �����ڴ��������ͣ���洢�������ͺ͸�֤ȯ����
		const auto pNewStock = make_shared<CInaccessible>();
		pNewStock->SetFunction(m_iInquireType);
		pNewStock->SetFunctionString(gl_FinnhubInquiryType.GetInquiryString(m_iInquireType));
		pNewStock->AddSymbol(m_strInquiringSymbol);
		gl_tiingoInaccessibleStock.SetStock(m_iInquireType, pNewStock);
	}
}

bool CProductTiingo::__Test_checkAccessRight(CWebDataPtr pWebData) {
	if (gl_pTiingoDataSource->GetWebErrorCode() == 404) { // 404 error
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return false;
	}
	if (m_iReceivedDataStatus == NO_ACCESS_RIGHT_) m_iReceivedDataStatus = GOOD_DATA__;
	return true;
}
