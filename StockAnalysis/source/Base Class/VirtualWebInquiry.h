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

class CVirtualWebInquiry : public CObject {
public:
	CVirtualWebInquiry();
	virtual ~CVirtualWebInquiry(void);

	virtual bool OpenFile(CString strInquiring);
	virtual bool ReadWebData(void); // ����ʵ�ʶ�ȡ����
	virtual UINT ReadWebFileOneTime(void); // �޷����ԣ��ʶ����⻯��ʹ��Mock�ࡣ
	CWebDataPtr TransferReceivedDataToWebData(void);

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

	char GetData(long lIndex) const { return m_vBuffer.at(lIndex); }
	void SetData(long lIndex, char value) { m_vBuffer.at(lIndex) = value; }

	INT64 GetByteReaded(void) const noexcept { return m_lByteRead; }
	void SetByteReaded(INT64 lValue) noexcept { m_lByteRead = lValue; }
	void AddByteReaded(INT64 lValue) noexcept { m_lByteRead += lValue; }

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

	INT64 GetInquiringNumber(void) const noexcept { return m_lInquiringNumber; }
	void SetInquiringNumber(INT64 lValue) noexcept { m_lInquiringNumber = lValue; }

	INT64 GetTotalByteReaded(void) const noexcept { return m_lTotalByteReaded; }
	void SetTotalByteReaded(INT64 lValue = 0) noexcept { m_lTotalByteReaded = lValue; }
	void ClearTotalByteReaded(void) noexcept { m_lTotalByteReaded = 0; }

	time_t GetCurrentInquiryTime(void) const noexcept { return m_tCurrentInquiryTime; }

public:
	// ����Ϊ�����ú���
	void __TESTSetBuffer(char* buffer, INT64 lTotalNumber);
	void __TESTSetBuffer(CString str);

protected:
	CInternetSession* m_pSession;
	CHttpFile* m_pFile; // �����ļ�ָ��
	DWORD m_dwWebErrorCode; //�����ȡ�������
	CString m_strInquire;// ��ѯ������ַ���
	vector<char> m_vBuffer; // ���յ����ݵĻ�����
	INT64 m_lByteRead; // ���յ����ַ���.�洢ʱ��Ҫ����һ���ַ�0x000���ʶ�ʵ���ַ���Ϊm_lByteRead+1��

	CString m_strWebDataInquireMiddle; // ��ѯ�ַ����м��ֶ�
	CString m_strWebDataInquirePrefix; // ��ѯ�ַ���ǰ׺
	CString m_strWebDataInquireSuffix; // ��ѯ�ַ�����׺

	atomic_bool m_fReadingWebData; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
	atomic_bool m_fWebError; // ��ȡ��������ʱ�Ƿ���ִ����ʶ

	bool m_fReportStatus; //

	INT64 m_lInquiringNumber; // ÿ�β�ѯ����
	time_t m_tCurrentInquiryTime; // ��ǰ������������ʱ�䣨�Ժ���ƣ�

	CString m_strConnectionName; // �������ȡ��������
	static atomic_llong m_lTotalByteReaded; // ��ǰ�����ȡ�ֽ��������е������ȡ�����޸Ĵ˱������ʶ�����Ϊ��̬��
};

typedef shared_ptr<CVirtualWebInquiry> CVirtualWebInquiryPtr;