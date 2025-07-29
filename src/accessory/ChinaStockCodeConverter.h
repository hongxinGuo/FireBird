#pragma once

using std::string;

//import std;

// 股票代码采用国家标准制式
[[nodiscard]] inline bool IsShanghaiExchange(const string& strStandardCode) { return strStandardCode.substr(strStandardCode.length() - 2, 2).compare(_T("SS")) == 0; }
[[nodiscard]] inline bool IsShenzhenExchange(const string& strStandardCode) { return strStandardCode.substr(strStandardCode.length() - 2, 2).compare(_T("SZ")) == 0; }
[[nodiscard]] inline string GetStockSymbol(const string& strStandardCode) { return strStandardCode.substr(0, strStandardCode.length() - 3); }
[[nodiscard]] inline string GetStockExchange(const string& strStandardCode) { return (strStandardCode.substr(strStandardCode.length() - 2, 2)); }
[[nodiscard]] inline string CreateStockCode(const string& strStockExchange, const string& strStockSymbol) { return strStockSymbol + _T(".") + strStockExchange; }

// 股票代码转
[[nodiscard]] string XferSinaToStandard(const string& strSina);
[[nodiscard]] string XferSinaToStandard(const string_view& svSina);
[[nodiscard]] string XferSinaToNetease(const string& strSina);
[[nodiscard]] string XferNeteaseToStandard(const string& strNetease);
[[nodiscard]] string XferNeteaseToStandard(const string_view& svNetease);
[[nodiscard]] string XferNeteaseToSina(const string& strNetease);
[[nodiscard]] inline string XferTengxunToStandard(const string& strTengxun) { return XferSinaToStandard(strTengxun); }
[[nodiscard]] string XferStandardToSina(const string& strStandard);
[[nodiscard]] string XferStandardToNetease(const string& strStandard);
[[nodiscard]] inline string XferStandardToTengxun(const string& strStandardCode) { return XferStandardToSina(strStandardCode); }
