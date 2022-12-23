#pragma once

// 股票代码采用国家标准制式
bool IsShanghaiExchange(const CString& strStockCode);
bool IsShenzhenExchange(const CString& strStockCode);
CString GetStockSymbol(const CString& strStockCode);
CString GetStockExchange(const CString& strStockCode);
CString CreateStockCode(const CString& strStockExchange, const CString& strStockSymbol);
// 股票代码转换
CString XferSinaToStandard(const CString& strSina);
CString XferSinaToNetease(const CString& strSina);
CString XferNeteaseToStandard(const CString& strNetease);
CString XferNeteaseToSina(const CString& strNetease);
CString XferTengxunToStandard(const CString& strTengxun);
CString XferStandardToSina(const CString& strStandard);
CString XferStandardToNetease(const CString& strStandard);
CString XferStandardToTengxun(const CString& strStandard);
