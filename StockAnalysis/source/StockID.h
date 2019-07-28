#pragma once

#include"stdafx.h"

using namespace std;
#include<memory>

enum {
  __STOCK_NOT_CHECKED__ = 128, // ��δ�����Ĺ�Ʊ����
  __STOCK_IPOED__ = 255,    // ������Ʊ��ʶ
  __STOCK_NULL__ = 0,       // ��Ч���루�˴���Ĺ�Ʊ�����ڣ�
  __STOCK_DELISTED__ = 1,   // �����еĹ�Ʊ��ʶ
};

class CStockID {					//֤ȯ��ʶ
public:
  CStockID();
  ~CStockID();
  void Reset(void);

  WORD      GetMarket(void) noexcept { return m_wMarket; }
  void      SetMarket(WORD wValue) noexcept { m_wMarket = wValue; }
  CString   GetStockCode(void) { return m_strStockCode; }
  void      SetStockCode(CString str) { m_strStockCode = str; }
  CString   GetStockName(void) { return m_strStockName; }
  void      SetStockName(CString str) { m_strStockName = str; }
  long      GetIndex(void) noexcept { return m_nIndex; }
  void      SetIndex(long lValue) noexcept { m_nIndex = lValue; }
  long      GetDayLineStartDay(void) noexcept { return m_lDayLineStartDay; }
  void      SetDayLineStartDay(long lDay) noexcept { m_lDayLineStartDay = lDay; }
  long      GetDayLineEndDay(void) noexcept { return m_lDayLineEndDay; }
  void      SetDayLineEndDay(long lDay) noexcept { m_lDayLineEndDay = lDay; }
  long      GetNewestDayLineDay(void) noexcept { return m_lNewestDayLineDay; }
  void      SetNewestDayLineDay(long lDay) noexcept { m_lNewestDayLineDay = lDay; }
  long      GetIPOStatus(void) noexcept { return m_lIPOed; }
  void      SetIPOStatus(long lValue) noexcept { m_lIPOed = lValue; }
  bool      IsActive(void) noexcept { return m_fActive; }
  void      SetActive(bool fFlag) noexcept { m_fActive = fFlag; }
  bool      IsDayLineNeedUpdate(void) noexcept { return m_fDayLineNeedUpdate; }
  void      SetDayLineNeedUpdate(bool fFlag) noexcept { m_fDayLineNeedUpdate = fFlag; }
  bool      IsInquiringOnce(void) noexcept { return m_fInquiringOnce; }
  void      SetInquiringOnce(bool fFlag) noexcept { m_fInquiringOnce = fFlag; }

protected:
  WORD			m_wMarket;				// �г���1���Ϻ����壻2���������壻3���Ϻ�ָ����4������ָ����5���Ϻ����棻6��������С�壻7�����ڴ�ҵ�壻8���Ϻ�B�ɣ�9������B�ɣ�10���Ϻ��ƴ���
  CString		m_strStockCode;		// ��Ʊ���롣��λ��ǰ��λΪ�г�ǰ׺������λΪ���ִ��롣��sh600601��sz000001
  CString   m_strStockName;		// ��Ʊ����
  long			m_nIndex;					// �������е�ƫ����
  long      m_lDayLineStartDay; // ������ʼ�ա�Ӧ�����ú��û�б仯�ˣ����Դ���������֤����ʼ�յ�ǰ���̼۵�����������Щ����
  long			m_lDayLineEndDay;	// �������ݸ����ա�����Ǵ���������ʷ����ʱ�õ��������գ�
  long      m_lNewestDayLineDay; // ���µ����������ա����������ʵʱ���ݣ��������Ҫ����m_lDayLineEndDay
  long			m_lIPOed;					// ͨ��������ʷ���߲�ѯ�����ֻ��ǰ׺��Ϣ��û��ʵ�����ݣ�����ȷ��û��ʵ�ʽ��ס�����������£�����ʵʱ���������ݣ�ֻ��Ϊ����ѡ�Ĭ�������Ϊ������
                              // δ���У���Ч��Ʊ���룩Ϊ__STOCK_NULL__������Ϊ__STOCK_IPOED__����ͨ��IPO����δ���л�����Ϊ__STOCK_DELISTED�����������δ���֣������Ժ���
  bool			m_fActive;				// �Ƿ����������ݶ��롣������ʵʱ���鴦���������á�
  bool			m_fDayLineNeedUpdate; // ������Ҫ���¡�Ĭ��Ϊ��
  bool			m_fInquiringOnce;// �Ƿ񱻲�ѯһ�Ρ�������m_fIPOed�Ƿ�Ϊ�棬��Ҫ�������в�ѯһ�ι�Ʊ�����������Ȼ����������ٲ飩��

};

typedef shared_ptr<CStockID> StockIDPtr;

