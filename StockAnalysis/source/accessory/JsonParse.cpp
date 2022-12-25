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

#include<string>

#include "JsonParse.h"

#include"WebRTData.h"
#include"SaveAndLoad.h"

wstring to_wide_string(const std::string& input) {
	const long lLength = input.size();
	const auto pBuffer = new char[lLength + 1];

	for (int i = 0; i < input.size(); i++) { pBuffer[i] = input.at(i); }
	pBuffer[lLength] = 0x000;
	const auto pBufferW = new WCHAR[lLength * 2];

	const long lReturnSize = MultiByteToWideChar(CP_UTF8, 0, pBuffer, lLength, pBufferW, lLength * 2);
	pBufferW[lReturnSize] = 0x000;
	wstring ws = pBufferW;

	delete[]pBuffer;
	delete[]pBufferW;

	return ws;
}

string to_byte_string(const wstring& input) {
	const long lLength = input.size();
	const auto pBufferW = new WCHAR[lLength + 1];

	for (int i = 0; i < lLength + 1; i++) pBufferW[i] = 0x000;
	for (int i = 0; i < input.size(); i++) { pBufferW[i] = input.at(i); }
	const auto pBuffer = new char[lLength * 2];

	const long lReturnSize = WideCharToMultiByte(CP_UTF8, 0, pBufferW, lLength, pBuffer, lLength * 2, NULL, NULL);
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
		if ((e.byte - 90 + i) < s.size()) { buffer[i] = s.at(e.byte - 90 + i); }
		else break;
	}
	sprintf_s(buffer2, _T("%d  "), static_cast<long>(s.size()));
	str = buffer2;
	sprintf_s(buffer2, _T("%d  "), static_cast<long>(e.byte));
	str += buffer2;
	sprintf_s(buffer2, _T("%d  "), i);
	str += buffer2;
	str += buffer;
	gl_systemMessage.PushErrorMessage(str);
}

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat) { gl_systemMessage.PushErrorMessage(strPrefix + strWhat); }

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
//
//  ����ʵʱ����վ�㣺https://hq.sinajs.cn/list=sh601006
// OpenURLʱ����Ҫ����	m_strHeaders = _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n");
//
// var hq_str_sh601006="������·,27.55,27.25,26.91,27.55,26.20,26.91,26.92,
//                     22114263,589824680,4695,26.91,57590,26.90,14700,26.89,14300,
//                     26.88,15100,26.87,3100,26.92,8900,26.93,14230,26.94,25150,26.95,15220,26.96,2008-01-11,15:05:32,00";
//
// ��Ч���ݸ�ʽΪ��var hq_str_sh688801="";
//
// ����ַ������������ƴ����һ�𣬲�ͬ����������ö��Ÿ����ˣ����ճ���Ա��˼·��˳��Ŵ�0��ʼ��
// 0����������·������Ʊ���֣�
// 1����27.55�壬���տ��̼ۣ�
// 2����27.25�壬�������̼ۣ�
// 3����26.91�壬��ǰ�۸�
// 4����27.55�壬������߼ۣ�
// 5����26.20�壬������ͼۣ�
// 6����26.91�壬����ۣ�������һ�����ۣ�
// 7����26.92�壬�����ۣ�������һ�����ۣ�
// 8����22114263�壬�ɽ��Ĺ�Ʊ�������ڹ�Ʊ������һ�ٹ�Ϊ������λ��������ʹ��ʱ��ͨ���Ѹ�ֵ����һ�٣�
// 9����589824680�壬�ɽ�����λΪ��Ԫ����Ϊ��һĿ��Ȼ��ͨ���ԡ���Ԫ��Ϊ�ɽ����ĵ�λ������ͨ���Ѹ�ֵ����һ��
// 10����4695�壬����һ������4695�ɣ���47�֣�
// 11����26.91�壬����һ�����ۣ�
// 12����57590�壬�������
// 13����26.90�壬�������
// 14����14700�壬��������
// 15����26.89�壬��������
// 16����14300�壬�����ġ�
// 17����26.88�壬�����ġ�
// 18����15100�壬�����塱
// 19����26.87�壬�����塱
// 20����3100�壬����һ���걨3100�ɣ���31�֣�
// 21����26.92�壬����һ������
// (22, 23), (24, 25), (26, 27), (28, 29)�ֱ�Ϊ���������������ĵ������
// 30����2008 - 01 - 11�壬���ڣ���������Ϊ�����г������ڣ�
// 31����15:05:32�壬ʱ�䣻����ʱ��Ϊ�����г���ʱ�䣬�˴�Ϊ������������׼ʱ�䣩
// 32����00����  ��������
//////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(CWebDataPtr pWebData) {
	int iTotal = 0;
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	// ��ȡʵʱ����ʱ�á�Ϊ�˲��Խ����ٶ�
	if (static int i = 0; i < pWebData->GetBufferLength()) {
		string s = pWebData->GetDataBuffer();
		//SaveToFile(_T("C:\\StockAnalysis\\SinaRTData.dat"), s.c_str());
		i = pWebData->GetBufferLength();
	}
	pWebData->ResetCurrentPos();
	while (!pWebData->IsProcessedAllTheData()) {
		auto pRTData = make_shared<CWebRTData>();
		if (pRTData->ReadSinaData(pWebData)) {
			iTotal++;
			pvWebRTData->push_back(pRTData);
		}
		else {
			gl_systemMessage.PushErrorMessage(_T("����ʵʱ���ݽ�������ʧ����Ϣ"));
			break; // ��������ݳ����⣬�׵����á�
		}
	}
	return pvWebRTData;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����б���ѯ�Ĺ�Ʊ��Ϊ�����й�Ʊʱ����Ѷʵʱ��Ʊ�������᷵��һ��21���ַ������ַ�����v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IsTengxunRTDataInvalid(CWebData& WebDataReceived) {
	char buffer[50]{};
	char* pBuffer = buffer;
	const CString strInvalidStock = _T("v_pv_none_match=\"1\";\n"); // ��Ϊ��Ч��Ʊ��ѯ�������ݸ�ʽ����21���ַ�

	WebDataReceived.GetData(pBuffer, 21, WebDataReceived.GetCurrentPos());
	buffer[21] = 0x000;
	const CString str1 = buffer;

	if (str1.Compare(strInvalidStock) == 0) {
		ASSERT(WebDataReceived.GetBufferLength() == 21);
		return true;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// ��Ѷʵʱ���ݣ������������ѯ��Ʊ����Ϊ������ʱ��ֻ�Ǽ��Թ������������ݡ��ʶ���ѯ900����Ʊ�����ص�������ҪС��900.
// ֻ�е����еĲ�ѯ��Ʊ��Ϊ������ʱ���ŷ���һ��21���ַ�����v_pv_none_match=\"1\";\n
//
//
// �������ļ�file�ж�ȡ��Ѷ��ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
//
// Ҫ��ȡ�������飬�������ݽӿڣ�http://qt.gtimg.cn/q=sz002818
//
// v_sz000001="51~ƽ������~000001~15.59~15.90~15.75~1046363~518391~527971~
//             15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~
//             20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~
//             15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~
//             0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82";\n
//
// 0: �г����Ϻ���1�����ڣ�51, �۹ɣ�100�� ���ɣ�200����
// 1 : ����
// 2 : ����
// 3 : �ּ�
// 4 : ����
// 5 : ��
// 6 : �ɽ������֣�
// 7 : ����
// 8 : ���� ����һ�н�����
// 9 : ��һ
// 10 : ��һ�����֣�
// 11 - 18 : ��� - ����
// 19 : ��һ
// 20 : ��һ��
// 21 - 28 : ���� - ����
// 29 : �����ʳɽ� ( �ڶ��н�����
// 30 : ʱ�䣨��ʱ��Ϊ�����г�ʱ�䣬�˴�Ϊ������������׼ʱ�䣩
// 31 : �ǵ�
// 32 : �ǵ� %
// 33 : ���
// 34 : ���
// 35 : �۸� / �ɽ������֣� / �ɽ��Ԫ���� ����ʹ�ô˴������ݴ���36��37�������ݣ������Ϳ���ʹ����Ѷʵʱ�����ˡ�
// 36 : �ɽ������֣�
// 37 : �ɽ����
// 38 : ������
// 39 : ��ӯ��
// 40 : /       (�����н�����
// 41 : ���
// 42 : ���
// 43 : ���
// 44 : ��ͨ��ֵ(��λΪ���ڣ�
// 45 : ����ֵ����λΪ���ڣ�
// 46 : �о���
// 47 : ��ͣ��
// 48 : ��ͣ��   �������н�����
// 49 ������
// 50 :
// 51 : ����
// 52 ����̬��ӯ��
// 53 ����̬��ӯ��
// 54 ��
// 55 ��
// 56 ��
// 57 ���ɽ���
// 58 ��
// 59 ��
// 60 ��
// 61 ��GP-A
// 62 ��
// 63 ��
// 64 ��
// 65 ��
// 66 ��
//
//
// ��Ѷʵʱ�����У��ɽ����ĵ�λΪ�֣��޷��ﵽ��������ľ��ȣ��ɣ����ʶ�ֻ����Ϊ���ݲ���֮�á�
//
//////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseTengxunRTData(CWebDataPtr pWebData) {
	static int i = 0;
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	// ��ȡʵʱ����ʱ�á�Ϊ�˲��Խ����ٶ�
	if (i <= pWebData->GetBufferLength()) {
		string s = pWebData->GetDataBuffer();
		//SaveToFile(_T("C:\\StockAnalysis\\TengxunRTData.dat"), s.c_str());
		i = pWebData->GetBufferLength();
	}

	pWebData->ResetCurrentPos();
	if (!IsTengxunRTDataInvalid(*pWebData)) {
		// ������21���ַ����ĺ������Է������Ҳ���Է�����ǰ�档
		while (!pWebData->IsProcessedAllTheData()) {
			auto pRTData = make_shared<CWebRTData>();
			if (pRTData->ReadTengxunData(pWebData)) { pvWebRTData->push_back(pRTData); }
			else {
				break; // ��������ݳ����⣬�׵����á�
			}
		}
	}
	return pvWebRTData;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// ������������߷������϶�ȡ�Ĺ�Ʊ�������ݡ�
// ������ʽΪ�� ����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n
//
// ��������������ģ��������ڵ���ǰ�档
//
/////////////////////////////////////////////////////////////////////////////////////////////////
CNeteaseDayLineWebDataPtr ParseNeteaseDayLine(CWebDataPtr pWebData) {
	auto pData = make_shared<CNeteaseDayLineWebData>();

	pData->TransferWebDataToBuffer(pWebData);
	pData->ProcessNeteaseDayLineData(); //pData����������������ģ��������ڵ���ǰ�档

	return pData;
}

// ��PTree����ȡ��utf-8�ַ���ת��ΪCString
CString XferToCString(const string& s) {
	CString strName3;
	wstring wsName;
	CStringW strWName;

	wsName = to_wide_string(s); // ������utf8ת�ɿ��ֽ��ַ���
	strWName = wsName.c_str(); // ����׼��Ŀ��ֽ��ַ���ת����CStringW��ʽ��
	strName3 = strWName; // ��CStringW��ʽת����CString
	return strName3;
}
