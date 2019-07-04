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
  bool			m_fDayLineNeedUpdated; // ������Ҫ���¡�Ĭ��Ϊ��
  bool			m_fInquiriingOnce;// �Ƿ񱻲�ѯһ�Ρ�������m_fIPOed�Ƿ�Ϊ�棬��Ҫ�������в�ѯһ�ι�Ʊ�����������Ȼ����������ٲ飩��
};

typedef shared_ptr<CStockID> StockIDPtr;

