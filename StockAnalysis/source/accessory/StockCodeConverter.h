#pragma once

using namespace std;
#include<vector>
#include<string>

// ��Ʊ������ù��ұ�׼��ʽ
bool IsShanghaiExchange(CString strStockCode);
bool IsShenzhenExchange(CString strStockCode);
CString GetStockSymbol(CString strStockCode);
CString GetStockExchange(CString strStockCode);
CString CreateStockCode(CString strStockExchange, CString strStockSymbol);
// ��Ʊ����ת��
CString XferSinaToStandred(CString strSina);
CString XferSinaToNetease(CString strSina);
CString XferNeteaseToStandred(CString strNetease);
CString XferNeteaseToSina(CString strNetease);
CString XferTengxunToStandred(CString strTengxun);
CString XferStandredToSina(CString strStandred);
CString XferStandredToNetease(CString strStandred);
CString XferStandredToTengxun(CString strStandred);
