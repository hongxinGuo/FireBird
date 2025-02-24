/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Note ��Ҫ���Ϊ�������������һ�����ݲɼ���product��ʹ�ò��й����߳�ͬʱ���룬���յ��������ݺ�һ�𴫵ݸ�product������
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "VirtualDataSource.h"
#include "InquireEngine.h"

#include"Thread.h"
#include "WebData.h"

atomic_int64_t CVirtualDataSource::sm_lTotalByteRead = 0;
atomic_int64_t CVirtualDataSource::sm_lTotalByteReadPerSecond = 0;

CVirtualDataSource::CVirtualDataSource() {
	SetDefaultSessionOption();
}

///////////////////////////////////////////////////////////////////////////////////
///<summary>
/// DataSource�Ķ��㺯����
///
/// Note ���ú�������ʹ��thread_pool_executor����background_executor��ֻ��ʹ��thread_executor������thread_pool_executor�������̵߳ķ���ֵ�޷���ȡ��ԭ����顣
///
/// Note ����ʹ�ö�����thread_executor�������У�����ʹ��thread_pool_executor����background_executor�������������ʹ�õ�thread_pool_executor����֮������ͻ�����²���ͬ�����⡣ԭ������
///
///</summary>
///
/// lMarketTime����ǰ�г�ʱ��
///
////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::Run(long lMarketTime) {
	CVirtualDataSourcePtr p = this->GetShared();
	if (!IsInquiring()) {
		gl_runtime.thread_executor()->post([p, lMarketTime] { //Note �˴�����ʹ��thread_executor
				p->GenerateInquiryMessage(lMarketTime);
				if (p->IsInquiring()) {
					p->InquireData();
				}
			});
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ݵ�����ʹ����λ�ڴ˴���
// ���ڴ���������ݿ����ж�����ʶ����ɶ���߳������룬���ҵȴ����е��߳���ɡ�
// ��Ҫ������������Դ�У���Ѷ�������ݡ�
//
//Note ֻ��ʹ��thread_pool_executor����background_executor������ʹ��thread_executor��
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::InquireData() {
	CString str = typeid(*this).name();
	ASSERT(gl_systemConfiguration.IsWorkingMode()); // ���������
	ASSERT(IsInquiring());

	auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());

	vector<result<CWebDataPtr>> vResults;
	while (HaveInquiry()) { // һ����������ж������
		GetCurrentProduct();
		CreateCurrentInquireString();
		CInquireEnginePtr pEngine = make_shared<CInquireEngine>(m_internetOption, GetInquiringString(), GetHeaders());
		auto background = gl_runtime.background_executor();
		auto result = gl_runtime.background_executor()->submit([this, pEngine] { //Note ֻ��ʹ��thread_pool_executor����background_executor
				auto pWebData = pEngine->GetWebData();
				if (!pEngine->IsWebError()) this->UpdateStatus(pWebData);
				SetHTTPStatusCode(pEngine->GetHTTPStatusCode());
				SetWebErrorCode(pEngine->GetErrorCode());
				return pWebData;
			});
		vResults.emplace_back(std::move(result));
	}
	const shared_ptr<vector<CWebDataPtr>> pvWebData = make_shared<vector<CWebDataPtr>>();
	for (auto& pWebData : vResults) {
		auto p = pWebData.get(); // ������ȴ����е��߳�ִ�����
		if (p != nullptr) {
			sm_lTotalByteRead += p->GetBufferLength();
			pvWebData->push_back(p);
		}
	}
	if (!gl_systemConfiguration.IsExitingSystem() && !pvWebData->empty()) {
		m_eErrorMessageData = IsAErrorMessageData(pvWebData->at(0)); // ���ص������Ǵ�����Ϣ���������ж������ʸ񣬸��½�ֹĿ¼
		m_pCurrentProduct->ParseAndStoreWebData(pvWebData);
		m_pCurrentProduct->UpdateDataSourceStatus(this->GetShared()); // ���ﴫ�ݵ���ʵ��DataSource������ָ��
	}
	auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
	SetCurrentInquiryTime((end - start).count());
	ASSERT(!HaveInquiry()); // û���ִ������
	ASSERT(IsInquiring()); // ִ�е���ʱ���в����������´ε����ݡ�
	SetInquiring(false); // �˱�ʶ��������Ҫλ��λ�����һ��
}

void CVirtualDataSource::SetDefaultSessionOption() {
	m_internetOption.option_connect_timeout = 120000;
	m_internetOption.option_receive_timeout = 120000;
	m_internetOption.option_data_receive_timeout = 120000;
	m_internetOption.option_send_timeout = 2000;
	m_internetOption.option_connect_retries = 1;
}

void CVirtualDataSource::CreateCurrentInquireString() {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiryFunction = m_pCurrentProduct->CreateMessage();
	CreateTotalInquiringString();
}

void CVirtualDataSource::CreateTotalInquiringString() {
	m_strInquiry = m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken;
}

void CVirtualDataSource::ReportErrorNotHandled(const string& sError) {
	gl_dailyLogger->warn("error not processed: {}", sError);
	gl_SoftwareDevelopingLogger->warn("error not processed: {}", sError);
	CString s = _T("error not processed:");
	s += sError.c_str();
	gl_systemMessage.PushInnerSystemInformationMessage(s);
}
