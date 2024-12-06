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

#include "ChinaStockCodeConverter.h"
#include "InfoReport.h"
import FireBird.Thread;

import simdjson.GetValue;
#include"simdjson.h"
using namespace simdjson;

#include"ConvertToString.h"
#include"SystemMessage.h"

#include"JsonGetValue.h"
#include"NlohmannJsonDeclaration.h"

#include"spdlog/spdlog.h"


#include "ChinaMarket.h"
#undef max // ����concurrencpp.h֮ǰ����Ҫע��max�Ķ���
#include"concurrencpp/concurrencpp.h"
using namespace concurrencpp;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��������ַ���ת��ɷŴ���10^power���ĳ����͡�Ҫȷ����ȷ��ת������ʹ�ø��������ɡ�
//
///////////////////////////////////////////////////////////////////////////////////////////////////
long StrToDecimal(const string_view& svData, int power) {
	char buffer[100]{};
	const auto iPointPosition = svData.find_first_of('.');
	if (iPointPosition == std::string_view::npos) {		// û��С���㣿
		long l = 10;
		if (power > 0) {
			for (int i = 1; i < power; i++) l *= 10;
		}
		else l = 1;
		return atol(svData.data()) * l;
	}
	// ��С����
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
string_view GetNextField(const string_view& svData, size_t& lCurrentPos, char delimiter) {
	const string_view sv(svData.data() + lCurrentPos, svData.length() - lCurrentPos);
	const auto lEnd = sv.find_first_of(delimiter);
	if (lEnd > sv.length()) throw exception("GetNextField() out of range"); // û�ҵ��Ļ��׳��쳣
	lCurrentPos += lEnd + 1; // ����ǰλ������������֮��
	return string_view(sv.data(), lEnd);
}

void ReportJsonError(const json::parse_error& e, const std::string& s) {
	char buffer[180]{}, buffer2[100]{};
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
// ʹ��thread pool + coroutine���н�����ÿ�������߳̽���1/gl_concurrency_level�������ݣ�������������ݴ��뻺����С�
// ���������в��������ͬ��Ʊ��ʵʱ���ݣ��ʶ��������ͬʱ����ͬһ����Ʊ�����⣬���Կ��Բ��н���
// ֻ�й����̶߳�ִ����󣬱����������˳���
//
// ʹ�����ֶ��߳�ģʽ�뵥�߳�ģʽ��ȣ��ٶȿ�1�����ϡ�
//
// Note ���ô˺������̲߳���ʹ��thread_pool_executor����background_executor���ɣ�ֻ��ʹ��thread_executor���ɣ�ԭ����顣
// Note ʹ��max_concurrency_levelʱ��ʵ��Ч������Ϊ4��������������йأ���CPU��ռ������100%��������ʱ�䲢û�м��١��ʶ���gl_concurrency_level��Ϊ4.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
result<bool> ParseSinaRTDataUsingCoroutine(shared_ptr<thread_pool_executor> tpe, shared_ptr<vector<string_view>> pvStringView) {
	vector<result<bool>> results;
	const auto DataSize = pvStringView->size();
	const auto chunk_size = 1 + DataSize / gl_concurrency_level;
	for (int i = 0; i < gl_concurrency_level; i++) {
		auto chunk_begin = i * chunk_size;
		auto chunk_end = chunk_begin + chunk_size;
		if (chunk_end > DataSize) chunk_end = DataSize;
		auto result = tpe->submit([pvStringView, chunk_begin, chunk_end] {
			try {
				for (auto j = chunk_begin; j < chunk_end; j++) {
					const auto pRTData = make_shared<CWebRTData>();
					pRTData->ParseSinaData(pvStringView->at(j));
					gl_qChinaMarketRTData.enqueue(pRTData); // Note ���Э�̲��������ʱ���޷�ͨ��size_approx()�����õ�����������
				}
			} catch (exception& e) {
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

void ParseSinaRTData(const CWebDataPtr& pWebData) {
	pWebData->ResetCurrentPos();
	const shared_ptr<vector<string_view>> pvStringView = make_shared<vector<string_view>>();
	try {
		while (!pWebData->IsLastDataParagraph()) {
			pvStringView->emplace_back(pWebData->GetCurrentSinaData());
		}
	} catch (exception& e) {
		ReportErrorToSystemMessage(_T("ParseSinaData�쳣 "), e);
	}
	auto result = ParseSinaRTDataUsingCoroutine(gl_runtime.thread_pool_executor(), pvStringView); //Note ����ʹ��thread_pool_executor
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

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������Ѷʵʱ����
//
// ʹ��thread pool + coroutine���н�����ÿ�������߳̽���1/gl_concurrency_level�������ݣ�������������ݴ��뻺����С�
// ���������в��������ͬ��Ʊ��ʵʱ���ݣ��ʶ��������ͬʱ����ͬһ����Ʊ�����⣬���Կ��Բ��н���
//
// ʹ�����ֶ��߳�ģʽ�뵥�߳�ģʽ��ȣ��ٶȿ�1�����ϡ�
//
// Note ���ô˺������̲߳���ʹ��thread_pool_executor����background_executor���ɣ�ֻ��ʹ��thread_executor���ɣ�ԭ����顣
// Note ʹ��max_concurrency_levelʱ��ʵ��Ч������Ϊ4��������������йأ���CPU��ռ������100%��������ʱ�䲢û�м��١��ʶ���gl_concurrency_level��Ϊ4.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
concurrencpp::result<bool> ParseTengxunRTDataUsingCoroutine(shared_ptr<concurrencpp::thread_pool_executor> tpe, shared_ptr<vector<string_view>> pvStringView) {
	bool succeed = true;
	vector<concurrencpp::result<bool>> results;
	const auto DataSize = pvStringView->size();
	const auto chunk_size = 1 + DataSize / gl_concurrency_level;
	for (int i = 0; i < gl_concurrency_level; i++) {
		auto chunk_begin = i * chunk_size;
		auto chunk_end = chunk_begin + chunk_size;
		if (chunk_end > DataSize) chunk_end = DataSize;
		auto result = tpe->submit([pvStringView, chunk_begin, chunk_end] {
			try {
				for (auto j = chunk_begin; j < chunk_end; j++) {
					const auto pRTData = make_shared<CWebRTData>();
					const string_view sv = pvStringView->at(j);
					pRTData->ParseTengxunData(sv);
					gl_qChinaMarketRTData.enqueue(pRTData); // Note ���Э�̲��������ʱ���޷�ͨ��size_approx()�����õ�����������
				}
			} catch (exception& e) {
				ReportErrorToSystemMessage(_T("ParseSinaData�쳣 "), e);
			}
			return true;
		});
		results.emplace_back(std::move(result));
	}
	for (auto& r : results) {
		succeed = succeed & r.get();
	}
	co_return succeed;
}

void ParseTengxunRTData(const CWebDataPtr& pWebData) {
	pWebData->ResetCurrentPos();
	const shared_ptr<vector<string_view>> pvStringView = make_shared<vector<string_view>>();
	try {
		while (!pWebData->IsLastDataParagraph()) {
			pvStringView->emplace_back(pWebData->GetCurrentTengxunData());
		}
	} catch (exception& e) {
		ReportErrorToSystemMessage(_T("ParseTengxunData�쳣 "), e);
	}

	auto result = ParseTengxunRTDataUsingCoroutine(gl_runtime.thread_pool_executor(), pvStringView); //Note ����ʹ��thread_pool_executor
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
	} catch (json::exception&) {
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
	ASSERT(gl_dataContainerChinaStock.IsSymbol(strSymbol));
	const CString strDisplaySymbol = gl_dataContainerChinaStock.GetStock(strSymbol)->GetDisplaySymbol();
	const string_view svData = pWebData->GetStringView(0, pWebData->GetBufferLength());

	const shared_ptr<vector<CDayLinePtr>> pvDayLine = ParseTengxunDayLine(svData, XferStandardToTengxun(pWebData->GetStockCode()));
	std::ranges::sort(*pvDayLine, [](const CDayLinePtr& pData1, const CDayLinePtr& pData2) { return pData1->GetMarketDate() < pData2->GetMarketDate(); });
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
	} catch (json::parse_error&) {
		js.clear();
		return false;
	}
	return true;
}

bool CreateJsonWithNlohmann(json& js, CString& str, const long lBeginPos, const long lEndPos) {
	const string s = str.GetBuffer();
	try {
		js = json::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	} catch (json::parse_error&) {
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
		strSymbol4 = XferNeteaseToStandard(symbolName);
		pWebRTData->SetSymbol(strSymbol4);
		const string sName = jsonGetString(js, "name");
		pWebRTData->SetStockName(XferToCString(sName)); // ��utf-8�ַ���ת��Ϊ���ֽ��ַ���
		strTime = jsonGetString(js, _T("time"));
		string strSymbol2 = jsonGetString(js, _T("code"));
		std::stringstream ss(strTime);
		chrono::sys_seconds tpTime;
		chrono::from_stream(ss, "%Y/%m/%d %T", tpTime);
		tpTime -= gl_pChinaMarket->GetMarketTimeZoneOffset();
		pWebRTData->SetTimePoint(tpTime);
		auto tt = tpTime.time_since_epoch().count();
		pWebRTData->SetTransactionTime(tt);
	} catch (json::exception& e) {// �ṹ������
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
	} catch (json::exception&) {// �ǻ�Ծ��Ʊ�������еȣ�
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
		const padded_string jsonPadded(svJsonData);
		ondemand::document doc = parser.iterate(jsonPadded).value();
		for (ondemand::field item_key : doc.get_object()) {
			auto pWebRTData = make_shared<CWebRTData>();
			pWebRTData->SetDataSource(NETEASE_RT_WEB_DATA_);
			//auto key = item_key.key();
			ondemand::object item = item_key.value();
			CString strSymbol4 = XferNeteaseToStandard(jsonGetStringView(item, _T("code")));
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
			std::stringstream ss(strTime);
			chrono::sys_seconds tpTime;
			chrono::from_stream(ss, "%Y/%m/d %T", tpTime);
			tpTime -= gl_pChinaMarket->GetMarketTimeZoneOffset();
			auto tt = tpTime.time_since_epoch().count();
			pWebRTData->SetTransactionTime(tt);
			pWebRTData->SetLastClose(StrToDecimal(jsonGetRawJsonToken(item, _T("yestclose")), 3));
			pWebRTData->SetAmount(StrToDecimal(jsonGetRawJsonToken(item, _T("turnover")), 0));

			pWebRTData->CheckNeteaseRTDataActive();
			pvWebRTData->push_back(pWebRTData);
		}
	} catch (simdjson_error& error) {
		const string sError = error.what();
		CString str = "Netease RT Data Error: ";
		str += sError.c_str();
		gl_systemMessage.PushErrorMessage(str);
	}
	return pvWebRTData;
}

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(const CWebDataPtr& pData) {
	return ParseNeteaseRTDataWithSimdjson(pData->GetStringView(21, pData->GetBufferLength() - 21 - 2)); // ����json����
}
