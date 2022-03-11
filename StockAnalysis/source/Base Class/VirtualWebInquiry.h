////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读取网络数据的基类。
// 由于网络数据使用特定的字符集（一般皆为多字节字符集），故而此基类也应使用固定的字符集，以便于采集数据。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"afxinet.h"
#include"WebData.h"

using namespace std;
#include<atomic>
#include<string>

class CVirtualWebInquiry : public CObject {
public:
	CVirtualWebInquiry();
	virtual ~CVirtualWebInquiry(void);

	virtual bool OpenFile(CString strInquiring);
	virtual bool ReadWebData(void); // 网络实际读取函数
	virtual UINT ReadWebFileOneTime(void); // 无法测试，故而虚拟化后使用Mock类。

	virtual bool TransferData(CWebDataPtr pWebData); // 将接收到的数移至pWebData中
	virtual bool ParseData(CWebDataPtr pWebData); // 解析接收到的数据， 默认数据格式为JSon，解析之。
	virtual bool ProcessData(CWebDataPtr pWebData) { return false; } // 处理接收到的数据。 默认不处理。
	void ResetBuffer(void) { m_sBuffer.resize(1024 * 1024); }

	// 唯一的公共接口函数
	virtual bool GetWebData(void);

	void Reset(void) noexcept;
	virtual bool ReportStatus(long lNumberOfData) const;

	// 下列为继承类必须实现的几个功能函数，完成具体任务。不允许调用本基类函数
	// 由于测试的原因，此处保留了函数定义，没有将其声明为=0.
	virtual bool PrepareNextInquiringStr(void) { ASSERT(0); return true; }
	virtual CString GetNextInquiringMiddleStr(long, bool) { ASSERT(0); return _T(""); } // 申请下一个查询用字符串
	virtual void PrepareReadingWebData(void) { /* do nothing in default*/ } // 在读取网络数据前的准备工作，默认无动作。
	virtual void StartReadingThread(void); // 调用网络读取线程。为了Mock方便，声明为虚函数。
	virtual void UpdateStatusWhenSecceed(CWebDataPtr pData) { /* do nothing in default*/ } // 成功接收后更新系统状态。 默认无动作
	virtual void ClearUpIfReadingWebDataFailed(void) { /* do nothing in default*/ } // 处理失败的接收过程
	virtual void UpdateAfterReadingWebData(void) { /*default do nothing*/ } // 接收数据后更新系统状态。默认为不做任何事情。
	virtual void SetTime(CWebDataPtr pData);
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) { ASSERT(0); } // 将网络上读取到的数据存入本地

	// 以下为实现函数
	void CreateTotalInquiringString(CString strMIddle);
	CString GetInquiringString(void) const { return m_strInquire; }
	void SetInquiringString(CString str) { m_strInquire = str; }
	void AppendInquiringString(CString str) { m_strInquire += str; }

	CString GetHeaders(void) { return m_strHeaders; }
	void SetHeaders(CString strHeaders) { m_strHeaders = strHeaders; }

	char GetData(long lIndex) const { return m_sBuffer.at(lIndex); }
	void SetData(long lIndex, char value) { m_sBuffer.at(lIndex) = value; }

	INT64 GetByteReaded(void) const noexcept { return m_lByteRead; }
	void SetByteReaded(INT64 lValue) noexcept { m_lByteRead = lValue; }
	void AddByteReaded(INT64 lValue) noexcept { m_lByteRead += lValue; }
	size_t GetBufferSize(void) noexcept { return m_sBuffer.size(); }

	CString GetInquiringStringPrefix(void) const { return m_strWebDataInquirePrefix; }
	void SetInquiryingStringPrefix(CString strPrefix) { m_strWebDataInquirePrefix = strPrefix; }
	CString GetInquiringStringSuffix(void) const { return m_strWebDataInquireSuffix; }
	void SetInquiryingStringSuffix(CString strSuffix) { m_strWebDataInquireSuffix = strSuffix; }
	CString GetInquiringStringMiddle(void) const { return m_strWebDataInquireMiddle; }
	void SetInquiryingStringMiddle(CString strSuffix) { m_strWebDataInquireMiddle = strSuffix; }

	bool IsReadingWebData(void) const noexcept { return m_fReadingWebData; }
	void SetReadingWebData(bool fFlag) noexcept { m_fReadingWebData = fFlag; }

	bool IsWebError(void) const noexcept { return m_fWebError; }
	void SetWebError(bool fFlag) noexcept { m_fWebError = fFlag; }

	bool IsReportStatus(void) const noexcept { return m_fReportStatus; }

	CString GetConnectionName(void) const { return m_strConnectionName; }

	long GetShortestInquiringInterval(void) noexcept { return m_lShortestInquiringInterval; }
	void SetShortestINquiringInterval(long lValue) noexcept { m_lShortestInquiringInterval = lValue; }

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
	CInternetSession* m_pSession;
	CHttpFile* m_pFile; // 网络文件指针
	CString m_strHeaders; // OpenURL时的headers字符串值， 默认为_T("")
	DWORD m_dwWebErrorCode; //网络读取错误代码
	CString m_strInquire;// 查询所需的字符串
	string m_sBuffer; // 接收到数据的缓冲区
	INT64 m_lByteRead; // 接收到的字符数.

	CString m_strWebDataInquireMiddle; // 查询字符串中间字段
	CString m_strWebDataInquirePrefix; // 查询字符串前缀
	CString m_strWebDataInquireSuffix; // 查询字符串后缀

	atomic_bool m_fReadingWebData; // 接收实时数据线程是否执行标识
	atomic_bool m_fWebError; // 读取网络数据时是否出现错误标识

	bool m_fReportStatus; //
	bool m_fFSonContentType; // 数据格式是否为JSon
	long m_lContentLength; // 数据长度

	long m_lShortestInquiringInterval; // 允许的最短查询间隔. 单位为毫秒。默认为1秒每次， 即1000.
	INT64 m_lInquiringNumber; // 每次查询数量
	time_t m_tCurrentInquiryTime; // 当前接收数据所需时间（以毫秒计）

	CString m_strConnectionName; // 此网络读取器的名称
	static atomic_llong m_lTotalByteReaded; // 当前网络读取字节数。所有的网络读取器都修改此变量，故而声明为静态。
};

typedef shared_ptr<CVirtualWebInquiry> CVirtualWebInquiryPtr;