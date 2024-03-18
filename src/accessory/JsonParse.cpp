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
#include "Thread.h"

using namespace std;
#include <algorithm>

#include"simdjsonGetValue.h"
//using namespace simdjson;

#include"TimeConvert.h"
#include"ConvertToString.h"
#include"SystemMessage.h"

#include"JsonGetValue.h"
#include"NlohmannJsonDeclaration.h"

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::string;

#undef max
#include"concurrencpp/concurrencpp.h"
using namespace concurrencpp;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��������ַ���ת��ɷŴ���10^power���ĳ����͡�Ҫȷ����ȷ��ת������ʹ�ø��������ɡ�
//
///////////////////////////////////////////////////////////////////////////////////////////////////
long StrToDecimal(const string_view& svData, int power) {
	const int iPointPosition = svData.find_first_of('.');
	if (iPointPosition > svData.length()) {
		long l = 10;
		// û��С���㣿
		if (power > 0) {
			for (int i = 1; i < power; i++) l *= 10;
		}
		else l = 1;
		return atol(svData.data()) * l;
	}
	char buffer[100];
	int i;
	for (i = 0; i < iPointPosition; i++) {
		buffer[i] = svData.at(i);
	}
	while (++i < svData.length()) {
		buffer[i - 1] = svData.at(i);
		if (--power < 0) break;
	}
	while (power-- > 0) {
		buffer[i - 1] = '0';
		i++;
	}
	buffer[i - 1] = 0x000;
	ASSERT(i < 99);
	return atol(buffer);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����exceptionʱ���ϼ����ú���������
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
string_view GetNextField(const string_view& svData, long& lCurrentPos, char delimiter) {
	const string_view sv(svData.data() + lCurrentPos, svData.length() - lCurrentPos);
	const long lEnd = sv.find_first_of(delimiter);
	if (lEnd > sv.length()) throw exception("GetNextField() out of range"); // û�ҵ��Ļ��׳��쳣
	lCurrentPos += lEnd + 1; // ����ǰλ������������֮��
	return string_view(sv.data(), lEnd);
}

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
// ��������ʵʱ����
// 
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void ParseSinaRTData(const CWebDataPtr& pWebData) {
	try {
		pWebData->ResetCurrentPos();
		while (!pWebData->IsLastDataParagraph()) {
			CWebRTDataPtr pRTData = make_shared<CWebRTData>();
			pRTData->ParseSinaData(pWebData->GetCurrentSinaData());
			gl_qChinaMarketRTData.enqueue(pRTData); // �����������ֱ�Ӵ��������ݴ����
		}
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ParseSinaData�쳣 "), e);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��������ʵʱ����
//
// ʹ��thread pool + coroutine���н�����ÿ�ν���һ�����ݣ�������������ݴ��뻺����С�
// ���������в��������ͬ��Ʊ��ʵʱ���ݣ��ʶ��������ͬʱ����ͬһ����Ʊ�����⣬���Կ��Բ��н���
// ֻ�й����̶߳�ִ����󣬱����������˳���
//
// ʹ�����ֶ��߳�ģʽ�뵥�߳�ģʽ��ȣ��ٶȿ�1�����ϡ�
//
// Note ���ô˺������̲߳���ʹ��thread_pool_executor����background_executor���ɣ�ֻ��ʹ��thread_executor���ɣ�ԭ����顣
//
//////////////////////////////////////////////////////////////////////////////////////////////////
result<bool> ParseSinaRTDataUsingCoroutine(shared_ptr<thread_pool_executor> tpe, shared_ptr<vector<string_view>> pvStringView) {
	const auto concurrency_level = tpe->max_concurrency_level();
	vector<result<bool>> results;
	const auto chunk_size = 1 + pvStringView->size() / concurrency_level;
	for (auto i = 0; i < concurrency_level; i++) { // ʹ�õ�ǰCPU�����к���
		long chunk_begin = i * chunk_size;
		long chunk_end = chunk_begin + chunk_size;
		if (chunk_end > pvStringView->size()) chunk_end = pvStringView->size();
		auto result = tpe->submit([pvStringView, chunk_begin, chunk_end] {
			try {
				for (int j = chunk_begin; j < chunk_end; j++) {
					const auto pRTData = make_shared<CWebRTData>();
					pRTData->ParseSinaData(pvStringView->at(j));
					gl_qChinaMarketRTData.enqueue(pRTData); // Note ���Э��ͬʱ�����ʱ���޷�ͨ��size_approx()�����õ�����������
				}
			}
			catch (exception& e) {
				ReportErrorToSystemMessage(_T("ParseSinaData�쳣 "), e);
			}
			return true;
		});
		results.emplace_back(std::move(result));
	}
	bool succeed = true;
	for (auto& r : results) {
		succeed = succeed & co_await r;
	}
	co_return succeed;
}

void ParseSinaRTDataUsingWorkingThread(const CWebDataPtr& pWebData) {
	pWebData->ResetCurrentPos();
	const shared_ptr<vector<string_view>> pvStringView = make_shared<vector<string_view>>();
	try {
		while (!pWebData->IsLastDataParagraph()) {
			auto sv = pWebData->GetCurrentSinaData();
			pvStringView->emplace_back(sv);
		}
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ParseSinaData�쳣 "), e);
	}
	auto result = ParseSinaRTDataUsingCoroutine(gl_runtime.thread_pool_executor(), pvStringView);
	result.get();
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
void ParseTengxunRTData(const CWebDataPtr& pWebData) {
	try {
		pWebData->ResetCurrentPos();
		if (IsTengxunRTDataInvalid(pWebData)) return; // ������21���ַ����ĺ������Է������Ҳ���Է�����ǰ�档
		while (!pWebData->IsLastDataParagraph()) {
			CWebRTDataPtr pRTData = make_shared<CWebRTData>();
			const string_view svData = pWebData->GetCurrentTengxunData();
			pRTData->ParseTengxunData(svData);
			gl_qChinaMarketRTData.enqueue(pRTData);
		}
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ParseTengxunData�쳣 "), e);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��������ʵʱ����
//
// ʹ��thread pool + coroutine���н�����ÿ�������߳̽���1/cpu�������ݣ�������������ݴ��뻺����С�
// ���������в��������ͬ��Ʊ��ʵʱ���ݣ��ʶ��������ͬʱ����ͬһ����Ʊ�����⣬���Կ��Բ��н���
//
// ʹ�����ֶ��߳�ģʽ�뵥�߳�ģʽ��ȣ��ٶȿ�1�����ϡ�
//
// Note ���ô˺������̲߳���ʹ��thread_pool_executor����background_executor���ɣ�ֻ��ʹ��thread_executor���ɣ�ԭ����顣
//
//////////////////////////////////////////////////////////////////////////////////////////////////
concurrencpp::result<bool> ParseTengxunRTDataUsingCoroutine(shared_ptr<concurrencpp::thread_pool_executor> tpe, shared_ptr<vector<string_view>> pvStringView) {
	const auto concurrency_level = tpe->max_concurrency_level();
	bool succeed = true;
	vector<concurrencpp::result<bool>> results;
	const auto chunk_size = 1 + pvStringView->size() / concurrency_level;
	for (auto i = 0; i < concurrency_level; i++) {
		long chunk_begin = i * chunk_size;
		long chunk_end = chunk_begin + chunk_size;
		if (chunk_end > pvStringView->size()) chunk_end = pvStringView->size();
		auto result = tpe->submit([pvStringView, chunk_begin, chunk_end] {
			try {
				for (long j = chunk_begin; j < chunk_end; j++) {
					const auto pRTData = make_shared<CWebRTData>();
					const string_view sv = pvStringView->at(j);
					pRTData->ParseTengxunData(sv);
					gl_qChinaMarketRTData.enqueue(pRTData); // Note ���Э��ͬʱ�����ʱ���޷�ͨ��size_approx()�����õ�����������
				}
			}
			catch (exception& e) {
				ReportErrorToSystemMessage(_T("ParseSinaData�쳣 "), e);
			}
			return true;
		});
		results.emplace_back(std::move(result));
	}
	for (auto& r : results) {
		succeed = succeed & co_await r;
	}
	co_return succeed;
}

void ParseTengxunRTDataUsingWorkingThread(const CWebDataPtr& pWebData) {
	pWebData->ResetCurrentPos();
	const shared_ptr<vector<string_view>> pvStringView = make_shared<vector<string_view>>();
	try {
		while (!pWebData->IsLastDataParagraph()) {
			auto sv = pWebData->GetCurrentSinaData();
			pvStringView->emplace_back(sv);
		}
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ParseSinaData�쳣 "), e);
	}

	auto result = ParseTengxunRTDataUsingCoroutine(gl_runtime.thread_pool_executor(), pvStringView);
	result.get(); // �ȴ��߳�ִ����󷽼�����
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
		long lLastClose = 0;
		ondemand::parser parser;
		ondemand::document doc;

		//const padded_string_view jsonPaddedView(svData, svData.length()); // Note ��ʱ��svData���г���ΪSIMDJSON_PADDING���ȵĺ�׺
		//doc = parser.iterate(jsonPaddedView).value();
		const padded_string jsonPadded(svData);
		doc = parser.iterate(jsonPadded).value();
		ondemand::array dayArray = doc["data"][strStockCode]["day"].get_array().value(); // ʹ������strStockCode�ҵ���������
		// ����Ϊ��ʹ������strStockCode�ҵ���������ķ���
		//ondemand::value data = doc["data"];
		//ondemand::field field = *data.get_object().begin();
		//ondemand::value stock = field.value();
		//ondemand::array dayArray = stock["day"].get_array();
		for (auto dayLine : dayArray) {
			auto pDayLine = make_shared<CDayLine>();
			pDayLine->SetStockSymbol(strStockSymbol);
			pDayLine->SetLastClose(lLastClose);
			ondemand::array_iterator it = dayLine.get_array().begin();
			ondemand::value item = (*it).value();
			sv = jsonGetStringView(item);
			string str1(sv.data(), sv.length()); // ������Ҫת��һ�£�ֱ��ʹ��string_view�ᵼ���ڴ����
			sscanf_s(str1.data(), _T("%4d-%02d-%02d"), &year, &month, &day);
			pDayLine->SetDate(year * 10000 + month * 100 + day);
			item = (*++it).value();
			sv = jsonGetStringView(item);
			pDayLine->SetOpen(StrToDecimal(sv, 3));
			item = (*++it).value();
			sv = jsonGetStringView(item);
			const long lClose = StrToDecimal(sv, 3);
			pDayLine->SetClose(lClose);
			lLastClose = lClose;
			item = (*++it).value();
			sv = jsonGetStringView(item);
			pDayLine->SetHigh(StrToDecimal(sv, 3));
			item = (*++it).value();
			sv = jsonGetStringView(item);
			pDayLine->SetLow(StrToDecimal(sv, 3));
			item = (*++it).value();
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

bool CreateJsonWithNlohmann(json& js, const std::string& s, const long lBeginPos, const long lEndPos) {
	try {
		js = json::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	}
	catch (json::parse_error&) {
		js.clear();
		return false;
	}
	return true;
}

bool CreateJsonWithNlohmann(json& js, CString& str, const long lBeginPos, const long lEndPos) {
	const string s = str.GetBuffer();
	try {
		js = json::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	}
	catch (json::parse_error&) {
		js.clear();
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
// ��ʼ��Ϊ��һ��{��������Ϊ�����ڶ���}��������������账���򱻴�����ַ�Ϊ','�����û�������ˣ��򱻴�����ַ�Ϊ' '��
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
//
//  �������Ĺ�Ʊ���Ƶ���ʽ��������ʱ��ʹ�ã�����boost ptree�����ĵ�֧�ֲ��㣬��ֻ֧��utf8��ʽ��������ȡ�����ַ�ʱ�������룩��
// ���ڲ���wstring��CStringW���ι��ɣ��Ϳ���������ʾ�ˡ�
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr& pWebRTData) {
	string strTime;
	CString strSymbol4;
	json js = it.value();
	const string symbolName = it.key();

	try {
		strSymbol4 = XferNeteaseToStandard(symbolName.c_str());
		pWebRTData->SetSymbol(strSymbol4);
		const string sName = jsonGetString(js, "name");
		pWebRTData->SetStockName(XferToCString(sName)); // ��utf-8�ַ���ת��Ϊ���ֽ��ַ���
		strTime = jsonGetString(js, _T("time"));
		string strSymbol2 = jsonGetString(js, _T("code"));
		pWebRTData->SetTransactionTime(ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str(), -8));
	}
	catch (json::exception& e) {// �ṹ������
		// do nothing
		CString strError2 = strSymbol4;
		strError2 += _T(" ");
		strError2 += e.what();
		gl_systemMessage.PushErrorMessage(strError2);
	}
	try {
		pWebRTData->SetVolume(jsonGetLongLong(js, _T("volume")));
		pWebRTData->SetAmount(jsonGetDouble(js, _T("turnover")));
		pWebRTData->SetHigh(static_cast<long>(jsonGetDouble(js, _T("high")) * 1000));
		pWebRTData->SetLow(static_cast<long>(jsonGetDouble(js, _T("low")) * 1000));
		pWebRTData->SetNew(static_cast<long>(jsonGetDouble(js, _T("price")) * 1000));
		pWebRTData->SetLastClose(static_cast<long>(jsonGetDouble(js, _T("yestclose")) * 1000));
		pWebRTData->SetOpen(static_cast<long>(jsonGetDouble(js, _T("open")) * 1000));

		pWebRTData->SetVBuy(0, jsonGetLong(js, _T("bidvol1")));
		pWebRTData->SetVBuy(1, jsonGetLong(js, _T("bidvol2")));
		pWebRTData->SetVBuy(2, jsonGetLong(js, _T("bidvol3")));
		pWebRTData->SetVBuy(3, jsonGetLong(js, _T("bidvol4")));
		pWebRTData->SetVBuy(4, jsonGetLong(js, _T("bidvol5")));
		pWebRTData->SetVSell(0, jsonGetLong(js, _T("askvol1")));
		pWebRTData->SetVSell(1, jsonGetLong(js, _T("askvol2")));
		pWebRTData->SetVSell(2, jsonGetLong(js, _T("askvol3")));
		pWebRTData->SetVSell(3, jsonGetLong(js, _T("askvol4")));
		pWebRTData->SetVSell(4, jsonGetLong(js, _T("askvol5")));

		pWebRTData->SetPSell(0, static_cast<long>(jsonGetDouble(js, _T("ask1")) * 1000));
		pWebRTData->SetPSell(1, static_cast<long>(jsonGetDouble(js, _T("ask2")) * 1000));
		pWebRTData->SetPSell(2, static_cast<long>(jsonGetDouble(js, _T("ask3")) * 1000));
		pWebRTData->SetPSell(3, static_cast<long>(jsonGetDouble(js, _T("ask4")) * 1000));
		pWebRTData->SetPSell(4, static_cast<long>(jsonGetDouble(js, _T("ask5")) * 1000));
		pWebRTData->SetPBuy(0, static_cast<long>(jsonGetDouble(js, _T("bid1")) * 1000));
		pWebRTData->SetPBuy(1, static_cast<long>(jsonGetDouble(js, _T("bid2")) * 1000));
		pWebRTData->SetPBuy(2, static_cast<long>(jsonGetDouble(js, _T("bid3")) * 1000));
		pWebRTData->SetPBuy(3, static_cast<long>(jsonGetDouble(js, _T("bid4")) * 1000));
		pWebRTData->SetPBuy(4, static_cast<long>(jsonGetDouble(js, _T("bid5")) * 1000));

		pWebRTData->CheckNeteaseRTDataActive();
	}
	catch (json::exception&) {// �ǻ�Ծ��Ʊ�������еȣ�
		pWebRTData->SetActive(false);
	}
}

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	for (auto it = pjs->begin(); it != pjs->end(); ++it) {
		auto pRTData = make_shared<CWebRTData>();
		pRTData->SetDataSource(NETEASE_RT_WEB_DATA_);
		ParseOneNeteaseRTData(it, pRTData);
		pRTData->CheckNeteaseRTDataActive();
		pvWebRTData->push_back(pRTData);
	}
	return pvWebRTData;
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
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(const CWebDataPtr& pData) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>(); // Ҫȷ�����ص�����ָ�벻Ϊ�գ��ϼ�������Ҫ��ָ�룬��ʹ�����ݿ���Ϊ�գ�
	const string_view svHeader = pData->GetStringView(0, 21);
	if (svHeader.compare(_T("_ntes_quote_callback(")) != 0) {
		CString str = _T("Bad netease RT data header : ");
		str.Append(svHeader.data(), svHeader.size());
		gl_systemMessage.PushErrorMessage(str);
		return pvWebRTData;
	}
	json js;

	if (!pData->CreateJson(js, 21, 2)) {	// ��������ǰ21λΪǰ׺������λΪ��׺
		gl_systemMessage.PushErrorMessage(_T("Netease RT data json parse error"));
		return pvWebRTData;
	}
	pvWebRTData = ParseNeteaseRTData(&js);

	return pvWebRTData;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
// ��ʼ��Ϊ��һ��{��������Ϊ�����ڶ���}��������������账���򱻴�����ַ�Ϊ','�����û�������ˣ��򱻴�����ַ�Ϊ' '��
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
// Ŀǰ�����±귽���������ݣ����ٶ��ܴﵽNlohmann json���������ϡ�
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(string_view svJsonData) {
	string symbolCode, strTime;
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();
	try {
		ondemand::parser parser;
		ondemand::document doc;
		//const padded_string_view jsonPaddedView(svJsonData, svJsonData.length());
		//doc = parser.iterate(jsonPaddedView).value();
		const padded_string jsonPadded(svJsonData);
		doc = parser.iterate(jsonPadded).value();
		for (ondemand::field item_key : doc.get_object()) {
			auto pWebRTData = make_shared<CWebRTData>();
			pWebRTData->SetDataSource(NETEASE_RT_WEB_DATA_);
			//auto key = item_key.key();
			ondemand::object item = item_key.value();
			const string_view strSymbolView2 = jsonGetStringView(item, _T("code"));
			symbolCode = strSymbolView2;
			CString strSymbol4 = XferNeteaseToStandard(symbolCode.c_str());
			pWebRTData->SetSymbol(strSymbol4);
			pWebRTData->SetVSell(0, jsonGetInt64(item, _T("askvol1")));
			pWebRTData->SetVSell(2, jsonGetInt64(item, _T("askvol3")));
			pWebRTData->SetVSell(1, jsonGetInt64(item, _T("askvol2")));
			pWebRTData->SetVSell(4, jsonGetInt64(item, _T("askvol5")));
			pWebRTData->SetVSell(3, jsonGetInt64(item, _T("askvol4")));
			pWebRTData->SetNew(jsonGetDouble(item, _T("price")) * 1000);
			pWebRTData->SetOpen(jsonGetDouble(item, _T("open")) * 1000);
			pWebRTData->SetPBuy(4, jsonGetDouble(item, _T("bid5")) * 1000);
			pWebRTData->SetPBuy(3, jsonGetDouble(item, _T("bid4")) * 1000);
			pWebRTData->SetPBuy(2, jsonGetDouble(item, _T("bid3")) * 1000);
			pWebRTData->SetPBuy(1, jsonGetDouble(item, _T("bid2")) * 1000);
			pWebRTData->SetPBuy(0, jsonGetDouble(item, _T("bid1")) * 1000);
			pWebRTData->SetHigh(jsonGetDouble(item, _T("high")) * 1000);
			pWebRTData->SetLow(jsonGetDouble(item, _T("low")) * 1000);
			pWebRTData->SetVBuy(2, jsonGetInt64(item, _T("bidvol3")));
			pWebRTData->SetVBuy(0, jsonGetInt64(item, _T("bidvol1")));
			pWebRTData->SetVBuy(1, jsonGetInt64(item, _T("bidvol2")));
			pWebRTData->SetVBuy(4, jsonGetInt64(item, _T("bidvol5")));
			pWebRTData->SetVBuy(3, jsonGetInt64(item, _T("bidvol4")));
			pWebRTData->SetVolume(jsonGetInt64(item, _T("volume")));
			pWebRTData->SetPSell(4, jsonGetDouble(item, _T("ask5")) * 1000);
			pWebRTData->SetPSell(3, jsonGetDouble(item, _T("ask4")) * 1000);
			pWebRTData->SetPSell(0, jsonGetDouble(item, _T("ask1")) * 1000);

			string_view sNameView = jsonGetStringView(item, "name");
			string sName(sNameView);
			pWebRTData->SetStockName(XferToCString(sName)); // ��utf-8�ַ���ת��Ϊ���ֽ��ַ���
			pWebRTData->SetPSell(2, jsonGetDouble(item, _T("ask3")) * 1000);
			pWebRTData->SetPSell(1, jsonGetDouble(item, _T("ask2")) * 1000);
			strTime = jsonGetStringView(item, _T("time"));
			pWebRTData->SetTransactionTime(ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str(), -8));

			pWebRTData->SetLastClose(jsonGetDouble(item, _T("yestclose")) * 1000);
			pWebRTData->SetAmount(jsonGetDouble(item, _T("turnover")));

			pWebRTData->CheckNeteaseRTDataActive();
			pvWebRTData->push_back(pWebRTData);
		}
	}
	catch (simdjson_error& error) {
		const string sError = error.what();
		CString str = "Netease RT Data Error: ";
		str += sError.c_str();
		gl_systemMessage.PushErrorMessage(str);
	}
	return pvWebRTData;
}

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson2(string_view svJsonData) {
	string symbolCode, strTime;
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();
	try {
		ondemand::parser parser;
		const padded_string jsonPadded(svJsonData);
		//const padded_string_view jsonPadded(svJsonData.data(), svJsonData.length() + SIMDJSON_PADDING + 200); // todo ���ַ�ʽ�ٶȿ죬��Ŀǰʹ�ò���
		ondemand::document doc = parser.iterate(jsonPadded).value();
		for (ondemand::field item_key : doc.get_object()) {
			auto pWebRTData = make_shared<CWebRTData>();
			pWebRTData->SetDataSource(NETEASE_RT_WEB_DATA_);
			//auto key = item_key.key();
			ondemand::object item = item_key.value();
			const string_view strSymbolView2 = jsonGetStringView(item, _T("code"));
			symbolCode = strSymbolView2;
			CString strSymbol4 = XferNeteaseToStandard(symbolCode.c_str());
			pWebRTData->SetSymbol(strSymbol4);
			pWebRTData->SetVSell(0, jsonGetInt64(item, _T("askvol1")));
			pWebRTData->SetVSell(2, jsonGetInt64(item, _T("askvol3")));
			pWebRTData->SetVSell(1, jsonGetInt64(item, _T("askvol2")));
			pWebRTData->SetVSell(4, jsonGetInt64(item, _T("askvol5")));
			pWebRTData->SetVSell(3, jsonGetInt64(item, _T("askvol4")));
			pWebRTData->SetNew(StrToDecimal(jsonGetRawJsonToken(item, _T("price")), 3));
			pWebRTData->SetOpen(StrToDecimal(jsonGetRawJsonToken(item, _T("open")), 3));
			pWebRTData->SetPBuy(4, StrToDecimal(jsonGetRawJsonToken(item, _T("bid5")), 3));
			pWebRTData->SetPBuy(3, StrToDecimal(jsonGetRawJsonToken(item, _T("bid4")), 3));
			pWebRTData->SetPBuy(2, StrToDecimal(jsonGetRawJsonToken(item, _T("bid3")), 3));
			pWebRTData->SetPBuy(1, StrToDecimal(jsonGetRawJsonToken(item, _T("bid2")), 3));
			pWebRTData->SetPBuy(0, StrToDecimal(jsonGetRawJsonToken(item, _T("bid1")), 3));
			pWebRTData->SetHigh(StrToDecimal(jsonGetRawJsonToken(item, _T("high")), 3));
			pWebRTData->SetLow(StrToDecimal(jsonGetRawJsonToken(item, _T("low")), 3));
			pWebRTData->SetVBuy(2, jsonGetInt64(item, _T("bidvol3")));
			pWebRTData->SetVBuy(0, jsonGetInt64(item, _T("bidvol1")));
			pWebRTData->SetVBuy(1, jsonGetInt64(item, _T("bidvol2")));
			pWebRTData->SetVBuy(4, jsonGetInt64(item, _T("bidvol5")));
			pWebRTData->SetVBuy(3, jsonGetInt64(item, _T("bidvol4")));
			pWebRTData->SetVolume(jsonGetInt64(item, _T("volume")));
			pWebRTData->SetPSell(4, StrToDecimal(jsonGetRawJsonToken(item, _T("ask5")), 3));
			pWebRTData->SetPSell(3, StrToDecimal(jsonGetRawJsonToken(item, _T("ask4")), 3));
			pWebRTData->SetPSell(0, StrToDecimal(jsonGetRawJsonToken(item, _T("ask1")), 3));

			string_view sNameView = jsonGetStringView(item, "name");
			string sName(sNameView);
			pWebRTData->SetStockName(XferToCString(sName)); // ��utf-8�ַ���ת��Ϊ���ֽ��ַ���
			pWebRTData->SetPSell(2, StrToDecimal(jsonGetRawJsonToken(item, _T("ask3")), 3));
			pWebRTData->SetPSell(1, StrToDecimal(jsonGetRawJsonToken(item, _T("ask2")), 3));
			strTime = jsonGetStringView(item, _T("time"));
			pWebRTData->SetTransactionTime(ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str(), -8));

			pWebRTData->SetLastClose(StrToDecimal(jsonGetRawJsonToken(item, _T("yestclose")), 3));
			pWebRTData->SetAmount(StrToDecimal(jsonGetRawJsonToken(item, _T("turnover")), 0));

			pWebRTData->CheckNeteaseRTDataActive();
			pvWebRTData->push_back(pWebRTData);
		}
	}
	catch (simdjson_error& error) {
		const string sError = error.what();
		CString str = "Netease RT Data Error: ";
		str += sError.c_str();
		gl_systemMessage.PushErrorMessage(str);
	}
	return pvWebRTData;
}

// bug ������������ᵼ�±���ʱ���ӳ�4�������ң������Ǳ����������⡣����΢�����˸����⣬�ȴ���Ӧ��
// neteaseʵʱ���ݵ�˳���г��仯������ʹ�ô���˳�����������
/*
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson3(string_view svJsonData) {
	string symbolCode, strTime;
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();
	try {
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJsonData);
		ondemand::document doc = parser.iterate(jsonPadded);
		for (ondemand::field item_key : doc.get_object()) {
			auto pWebRTData = make_shared<CWebRTData>();
			pWebRTData->SetDataSource(NETEASE_RT_WEB_DATA_);
			ondemand::object item = item_key.value();
			const string_view strSymbolView2 = item.find_field("code").get_string();
			symbolCode = strSymbolView2;
			CString strSymbol4 = XferNeteaseToStandard(symbolCode.c_str());
			pWebRTData->SetSymbol(strSymbol4);
			pWebRTData->SetVSell(0, item.find_field("askvol1").get_int64());
			pWebRTData->SetVSell(2, item.find_field("askvol3").get_int64());
			pWebRTData->SetVSell(1, item.find_field("askvol2").get_int64());
			pWebRTData->SetVSell(4, item.find_field("askvol5").get_int64());
			pWebRTData->SetVSell(3, item.find_field("askvol4").get_int64());
			pWebRTData->SetNew(item.find_field("price").get_double() * 1000);
			pWebRTData->SetOpen(item.find_field("open").get_double() * 1000);
			pWebRTData->SetPBuy(4, item.find_field("bid5").get_double() * 1000);
			pWebRTData->SetPBuy(3, item.find_field("bid4").get_double() * 1000);
			pWebRTData->SetPBuy(2, item.find_field("bid3").get_double() * 1000);
			pWebRTData->SetPBuy(1, item.find_field("bid2").get_double() * 1000);
			pWebRTData->SetPBuy(0, item.find_field("bid1").get_double() * 1000);
			pWebRTData->SetHigh(item.find_field("high").get_double() * 1000);
			pWebRTData->SetLow(item.find_field("low").get_double() * 1000);
			pWebRTData->SetVBuy(0, item.find_field("bidvol1").get_int64());
			pWebRTData->SetVBuy(2, item.find_field("bidvol3").get_int64());
			pWebRTData->SetVBuy(1, item.find_field("bidvol2").get_int64());
			pWebRTData->SetVBuy(4, item.find_field("bidvol5").get_int64());
			pWebRTData->SetVBuy(3, item.find_field("bidvol4").get_int64());
			pWebRTData->SetVolume(item.find_field("volume").get_int64());
			pWebRTData->SetPSell(4, item.find_field("ask5").get_double() * 1000);
			pWebRTData->SetPSell(3, item.find_field("ask4").get_double() * 1000);
			pWebRTData->SetPSell(0, item.find_field("ask1").get_double() * 1000);

			string_view sNameView = item.find_field("name").get_string();
			string sName(sNameView);
			pWebRTData->SetStockName(XferToCString(sName)); // ��utf-8�ַ���ת��Ϊ���ֽ��ַ���
			pWebRTData->SetPSell(2, item.find_field("ask3").get_double() * 1000);
			pWebRTData->SetPSell(1, item.find_field("ask2").get_double() * 1000);
			const string_view svTime = item.find_field("time").get_string();
			strTime = svTime;
			pWebRTData->SetTransactionTime(ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str(), -8));
			pWebRTData->SetLastClose(item.find_field("yestclose").get_double() * 1000);
			pWebRTData->SetAmount(item.find_field("turnover").get_double());
			pWebRTData->CheckNeteaseRTDataActive();
			pvWebRTData->push_back(pWebRTData);
		}
	}
	catch (simdjson_error& error) {
		string sError = error.what();
	}
	return pvWebRTData;
}
*/

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(const CWebDataPtr& pData) {
	return ParseNeteaseRTDataWithSimdjson(pData->GetStringView(21, pData->GetBufferLength() - 21 - 2)); // ����json����
}

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson2(const CWebDataPtr& pData) {
	return ParseNeteaseRTDataWithSimdjson2(pData->GetStringView(21, pData->GetBufferLength() - 21 - 2)); // ����json����
}
