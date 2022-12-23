#pragma once

// ��Ʊ������ù��ұ�׼��ʽ
bool IsShanghaiExchange(const CString& strStockCode);
bool IsShenzhenExchange(const CString& strStockCode);
CString GetStockSymbol(const CString& strStockCode);
CString GetStockExchange(const CString& strStockCode);
CString CreateStockCode(const CString& strStockExchange, const CString& strStockSymbol);
// ��Ʊ����ת��
CString XferSinaToStandard(const CString& strSina);
CString XferSinaToNetease(const CString& strSina);
CString XferNeteaseToStandard(const CString& strNetease);
CString XferNeteaseToSina(const CString& strNetease);
CString XferTengxunToStandard(const CString& strTengxun);
CString XferStandardToSina(const CString& strStandard);
CString XferStandardToNetease(const CString& strStandard);
CString XferStandardToTengxun(const CString& strStandard);
