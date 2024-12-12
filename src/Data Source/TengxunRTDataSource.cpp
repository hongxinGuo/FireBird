#include"pch.h"
module;
module FireBird.DataSource.TengxunRT;

import FireBird.Product.TengxunRT;
import FireBird.Market.China;
import FireBird.System.Configuration;

CTengxunRTDataSource::CTengxunRTDataSource() {
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 900; // ��Ѷʵʱ���ݲ�ѯĬ��ֵ

	CTengxunRTDataSource::ConfigureInternetOption();

	CTengxunRTDataSource::Reset();
}

bool CTengxunRTDataSource::Reset() {
	return true;
}

bool CTengxunRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount();

	if (llTickCount > m_llLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()) {
		// ���ж��´�����ʱ�䡣�����������ʱ����֮�������´����롣
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) {
			m_llLastTimeTickCount = llTickCount + 60000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
		}
		else {
			if (!IsInquiring()) m_llLastTimeTickCount = llTickCount; // ֻ�е���һ����������󷽵�����ʱ���㣬���������һ�����볬ʱ�����󣬱�֤���������һ�����롣
		}
		if (!IsInquiring()) {
			const auto product = make_shared<CProductTengxunRT>();
			product->SetMarket(gl_pChinaMarket);
			ASSERT(!HaveInquiry());
			StoreInquiry(product);
			SetInquiring(true);
			return true;
		}
	}
	return false;
}

void CTengxunRTDataSource::CreateCurrentInquireString() {
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

void CTengxunRTDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 1000;
	m_internetOption.option_receive_timeout = 1000;
	m_internetOption.option_data_receive_timeout = 1000;
	m_internetOption.option_send_timeout = 200;
	m_internetOption.option_connect_retries = 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����б���ѯ�Ĺ�Ʊ��Ϊ�����й�Ʊʱ����Ѷʵʱ��Ʊ�������᷵��һ��21���ַ������ַ�����v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTengxunRTDataSource::IsInvalidTengxunRTData(const CWebData& WebDataReceived) {
	const string_view sv = WebDataReceived.GetStringView(0, 21);

	if (sv.compare(_T("v_pv_none_match=\"1\";\n")) == 0) {
		ASSERT(WebDataReceived.GetBufferLength() == 21);
		return true;
	}
	else return false;
}
