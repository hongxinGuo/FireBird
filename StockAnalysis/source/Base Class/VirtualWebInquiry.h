////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ȡ�������ݵĻ��ࡣ
// ������������ʹ���ض����ַ�����һ���Ϊ���ֽ��ַ��������ʶ��˻���ҲӦʹ�ù̶����ַ������Ա��ڲɼ����ݡ�
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
	virtual bool ReadWebData(void); // ����ʵ�ʶ�ȡ����
	virtual UINT ReadWebFileOneTime(void); // �޷����ԣ��ʶ����⻯��ʹ��Mock�ࡣ

	virtual bool TransferData(CWebDataPtr pWebData); // �����յ���������pWebData��
	virtual bool ParseData(CWebDataPtr pWebData); // �������յ������ݣ� Ĭ�����ݸ�ʽΪJSon������֮��
	virtual bool ProcessData(CWebDataPtr pWebData) { return false; } // ������յ������ݡ� Ĭ�ϲ�����
	void ResetBuffer(void) { m_sBuffer.resize(1024 * 1024); }

	// Ψһ�Ĺ����ӿں���
	virtual bool GetWebData(void);

	void Reset(void) noexcept;
	virtual bool ReportStatus(long lNumberOfData) const;

	// ����Ϊ�̳������ʵ�ֵļ������ܺ�������ɾ������񡣲�������ñ����ຯ��
	// ���ڲ��Ե�ԭ�򣬴˴������˺������壬û�н�������Ϊ=0.
	virtual bool PrepareNextInquiringStr(void) { ASSERT(0); return true; }
	virtual CString GetNextInquiringMiddleStr(long, bool) { ASSERT(0); return _T(""); } // ������һ����ѯ���ַ���
	virtual void PrepareReadingWebData(void) { /* do nothing in default*/ } // �ڶ�ȡ��������ǰ��׼��������Ĭ���޶�����
	virtual void StartReadingThread(void); // ���������ȡ�̡߳�Ϊ��Mock���㣬����Ϊ�麯����
	virtual void UpdateStatusWhenSecceed(CWebDataPtr pData) { /* do nothing in default*/ } // �ɹ����պ����ϵͳ״̬�� Ĭ���޶���
	virtual void ClearUpIfReadingWebDataFailed(void) { /* do nothing in default*/ } // ����ʧ�ܵĽ��չ���
	virtual void UpdateAfterReadingWebData(void) { /*default do nothing*/ } // �������ݺ����ϵͳ״̬��Ĭ��Ϊ�����κ����顣
	virtual void SetTime(CWebDataPtr pData);
	virtual void StoreWebData(CWebDataPtr pWebDataBeStored) { ASSERT(0); } // �������϶�ȡ�������ݴ��뱾��

	// ����Ϊʵ�ֺ���
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
	// ����Ϊ�����ú���
	void __TESTSetBuffer(char* buffer, INT64 lTotalNumber);
	void __TESTSetBuffer(CString str);

protected:
	CInternetSession* m_pSession;
	CHttpFile* m_pFile; // �����ļ�ָ��
	CString m_strHeaders; // OpenURLʱ��headers�ַ���ֵ�� Ĭ��Ϊ_T("")
	DWORD m_dwWebErrorCode; //�����ȡ�������
	CString m_strInquire;// ��ѯ������ַ���
	string m_sBuffer; // ���յ����ݵĻ�����
	INT64 m_lByteRead; // ���յ����ַ���.

	CString m_strWebDataInquireMiddle; // ��ѯ�ַ����м��ֶ�
	CString m_strWebDataInquirePrefix; // ��ѯ�ַ���ǰ׺
	CString m_strWebDataInquireSuffix; // ��ѯ�ַ�����׺

	atomic_bool m_fReadingWebData; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
	atomic_bool m_fWebError; // ��ȡ��������ʱ�Ƿ���ִ����ʶ

	bool m_fReportStatus; //
	bool m_fFSonContentType; // ���ݸ�ʽ�Ƿ�ΪJSon
	long m_lContentLength; // ���ݳ���

	long m_lShortestInquiringInterval; // �������̲�ѯ���. ��λΪ���롣Ĭ��Ϊ1��ÿ�Σ� ��1000.
	INT64 m_lInquiringNumber; // ÿ�β�ѯ����
	time_t m_tCurrentInquiryTime; // ��ǰ������������ʱ�䣨�Ժ���ƣ�

	CString m_strConnectionName; // �������ȡ��������
	static atomic_llong m_lTotalByteReaded; // ��ǰ�����ȡ�ֽ��������е������ȡ�����޸Ĵ˱������ʶ�����Ϊ��̬��
};

typedef shared_ptr<CVirtualWebInquiry> CVirtualWebInquiryPtr;