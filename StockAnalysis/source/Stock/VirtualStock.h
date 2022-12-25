#pragma once

#include"VirtualSetStockSymbol.h"

#include<memory>
#include<atomic>

class CVirtualStock;
using CVirtualStockPtr = shared_ptr<CVirtualStock>;

class CVirtualStock : public CObject {
public:
	CVirtualStock();
	// ������ֵ��
	CVirtualStock(const CVirtualStock&) = delete;
	CVirtualStock& operator=(const CVirtualStock&) = delete;
	CVirtualStock(const CVirtualStock&&) noexcept = delete;
	CVirtualStock& operator=(const CVirtualStock&&) noexcept = delete;
	~CVirtualStock() override = default;
	virtual void Reset(void);
	virtual int GetRatio(void) const = 0;

	virtual void LoadSymbol(CVirtualSetStockSymbol& setStockSymbol);
	virtual void AppendSymbol(CVirtualSetStockSymbol& setStockSymbol);
	virtual void UpdateSymbol(CVirtualSetStockSymbol& setStockSymbol);
	virtual void SaveSymbol(CVirtualSetStockSymbol& setStockSymbol);

public:
	void UpdateRealTimeData(CVirtualStockPtr pStock);

	CString GetDescription(void) const noexcept { return m_strDescription; }
	void SetDescription(const CString& strDescription) noexcept { m_strDescription = strDescription; }
	CString GetExchangeCode(void) const noexcept { return m_strExchangeCode; }
	void SetExchangeCode(const CString& strExchangeCode) noexcept { m_strExchangeCode = strExchangeCode; }
	CString GetSymbol(void) const noexcept { return m_strSymbol; }
	void SetSymbol(const CString& str) noexcept { m_strSymbol = str; }
	CString GetDisplaySymbol(void) const noexcept { return m_strDisplaySymbol; }
	void SetDisplaySymbol(const CString& str) noexcept { m_strDisplaySymbol = str; }

	// ����ʵʱ���ݣ���Ҫ����
	time_t GetTransactionTime(void) const noexcept { return m_TransactionTime; }
	virtual void SetTransactionTime(const time_t time) noexcept { m_TransactionTime = time; }
	long GetLastClose(void) const noexcept { return m_lLastClose; }
	virtual void SetLastClose(const long lValue) noexcept { m_lLastClose = lValue; }
	long GetOpen(void) const noexcept { return m_lOpen; }
	virtual void SetOpen(const long lValue) noexcept { m_lOpen = lValue; }
	long GetHigh(void) const noexcept { return m_lHigh; }
	virtual void SetHigh(const long lValue) noexcept { m_lHigh = lValue; }
	long GetLow(void) const noexcept { return m_lLow; }
	virtual void SetLow(const long lValue) noexcept { m_lLow = lValue; }
	long GetNew(void) const noexcept { return m_lNew; }
	virtual void SetNew(const long lValue) noexcept { m_lNew = lValue; }
	INT64 GetAmount(void) const noexcept { return m_llAmount; }
	virtual void SetAmount(const INT64 llValue) noexcept { m_llAmount = llValue; }
	INT64 GetVolume(void) const noexcept { return m_llVolume; }
	virtual void SetVolume(const INT64 llValue) noexcept { m_llVolume = llValue; }

	long GetUpDown(void) const noexcept { return m_lUpDown; }
	virtual void SetUpDown(const long lValue) noexcept { m_lUpDown = lValue; }
	double GetUpDownRate(void) const noexcept { return m_dUpDownRate; }
	virtual void SetUpDownRate(const double dValue) noexcept { m_dUpDownRate = dValue; }
	double GetChangeHandRate(void) const noexcept { return m_dChangeHandRate; }
	virtual void SetChangeHandRate(const double dValue) noexcept { m_dChangeHandRate = dValue; }
	INT64 GetTotalValue(void) const noexcept { return m_llTotalValue; }
	virtual void SetTotalValue(const INT64 llValue) noexcept { m_llTotalValue = llValue; }
	INT64 GetCurrentValue(void) const noexcept { return m_llCurrentValue; }
	virtual void SetCurrentValue(const INT64 llValue) noexcept { m_llCurrentValue = llValue; }

	//
	bool IsTodayNewStock(void) const noexcept { return m_fTodayNewStock; }
	void SetTodayNewStock(const bool fFlag) noexcept { m_fTodayNewStock = fFlag; }
	bool IsUpdateProfileDB(void) const noexcept { return m_fUpdateProfileDB; }

	bool IsUpdateProfileDBAndClearFlag(void) noexcept {
		const bool fNeedSaving = m_fUpdateProfileDB.exchange(false);
		return fNeedSaving;
	}

	void SetUpdateProfileDB(const bool fFlag) noexcept { m_fUpdateProfileDB = fFlag; }
	bool IsUpdateCompanyNewsDB(void) const noexcept { return m_fUpdateCompanyNewsDB; }

	bool IsUpdateCompanyNewsDBAndClearFlag(void) noexcept {
		const bool fNeedSaving = m_fUpdateCompanyNewsDB.exchange(false);
		return fNeedSaving;
	}

	void SetUpdateCompanyNewsDB(const bool fFlag) noexcept { m_fUpdateCompanyNewsDB = fFlag; }
	bool IsClearDayLineVector(void) const noexcept { return m_fClearDayLineVector; }

	bool IsClearDayLineVectorAndClearFlag(void) noexcept {
		const bool fNeedSaving = m_fClearDayLineVector.exchange(false);
		return fNeedSaving;
	}

	void SetClearDayLineVector(const bool fFlag) noexcept { m_fClearDayLineVector = fFlag; }
	bool IsActive(void) const noexcept { return m_fActive; }
	void SetActive(const bool fFlag) noexcept { m_fActive = fFlag; }

	long GetDayLineStartDate(void) const noexcept { return m_lDayLineStartDate; }
	void SetDayLineStartDate(const long lDate) noexcept { m_lDayLineStartDate = lDate; }
	long GetDayLineEndDate(void) const noexcept { return m_lDayLineEndDate; }
	void SetDayLineEndDate(const long lDate) noexcept { m_lDayLineEndDate = lDate; }

	long GetIPOStatus(void) const noexcept { return m_lIPOStatus; }
	void SetIPOStatus(const long lValue) noexcept { m_lIPOStatus = lValue; }
	bool IsDelisted(void) const noexcept { return (m_lIPOStatus == _STOCK_DELISTED_); }
	bool IsNotYetList(void) const noexcept { return (m_lIPOStatus == _STOCK_NOT_YET_LIST_); }
	bool IsNullStock(void) const noexcept { return (m_lIPOStatus == _STOCK_NULL_); }
	bool IsIPOed(void) const noexcept { return (m_lIPOStatus == _STOCK_IPOED_); }
	bool IsNotChecked(void) const noexcept { return (m_lIPOStatus == _STOCK_NOT_CHECKED_); }

	// ���ڴ���������ʷ���ݵĺ���λ�ڲ�ͬ���߳��У��ʶ���Ҫͬ���������ñ�ʶ
	bool IsDayLineNeedUpdate(void) const noexcept { return m_fDayLineNeedUpdate; }
	void SetDayLineNeedUpdate(const bool fFlag) noexcept { m_fDayLineNeedUpdate = fFlag; }
	bool IsDayLineNeedSaving(void) const noexcept { return m_fDayLineNeedSaving; }
	void SetDayLineNeedSaving(const bool fFlag) noexcept { m_fDayLineNeedSaving = fFlag; }

	bool IsDayLineNeedSavingAndClearFlag(void) noexcept {
		const bool fNeedSaving = m_fDayLineNeedSaving.exchange(false);
		return fNeedSaving;
	}

protected:
	CString m_strDescription; // ��֤ȯ������
	CString m_strExchangeCode; // ֤ȯ����������������ΪUS���Ϻ�ΪSS������ΪSZ�����Ϊforex�ȡ�
	CString m_strSymbol; // ��Ʊ���롣��ʮλ���ڣ�����λΪ�г�ǰ׺����600601.SS��000001.SZ, AAPL.US, RIG.US
	CString m_strDisplaySymbol;

	// ʵʱ������
	time_t m_TransactionTime; // ʵʱ���ݽ���ʱ��
	long m_lLastClose; // ��0.001Ԫ�Ƶ����̼�
	long m_lOpen; // ��0.001Ԫ�ƵĿ��̼�
	long m_lHigh; // ��0.001Ԫ�Ƶ���߼�
	long m_lLow; // ��0.001Ԫ�Ƶ���ͼ�
	long m_lNew; // ��0.001Ԫ�Ƶ����¼�
	long m_lUpDown; // �ǵ�ֵ
	double m_dUpDownRate; // �ǵ���
	INT64 m_llVolume; // ��1�ɼƵĳɽ���
	INT64 m_llAmount; // ��Ԫ�Ƶĳɽ����
	double m_dChangeHandRate; // ������
	INT64 m_llTotalValue; // ����ֵ����λ����Ԫ
	INT64 m_llCurrentValue; // ��ͨ��ֵ����λ����Ԫ

	bool m_fTodayNewStock; // �����·��ֵĹ�Ʊ
	atomic_bool m_fUpdateProfileDB;
	atomic_bool m_fUpdateCompanyNewsDB;
	atomic_bool m_fClearDayLineVector;
	bool m_fActive; // �Ƿ����������ݶ��롣������ʵʱ���鴦����������������ʷ���ݴ����������á�
	long m_lDayLineStartDate; // ����������ʼ�ա�����Ǵ���������ʷ����ʱ�õ�����ʼ�����գ�
	long m_lDayLineEndDate; // �������ݸ����ա�����Ǵ���������ʷ����ʱ�õ��������գ�
	long m_lIPOStatus; // ͨ��������ʷ���߲�ѯ�����ֻ��ǰ׺��Ϣ��û��ʵ�����ݣ�����ȷ��û��ʵ�ʽ��ס�����������£�����ʵʱ���������ݣ�ֻ��Ϊ����ѡ�Ĭ�������Ϊ������
	// δ���У���Ч��Ʊ���룩Ϊ_STOCK_NULL_������Ϊ_STOCK_IPOED_����ͨ��IPO����δ���л�����Ϊ_STOCK_DELISTED�����������δ���֣������Ժ���

	atomic_bool m_fDayLineNeedUpdate; // ������Ҫ���¡�Ĭ��Ϊ��
	atomic_bool m_fDayLineNeedSaving; // ������ʷ�����Ѵ����ȴ��洢��
};
