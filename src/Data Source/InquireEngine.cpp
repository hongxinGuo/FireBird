/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// todo 准备将DataSource改为可以一次处理多个申请的并行模式。有些数据申请需要拆分成多个（如腾讯日线单次申请最多提供2000个数据，多于2000个的申请必须拆分成多次）。
// 
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "InquireEngine.h"

#include <simdjson.h>

#include"Thread.h"

#include "InfoReport.h"

using std::thread;

CInquireEngine::CInquireEngine(): m_dataBuffer{} {
	m_pSession = make_shared<CInternetSession>(_T("FireBird")); // 此处需要加上调用程序的名称，否则无法运行单元测试程序（原因不明）。
	m_pFile = nullptr;

	m_dwHTTPStatusCode = 0;
	m_sBuffer = _T("");
	m_lByteRead = 0;
	m_dwWebErrorCode = 0;
	m_strInquiry = _T("");
	m_strHeaders = _T("");

	m_tCurrentInquiryTime = 0;

	m_lContentLength = 0;

	m_llLastTimeTickCount = 0;
}

CInquireEngine::CInquireEngine(const CString& strInquire, const CString& strHeaders): m_dataBuffer{} {
	m_pSession = make_shared<CInternetSession>(_T("FireBird")); // 此处需要加上调用程序的名称，否则无法运行单元测试程序（原因不明）。
	m_pFile = nullptr;

	m_dwHTTPStatusCode = 0;
	m_sBuffer = _T("");
	m_lByteRead = 0;
	m_dwWebErrorCode = 0;
	m_strInquiry = strInquire;
	m_strHeaders = strHeaders;

	m_tCurrentInquiryTime = 0;

	m_lContentLength = 0;

	m_llLastTimeTickCount = 0;
}

CWebDataPtr CInquireEngine::GetWebData() {
	ReadWebData();
	if (!IsWebError()) {
		VerifyDataLength();
		return CreateWebData();
	}
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////
//
// 从网络读取数据。每次读16KB，直到读不到为止。
//
//
//
///////////////////////////////////////////////////////////////////////////
void CInquireEngine::ReadWebData() {
	ASSERT(m_pFile == nullptr);
	try {
		OpenFile();
		GetFileHeaderInformation();
		if (m_lContentLength > 0) {
			m_sBuffer.resize(m_lContentLength + 1); // 调整缓存区大小，比实际数据大1字节,以防止越界访问。
		}
		else {
			m_sBuffer.resize(1024 * 1024);// 服务器不回报数据长度时，设置初始缓冲区为1M。
		}
		UINT lCurrentByteRead;
		do {
			if (gl_systemConfiguration.IsExitingSystem()) {
				CInternetException* e = new CInternetException(1); // 不为零即可。使用五位数以下的数据，避开系统预先声明的范围
				throw(e); // 当系统退出时，要立即中断此进程，以防止内存泄露。
			}
			lCurrentByteRead = ReadWebFileOneTime(); // 每次读取16K数据。
			XferReadingToBuffer(lCurrentByteRead);
			m_lByteRead += lCurrentByteRead;
			if (m_lContentLength == 0) IncreaseBufferSizeIfNeeded(1024 * 1024);
		} while (lCurrentByteRead > 0);
		// 清除网络错误代码的动作，只在此处进行。以保证只有当顺利读取到网络数据后，方才清除之前的错误标识。
		m_dwWebErrorCode = 0; // 清除错误代码（如果有的话）。只在此处重置该错误代码。
	}
	catch (CInternetException* exception) {//这里一般是使用引用。但我准备在处理完后就删除这个例外，故而直接使用指针。否则由于系统不处理此例外，会导致程序自动退出。
		m_dwWebErrorCode = exception->m_dwError;
		ReportWebError(m_dwWebErrorCode, m_strInquiry);
		exception->Delete();
	}
	DeleteWebFile();
}

// <summary>
//
// 由于新浪实时数据服务器需要提供头部验证数据，故而OpenURL不再使用默认值，调用者需要各自设置m_strHeaders（默认为空）。
// 其他的数据尚未需要提供头部验证数据。
// 调用函数需要处理exception。
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
	if (str.GetLength() > 0) { // 正常时此字符串不为零
		char* p;
		m_lContentLength = strtol(str.GetBuffer(), &p, 10);
		ASSERT(m_lContentLength > 0);
	}
	else { // 服务器无响应
		m_lContentLength = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// 每次读取16K数据，然后将读取到的数据存入缓冲区
// 此函数的耗时，皆在Read函数，故而无法加快执行速度了。
//
////////////////////////////////////////////////////////////////////////////////////////////
UINT CInquireEngine::ReadWebFileOneTime() {
	return m_pFile->Read(m_dataBuffer, 1024 * 16);
}

//
// Debug编译模式下，使用memcpy函数完成，耗时154纳秒
// release编译模式下：使用逐字节拷贝，16KB耗时11微秒，使用memcpy函数完成，耗时120纳秒。
//
void CInquireEngine::XferReadingToBuffer(UINT uByteRead) {
	ASSERT(m_sBuffer.size() >= m_lByteRead + uByteRead);
	memcpy(&m_sBuffer.at(m_lByteRead), m_dataBuffer, uByteRead);
}

bool CInquireEngine::IncreaseBufferSizeIfNeeded(long lIncreaseSize) {
	if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) { // 数据可存储空间不到128K时
		m_sBuffer.resize(m_sBuffer.size() + lIncreaseSize); // 扩大lSize（默认为1M）数据范围
	}
	return true;
}

CWebDataPtr CInquireEngine::CreateWebData() {
	const auto pWebData = make_shared<CWebData>();
	pWebData->ResetCurrentPos();
	pWebData->SetTime(GetUTCTime());
	TransferDataToWebData(pWebData); // 将接收到的数据转移至pWebData中。由于使用std::move来加快速度，源数据不能再被使用。

	return pWebData;
}

void CInquireEngine::VerifyDataLength() const {
	if (m_lContentLength > 0) {
		if (m_lContentLength != m_lByteRead) {
			CString str = _T("网络数据长度不符。预期长度：");
			char buffer[100];
			sprintf_s(buffer, _T("%d"), m_lContentLength);
			str += buffer;
			str += _T("，实际长度：");
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
	ASSERT(m_sBuffer.size() > m_lByteRead); // Note 即使知道数据总长度，也要多加上一个字节以防止越界。
	m_sBuffer.resize(m_lByteRead); // Note 缓冲区大小为实际数据量
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // 使用std::move以加速执行速度
}

void CInquireEngine::TESTSetBuffer(const char* buffer, const INT64 lTotalNumber) {
	m_sBuffer.resize(lTotalNumber);
	for (INT64 i = 0; i < lTotalNumber; i++) { m_sBuffer.at(i) = buffer[i]; }
	m_lByteRead = lTotalNumber;
}

void CInquireEngine::TESTSetBuffer(CString str) {
	const INT64 lTotalNumber = str.GetLength();
	const char* buffer = str.GetBuffer();

	m_sBuffer.resize(lTotalNumber);
	for (INT64 i = 0; i < lTotalNumber; i++) { m_sBuffer.at(i) = buffer[i]; }
	m_lByteRead = lTotalNumber;
}

void CInquireEngine::TESTSetWebBuffer(const char* buffer, const INT64 lTotalNumber) {
	for (INT64 i = 0; i < lTotalNumber; i++) { m_dataBuffer[i] = buffer[i]; }
}
