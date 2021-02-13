// SetRealTimeData.h : CSetRealTimeData ���ʵ��

// CSetRealTimeData ʵ��

// ���������� 2019��5��26��, 8:09

#include "stdafx.h"
#include"globedef.h"
#include"accessory.h"
#include "SetRealTimeData.h"
IMPLEMENT_DYNAMIC(CSetRealTimeData, CRecordset)

CSetRealTimeData::CSetRealTimeData(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Time = _T("0");
  m_StockCode = _T("");
  m_LastClose = _T("0");
  m_Open = _T("0");
  m_High = _T("0");
  m_Low = _T("0");
  m_New = _T("0");
  m_Volume = _T("0");
  m_Amount = _T("0");
  m_Stroke = _T("0");
  m_PBuy5 = _T("0");
  m_VBuy5 = _T("0");
  m_PBuy4 = _T("0");
  m_VBuy4 = _T("0");
  m_PBuy3 = _T("0");
  m_VBuy3 = _T("0");
  m_PBuy2 = _T("0");
  m_VBuy2 = _T("0");
  m_PBuy1 = _T("0");
  m_VBuy1 = _T("0");
  m_PSell1 = _T("0");
  m_VSell1 = _T("0");
  m_PSell2 = _T("0");
  m_VSell2 = _T("0");
  m_PSell3 = _T("0");
  m_VSell3 = _T("0");
  m_PSell4 = _T("0");
  m_VSell4 = _T("0");
  m_PSell5 = _T("0");
  m_VSell5 = _T("0");
  m_nFields = 31;
}
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CSetRealTimeData::GetDefaultConnect() {
  return GetChinaMarketSchemaConnect();
}

CString CSetRealTimeData::GetDefaultSQL() {
  return _T("[RealTimeData]");
}

void CSetRealTimeData::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Time]"), m_Time);
  RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
  RFX_Text(pFX, _T("[LastClose]"), m_LastClose);
  RFX_Text(pFX, _T("[Open]"), m_Open);
  RFX_Text(pFX, _T("[High]"), m_High);
  RFX_Text(pFX, _T("[Low]"), m_Low);
  RFX_Text(pFX, _T("[New]"), m_New);
  RFX_Text(pFX, _T("[Volume]"), m_Volume);
  RFX_Text(pFX, _T("[Amount]"), m_Amount);
  RFX_Text(pFX, _T("[Stroke]"), m_Stroke);
  RFX_Text(pFX, _T("[PBuy5]"), m_PBuy5);
  RFX_Text(pFX, _T("[VBuy5]"), m_VBuy5);
  RFX_Text(pFX, _T("[PBuy4]"), m_PBuy4);
  RFX_Text(pFX, _T("[VBuy4]"), m_VBuy4);
  RFX_Text(pFX, _T("[PBuy3]"), m_PBuy3);
  RFX_Text(pFX, _T("[VBuy3]"), m_VBuy3);
  RFX_Text(pFX, _T("[PBuy2]"), m_PBuy2);
  RFX_Text(pFX, _T("[VBuy2]"), m_VBuy2);
  RFX_Text(pFX, _T("[PBuy1]"), m_PBuy1);
  RFX_Text(pFX, _T("[VBuy1]"), m_VBuy1);
  RFX_Text(pFX, _T("[PSell1]"), m_PSell1);
  RFX_Text(pFX, _T("[VSell1]"), m_VSell1);
  RFX_Text(pFX, _T("[PSell2]"), m_PSell2);
  RFX_Text(pFX, _T("[VSell2]"), m_VSell2);
  RFX_Text(pFX, _T("[PSell3]"), m_PSell3);
  RFX_Text(pFX, _T("[VSell3]"), m_VSell3);
  RFX_Text(pFX, _T("[PSell4]"), m_PSell4);
  RFX_Text(pFX, _T("[VSell4]"), m_VSell4);
  RFX_Text(pFX, _T("[PSell5]"), m_PSell5);
  RFX_Text(pFX, _T("[VSell5]"), m_VSell5);
}
/////////////////////////////////////////////////////////////////////////////
// CSetRealTimeData ���

#ifdef _DEBUG
void CSetRealTimeData::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetRealTimeData::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG