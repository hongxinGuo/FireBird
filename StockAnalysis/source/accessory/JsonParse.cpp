//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// json���ݽ����������λ�ڴ��ļ��С�
// ������Ŀǰ���ʱ�ĺ������ʶ�DEBUGģʽʱҲҪ����ȫ���Ż���/GL����ֻ�����������ڹ涨ʱ���ڴ��������ݡ�
// Neteaseʵʱ������Ŀǰ����Ҫ��ע�ģ�ÿ300�������900������ʱ��ϵͳ�����϶�ռһ�����Ĵ������ˡ�������к�ʱ�����񣬾���Ҫ
// ������䵽�������Ĵ������ϣ������ͻᵼ��ϵͳ����Ӧʱ�����ӣ�����ϵͳ�ĸ���Ҳ�����ӡ�
//
// ��DEBUGģʽ�£�boost PTree�ٶȱ�Nlohmann json�죬��Releaseģʽ��nlohmann json���ٶȱ�boost ptree��50%���ҡ�
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "JsonParse.h"

#include"WebRTData.h"
#include"WebInquirer.h"
#include"ChinaMarket.h"
#include"WebRTDataContainer.h"

using namespace std;
#include<string>

bool ParseWithPTree(ptree& pt, string& s) {
	stringstream ss(s);
	try {
		read_json(ss, pt);
	}
	catch (ptree_error& e) {
#ifndef _DEBUG
		char buffer[180];
		for (int i = 0; i < 80; i++) {
			buffer[i] = s.at(i);
		}
		buffer[80] = 0x000;
		CString str = buffer;
		ReportJSonErrorToSystemMessage(_T("PTree JSon Reading Error ") + str + _T(" "), e);
#endif
		return false;
	}
	return true;
}

bool ParseWithPTree(shared_ptr<ptree>& ppt, string& s) {
	ASSERT(ppt != nullptr);
	stringstream ss(s);
	try {
		read_json(ss, *ppt);
	}
	catch (ptree_error& e) {
#ifndef _DEBUG
		CString str = s.c_str();
		str = str.Left(160);
		ReportJSonErrorToSystemMessage(_T("PTree JSon Reading Error ") + str + _T(" "), e);
#endif
		return false;
	}
	return true;
}

bool ConvertToWJSON(wptree& pt, string& s) {
	wstring ws = to_wide_string(s);
	wstringstream ss(ws);
	try {
		read_json(ss, pt);
	}
	catch (ptree_error& e) {
		//ReportJSonErrorToSystemMessage(_T("PTree JSon Reading Error "), e);
		return false;
	}
	return true;
}

wstring to_wide_string(const std::string& input) {
	char* pBuffer;
	long lLength = input.size();
	pBuffer = new char[lLength + 1];
	for (int i = 0; i < input.size(); i++) {
		pBuffer[i] = input.at(i);
	}
	pBuffer[lLength] = 0x000;
	WCHAR* pBufferW = new WCHAR[lLength * 2];

	long lReturnSize = MultiByteToWideChar(CP_UTF8, 0, pBuffer, lLength, pBufferW, lLength * 2);
	pBufferW[lReturnSize] = 0x000;
	wstring ws = pBufferW;

	delete[]pBuffer;
	delete[]pBufferW;

	return ws;
}

string to_byte_string(const wstring& input) {
	WCHAR* pBufferW;
	long lLength = input.size();
	pBufferW = new WCHAR[lLength + 1];
	for (int i = 0; i < lLength + 1; i++) pBufferW[i] = 0x000;
	for (int i = 0; i < input.size(); i++) {
		pBufferW[i] = input.at(i);
	}
	char* pBuffer = new char[lLength * 2];

	long lReturnSize = WideCharToMultiByte(CP_UTF8, 0, pBufferW, lLength, pBuffer, lLength * 2, NULL, NULL);
	pBuffer[lReturnSize] = 0x000;
	string s = pBuffer;

	delete[]pBuffer;
	delete[]pBufferW;

	return s;
}

void ReportJsonError(json::parse_error& e, std::string& s) {
	char buffer[180]{}, buffer2[100];
	int i = 0;
	CString str = e.what();
	gl_systemMessage.PushErrorMessage(_T("Nlohmann JSon Reading Error ") + str);
	for (i = 0; i < 180; i++) buffer[i] = 0x000;
	for (i = 0; i < 180; i++) {
		if ((e.byte - 90 + i) < s.size()) {
			buffer[i] = s.at(e.byte - 90 + i);
		}
		else break;
	}
	sprintf_s(buffer2, _T("%d  "), (long)(s.size()));
	str = buffer2;
	sprintf_s(buffer2, _T("%d  "), (long)(e.byte));
	str += buffer2;
	sprintf_s(buffer2, _T("%d  "), i);
	str += buffer2;
	str += buffer;
	gl_systemMessage.PushErrorMessage(str);
}

string ptreeGetString(ptree& pt, const char* szKey, const char* szDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		s = szDefault;
	}
	if (s.compare(_T("null")) == 0) return szDefault;
	return s;
}

string ptreeGetString(ptree* ppt, const char* szKey, const char* szDefault) {
	string s;
	try {
		s = ppt->get<string>(szKey);
	}
	catch (ptree_error&) {
		s = szDefault;
	}
	if (s.compare(_T("null")) == 0) return szDefault;
	return s;
}

string ptreeGetString(shared_ptr<ptree> ppt, const char* szKey, const char* szDefault) {
	string s;
	try {
		s = ppt->get<string>(szKey);
	}
	catch (ptree_error&) {
		s = szDefault;
	}
	if (s.compare(_T("null")) == 0) return szDefault;
	return s;
}

double ptreeGetDouble(ptree& pt, const char* szKey, double dDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		return dDefault;
	}
	if (s.compare(_T("null")) == 0) return dDefault;
	else return stod(s);
}

int ptreeGetInt(ptree& pt, const char* szKey, int iDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		return iDefault;
	}
	if (s.compare(_T("null")) == 0) return iDefault;
	else return stoi(s);
}

long long ptreeGetLongLong(ptree& pt, const char* szKey, long long llDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		return llDefault;
	}
	if (s.compare(_T("null")) == 0) return llDefault;
	else return stoll(s);
}

long ptreeGetLong(ptree& pt, const char* szKey, long lDefault) {
	string s;
	try {
		s = pt.get<string>(szKey);
	}
	catch (ptree_error&) {
		return lDefault;
	}
	if (s.compare(_T("null")) == 0) return lDefault;
	else return stol(s);
}

bool ptreeGetChild(ptree& inputPt, const char* szKey, ptree* outputPpt) {
	try {
		*outputPpt = inputPt.get_child(szKey);
	}
	catch (ptree_error&) {
		return false;
	}
	return true;
}

void ReportJSonErrorToSystemMessage(CString strPrefix, ptree_error& e) {
	CString strError = strPrefix;
	strError += e.what();
	gl_systemMessage.PushErrorMessage(strError);
}

void ReportJSonErrorToSystemMessage(CString strPrefix, std::string data, ptree_error& e) {
	CString strData = data.c_str();

	ReportJSonErrorToSystemMessage(strPrefix + strData.Left(80) + _T(" "), e);
}

bool ParseWithNlohmannJSon(json* pjs, std::string& s, long lBeginPos, long lEndPos) {
	try {
		*pjs = json::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	}
	catch (json::parse_error& e) {
		gl_systemMessage.PushErrorMessage("nlohmann json parse error");
		//ReportJsonError(e, s);
		pjs = nullptr;
		return false;
	}
	return true;
}

void ParseNeteaseRTData(json* pjs, vector<CWebRTDataPtr>& vWebData) {
	for (json::iterator it = pjs->begin(); it != pjs->end(); ++it) {
		if (gl_systemStatus.IsExitingSystem()) return;
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		pRTData->SetDataSource(__NETEASE_RT_WEB_DATA__);
		if (pRTData->ParseNeteaseDataWithNlohmannJSon(it)) {
			vWebData.push_back(pRTData);
		}
	}
}

void ParseNeteaseRTData(ptree* ppt, vector<CWebRTDataPtr>& vWebData) {
	for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		pRTData->SetDataSource(__NETEASE_RT_WEB_DATA__);
		if (pRTData->ParseNeteaseDataWithPTree(it)) {
			vWebData.push_back(pRTData);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// Ҫ��ȡ�������飬�������ݽӿڣ�http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// ʹ��json�������Ѿ�û�д��������ˡ�(ż�������У�����ÿ���ӳ���һ�Σ���
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool ParseNeteaseRTDataWithPTree(void) {
	CWebDataPtr pWebDataReceived = nullptr;
	const size_t lTotalData = gl_WebInquirer.NeteaseRTDataSize();
	string ss;
	shared_ptr<ptree> ppt = nullptr;
	int iTotal = 0;
	bool fProcess = true;
	vector<CWebRTDataPtr> vWebRTData;
	ptree* ppt2 = nullptr;

	for (int i = 0; i < lTotalData; i++) {
		fProcess = true;
		pWebDataReceived = gl_WebInquirer.PopNeteaseRTData();
		if (!pWebDataReceived->IsParsed()) {
			if (!pWebDataReceived->CreatePropertyTree(21, 2)) { // ��������ǰ21λΪǰ׺������λΪ��׺
				fProcess = false;
			}
		}
		else {
			ASSERT(pWebDataReceived->GetJSon() == nullptr);
		}
		if (fProcess && pWebDataReceived->IsParsed()) {
			ppt = pWebDataReceived->GetPTree();
			ppt2 = ppt.get();
			ParseNeteaseRTData(ppt2, vWebRTData);
			for (auto& pRTData : vWebRTData) {
				gl_WebRTDataContainer.PushNeteaseData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
			}
			iTotal = vWebRTData.size();
		}
		pWebDataReceived = nullptr;
	}
	gl_pChinaMarket->IncreaseRTDataReceived(iTotal);

	return true;
}

int ParseNeteaseRTDataWithPTree(CWebDataPtr pData) {
	string ss;
	shared_ptr<ptree> ppt = nullptr;
	int iTotal = 0;
	bool fProcess = true;
	vector<CWebRTDataPtr> vWebRTData;
	ptree* ppt2 = nullptr;

	fProcess = true;
	if (!pData->IsParsed()) {
		if (!pData->CreatePropertyTree(21, 2)) { // ��������ǰ21λΪǰ׺������λΪ��׺
			fProcess = false;
		}
	}
	else {
		ASSERT(pData->GetJSon() == nullptr);
	}
	if (fProcess && pData->IsParsed()) {
		ppt = pData->GetPTree();
		ppt2 = ppt.get();
		ParseNeteaseRTData(ppt2, vWebRTData);
		for (auto& pRTData : vWebRTData) {
			gl_WebRTDataContainer.PushNeteaseData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
		}
	}
	return vWebRTData.size();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// Ҫ��ȡ�������飬�������ݽӿڣ�http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// ʹ��json�������Ѿ�û�д��������ˡ�(ż�������У�����ÿ���ӳ���һ�Σ���
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool ParseNeteaseRTDataWithNlohmannJSon(void) {
	CWebDataPtr pWebDataReceived = nullptr;
	const size_t lTotalData = gl_WebInquirer.NeteaseRTDataSize();
	string ss;
	json* pjs = nullptr;
	int iTotalActive = 0;
	bool fProcess = true;
	vector<CWebRTDataPtr> vWebRTData;

	for (int i = 0; i < lTotalData; i++) {
		fProcess = true;
		pWebDataReceived = gl_WebInquirer.PopNeteaseRTData();
		if (!pWebDataReceived->IsParsed()) {
			// ��ȡ����ʵʱ����ʱ�á�
			//SaveToFile(_T("C:\\StockAnalysis\\NeteaseRTData.json"), pWebDataReceived->GetDataBuffer());
			if (!pWebDataReceived->CreateNlohmannJSon(21, 2)) { // ��������ǰ21λΪǰ׺������λΪ��׺
				fProcess = false;
			}
		}
		if (fProcess && pWebDataReceived->IsParsed()) {
			pjs = pWebDataReceived->GetJSon();
			ParseNeteaseRTData(pjs, vWebRTData);
			for (auto& pRTData : vWebRTData) {
				gl_WebRTDataContainer.PushNeteaseData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
			}
			iTotalActive = vWebRTData.size();
		}
		vWebRTData.clear();
	}
	gl_pChinaMarket->IncreaseRTDataReceived(iTotalActive);

	return true;
}

int ParseNeteaseRTDataWithNlohmannJSon(CWebDataPtr pData) {
	string ss;
	json* pjs = nullptr;
	bool fProcess = true;
	vector<CWebRTDataPtr> vWebRTData;

	fProcess = true;
	if (!pData->IsParsed()) {
		// ��ȡ����ʵʱ����ʱ�á�
		//SaveToFile(_T("C:\\StockAnalysis\\NeteaseRTData.json"), pWebDataReceived->GetDataBuffer());
		if (!pData->CreateNlohmannJSon(21, 2)) { // ��������ǰ21λΪǰ׺������λΪ��׺
			fProcess = false;
		}
	}
	if (fProcess && pData->IsParsed()) {
		pjs = pData->GetJSon();
		ParseNeteaseRTData(pjs, vWebRTData);
		for (auto& pRTData : vWebRTData) {
			gl_WebRTDataContainer.PushNeteaseData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
		}
	}
	return vWebRTData.size();
}

// ��PTree����ȡ��utf-8�ַ���ת��ΪCString
CString XferToCString(string s) {
	CString strName3;
	wstring wsName;
	CStringW strWName;

	wsName = to_wide_string(s); // ������utf8ת�ɿ��ֽ��ַ���
	strWName = wsName.c_str(); // ����׼��Ŀ��ֽ��ַ���ת����CStringW��ʽ��
	strName3 = strWName; // ��CStringW��ʽת����CString
	return strName3;
}