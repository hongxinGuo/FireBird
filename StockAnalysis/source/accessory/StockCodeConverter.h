#pragma once

// 股票代码采用国家标准制式
bool IsShanghaiExchange(CString strStockCode);
bool IsShenzhenExchange(CString strStockCode);
CString GetStockSymbol(CString strStockCode);
CString GetStockExchange(CString strStockCode);
CString CreateStockCode(CString strStockExchange, CString strStockSymbol);
// 股票代码转换
CString XferSinaToStandard(CString strSina);
CString XferSinaToNetease(CString strSina);
CString XferNeteaseToStandard(CString strNetease);
CString XferNeteaseToSina(CString strNetease);
CString XferTengxunToStandard(CString strTengxun);
CString XferStandardToSina(CString strStandard);
CString XferStandardToNetease(CString strStandard);
CString XferStandardToTengxun(CString strStandard);
