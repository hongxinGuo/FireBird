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
	bool IncreaseBufferSizeIfNeeded(long lSize = 1024 * 1024);

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
	virtual void UpdateStatusWhenSecceed(CWebDataPtr pData); // �ɹ����պ����ϵͳ״̬��

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
	INT64 m_lByteRead; // ���յ����ַ���.

	CString m_strInquiry;// ��ѯ������ַ�����m_strInquiryFunction + m_strInquiryToken).
	CString m_strInquiryFunction; // ��ѯ�ַ������ܲ���
	CString m_strInquiryToken; // ��ѯ�ַ�������

	atomic_bool m_fReadingWebData; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
	atomic_bool m_fWebError; // ��ȡ��������ʱ�Ƿ���ִ����ʶ

	bool m_fReportStatus; //
	bool m_fFSonContentType; // ���ݸ�ʽ�Ƿ�ΪJSon
	long m_lContentLength; // ���ݳ���

	INT64 m_lInquiringNumber; // ÿ�β�ѯ����
	time_t m_tCurrentInquiryTime; // ��ǰ������������ʱ�䣨�Ժ���ƣ�

	CString m_strConnectionName; // �������ȡ��������
	static atomic_llong m_lTotalByteReaded; // ��ǰ�����ȡ�ֽ��������е������ȡ�����޸Ĵ˱������ʶ�����Ϊ��̬��
};

typedef shared_ptr<CVirtualWebInquiry> CVirtualWebInquiryPtr;