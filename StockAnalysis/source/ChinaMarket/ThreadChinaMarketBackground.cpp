/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ChinaMarket�еĺ�ʱ�������������˴���
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

#include"thread.h"
#include"WebInquirer.h"

#include"Parse.h"

UINT ThreadChinaMarketBackground(void) {
	gl_ThreadStatus.SetChinaMarketBackground(true);
	while (!gl_fExitingSystem) {
		// ����������ȽϷ�ʱ������ʱ����ʵʱ���ݽ���ʱ��Ҫʹ��json���������ʶ����ڴ˶����߳��С�
		ParseWebRTDataGetFromSinaServer(); // ��������ʵʱ����
		ParseWebRTDataGetFromNeteaseServer(); // ��������ʵʱ����
		ParseWebRTDataGetFromTengxunServer(); // ������Ѷʵʱ����
		ParseDayLineGetFromNeeteaseServer();
		Sleep(50); // ���ټ��50ms
	}
	gl_ThreadStatus.SetChinaMarketBackground(false);
	return 201;
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
// ����ַ�������������ƴ����һ�𣬲�ͬ����������ö��Ÿ����ˣ����ճ���Ա��˼·��˳��Ŵ�0��ʼ��
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
bool ParseWebRTDataGetFromSinaServer(void) {
	CWebDataPtr pWebDataReceived = nullptr;
	const size_t lTotalData = gl_WebInquirer.GetSinaRTDataSize();
	INT64 llTotal = 0;
	bool fSucceed = true;
	for (int i = 0; i < lTotalData; i++) {
		pWebDataReceived = gl_WebInquirer.PopSinaRTData();
		pWebDataReceived->ResetCurrentPos();
		while (!pWebDataReceived->IsProcessedAllTheData()) {
			CWebRTDataPtr pRTData = make_shared<CWebRTData>();
			if (pRTData->ReadSinaData(pWebDataReceived)) {
				llTotal++;
				gl_WebRTDataContainer.PushSinaData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
			}
			else {
				fSucceed = false;
				break;  // ��������ݳ����⣬�׵����á�
			}
		}
	}
	gl_pChinaMarket->IncreaseRTDataReceived(llTotal);
	return fSucceed;
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
// ʹ��json�������Ѿ�û�д��������ˡ�
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool ParseWebRTDataGetFromNeteaseServer(void) {
	CWebDataPtr pWebDataReceived = nullptr;
	const size_t lTotalData = gl_WebInquirer.GetNeteaseRTDataSize();
	string ss;
	ptree pt;
	INT64 llTotal = 0;

	for (int i = 0; i < lTotalData; i++) {
		pWebDataReceived = gl_WebInquirer.PopNeteaseRTData();
		if (pWebDataReceived->CreatePTree(pt, 21, 2)) { // ��������ǰ21λΪǰ׺������λΪ��׺
			for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
				CWebRTDataPtr pRTData = make_shared<CWebRTData>();
				pRTData->SetDataSource(__NETEASE_RT_WEB_DATA__);
				if (pRTData->ReadNeteaseData(it)) {
					llTotal++;
					gl_WebRTDataContainer.PushNeteaseData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
				}
				else break;
			}
		}
	}
	gl_pChinaMarket->IncreaseRTDataReceived(llTotal);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����б���ѯ�Ĺ�Ʊ��Ϊ�����й�Ʊʱ����Ѷʵʱ��Ʊ�������᷵��һ��21���ַ������ַ�����v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IsTengxunRTDataInvalid(CWebData& WebDataReceived) {
	char buffer[50]{};
	char* pBuffer = buffer;
	CString strInvalidStock = _T("v_pv_none_match=\"1\";\n"); // ��Ϊ��Ч��Ʊ��ѯ�������ݸ�ʽ����21���ַ�

	WebDataReceived.GetData(pBuffer, 21, WebDataReceived.GetCurrentPos());
	buffer[21] = 0x000;
	CString str1 = buffer;

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
// 0: �г����Ϻ�Ϊ1������Ϊ51����
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
// 49 ��        ֮�����Щ���ֲ�����京��
//
//
// ��Ѷʵʱ�����У��ɽ����ĵ�λΪ�֣��޷��ﵽ��������ľ��ȣ��ɣ����ʶ�ֻ����Ϊ���ݲ���֮�á�
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool ParseWebRTDataGetFromTengxunServer(void) {
	CWebDataPtr pWebDataReceived = nullptr;
	bool fSucceed = true;

	const size_t lTotalData = gl_WebInquirer.GetTengxunRTDataSize();
	for (int i = 0; i < lTotalData; i++) {
		pWebDataReceived = gl_WebInquirer.PopTengxunRTData();
		pWebDataReceived->ResetCurrentPos();
		if (!IsTengxunRTDataInvalid(*pWebDataReceived)) { // ������21���ַ����ĺ������Է������Ҳ���Է�����ǰ�档
			while (!pWebDataReceived->IsProcessedAllTheData()) {
				CWebRTDataPtr pRTData = make_shared<CWebRTData>();
				if (pRTData->ReadTengxunData(pWebDataReceived)) {
					gl_WebRTDataContainer.PushTengxunData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
				}
				else {
					fSucceed = false;
					break;// ��������ݳ����⣬�׵����á�
				}
			}
		}
	}

	return fSucceed;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// �������������߷������϶�ȡ�Ĺ�Ʊ�������ݡ�
// ������ʽΪ�� ����,��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,ǰ����,�ǵ���,������,�ɽ���,�ɽ����,����ֵ,��ͨ��ֵ\r\n
//
// ��������������ģ��������ڵ���ǰ�档
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool ParseDayLineGetFromNeeteaseServer(void) {
	CNeteaseDayLineWebDataPtr pData = nullptr;
	CWebDataPtr pWebData = nullptr;

	while (gl_WebInquirer.GetNeteaseDayLineDataSize() > 0) {
		pWebData = gl_WebInquirer.PopNeteaseDayLineData();
		pData = make_shared<CNeteaseDayLineWebData>();
		pData->TransferWebDataToBuffer(pWebData);
		pData->ProcessNeteaseDayLineData();// pData����������������ģ��������ڵ���ǰ�档
		gl_WebInquirer.PushParsedNeteaseDayLineData(pData);
	}
	return true;
}