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

#include "ChinaMarket.h"
#include "ChinaStockCodeConverter.h"
#include "JsonGetValue.h"

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
shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(const CWebDataPtr& pWebData) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	pWebData->ResetCurrentPos();
	while (!pWebData->IsLastDataParagraph()) {
		auto pRTData = make_shared<CWebRTData>();
		if (pRTData->ReadSinaData(pWebData)) {
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
bool IsTengxunRTDataInvalid(const CWebDataPtr& pWebDataReceived) {
	const string_view sv = pWebDataReceived->GetStringViewData(21);

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

	pWebData->ResetCurrentPos();
	if (IsTengxunRTDataInvalid(pWebData)) return pvWebRTData; // ������21���ַ����ĺ������Է������Ҳ���Է�����ǰ�档
	while (!pWebData->IsLastDataParagraph()) {
		auto pRTData = make_shared<CWebRTData>();
		if (pRTData->ReadTengxunData(pWebData)) {
			pvWebRTData->push_back(pRTData);
		}
		else {
			break; // ��������ݳ����⣬�׵����á�
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
CDayLineWebDataPtr ParseNeteaseDayLine(const CWebDataPtr& pWebData) {
	auto pData = make_shared<CDayLineWebData>();

	pData->TransferWebDataToBuffer(pWebData);
	pData->ProcessNeteaseDayLineData(); //pData����������������ģ��������ڵ���ǰ�档

	return pData;
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
shared_ptr<vector<CDayLinePtr>> ParseTengxunDayLine(json* pjs, CString strStockCode) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	string sTemp;
	try {
		json js2 = pjs->at(_T("data"));
		json js3 = jsonGetChild(&js2, strStockCode);
		double dLastClose = 0;
		//json js6 = js2.();
		json js4 = js3.at("day");
		for (auto it = js4.begin(); it != js4.end(); ++it) {
			auto pDayLine = make_shared<CDayLine>();
			long year, month, day;
			json js5 = it.value();
			pDayLine->SetLastClose(dLastClose * 1000);
			sTemp = js5.at(0);
			sscanf_s(sTemp.c_str(), _T("%4d-%02d-%02d"), &year, &month, &day);
			pDayLine->SetDate(year * 10000 + month * 100 + day);
			sTemp = js5.at(1);
			pDayLine->SetOpen(atof(sTemp.c_str()) * 1000);
			sTemp = js5.at(2);
			const double dClose = atof(sTemp.c_str());
			pDayLine->SetClose(dClose * 1000);
			dLastClose = dClose;
			sTemp = js5.at(3);
			pDayLine->SetHigh(atof(sTemp.c_str()) * 1000);
			sTemp = js5.at(4);
			pDayLine->SetLow(atof(sTemp.c_str()) * 1000);
			sTemp = js5.at(5);
			pDayLine->SetVolume(atof(sTemp.c_str()) * 100);
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
	const CString strDisplaySymbol = gl_pChinaMarket->GetStock(strSymbol)->GetDisplaySymbol();

	if (!pWebData->IsParsed()) {
		if (!pWebData->CreateJson()) {
			const CString strMessage = pWebData->GetStockCode() + _T(": Tengxun DayLine data json parse error");
			gl_systemMessage.PushErrorMessage(strMessage);
			return pDayLineData;
		}
	}
	ASSERT(pWebData->IsParsed());
	json* pjs = pWebData->GetJSon();
	const shared_ptr<vector<CDayLinePtr>> pvDayLine = ParseTengxunDayLine(pjs, XferStandardToTengxun(pWebData->GetStockCode()));
	ranges::sort(*pvDayLine, [](const CDayLinePtr& pData1, const CDayLinePtr& pData2) { return pData1->GetMarketDate() < pData2->GetMarketDate(); });
	for (const auto& pDayLine : *pvDayLine) {
		pDayLine->SetStockSymbol(strSymbol);
		pDayLine->SetDisplaySymbol(strDisplaySymbol);
		pDayLineData->AppendDayLine(pDayLine);
	}
	return pDayLineData;
}
