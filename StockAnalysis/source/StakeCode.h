#pragma once

enum {
  __STOCK_NOT_CHECKED__ = 128, // ��δ�����Ĺ�Ʊ����
  __STOCK_IPOED__ = 255, // ������Ʊ��ʶ
  __STOCK_NULL__ = 0, // ��Ч���루�˴���Ĺ�Ʊ�����ڣ�
  __STOCK_DELISTED__ = 1, // �����У���ͣ�ƣ��Ĺ�Ʊ��ʶ
};

#include"stdafx.h"

#include"SetStakeCode.h"

using namespace std;
#include<memory>

class CStakeCode : public CObject {
public:
  CStakeCode();
  ~CStakeCode(void);

  void LoadStakeCodeDB(CSetStakeCode& setStakeCode);
  void SaveToStakeCodeDB(CSetStakeCode& setStakeCode);

  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wValue) noexcept { m_wMarket = wValue; }
  CString GetStakeCode(void) { return m_strStakeCode; }
  void SetStakeCode(CString str) { m_strStakeCode = str; }
  CString GetStakeName(void) { return m_strStakeName; }
  void SetStakeName(CString str) { m_strStakeName = str; }

  long GetOffset(void) noexcept { return m_lOffsetInContainer; }
  void SetOffset(long lValue) noexcept { m_lOffsetInContainer = lValue; }

  long GetDayLineStartDate(void) noexcept { return m_lDayLineStartDate; }
  void SetDayLineStartDate(long lDate) noexcept { m_lDayLineStartDate = lDate; }
  long GetDayLineEndDate(void) noexcept { return m_lDayLineEndDate; }
  void SetDayLineEndDate(long lDate) noexcept { m_lDayLineEndDate = lDate; }

  long GetIPOStatus(void) noexcept { return m_lIPOStatus; }
  void SetIPOStatus(long lValue) noexcept { m_lIPOStatus = lValue; }
  bool IsDelisted(void) noexcept { return (m_lIPOStatus == __STOCK_DELISTED__); }
  bool IsNullStock(void) noexcept { return (m_lIPOStatus == __STOCK_NULL__); }
  bool IsIPOed(void) noexcept { return(m_lIPOStatus == __STOCK_IPOED__); }
  bool IsNotChecked(void) noexcept { return(m_lIPOStatus == __STOCK_NOT_CHECKED__); }
protected:
  WORD m_wMarket;	// 1���Ϻ��г���������ϸ�ࣩ��2�������г���������ϸ�ࣩ��3���Ϻ�ָ����4������ָ�������Ϻ����棻6��������С�壻
                // 7���Ϻ�B�ɣ�8������B�ɣ�9���Ϻ��ƴ���; 10�����ڴ�ҵ�壻
                // �Ϻ��г����õ�����ʾ�������г�����˫����ʾ��Ŀǰ��ʱ���ã��Ȳ鿴�漰����Щ������Ҫ�޸ġ�
  CString m_strStakeCode; // ��Ʊ���롣��λ��ǰ��λΪ�г�ǰ׺������λΪ���ִ��롣��sh600601��sz000001
  CString m_strStakeName; // ��Ʊ����
  CStringW m_strStakeNameReadIn; // ����Ĺ�Ʊ���ƣ�UniCode��ʽ��Ŀǰ��δʹ�ã�
  long m_lDayLineStartDate;	// ����������ʼ�ա�����Ǵ���������ʷ����ʱ�õ�����ʼ�����գ�
  long m_lOffsetInContainer; // �������е�ƫ����
  long m_lDayLineEndDate;	// �������ݸ����ա�����Ǵ���������ʷ����ʱ�õ��������գ�
  long m_lIPOStatus; // ͨ��������ʷ���߲�ѯ�����ֻ��ǰ׺��Ϣ��û��ʵ�����ݣ�����ȷ��û��ʵ�ʽ��ס�����������£�����ʵʱ���������ݣ�ֻ��Ϊ����ѡ�Ĭ�������Ϊ������
                 // δ���У���Ч��Ʊ���룩Ϊ__STOCK_NULL__������Ϊ__STOCK_IPOED__����ͨ��IPO����δ���л�����Ϊ__STOCK_DELISTED�����������δ���֣������Ժ���
  short	m_nHand;	// ÿ�ֹ���
};

typedef shared_ptr<CStakeCode> CStakeCodePtr;