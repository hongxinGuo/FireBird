#pragma once

#include"stdafx.h"

#include"SetStakeCode.h"

using namespace std;
#include<memory>

class CStakeCode : public CObject {
public:
  void LoadStakeCodeDB(CSetStakeCode& setStakeCode);
  void SaveToDB(CSetStakeCode& setStakeCode);

  CString GetStakeCode(void) noexcept { return m_strStakeCode; }
  void SetStakeCode(CString str) noexcept { m_strStakeCode = str; }

  void SetStakeName(CString str) noexcept { m_strStakeName = str; }

protected:
  WORD m_wMarket;	// 1���Ϻ��г���������ϸ�ࣩ��2�������г���������ϸ�ࣩ��3���Ϻ�ָ����4������ָ�������Ϻ����棻6��������С�壻
                // 7���Ϻ�B�ɣ�8������B�ɣ�9���Ϻ��ƴ���; 10�����ڴ�ҵ�壻
                // �Ϻ��г����õ�����ʾ�������г�����˫����ʾ��Ŀǰ��ʱ���ã��Ȳ鿴�漰����Щ������Ҫ�޸ġ�
  CString m_strStakeCode; // ��Ʊ���롣��λ��ǰ��λΪ�г�ǰ׺������λΪ���ִ��롣��sh600601��sz000001
  CString m_strStakeName; // ��Ʊ����
  CStringW m_strStockNameReadIn; // ����Ĺ�Ʊ���ƣ�UniCode��ʽ��Ŀǰ��δʹ�ã�
  long m_lDayLineStartDate;	// ����������ʼ�ա�����Ǵ���������ʷ����ʱ�õ�����ʼ�����գ�
  long m_lDayLineEndDate;	// �������ݸ����ա�����Ǵ���������ʷ����ʱ�õ��������գ�
  long m_lIPOStatus; // ͨ��������ʷ���߲�ѯ�����ֻ��ǰ׺��Ϣ��û��ʵ�����ݣ�����ȷ��û��ʵ�ʽ��ס�����������£�����ʵʱ���������ݣ�ֻ��Ϊ����ѡ�Ĭ�������Ϊ������
                 // δ���У���Ч��Ʊ���룩Ϊ__STOCK_NULL__������Ϊ__STOCK_IPOED__����ͨ��IPO����δ���л�����Ϊ__STOCK_DELISTED�����������δ���֣������Ժ���
  short	m_nHand;	// ÿ�ֹ���
};

typedef shared_ptr<CStakeCode> CStakeCodePtr;