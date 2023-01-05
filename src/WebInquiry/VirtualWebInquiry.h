////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读取网络数据的基类。
// 由于网络数据使用特定的字符集（一般皆为多字节字符集），故而此基类也应使用固定的字符集，以便于采集数据。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"afxinet.h"
#include"WebData.h"

#include"ClassDeclaration.h"

#include<atomic>
#include<string>
using std::atomic_bool;
using std::atomic_long;
using std::string;

class CVirtualWebInquiry {
public:
	CVirtualWebInquiry();
	// 只能有一个实例,不允许赋值。
	CVirtualWebInquiry(const CVirtualWebInquiry&) = delete;
	CVirtualWebInquiry& operator=(const CVirtualWebInquiry&) = delete;
	CVirtualWebInquiry(const CVirtualWebInquiry&&) noexcept = delete;
	CVirtualWebInquiry& operator=(const CVirtualWebInquiry&&) noexcept = delete;
	virtual ~CVirtualWebInquiry(void);

	void SetDefaultSessionOption(void);

	virtual bool ReadingWebData(void); // 网络实际读取函数
	void Read2();
	bool ReadingWebData2();
	virtual bool OpenFile(const CString& strInquiring);
	void DeleteWebFile();
	long QueryDataLength();
	virtual UINT ReadWebFileOneTime(void); // 无法测试，故而虚拟化后使用Mock类。
	bool IncreaseBufferSizeIfNeeded(long lIncreaseSize = 1024 * 1024);

	bool VerifyDataLength() const;
	virtual bool TransferDataToWebData(CWebDataPtr pWebData); // 将接收到的数移至pWebData中
	virtual bool ParseData(CWebDataPtr pWebData) {
		TRACE("调用了基类函数\n");
		return false;
	} //解析接收到的数据。继承类必须实现此函数。
	void ResetBuffer(void) { m_sBuffer.resize(DefaultWebDataBufferSize_); }

	// 唯一的公共接口函数
	virtual bool GetWebData(void);
	virtual bool GetWebData2(); // 采用libcurl的用于对比;

	void Read(void); // 实际读取处理函数，完成工作线程的实际功能

	void Reset(void) noexcept;
	virtual bool ReportStatus(long lNumberOfData) const;

	// 下列为继承类必须实现的几个功能函数，完成具体任务。不允许调用本基类函数
	// 由于测试的原因，此处保留了函数定义，没有将其声明为=0.
	virtual bool PrepareNextInquiringString(void) {
		ASSERT(0);
		return true;
	}

	virtual CString GetNextInquiringMiddleString(long, bool) {
		ASSERT(0);
		return _T("");
	} //申请下一个查询用字符串
	virtual void PrepareReadingWebData(void); // 在读取网络数据前的准备工作，默认为设置m_pSession状态。
	virtual void ConfigureSession(void) {
		TRACE("调用了基类函数ConfigureSession\n");
	} // 配置m_pSession。继承类必须实现此功能，每个网站的状态都不一样，故而需要单独配置。
	virtual void StartReadingThread(void); // 调用网络读取线程。为了Mock方便，声明为虚函数。
	virtual void UpdateStatusAfterSucceed(CWebDataPtr pData) { } //成功接收后更新系统状态。默认无动作

	void CreateTotalInquiringString(CString strMiddle);
	CString GetInquiringString(void) const noexcept { return m_strInquiry; }
	void SetInquiringString(const CString& str) noexcept { m_strInquiry = str; }
	void AppendInquiringString(const CString& str) noexcept { m_strInquiry += str; }

	void SetDataSource(CVirtualDataSource* pDataSource) { m_pDataSource = pDataSource; }

	CString GetHeaders(void) const noexcept { return m_strHeaders; }
	void SetHeaders(const CString& strHeaders) noexcept { m_strHeaders = strHeaders; }

	char GetData(const long lIndex) const { return m_sBuffer.at(lIndex); }
	void SetData(const long lIndex, const char value) { m_sBuffer.at(lIndex) = value; }

	long GetByteRead(void) const noexcept { return m_lByteRead; }
	void SetByteRead(const long lValue) noexcept { m_lByteRead = lValue; }
	void AddByteRead(const long lValue) noexcept { m_lByteRead += lValue; }
	size_t GetBufferSize(void) const noexcept { return m_sBuffer.size(); }

	CString GetInquiryFunction(void) const noexcept { return m_strInquiryFunction; }
	void SetInquiryFunction(const CString& strPrefix) noexcept { m_strInquiryFunction = strPrefix; }
	CString GetInquiryToken(void) const noexcept { return m_strInquiryToken; }
	void SetInquiryToken(const CString& strToken) noexcept { m_strInquiryToken = strToken; }

	bool IsReadingWebData(void) const noexcept { return m_fReadingWebData; }
	void SetReadingWebData(const bool fFlag) noexcept { m_fReadingWebData = fFlag; }

	bool IsWebError(void) const noexcept {
		if (m_dwWebErrorCode == 0) return false;
		else return true;
	}

	DWORD GetErrorCode(void) const noexcept { return m_dwWebErrorCode; }
	void SetErrorCode(const DWORD dwErrorCode) noexcept { m_dwWebErrorCode = dwErrorCode; }

	bool IsTimeout(void) const noexcept {
		if (m_dwWebErrorCode == 12002) return true;
		else return false;
	}

	bool IsReportStatus(void) const noexcept { return m_fReportStatus; }

	CString GetConnectionName(void) const { return m_strConnectionName; }

	long GetInquiringNumber(void) const noexcept { return m_lInquiringNumber; }
	void SetInquiringNumber(const long lValue) noexcept { m_lInquiringNumber = lValue; }

	static long GetTotalByteRead(void) noexcept { return sm_lTotalByteRead; }
	static void SetTotalByteRead(const long lValue = 0) noexcept { sm_lTotalByteRead = lValue; }
	static void ClearTotalByteRead(void) noexcept { sm_lTotalByteRead = 0; }

	void SetCurrentInquiryTime(const time_t tt) noexcept { m_tCurrentInquiryTime = tt; }
	time_t GetCurrentInquiryTime(void) const noexcept { return m_tCurrentInquiryTime; }

public:
	// 以下为测试用函数
	void TESTSetBuffer(char* buffer, INT64 lTotalNumber);
	void TESTSetBuffer(CString str);

protected:
	CVirtualDataSource* m_pDataSource; // 指向包含自己的数据源。此指针必须使用裸指针，不能使用智能指针，否则解析时会出现循环，导致不可知的结果。

	CInternetSession* m_pSession;
	CHttpFile* m_pFile; // 网络文件指针
	long m_lHTTPStatusCode; //网络状态码

	CString m_strHeaders; // OpenURL时的headers字符串值， 默认为_T("")
	DWORD m_dwWebErrorCode; //网络读取错误代码。也用于网络错误判断的依据：当为零时无错误。
	string m_sBuffer; // 接收到数据的缓冲区
	long m_lByteRead; // 接收到的字符数.
	CWebDataPtr m_pWebData;

	CString m_strInquiry; // 查询所需的字符串（m_strInquiryFunction + m_strInquiryToken).
	CString m_strInquiryFunction; // 查询字符串功能部分
	CString m_strInquiryToken; // 查询字符串令牌

	atomic_bool m_fReadingWebData; // 接收实时数据线程是否执行标识

	bool m_fReportStatus; //
	long m_lContentLength; // 数据长度

	long m_lInquiringNumber; // 每次查询数量
	time_t m_tCurrentInquiryTime; // 当前接收数据所需时间（以毫秒计）

	CString m_strConnectionName; // 此网络读取器的名称
	static atomic_long sm_lTotalByteRead; // 当前网络读取字节数。所有的网络读取器都修改此变量，故而声明为静态。
};

using CVirtualWebInquiryPtr = shared_ptr<CVirtualWebInquiry>;
