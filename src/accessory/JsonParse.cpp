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

#include"WebData.h"
#include"WebRTData.h"

#include<string>
#include<memory>

#include "ChinaStockCodeConverter.h"
#include "InfoReport.h"
#include "JsonGetValue.h"

#include"simdjsonGetValue.h"
using namespace simdjson;

using namespace std;

void ReportJsonError(const json::parse_error& e, const std::string& s) {
	char buffer[180]{}, buffer2[100];
	int i;
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

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat) {
	gl_systemMessage.PushErrorMessage(strPrefix + strWhat);
}

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat, const char* jsonData) {
	string s(jsonData);
	s = s.substr(0, 40);
	CString str = strWhat;
	str += " ";
	str += s.c_str();

	ReportJSonErrorToSystemMessage(strPrefix, str);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
// ����ʵʱ���ݵĸ�ʽΪjavascript
// 
// todo ���ǽ�pRTData->ReadSinaData������Ϊ�߳�ģʽ����ִ�н����������������в��������ͬ��Ʊ��ʵʱ���ݣ��ʶ��������ͬʱ����ͬһ����Ʊ�����⣬
// ���Կ��Բ��н���
//
//////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(const CWebDataPtr& pWebData) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();
	try {
		pWebData->ResetCurrentPos();
		while (!pWebData->IsLastDataParagraph()) {
			const string_view svData = pWebData->GetCurrentSinaData();
			auto pRTData = make_shared<CWebRTData>();
			pRTData->ReadSinaData(svData);
			pvWebRTData->push_back(pRTData);
		}
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadSinaData�쳣 "), e);
		return pvWebRTData;
	}
	return pvWebRTData;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����б���ѯ�Ĺ�Ʊ��Ϊ�����й�Ʊʱ����Ѷʵʱ��Ʊ�������᷵��һ��21���ַ������ַ�����v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IsTengxunRTDataInvalid(const CWebDataPtr& pWebDataReceived) {
	const string_view sv = pWebDataReceived->GetStringView(0, 21);

	if (sv.compare(_T("v_pv_none_match=\"1\";\n")) == 0) {
		ASSERT(pWebDataReceived->GetBufferLength() == 21);
		return true;
	}
	return false;
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
// 20200101�սṹ��
// v_sz000001="51~ƽ������~000001~15.59~15.90~15.75~1046363~518391~527971~
//             15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~
//             20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~
//             15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~
//             0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82";\n
//
// 20230627�սṹ��
// v_sh600026="1~��Զ����~600026~12.25~12.40~12.34~366190~150067~216123~
//						 12.24~36~12.23~803~12.22~501~12.21~905~12.20~1899~12.25~1026~12.26~1158~12.27~645~12.28~615~12.29~139~~
//						 20230627155915~-0.15~-1.21~12.44~11.95~12.25/366190/445074382~366190~44507~1.05~23.12~~
//						 12.44~11.95~3.95~425.66~584.42~1.79~13.64~11.16~1.01~561~12.15~13.33~40.10~~~1.53~44507.4382~0.0000~0~~GP-A~1.66~-8.85~0.00~
//						 7.73~4.16~21.04~8.58~1.16~7.64~-11.81~3474776395~4770776395~7.26~- 34.49~3474776395~~~17.22~0.16~~CNY~0~___D__F__N";\n
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
shared_ptr<vector<CWebRTDataPtr>> ParseTengxunRTData(const CWebDataPtr& pWebData) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();
	try {
		pWebData->ResetCurrentPos();
		if (IsTengxunRTDataInvalid(pWebData)) return pvWebRTData; // ������21���ַ����ĺ������Է������Ҳ���Է�����ǰ�档
		while (!pWebData->IsLastDataParagraph()) {
			auto pRTData = make_shared<CWebRTData>();
			const string_view svData = pWebData->GetCurrentTengxunData();
			pRTData->ReadTengxunData(svData);
			pvWebRTData->push_back(pRTData);
		}
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadTengxunData�쳣 "), e);
		return pvWebRTData;
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
CDayLineWebDataPtr ParseNeteaseDayLine(const CWebDataPtr& pWebData) {
	auto pData = make_shared<CDayLineWebData>();

	pData->TransferWebDataToBuffer(pWebData);
	pData->ProcessNeteaseDayLineData(); //pData����������������ģ��������ڵ���ǰ�档

	return pData;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// ��Ѷ�������ݽṹ��
// {
// "code":0,
// "msg":"",
// "data": { "sh600601": { "day":	[
//																	["2023-01-19","2.550","2.600","2.610","2.550","86162.000"],
//																	["2023-01-20","2.600","2.620","2.620","2.590","100735.000"]
//																],
//													"qt":{},
//													"mx_price":{"mx":[],"price":[]},
//													"prec":"2.560",
//													"version":"16"
//												}
//					}
// }
//
// ʹ��simdjson�����ٶ�releaseģʽ�±�Nholmann json��50%����debugģʽ����һ����
//
////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CDayLinePtr>> ParseTengxunDayLine(const string_view& svData, const CString& strStockCode) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	const CString strStockSymbol = XferTengxunToStandard(strStockCode);
	try {
		long year, month, day;
		string_view sv;
		double dLastClose = 0;
		const simdjson::padded_string jsonPadded(svData);
		ondemand::parser parser;
		ondemand::document doc = parser.iterate(jsonPadded);
		// ��ʹ������strStockCode�ҵ���������ķ���
		//ondemand::value data = doc["data"];
		//ondemand::field field = *data.get_object().begin();
		//ondemand::value stock = field.value();
		//ondemand::array dayArray = stock["day"].get_array();
		ondemand::array dayArray = doc["data"][strStockCode]["day"].get_array(); // ʹ��strStockCode�ҵ���������
		for (ondemand::value dayLine : dayArray) {
			auto pDayLine = make_shared<CDayLine>();
			pDayLine->SetStockSymbol(strStockSymbol);
			pDayLine->SetLastClose(dLastClose * 1000);
			ondemand::array_iterator it = dayLine.get_array().begin();
			ondemand::value item = *it;
			sv = jsonGetStringView(item);
			sscanf_s(sv.data(), _T("%4d-%02d-%02d"), &year, &month, &day);
			pDayLine->SetDate(year * 10000 + month * 100 + day);
			item = *++it;
			sv = jsonGetStringView(item);
			pDayLine->SetOpen(atof(sv.data()) * 1000);
			item = *++it;
			sv = jsonGetStringView(item);
			const double dClose = atof(sv.data());
			pDayLine->SetClose(dClose * 1000);
			dLastClose = dClose;
			item = *++it;
			sv = jsonGetStringView(item);
			pDayLine->SetHigh(atof(sv.data()) * 1000);
			item = *++it;
			sv = jsonGetStringView(item);
			pDayLine->SetLow(atof(sv.data()) * 1000);
			item = *++it;
			sv = jsonGetStringView(item);
			pDayLine->SetVolume(atof(sv.data()) * 100);

			pvDayLine->push_back(pDayLine);
		}
	}
	catch (json::exception&) {
		return pvDayLine;
	}
	return pvDayLine;
}

//
// ��Ѷ�������ݽṹ��
// {
// "code":0,
// "msg":"",
// "data":
//   {
//   "sh600601":
//      { "day":
//				[ ["2023-01-19","2.550","2.600","2.610","2.550","86162.000"],
//					["2023-01-20","2.600","2.620","2.620","2.590","100735.000"]],
//				"qt":{},
//				"mx_price":{"mx":[],"price":[]},
//				"prec":"2.560",
//				"version":"16"
//		  }
//	 }
// }
//
CDayLineWebDataPtr ParseTengxunDayLine(const CWebDataPtr& pWebData) {
	auto pDayLineData = make_shared<CDayLineWebData>();
	const CString strSymbol = pWebData->GetStockCode();
	const CString strDisplaySymbol = gl_dataContainerChinaStock.GetStock(strSymbol)->GetDisplaySymbol();
	const string_view svData = pWebData->GetStringView(0, pWebData->GetBufferLength());

	const shared_ptr<vector<CDayLinePtr>> pvDayLine = ParseTengxunDayLine(svData, XferStandardToTengxun(pWebData->GetStockCode()));
	ranges::sort(*pvDayLine, [](const CDayLinePtr& pData1, const CDayLinePtr& pData2) { return pData1->GetMarketDate() < pData2->GetMarketDate(); });
	for (const auto& pDayLine : *pvDayLine) {
		pDayLine->SetStockSymbol(strSymbol);
		pDayLine->SetDisplaySymbol(strDisplaySymbol);
		pDayLineData->AppendDayLine(pDayLine);
	}
	return pDayLineData;
}
