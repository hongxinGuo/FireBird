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

enum {
  __SHANGHAI_MARKET__ = 1, // �Ϻ��г�
  __SHENZHEN_MARKET__ = 2, // �����г�
  __SHANGHAI_INDEX__ = 3, // �Ϻ�ָ��
  __SHENZHEN_INDEX__ = 4, // ����ָ��
  __SHANGHAI_MAIN__ = 5,   // �Ϻ�����
  __SHENZHEN_MAIN__ = 6, // ��������
  __SHANGHAI_3BAN__ = 7, // �Ϻ�3��
  __SHENZHEN_3BAN__ = 8, //������С��
  __SHANGHAI_B_SHARE__ = 9, //�Ϻ�B��
  __SHENZHEN_B_SHARE__ = 10, // ����B��
  __SHANGHAI_KECHUANG__ = 11, // �Ϻ��ƴ���
  __SHENZHEN_CHUANGYE__ = 12, // ���ڴ�ҵ��
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
  long      GetIPOStatus(void) noexcept { return m_lIPOed; }
  void      SetIPOStatus(long lValue) noexcept { m_lIPOed = lValue; }
  
  bool      IsActive(void) noexcept { return m_fActive; }
  void      SetActive(bool fFlag) noexcept { m_fActive = fFlag; }
  bool      IsDayLineNeedUpdate(void) noexcept { return m_fDayLineNeedUpdate; }
  void      SetDayLineNeedUpdate(bool fFlag) noexcept { m_fDayLineNeedUpdate = fFlag; }
  bool      IsInquiringOnce(void) noexcept { return m_fInquiringOnce; }
  void      SetInquiringOnce(bool fFlag) noexcept { m_fInquiringOnce = fFlag; }

  bool      IsNeedUpdate(void) noexcept { return m_fNeedUpdate; }
  void      SetNeedUpdate(bool fFlag) noexcept { m_fNeedUpdate = fFlag; }

protected:
  WORD			m_wMarket;				// 1���Ϻ��г���������ϸ�ࣩ��2�������г���������ϸ�ࣩ��3���Ϻ�ָ����4������ָ�������Ϻ����棻6��������С�壻
                              // 7���Ϻ�B�ɣ�8������B�ɣ�9���Ϻ��ƴ���; 10�����ڴ�ҵ�壻
                              // �Ϻ��г����õ�����ʾ�������г�����˫����ʾ��Ŀǰ��ʱ���ã��Ȳ鿴�漰����Щ������Ҫ�޸ġ�
  CString		m_strStockCode;		// ��Ʊ���롣��λ��ǰ��λΪ�г�ǰ׺������λΪ���ִ��롣��sh600601��sz000001
  CString   m_strStockName;		// ��Ʊ����
  long			m_nIndex;					// �������е�ƫ����
  long      m_lDayLineStartDay; // ������ʼ�ա�Ӧ�����ú��û�б仯�ˣ����Դ���������֤����ʼ�յ�ǰ���̼۵�����������Щ����
  long			m_lDayLineEndDay;	// �������ݸ����ա�����Ǵ���������ʷ����ʱ�õ��������գ�
  long			m_lIPOed;					// ͨ��������ʷ���߲�ѯ�����ֻ��ǰ׺��Ϣ��û��ʵ�����ݣ�����ȷ��û��ʵ�ʽ��ס�����������£�����ʵʱ���������ݣ�ֻ��Ϊ����ѡ�Ĭ�������Ϊ������
                              // δ���У���Ч��Ʊ���룩Ϊ__STOCK_NULL__������Ϊ__STOCK_IPOED__����ͨ��IPO����δ���л�����Ϊ__STOCK_DELISTED�����������δ���֣������Ժ���
  
  // ����洢����
  bool			m_fActive;				// �Ƿ����������ݶ��롣������ʵʱ���鴦���������á�
  bool			m_fDayLineNeedUpdate; // ������Ҫ���¡�Ĭ��Ϊ��
  bool			m_fInquiringOnce;// �Ƿ񱻲�ѯһ�Ρ�������m_fIPOed�Ƿ�Ϊ�棬��Ҫ�������в�ѯһ�ι�Ʊ�����������Ȼ����������ٲ飩��

  bool      m_fNeedUpdate;      // �Ƿ���Ҫ�洢��ʶ
};

typedef shared_ptr<CStockID> StockIDPtr;

