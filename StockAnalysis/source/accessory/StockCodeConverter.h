#pragma once

// ��Ʊ������ù��ұ�׼��ʽ
bool IsShanghaiExchange(CString strStockCode);
bool IsShenzhenExchange(CString strStockCode);
CString GetStockSymbol(CString strStockCode);
CString GetStockExchange(CString strStockCode);
CString CreateStockCode(CString strStockExchange, CString strStockSymbol);
// ��Ʊ����ת��
CString XferSinaToStandard(CString strSina);
CString XferSinaToNetease(CString strSina);
CString XferNeteaseToStandard(CString strNetease);
CString XferNeteaseToSina(CString strNetease);
CString XferTengxunToStandard(CString strTengxun);
CString XferStandardToSina(CString strStandard);
CString XferStandardToNetease(CString strStandard);
CString XferStandardToTengxun(CString strStandard);
