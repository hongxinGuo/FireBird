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

  WORD      GetMarket(void) { return m_wMarket; }
  void      SetMarket(WORD wValue) { m_wMarket = wValue; }
  CString   GetStockCode(void) { return m_strStockCode; }
  void      SetStockCode(CString str) { m_strStockCode = str; }
  CString   GetStockName(void) { return m_strStockName; }
  void      SetStockName(CString str) { m_strStockName = str; }
  long      GetIndex(void) { return m_nIndex; }
  void      SetIndex(long lValue) { m_nIndex = lValue; }
  long      GetDayLineStartDay(void) { return m_lDayLineStartDay; }
  void      SetDayLineStartDay(long lDay) { m_lDayLineStartDay = lDay; }
  long      GetDayLineEndDay(void) { return m_lDayLineEndDay; }
  void      SetDayLineEndDay(long lDay) { m_lDayLineEndDay = lDay; }
  long      GetNewestDayLineDay(void) { return m_lNewestDayLineDay; }
  void      SetNewestDayLineDay(long lDay) { m_lNewestDayLineDay = lDay; }
  long      GetIPOStatus(void) { return m_lIPOed; }
  void      SetIPOStatus(long lValue) { m_lIPOed = lValue; }
  bool      IsActive(void) { return m_fActive; }
  void      SetActive(bool fFlag) { m_fActive = fFlag; }
  bool      IsDayLineNeedUpdate(void) { return m_fDayLineNeedUpdate; }
  void      SetDayLineNeedUpdate(bool fFlag) { m_fDayLineNeedUpdate = fFlag; }
  bool      IsInquiringOnce(void) { return m_fInquiringOnce; }
  void      SetInquiringOnce(bool fFlag) { m_fInquiringOnce = fFlag; }

protected:
  WORD			m_wMarket;				// �г�
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

