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

using namespace std;
#include<atomic>
#include<string>

class CVirtualWebInquiry : public CObject {
public:
	CVirtualWebInquiry();
	virtual ~CVirtualWebInquiry(void);

	void SetDefaultSessionOption(void);

	virtual bool ReadingWebData(void); // 网络实际读取函数
	virtual bool OpenFile(CString strInquiring);
	void DeleteWebFile();
	long QueryDataLength();
	virtual UINT ReadWebFileOneTime(void); // 无法测试，故而虚拟化后使用Mock类。
	bool IncreaseBufferSizeIfNeeded(long lSize = 1024 * 1024);

	bool VerifyDataLength();
	virtual bool TransferDataToWebData(CWebDataPtr pWebData); // 将接收到的数移至pWebData中
	virtual bool ParseData(CWebDataPtr pWebData) { TRACE("调用了基类函数\n"); return false; }; // 解析接收到的数据。继承类必须实现此函数。
	void ResetBuffer(void) { m_sBuffer.resize(__DefaultWebDataBufferSize__); }

	// 唯一的公共接口函数
	virtual bool GetWebData(void);

	void Read(void); // 实际读取处理函数，完成工作线程的实际功能

	void Reset(void) noexcept;
	virtual bool ReportStatus(long lNumberOfData) const;

	// 下列为继承类必须实现的几个功能函数，完成具体任务。不允许调用本基类函数
	// 由于测试的原因，此处保留了函数定义，没有将其声明为=0.
	virtual bool PrepareNextInquiringString(void) { ASSERT(0); return true; }
	virtual CString GetNextInquiringMiddleString(long, bool) { ASSERT(0); return _T(""); } // 申请下一个查询用字符串
	virtual void PrepareReadingWebData(void);// 在读取网络数据前的准备工作，默认为设置m_pSession状态。
	virtual void ConfigerateSession(void) { TRACE("调用了基类函数ConfigerateSession\n"); }// 配置m_pSession。继承类必须实现此功能，每个网站的状态都不一样，故而需要单独配置。
	virtual void StartReadingThread(void); // 调用网络读取线程。为了Mock方便，声明为虚函数。
	virtual void UpdateStatusWhenSecceed(CWebDataPtr pData); // 成功接收后更新系统状态。

	void CreateTotalInquiringString(CString strMIddle);
	CString GetInquiringString(void) const { return m_strInquiry; }
	void SetInquiringString(CString str) { m_strInquiry = str; }
	void AppendInquiringString(CString str) { m_strInquiry += str; }

	void SetDataSource(CVirtualDataSource* pDataSource) { m_pDataSource = pDataSource; }

	CString GetHeaders(void) { return m_strHeaders; }
	void SetHeaders(CString strHeaders) { m_strHeaders = strHeaders; }

	char GetData(long lIndex) const { return m_sBuffer.at(lIndex); }
	void SetData(long lIndex, char value) { m_sBuffer.at(lIndex) = value; }

	INT64 GetByteReaded(void) const noexcept { return m_lByteRead; }
	void SetByteReaded(INT64 lValue) noexcept { m_lByteRead = lValue; }
	void AddByteReaded(INT64 lValue) noexcept { m_lByteRead += lValue; }
	size_t GetBufferSize(void) noexcept { return m_sBuffer.size(); }

	CString GetInquiryFunction(void) const { return m_strInquiryFunction; }
	void SetInquiryFunction(CString strPrefix) { m_strInquiryFunction = strPrefix; }
	CString GetInquiryToken(void) const { return m_strInquiryToken; }
	void SetInquiryToken(CString strToken) { m_strInquiryToken = strToken; }

	bool IsReadingWebData(void) const noexcept { return m_fReadingWebData; }
	void SetReadingWebData(bool fFlag) noexcept { m_fReadingWebData = fFlag; }

	bool IsWebError(void) const noexcept { if (m_dwWebErrorCode == 0) return false; else return true; }
	void ClearWebError(void) noexcept { m_dwWebErrorCode = 0; }
	DWORD GetErrorCode(void) noexcept { return m_dwWebErrorCode; }

	bool IsReportStatus(void) const noexcept { return m_fReportStatus; }

	CString GetConnectionName(void) const { return m_strConnectionName; }

	INT64 GetInquiringNumber(void) const noexcept { return m_lInquiringNumber; }
	void SetInquiringNumber(INT64 lValue) noexcept { m_lInquiringNumber = lValue; }

	INT64 GetTotalByteReaded(void) const noexcept { return m_lTotalByteReaded; }
	void SetTotalByteReaded(INT64 lValue = 0) noexcept { m_lTotalByteReaded = lValue; }
	void ClearTotalByteReaded(void) noexcept { m_lTotalByteReaded = 0; }

	void SetCurrentInquiryTime(time_t tt) noexcept { m_tCurrentInquiryTime = tt; }
	time_t GetCurrentInquiryTime(void) const noexcept { return m_tCurrentInquiryTime; }

public:
	// 以下为测试用函数
	void __TESTSetBuffer(char* buffer, INT64 lTotalNumber);
	void __TESTSetBuffer(CString str);

protected:
	CVirtualDataSource* m_pDataSource; // 指向包含自己的数据源。此指针必须使用裸指针，不能使用智能指针，否则解析时会出现循环，导致不可知的结果。

	CInternetSession* m_pSession;
	CHttpFile* m_pFile; // 网络文件指针
	CString m_strHeaders; // OpenURL时的headers字符串值， 默认为_T("")
	DWORD m_dwWebErrorCode; //网络读取错误代码。也用于网络错误判断的依据：当为零时无错误。
	string m_sBuffer; // 接收到数据的缓冲区
	INT64 m_lByteRead; // 接收到的字符数.

	CString m_strInquiry;// 查询所需的字符串（m_strInquiryFunction + m_strInquiryToken).
	CString m_strInquiryFunction; // 查询字符串功能部分
	CString m_strInquiryToken; // 查询字符串令牌

	atomic_bool m_fReadingWebData; // 接收实时数据线程是否执行标识
	atomic_bool m_fWebError; // 读取网络数据时是否出现错误标识

	bool m_fReportStatus; //
	bool m_fFSonContentType; // 数据格式是否为JSon
	long m_lContentLength; // 数据长度

	INT64 m_lInquiringNumber; // 每次查询数量
	time_t m_tCurrentInquiryTime; // 当前接收数据所需时间（以毫秒计）

	CString m_strConnectionName; // 此网络读取器的名称
	static atomic_llong m_lTotalByteReaded; // 当前网络读取字节数。所有的网络读取器都修改此变量，故而声明为静态。
};

typedef shared_ptr<CVirtualWebInquiry> CVirtualWebInquiryPtr;