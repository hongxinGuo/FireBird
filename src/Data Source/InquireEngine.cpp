#include "pch.h"
module;
#include"globedef.h"
#include"NlohmannJsonHeader.h"
#include <afxinet.h>
module FireBird.DataSource.InquireEngine;

import FireBird.System.Data;
import FireBird.System.Message;
import FireBird.Type.WebData;
import FireBird.Accessory.InfoReport;
import FireBird.System.Configuration;

CInquireEngine::CInquireEngine(): m_dataBuffer{} {
	m_pSession = make_shared<CInternetSession>(_T("FireBird")); // �˴���Ҫ���ϵ��ó�������ƣ������޷����е�Ԫ���Գ���ԭ��������
}

CInquireEngine::CInquireEngine(const InternetOption& option, const CString& strInquire, const CString& strHeaders): m_dataBuffer{} {
	m_pSession = make_shared<CInternetSession>(_T("FireBird")); // �˴���Ҫ���ϵ��ó�������ƣ������޷����е�Ԫ���Գ���ԭ��������
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, option.option_connect_timeout);
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, option.option_receive_timeout);
	m_pSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, option.option_data_receive_timeout);
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, option.option_send_timeout);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, option.option_connect_retries);
	m_strInquiry = strInquire;
	m_strHeaders = strHeaders;
	m_pFile = nullptr;
	m_lContentLength = 0;
	m_dwHTTPStatusCode = 0;

	m_sBuffer.resize(0);
	m_lByteRead = 0;
	m_fWebError = false;
}

void CInquireEngine::ConfigureSession(const InternetOption& option) const {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, option.option_connect_timeout);
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, option.option_receive_timeout);
	m_pSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, option.option_data_receive_timeout);
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, option.option_send_timeout);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, option.option_connect_retries);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʱ����һ�������������ݵ�ָ�룬����������������߷������⣬�򷵻�һ����ָ�롣
//
//
// �������ȡ���ݡ�ÿ�ζ�16KB��ֱ��������Ϊֹ��
// ����������������߷������⣬�򱨸�����������������
//
//
///////////////////////////////////////////////////////////////////////////
CWebDataPtr CInquireEngine::GetWebData() {
	try {
		OpenFile();
		GetFileHeaderInformation();
		if (m_lContentLength > 0) {
			m_sBuffer.resize(m_lContentLength + 1); //Note ������������С����ʵ�����ݴ�1�ֽ�,���ڴ洢string������ص�0x000,�Է�ֹԽ����ʡ�
		}
		else {
			m_sBuffer.resize(1024 * 1024);// ���������ر����ݳ���ʱ�����ó�ʼ������Ϊ1M��
		}
		UINT lCurrentByteRead;
		do {
			if (gl_systemConfiguration.IsExitingSystem()) {
				CInternetException* e = new CInternetException(1); // ��Ϊ�㼴�ɡ�ʹ����λ�����µ����ݣ��ܿ�ϵͳԤ�������ķ�Χ
				throw(e); // ��ϵͳ�˳�ʱ��Ҫ�����жϴ˽��̣��Է�ֹ�ڴ�й¶��
			}
			lCurrentByteRead = ReadWebFileOneTime(); // ÿ�ζ�ȡ16K���ݡ�
			XferReadingToBuffer(lCurrentByteRead);
			m_lByteRead += lCurrentByteRead;
			if (m_lContentLength == 0) IncreaseBufferSizeIfNeeded(1024 * 1024);
		} while (lCurrentByteRead > 0);
		m_dwErrorCode = 0; // �����������������������
	} catch (CInternetException* exception) {//Note ����һ����ʹ�����á�����׼���ڴ�������ɾ��������⣬�ʶ�ֱ��ʹ��ָ�롣��������ϵͳ����������⣬�ᵼ�³����Զ��˳���
		SetWebError(true);
		m_dwErrorCode = exception->m_dwError;
		ReportWebError(exception->m_dwError, m_strInquiry);
		exception->Delete();
		DeleteWebFile();
		return nullptr;
	}
	DeleteWebFile();
	VerifyDataLength();
	return CreateWebData();
}

// <summary>
//
// ��������ʵʱ���ݷ�������Ҫ�ṩͷ����֤���ݣ��ʶ�OpenURL����ʹ��Ĭ��ֵ����������Ҫ��������m_strHeaders��Ĭ��Ϊ�գ���
// ������������δ��Ҫ�ṩͷ����֤���ݡ�
// ���ú�����Ҫ����exception��
//
void CInquireEngine::OpenFile() {
	m_pFile = static_cast<CHttpFile*>(m_pSession->OpenURL(m_strInquiry, 1,
	                                                      INTERNET_FLAG_TRANSFER_ASCII,
	                                                      m_strHeaders, m_strHeaders.GetLength()));
}

void CInquireEngine::GetFileHeaderInformation() {
	ASSERT(m_pFile != nullptr);
	m_pFile->QueryInfoStatusCode(m_dwHTTPStatusCode);
	QueryDataLength();
}

void CInquireEngine::DeleteWebFile() {
	if (m_pFile != nullptr) {
		m_pFile->Close();
		delete m_pFile;
		m_pFile = nullptr;
	}
}

void CInquireEngine::QueryDataLength() {
	CString str;
	m_pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, str);
	if (str.GetLength() > 0) { // ����ʱ���ַ�����Ϊ��
		char* p;
		m_lContentLength = strtol(str.GetBuffer(), &p, 10);
		ASSERT(m_lContentLength > 0);
	}
	else { // ����������Ӧ
		m_lContentLength = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// ÿ�ζ�ȡ16K���ݣ�Ȼ�󽫶�ȡ�������ݴ��뻺����
// �˺����ĺ�ʱ������Read�������ʶ��޷��ӿ�ִ���ٶ��ˡ�
//
////////////////////////////////////////////////////////////////////////////////////////////
UINT CInquireEngine::ReadWebFileOneTime() {
	return m_pFile->Read(m_dataBuffer, WEB_SOURCE_DATA_BUFFER_SIZE_);
}

//
// Debug����ģʽ�£�ʹ��memcpy������ɣ���ʱ154����
// release����ģʽ�£�ʹ��memcpy������ɣ���ʱ120���롣
//
void CInquireEngine::XferReadingToBuffer(UINT uByteRead) {
	ASSERT(m_sBuffer.size() > m_lByteRead + uByteRead);
	memcpy(&m_sBuffer.at(m_lByteRead), m_dataBuffer, uByteRead);
}

bool CInquireEngine::IncreaseBufferSizeIfNeeded(long lIncreaseSize) {
	if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) { // ���ݿɴ洢�ռ䲻��128Kʱ
		m_sBuffer.resize(m_sBuffer.size() + lIncreaseSize); // ����lSize��Ĭ��Ϊ1M�����ݷ�Χ
		return true;
	}
	return false;
}

CWebDataPtr CInquireEngine::CreateWebData() {
	const auto pWebData = make_shared<CWebData>();
	pWebData->SetTime(gl_tpNow);
	TransferDataToWebData(pWebData); // �����յ�������ת����pWebData�С�����ʹ��std::move���ӿ��ٶȣ�Դ���ݲ����ٱ�ʹ�á�

	return pWebData;
}

void CInquireEngine::VerifyDataLength() const {
	if (m_lContentLength > 0) {
		if (m_lContentLength != m_lByteRead) {
			CString str = _T("�������ݳ��Ȳ�����Ԥ�ڳ��ȣ�");
			char buffer[100];
			sprintf_s(buffer, _T("%d"), m_lContentLength);
			str += buffer;
			str += _T("��ʵ�ʳ��ȣ�");
			sprintf_s(buffer, _T("%d"), m_lByteRead);
			str += buffer;
			str += m_strInquiry.Left(200);
			gl_systemMessage.PushErrorMessage(str);
			sprintf_s(buffer, _T("%d"), m_dwHTTPStatusCode);
			str = _T("Status code : ");
			str += buffer;
			str += m_sBuffer.c_str();
			str = str.Left(400);
			gl_systemMessage.PushErrorMessage(str);
		}
	}
}

void CInquireEngine::TransferDataToWebData(const CWebDataPtr& pWebData) {
	ASSERT(m_sBuffer.size() > m_lByteRead); // Note ��ʹ֪�������ܳ��ȣ�ҲҪ�����һ���ֽ��Է�ֹԽ�磬��string�����һ�����ص��ַ�0x000
	m_sBuffer.resize(m_lByteRead); //Note ��������СΪʵ���������������������ַ�0x000. �м�
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // ʹ��std::move�Լ���ִ���ٶ�
}

void CInquireEngine::TESTSetBuffer(const char* buffer, const INT64 lTotalNumber) {
	m_sBuffer.resize(lTotalNumber + 1);
	for (INT64 i = 0; i < lTotalNumber; i++) {
		m_sBuffer.at(i) = buffer[i];
	}
	m_sBuffer.at(lTotalNumber) = 0x000;
	m_lByteRead = lTotalNumber;
}

void CInquireEngine::TESTSetBuffer(CString str) {
	const INT64 lTotalNumber = str.GetLength();
	const char* buffer = str.GetBuffer();

	m_sBuffer.resize(lTotalNumber + 1);
	for (INT64 i = 0; i < lTotalNumber; i++) {
		m_sBuffer.at(i) = buffer[i];
	}
	m_sBuffer.at(lTotalNumber) = 0x000;
	m_lByteRead = lTotalNumber;
}

void CInquireEngine::TESTSetWebBuffer(const char* buffer, const INT64 lTotalNumber) {
	for (INT64 i = 0; i < lTotalNumber; i++) {
		m_dataBuffer[i] = buffer[i];
	}
}
