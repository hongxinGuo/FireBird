#pragma once

#include"globeSymbolDef.h"
#include"VirtualSetStockSymbol.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CVirtualStock : public CObject {
public:
	CVirtualStock();
	virtual ~CVirtualStock();
	virtual void Reset(void);
	virtual int GetRatio(void)  const = 0;

	virtual void LoadSymbol(CVirtualSetStockSymbol& setStockSymbol);
	virtual void AppendSymbol(CVirtualSetStockSymbol& setStockSymbol);
	virtual void UpdateSymbol(CVirtualSetStockSymbol& setStockSymbol);
	virtual void SaveSymbol(CVirtualSetStockSymbol& setStockSymbol);

public:
	CString GetDescription(void) const { return m_strDescription; }
	void SetDescription(CString strDescription) { m_strDescription = strDescription; }
	CString GetExchangeCode(void) const { return m_strExchangeCode; }
	void SetExchangeCode(CString strExchangeCode) { m_strExchangeCode = strExchangeCode; }
	CString GetSymbol(void) const { return m_strSymbol; }
	void SetSymbol(CString str) { m_strSymbol = str; }
	CString GetDisplaySymbol(void) const { return m_strDisplaySymbol; }
	void SetDisplaySymbol(CString str) { m_strDisplaySymbol = str; }

	// ����ʵʱ���ݣ���Ҫ����
	time_t GetTransactionTime(void) const noexcept { return m_TransactionTime; }
	void SetTransactionTime(time_t time) noexcept { m_TransactionTime = time; }
	long GetLastClose(void) const noexcept { return m_lLastClose; }
	void SetLastClose(long lValue) noexcept { m_lLastClose = lValue; }
	long GetOpen(void) const noexcept { return m_lOpen; }
	void SetOpen(long lValue) noexcept { m_lOpen = lValue; }
	long GetHigh(void) const noexcept { return m_lHigh; }
	void SetHigh(long lValue) noexcept { m_lHigh = lValue; }
	long GetLow(void) const noexcept { return m_lLow; }
	void SetLow(long lValue) noexcept { m_lLow = lValue; }
	long GetNew(void) const noexcept { return m_lNew; }
	void SetNew(long lValue) noexcept { m_lNew = lValue; }
	INT64 GetAmount(void) const  noexcept { return m_llAmount; }
	void SetAmount(INT64 llValue) noexcept { m_llAmount = llValue; }
	INT64 GetVolume(void) const noexcept { return m_llVolume; }
	void SetVolume(INT64 llValue) noexcept { m_llVolume = llValue; }
	long GetUpDown(void) const noexcept { return m_lUpDown; }
	void SetUpDown(long lValue) noexcept { m_lUpDown = lValue; }
	double GetUpDownRate(void) const noexcept { return m_dUpDownRate; }
	void SetUpDownRate(double dValue) noexcept { m_dUpDownRate = dValue; }
	double GetChangeHandRate(void) const noexcept { return m_dChangeHandRate; }
	void SetChangeHandRate(double dValue) noexcept { m_dChangeHandRate = dValue; }
	void SetTotalValue(INT64 llValue) noexcept { m_llTotalValue = llValue; }
	INT64 GetTotalValue(void) const noexcept { return m_llTotalValue; }
	void SetCurrentValue(INT64 llValue) noexcept { m_llCurrentValue = llValue; }
	INT64 GetCurrentValue(void) const noexcept { return m_llCurrentValue; }

	//
	bool IsTodayNewStock(void) const noexcept { return m_fTodayNewStock; }
	void SetTodayNewStock(bool fFlag) noexcept { m_fTodayNewStock = fFlag; }
	bool IsUpdateProfileDB(void) const noexcept { return m_fUpdateProfileDB; }
	bool IsUpdateProfileDBAndClearFlag(void) noexcept { const bool fNeedSaving = m_fUpdateProfileDB.exchange(false); return fNeedSaving; }
	void SetUpdateProfileDB(bool fFlag) noexcept { m_fUpdateProfileDB = fFlag; }
	bool IsClearDayLineVector(void) const noexcept { return m_fClearDayLineVector; }
	bool IsClearDayLineVectorAndClearFlag(void) noexcept { const bool fNeedSaving = m_fClearDayLineVector.exchange(false); return fNeedSaving; }
	void SetClearDayLineVector(bool fFlag) noexcept { m_fClearDayLineVector = fFlag; }
	bool IsActive(void) const noexcept { return m_fActive; }
	void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }

	long GetDayLineStartDate(void) const noexcept { return m_lDayLineStartDate; }
	void SetDayLineStartDate(long lDate) noexcept { m_lDayLineStartDate = lDate; }
	long GetDayLineEndDate(void) const noexcept { return m_lDayLineEndDate; }
	void SetDayLineEndDate(long lDate) noexcept { m_lDayLineEndDate = lDate; }

	long GetIPOStatus(void) const noexcept { return m_lIPOStatus; }
	void SetIPOStatus(long lValue) noexcept { m_lIPOStatus = lValue; }
	bool IsDelisted(void) const noexcept { return (m_lIPOStatus == __STOCK_DELISTED__); }
	bool IsNullStock(void) const noexcept { return (m_lIPOStatus == __STOCK_NULL__); }
	bool IsIPOed(void) const noexcept { return(m_lIPOStatus == __STOCK_IPOED__); }
	bool IsNotChecked(void) const noexcept { return(m_lIPOStatus == __STOCK_NOT_CHECKED__); }

	// ���ڴ���������ʷ���ݵĺ���λ�ڲ�ͬ���߳��У��ʶ���Ҫͬ���������ñ�ʶ
	bool IsDayLineNeedUpdate(void) const noexcept { return m_fDayLineNeedUpdate; }
	void SetDayLineNeedUpdate(bool fFlag) noexcept { m_fDayLineNeedUpdate = fFlag; }
	bool IsDayLineNeedProcess(void) const  noexcept { return m_fDayLineNeedProcess; }
	void SetDayLineNeedProcess(bool fFlag) noexcept { m_fDayLineNeedProcess = fFlag; }
	bool IsDayLineNeedSaving(void) const noexcept { return m_fDayLineNeedSaving; }
	void SetDayLineNeedSaving(bool fFlag) noexcept { m_fDayLineNeedSaving = fFlag; }
	bool IsDayLineNeedSavingAndClearFlag(void) noexcept { const bool fNeedSaving = m_fDayLineNeedSaving.exchange(false); return fNeedSaving; }

protected:
	CString m_strDescription; // ��֤ȯ������
	CString m_strExchangeCode; // ֤ȯ����������������ΪUS���Ϻ�ΪSS������ΪSZ�����Ϊforex�ȡ�
	CString m_strSymbol; // ��Ʊ���롣ʮλ���ڣ�ǰ��λΪ���ִ��룬����λΪ�г�ǰ׺����600601.SS��000001.SZ
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
	INT64 m_llVolume;	// ��1�ɼƵĳɽ���
	INT64 m_llAmount; // ��Ԫ�Ƶĳɽ����
	double m_dChangeHandRate; // ������
	INT64 m_llTotalValue;	// ����ֵ����λ����Ԫ
	INT64 m_llCurrentValue;	// ��ͨ��ֵ����λ����Ԫ

	bool m_fTodayNewStock; // �����·��ֵĹ�Ʊ
	atomic_bool m_fUpdateProfileDB;
	atomic_bool m_fClearDayLineVector;
	bool m_fActive;	// �Ƿ����������ݶ��롣������ʵʱ���鴦����������������ʷ���ݴ����������á�
	long m_lDayLineStartDate;	// ����������ʼ�ա�����Ǵ���������ʷ����ʱ�õ�����ʼ�����գ�
	long m_lDayLineEndDate;	// �������ݸ����ա�����Ǵ���������ʷ����ʱ�õ��������գ�
	long m_lIPOStatus; // ͨ��������ʷ���߲�ѯ�����ֻ��ǰ׺��Ϣ��û��ʵ�����ݣ�����ȷ��û��ʵ�ʽ��ס�����������£�����ʵʱ���������ݣ�ֻ��Ϊ����ѡ�Ĭ�������Ϊ������
							 // δ���У���Ч��Ʊ���룩Ϊ__STOCK_NULL__������Ϊ__STOCK_IPOED__����ͨ��IPO����δ���л�����Ϊ__STOCK_DELISTED�����������δ���֣������Ժ���

	atomic_bool m_fDayLineNeedUpdate; // ������Ҫ���¡�Ĭ��Ϊ��
	atomic_bool m_fDayLineNeedProcess; // �Ѵ������϶�ȡ��������ʷ���ݣ��ȴ�����
	atomic_bool m_fDayLineNeedSaving; // ������ʷ�����Ѵ����ȴ��洢��
};

typedef shared_ptr<CVirtualStock> CVirtualStockPtr;
