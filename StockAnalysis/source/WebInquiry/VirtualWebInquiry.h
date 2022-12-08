////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ȡ�������ݵĻ��ࡣ
// ������������ʹ���ض����ַ�����һ���Ϊ���ֽ��ַ��������ʶ��˻���ҲӦʹ�ù̶����ַ������Ա��ڲɼ����ݡ�
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

	virtual bool ReadingWebData(void); // ����ʵ�ʶ�ȡ����
	virtual bool OpenFile(CString strInquiring);
	void DeleteWebFile();
	long QueryDataLength();
	virtual UINT ReadWebFileOneTime(void); // �޷����ԣ��ʶ����⻯��ʹ��Mock�ࡣ
	bool IncreaseBufferSizeIfNeeded(long lIncreaseSize = 1024 * 1024);

	bool VerifyDataLength();
	virtual bool TransferDataToWebData(CWebDataPtr pWebData); // �����յ���������pWebData��
	virtual bool ParseData(CWebDataPtr pWebData) { TRACE("�����˻��ຯ��\n"); return false; }; // �������յ������ݡ��̳������ʵ�ִ˺�����
	void ResetBuffer(void) { m_sBuffer.resize(__DefaultWebDataBufferSize__); }

	// Ψһ�Ĺ����ӿں���
	virtual bool GetWebData(void);

	void Read(void); // ʵ�ʶ�ȡ����������ɹ����̵߳�ʵ�ʹ���

	void Reset(void) noexcept;
	virtual bool ReportStatus(long lNumberOfData) const;

	// ����Ϊ�̳������ʵ�ֵļ������ܺ�������ɾ������񡣲�������ñ����ຯ��
	// ���ڲ��Ե�ԭ�򣬴˴������˺������壬û�н�������Ϊ=0.
	virtual bool PrepareNextInquiringString(void) { ASSERT(0); return true; }
	virtual CString GetNextInquiringMiddleString(long, bool) { ASSERT(0); return _T(""); } // ������һ����ѯ���ַ���
	virtual void PrepareReadingWebData(void);// �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����m_pSession״̬��
	virtual void ConfigerateSession(void) { TRACE("�����˻��ຯ��ConfigerateSession\n"); }// ����m_pSession���̳������ʵ�ִ˹��ܣ�ÿ����վ��״̬����һ�����ʶ���Ҫ�������á�
	virtual void StartReadingThread(void); // ���������ȡ�̡߳�Ϊ��Mock���㣬����Ϊ�麯����
	virtual void UpdateStatusAfterSucceed(CWebDataPtr pData) {} // �ɹ����պ����ϵͳ״̬��Ĭ���޶���

	void CreateTotalInquiringString(CString strMIddle);
	CString GetInquiringString(void) const noexcept { return m_strInquiry; }
	void SetInquiringString(CString const str) noexcept { m_strInquiry = str; }
	void AppendInquiringString(CString const str) noexcept { m_strInquiry += str; }

	void SetDataSource(CVirtualDataSource* pDataSource) { m_pDataSource = pDataSource; }

	CString GetHeaders(void) const noexcept { return m_strHeaders; }
	void SetHeaders(CString const strHeaders) noexcept { m_strHeaders = strHeaders; }

	char GetData(long lIndex) const { return m_sBuffer.at(lIndex); }
	void SetData(long lIndex, char value) { m_sBuffer.at(lIndex) = value; }

	long GetByteRead(void) const noexcept { return m_lByteRead; }
	void SetByteRead(long lValue) noexcept { m_lByteRead = lValue; }
	void AddByteRead(long lValue) noexcept { m_lByteRead += lValue; }
	size_t GetBufferSize(void) noexcept { return m_sBuffer.size(); }

	CString GetInquiryFunction(void) const noexcept { return m_strInquiryFunction; }
	void SetInquiryFunction(CString const strPrefix) noexcept { m_strInquiryFunction = strPrefix; }
	CString GetInquiryToken(void) const noexcept { return m_strInquiryToken; }
	void SetInquiryToken(CString const strToken) noexcept { m_strInquiryToken = strToken; }

	bool IsReadingWebData(void) const noexcept { return m_fReadingWebData; }
	void SetReadingWebData(bool fFlag) noexcept { m_fReadingWebData = fFlag; }

	bool IsWebError(void) const noexcept { if (m_dwWebErrorCode == 0) return false; else return true; }
	DWORD GetErrorCode(void) noexcept { return m_dwWebErrorCode; }
	void SetErrorCode(DWORD dwErrorCode) noexcept { m_dwWebErrorCode = dwErrorCode; }
	bool IsTimeout(void) noexcept { if (m_dwWebErrorCode == 12002) return true; else return false; }

	bool IsReportStatus(void) const noexcept { return m_fReportStatus; }

	CString GetConnectionName(void) const { return m_strConnectionName; }

	long GetInquiringNumber(void) const noexcept { return m_lInquiringNumber; }
	void SetInquiringNumber(long lValue) noexcept { m_lInquiringNumber = lValue; }

	static INT64 GetTotalByteRead(void) noexcept { return sm_lTotalByteRead; }
	static void SetTotalByteRead(INT64 lValue = 0) noexcept { sm_lTotalByteRead = lValue; }
	static void ClearTotalByteRead(void) noexcept { sm_lTotalByteRead = 0; }

	void SetCurrentInquiryTime(time_t tt) noexcept { m_tCurrentInquiryTime = tt; }
	time_t GetCurrentInquiryTime(void) const noexcept { return m_tCurrentInquiryTime; }

public:
	// ����Ϊ�����ú���
	void __TESTSetBuffer(char* buffer, INT64 lTotalNumber);
	void __TESTSetBuffer(CString str);

protected:
	CVirtualDataSource* m_pDataSource; // ָ������Լ�������Դ����ָ�����ʹ����ָ�룬����ʹ������ָ�룬�������ʱ�����ѭ�������²���֪�Ľ����

	CInternetSession* m_pSession;
	CHttpFile* m_pFile; // �����ļ�ָ��
	CString m_strHeaders; // OpenURLʱ��headers�ַ���ֵ�� Ĭ��Ϊ_T("")
	DWORD m_dwWebErrorCode; //�����ȡ������롣Ҳ������������жϵ����ݣ���Ϊ��ʱ�޴���
	string m_sBuffer; // ���յ����ݵĻ�����
	long m_lByteRead; // ���յ����ַ���.

	CString m_strInquiry;// ��ѯ������ַ�����m_strInquiryFunction + m_strInquiryToken).
	CString m_strInquiryFunction; // ��ѯ�ַ������ܲ���
	CString m_strInquiryToken; // ��ѯ�ַ�������

	atomic_bool m_fReadingWebData; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ

	bool m_fReportStatus; //
	long m_lContentLength; // ���ݳ���

	long m_lInquiringNumber; // ÿ�β�ѯ����
	time_t m_tCurrentInquiryTime; // ��ǰ������������ʱ�䣨�Ժ���ƣ�

	CString m_strConnectionName; // �������ȡ��������
	static atomic_llong sm_lTotalByteRead; // ��ǰ�����ȡ�ֽ��������е������ȡ�����޸Ĵ˱������ʶ�����Ϊ��̬��
};

typedef shared_ptr<CVirtualWebInquiry> CVirtualWebInquiryPtr;