#include"pch.h"
#include "globedef.h"
#include"SystemMessage.h"
#include"GlobeSymbolDef.h"
#include"Accessory.h"
#include"WebRTData.h"
#include"ChinaMarket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CWebRTData::Reset(void) {
	m_lDataSource = __INVALID_RT_WEB_DATA__;
	m_time = 0;
	m_strSymbol = _T("");
	m_strStockName = _T("");
	m_lLastClose = 0;
	m_lOpen = 0;
	m_lHigh = 0;
	m_lLow = 0;
	m_lNew = 0;
	m_llVolume = 0;
	m_llAmount = 0;
	m_llCurrentValue = m_llTotalValue = 0;
	m_lBuy = 0;
	m_lSell = 0;
	m_lHighLimit = 0;
	m_lLowLimit = 0;
	for (int i = 0; i < 5; i++) {
		m_lPBuy.at(i) = 0;
		m_lVBuy.at(i) = 0;
		m_lPSell.at(i) = 0;
		m_lVSell.at(i) = 0;
	}
	m_fActive = false;
}

CWebRTData::CWebRTData(void) : CObject() {
	Reset();
}

/////////////////////////////////////////////////////////////////////////////
// CWebRTData diagnostics

#ifdef _DEBUG
void CWebRTData::AssertValid() const {
	CObject::AssertValid();
}

void CWebRTData::Dump(CDumpContext& dc) const {
	CObject::Dump(dc);
}

#endif //_DEBUG

void CWebRTData::SetStockName(string& s) {
	CString strName3;
	wstring wsName;
	CStringW strWName;

	wsName = to_wide_string(s); // ������utf8ת�ɿ��ֽ��ַ���
	strWName = wsName.c_str(); // ����׼��Ŀ��ֽ��ַ���ת����CStringW��ʽ��
	strName3 = strWName; // ��CStringW��ʽת����CString
	m_strStockName = strName3;
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
bool CWebRTData::ReadSinaData(CWebDataPtr pSinaWebRTData) {
	char buffer1[100];
	char buffer2[20];
	char buffer3[100];
	CString strHeader = _T("var hq_str_s");
	long lStockCode = 0;
	double dTemp = 0;
	char bufferTest[2000];
	CString strStockSymbol = _T("");
	WORD wMarket;
	CString strSinaStockCode;
	bool fBadData = false;

	int i = 0;
	while ((!fBadData) && (pSinaWebRTData->GetData(i + pSinaWebRTData->GetCurrentPos()) != ';')) {
		bufferTest[i] = pSinaWebRTData->GetData(i + pSinaWebRTData->GetCurrentPos());
		i++;
		if ((i >= 1900) || ((i + pSinaWebRTData->GetCurrentPos()) >= (pSinaWebRTData->GetBufferLength() - 1))) {
			fBadData = true;
			break;
		}
	}
	bufferTest[i] = pSinaWebRTData->GetData(i + pSinaWebRTData->GetCurrentPos());
	i++;
	bufferTest[i] = 0x000;
	if (fBadData) {
		CString strTest = bufferTest;
		gl_systemMessage.PushInnerSystemInformationMessage(_T("SinaRTData�������ݳ����⣬�׵�����"));
		gl_systemMessage.PushInnerSystemInformationMessage(strTest);
		return false; // �������ݳ��ִ��󣬺���Ľ��׵�
	}

	try {
		m_fActive = false;    // ��ʼ״̬Ϊ��Ч����
		pSinaWebRTData->GetData(buffer1, 12, pSinaWebRTData->GetCurrentPos()); // ���롰var hq_str_s"
		buffer1[12] = 0x000;
		CString str1;
		str1 = buffer1;
		if (strHeader.Compare(str1) != 0) { // ���ݸ�ʽ����
			throw exception();
		}
		pSinaWebRTData->IncreaseCurrentPos(12);

		if (pSinaWebRTData->GetCurrentPosData() == 'h') { // �Ϻ���Ʊ
			wMarket = __SHANGHAI_MARKET__; // �Ϻ���Ʊ��ʶ
		}
		else if (pSinaWebRTData->GetCurrentPosData() == 'z') {
			wMarket = __SHENZHEN_MARKET__; // ���ڹ�Ʊ��ʶ
		}
		else {
			throw exception();
		}
		pSinaWebRTData->IncreaseCurrentPos();

		pSinaWebRTData->GetData(buffer2, 6, pSinaWebRTData->GetCurrentPos());
		buffer2[6] = 0x000;
		strStockSymbol = buffer2;
		switch (wMarket) {
		case __SHANGHAI_MARKET__:
			strSinaStockCode = _T("sh") + strStockSymbol; // �����Ϻ����ڹ�Ʊ�������ص����ʶ����еĹ�Ʊ���붼�����г�ǰ׺���Ϻ�Ϊsh
			break;
		case __SHENZHEN_MARKET__:
			strSinaStockCode = _T("sz") + strStockSymbol;// �����Ϻ����ڹ�Ʊ�������ص����ʶ����еĹ�Ʊ���붼�����г�ǰ׺������Ϊsz
			break;
		default:
			throw exception();
		}
		m_strSymbol = XferSinaToStandred(strSinaStockCode);
		lStockCode = static_cast<long>(atof(buffer2));
		pSinaWebRTData->IncreaseCurrentPos(6);

		pSinaWebRTData->GetData(buffer1, 2, pSinaWebRTData->GetCurrentPos()); // ����'="'
		if ((buffer1[0] != '=') || (buffer1[1] != '"')) {
			throw exception();
		}
		pSinaWebRTData->IncreaseCurrentPos(2);
		pSinaWebRTData->GetData(buffer1, 2, pSinaWebRTData->GetCurrentPos());
		if (buffer1[0] == '"') { // û������?
			if (buffer1[1] != ';') {
				throw exception();
			}
			pSinaWebRTData->IncreaseCurrentPos(2);
			if (pSinaWebRTData->GetCurrentPosData() != 0x00a) {
				return false;
			}
			pSinaWebRTData->IncreaseCurrentPos();
			m_fActive = false;
			SetDataSource(__SINA_RT_WEB_DATA__);
			return true;  // �ǻ�Ծ��Ʊû��ʵʱ���ݣ��ڴ˷��ء�
		}
		if ((buffer1[0] == 0x00a) || pSinaWebRTData->OutOfRange()) {
			throw exception();
		}
		if ((buffer1[1] == 0x00a) || pSinaWebRTData->OutOfRange()) {
			throw exception();
		}
		pSinaWebRTData->IncreaseCurrentPos(2);

		i = 2;
		while ((pSinaWebRTData->GetCurrentPosData() != ',') && (i < 10)) { // ����ʣ�µ��������֣���һ������buffer1�У�
			if ((pSinaWebRTData->GetCurrentPosData() == 0x00a) || pSinaWebRTData->OutOfRange()) {
				throw exception();
			}
			buffer1[i++] = pSinaWebRTData->GetCurrentPosData();
			pSinaWebRTData->IncreaseCurrentPos();
		}
		buffer1[i] = 0x000;
		m_strStockName = buffer1; // ���ù�Ʊ����

		pSinaWebRTData->IncreaseCurrentPos();

		// ���뿪�̼ۡ��Ŵ�һǧ����洢Ϊ�����͡������۸�����ˡ�
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lOpen = static_cast<long>((dTemp + 0.000001) * 1000);
		// ����ǰ���̼�
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lLastClose = static_cast<long>((dTemp + 0.000001) * 1000);
		// ���뵱ǰ��
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lNew = static_cast<long>((dTemp + 0.000001) * 1000);
		// ������߼�
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lHigh = static_cast<long>((dTemp + 0.000001) * 1000);
		// ������ͼ�
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lLow = static_cast<long>((dTemp + 0.000001) * 1000);
		// ���뾺���
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lBuy = static_cast<long>((dTemp + 0.000001) * 1000);
		// ���뾺����
		if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
			throw exception();
		}
		m_lSell = static_cast<long>((dTemp + 0.000001) * 1000);
		// ����ɽ��������ɽ������洢ʵ��ֵ
		if (!ReadSinaOneValue(pSinaWebRTData, m_llVolume)) {
			throw exception();
		}
		// ����ɽ����
		if (!ReadSinaOneValue(pSinaWebRTData, m_llAmount)) {
			throw exception();
		}
		// ������һ--����Ĺ����ͼ۸�
		for (int j = 0; j < 5; j++) {
			// ��������
			if (!ReadSinaOneValue(pSinaWebRTData, m_lVBuy.at(j))) {
				throw exception();
			}
			// ����۸�
			if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
				throw exception();
			}
			m_lPBuy.at(j) = static_cast<long>((dTemp + 0.000001) * 1000);
		}
		// ������һ--����Ĺ����ͼ۸�
		for (int j = 0; j < 5; j++) {
			// ��������
			if (!ReadSinaOneValue(pSinaWebRTData, m_lVSell.at(j))) {
				throw exception();
			}
			// ����۸�
			if (!ReadSinaOneValue(pSinaWebRTData, dTemp)) {
				throw exception();
			}
			m_lPSell.at(j) = static_cast<long>((dTemp + 0.000001) * 1000);
		}
		// ����ɽ����ں�ʱ�䡣��ʱ��Ϊ��������������׼ʱ�䣩��
		if (!ReadSinaOneValue(pSinaWebRTData, buffer1)) {
			throw exception();
		}
		CString strTime;
		strTime = buffer1;
		strTime += ' '; //���һ���ո������������ת��
		if (!ReadSinaOneValue(pSinaWebRTData, buffer3)) {
			throw exception();
		}
		strTime += buffer3;
		m_time = ConvertBufferToTime("%04d-%02d-%02d %02d:%02d:%02d", strTime.GetBuffer()); // ת��UTCʱ�䡣����ʵʱ���ݵ�ʱ����Ĭ�ϵĶ�������ͬ���ʶ��������ʱ��ƫ����

		// ��������ݽ�Ϊ��Ч���ݣ����������ݵĽ�β�����ɡ�
		while (pSinaWebRTData->GetCurrentPosData() != 0x00a) { // Ѱ���ַ�'\n'���س�����
			pSinaWebRTData->IncreaseCurrentPos();
			if ((pSinaWebRTData->GetCurrentPos() >= pSinaWebRTData->GetBufferLength())) {
				throw exception();
			}
		}
		pSinaWebRTData->IncreaseCurrentPos(); // �����ַ�'\n'
		// �жϴ�ʵʱ�����Ƿ���Ч�������ڴ��жϣ�������ǽ�����Ч��Ʊ������٣����еĹ�Ʊ�����ݣ�����ֵ��Ϊ�㣬�����ɽ��ջ��Ʊ��ʱ��Ҫʵʱ��������Ч�ģ���
		// 0.03�汾����֮ǰ�Ķ�û�����жϣ�0.04�汾����ʹ�ò��жϵ����ְɡ�
		// ��ϵͳ׼�����ǰ���ж����˻�Ծ��Ʊ����ֻʹ�óɽ�ʱ��һ��ʶ���Ȼ���ڷǻ�Ծ��Ʊ�����С�
		// 0.07��󣬲���ʮ�����ڵ�ʵʱ����Ϊ��Ծ��Ʊ���ݣ���Ĵ��ڷż����죬����ǰ�����Ϣ�գ���ʮ�죬�������죩
		CheckSinaRTDataActive();
		SetDataSource(__SINA_RT_WEB_DATA__);
		return true;
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadSinaData�쳣 "), e);
		return false;
	}
}

bool CWebRTData::CheckSinaRTDataActive(void) {
	if (IsValidTime(14)) m_fActive = true;
	else m_fActive = false;

	return m_fActive;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ��INT64ֵ���������Ž���������ֵ��llReturnValue��
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaOneValue(CWebDataPtr pSinaWebRTData, INT64& llReturnValue) {
	INT64 llTemp;
	char buffer3[200];

	if (!ReadSinaOneValue(pSinaWebRTData, buffer3)) {
		return false;
	}
	llTemp = static_cast<INT64>(atof(buffer3));
	if (llTemp < 0) return false;
	if (llTemp > 0) llReturnValue = llTemp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ��ֵ���������Ž���������ֵ��lReturnValue��
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaOneValue(CWebDataPtr pSinaWebRTData, long& lReturnValue) {
	long lTemp;
	char buffer3[200];

	if (!ReadSinaOneValue(pSinaWebRTData, buffer3)) {
		return false;
	}
	lTemp = static_cast<long>(atof(buffer3));
	if (lTemp < 0) return false;
	if (lTemp > 0) lReturnValue = lTemp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ��ֵ���������Ž���������ֵ��lReturnValue��
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaOneValue(CWebDataPtr pSinaWebRTData, double& dReturnValue) {
	char buffer3[200];

	if (!ReadSinaOneValue(pSinaWebRTData, buffer3)) {
		return false;
	}
	dReturnValue = atof(buffer3);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ��ֵ���������Ž�����
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadSinaOneValue(CWebDataPtr pSinaWebRTData, char* buffer) {
	int i = 0;
	try {
		while ((pSinaWebRTData->GetCurrentPosData() != ',')) {
			if ((pSinaWebRTData->GetCurrentPosData() == 0x00a) || pSinaWebRTData->OutOfRange()) throw exception();
			if (i > 150) throw exception();
			buffer[i++] = pSinaWebRTData->GetCurrentPosData();
			pSinaWebRTData->IncreaseCurrentPos();
		}
		buffer[i] = 0x000;
		// ���','�š�
		pSinaWebRTData->IncreaseCurrentPos();

		return true;
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadSinaDataOneValue�쳣 "), e);
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
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
bool CWebRTData::ReadTengxunData(CWebDataPtr pTengxunWebRTData) {
	char buffer1[200];
	char buffer2[7];
	char buffer3[200];
	CString strHeader = _T("v_s");
	long lTemp = 0;
	INT64 llTemp = 0;
	double dTemp = 0.0;
	float fTemp = 0.0;
	long lStockCode = 0;
	WORD wMarket;
	CString strStockSymbol, strTengxunStockCode;

	try {
		m_fActive = false;    // ��ʼ״̬Ϊ��Ч����
		pTengxunWebRTData->GetData(buffer1, 3, pTengxunWebRTData->GetCurrentPos()); // ���롰v_s"
		buffer1[3] = 0x000;
		CString str1;
		str1 = buffer1;
		if (strHeader.Compare(str1) != 0) { // ���ݸ�ʽ����
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData��ʽ����"));
			return false;
		}
		pTengxunWebRTData->IncreaseCurrentPos(3);
		if (pTengxunWebRTData->GetCurrentPosData() == 'h') { // �Ϻ���Ʊ
			wMarket = __SHANGHAI_MARKET__; // �Ϻ���Ʊ��ʶ
		}
		else if (pTengxunWebRTData->GetCurrentPosData() == 'z') {
			wMarket = __SHENZHEN_MARKET__; // ���ڹ�Ʊ��ʶ
		}
		else {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������Ч�г�����"));
			return false;
		}
		pTengxunWebRTData->IncreaseCurrentPos();

		// ��λ��Ʊ����
		pTengxunWebRTData->GetData(buffer2, 6, pTengxunWebRTData->GetCurrentPos());
		buffer2[6] = 0x000;
		strStockSymbol = buffer2;
		switch (wMarket) {
		case __SHANGHAI_MARKET__:
			strTengxunStockCode = _T("sh") + strStockSymbol; // �����Ϻ����ڹ�Ʊ�������ص����ʶ����еĹ�Ʊ���붼�����г�ǰ׺���Ϻ�Ϊsh
			break;
		case __SHENZHEN_MARKET__:
			strTengxunStockCode = _T("sz") + strStockSymbol;// �����Ϻ����ڹ�Ʊ�������ص����ʶ����еĹ�Ʊ���붼�����г�ǰ׺������Ϊsz
			break;
		default:
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������Ч�г�����"));
			return false;
		}
		m_strSymbol = XferTengxunToStandred(strTengxunStockCode);
		lStockCode = atoi(buffer2);
		pTengxunWebRTData->IncreaseCurrentPos(6);

		pTengxunWebRTData->GetData(buffer1, 2, pTengxunWebRTData->GetCurrentPos()); // ����'="'
		if (buffer1[0] != '=') {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������Ҫ'='"));
			return false;
		}
		if (buffer1[1] != '"') {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������Ҫ'\"'"));
			return false;
		}
		pTengxunWebRTData->IncreaseCurrentPos(2);

		// �г���ʶ���루51Ϊ���У�1Ϊ���У�
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData�����г���ʶ����"));
			return false;
		}
#ifdef DEBUG
		if (lTemp == 1) ASSERT(wMarket == __SHANGHAI_MARKET__);
		else if (lTemp == 51) ASSERT(wMarket == __SHENZHEN_MARKET__);
		else ASSERT(0); // ����
#endif
		if (!ReadTengxunOneValue(pTengxunWebRTData, buffer1)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���󣺹�Ʊ����"));
			return false;
		}
		m_strStockName = buffer1; // ���ù�Ʊ����
		// ��λ��Ʊ����
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���󣺹�Ʊ����"));
			return false;
		}
		if (lTemp != lStockCode) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���󣺹�Ʊ���벻��"));
			return false;
		}

		// ���ڳɽ��ۡ��Ŵ�һǧ����洢Ϊ�����͡������۸�����ˡ�
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���󣺳ɽ���"));
			return false;
		}
		m_lNew = static_cast<long>((dTemp + 0.000001) * 1000);
		// ǰ���̼�
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData����ǰ����"));
			return false;
		}
		m_lLastClose = static_cast<long>((dTemp + 0.000001) * 1000);
		// ���̼�
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���󣺿��̼�"));
			return false;
		}
		m_lOpen = static_cast<long>((dTemp + 0.000001) * 1000);
		// �ɽ��������ɽ������洢ʵ��ֵ
		// ��ʹ�ô˴��ĳɽ���������ʹ�õ���ʮ����ĳɽ�����
		if (!ReadTengxunOneValue(pTengxunWebRTData, llTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���󣺳ɽ�����"));
			return false;
		}
		// ����
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData��������"));
			return false;
		}
		// ����
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData��������"));
			return false;
		}
		// ������һ������ļ۸������
		for (int j = 0; j < 5; j++) {
			// ���̼۸�
			if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
				gl_systemMessage.PushErrorMessage(_T("ReadTengxunData�������̼۸�"));
				return false;
			}
			m_lPBuy.at(j) = static_cast<long>((dTemp + 0.000001) * 1000);

			// �����������֣�
			if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
				gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������������"));
				return false;
			}
			m_lVBuy.at(j) = lTemp * 100;
		}
		// ������һ������ļ۸������
		for (int j = 0; j < 5; j++) {
			//�������̼۸�
			if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
				gl_systemMessage.PushErrorMessage(_T("ReadTengxunData�������̼۸�"));
				return false;
			}
			m_lPSell.at(j) = static_cast<long>((dTemp + 0.000001) * 1000);
			// �����������֣�
			if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
				gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������������"));
				return false;
			}
			m_lVSell.at(j) = lTemp * 100;
		}
		// �����ʳɽ�
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���������ʳɽ�"));
			return false;
		}
		// �ɽ����ں�ʱ��.��ʽΪ��yyyymmddhhmmss. ��ʱ����õ�ʱ��Ϊ��������������׼ʱ�䣩
		if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���󣺳ɽ����ں�ʱ��"));
			return false;
		}
		m_time = ConvertBufferToTime("%04d%02d%02d%02d%02d%02d", buffer3); // ת��UTCʱ�䡣��Ѷʵʱ���ݵ�ʱ����Ĭ�ϵĶ�������ͬ���ʶ��������ʱ��ƫ����
		// �ǵ�
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData�����ǵ�"));
			return false;
		}
		// �ǵ���
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData�����ǵ���"));
			return false;
		}
		// ��߼�
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������߼�"));
			return false;
		}
		m_lHigh = static_cast<long>((dTemp + 0.000001) * 1000);
		// ��ͼ�
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������ͼ�"));
			return false;
		}
		m_lLow = static_cast<long>((dTemp + 0.000001) * 1000);
		// ����ʮ����ɽ���/�ɽ������֣�/�ɽ���Ԫ��
		// �ɽ����ͳɽ����ʹ�ô˴������ݣ������Ϳ���ʹ����Ѷʵʱ������
		if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���󣺳ɽ��ۡ��ɽ��������ɽ���"));
			return false;
		}
		sscanf_s(buffer3, _T("%f/%d/%I64d"), &fTemp, &lTemp, &m_llAmount);
		m_llVolume = lTemp * 100; // ��Ѷ�ɽ������ݵ�λΪ�֣�100�ɣ���
		// �ɽ�����
		// ��ʹ�ô˴��ĳɽ���������ĳɽ�������ڵ���ʮ�崦�ĳɽ�����
		if (!ReadTengxunOneValue(pTengxunWebRTData, lTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���󣺳ɽ���"));
			return false;
		}
		// �ɽ�����Ԫ��
		if (!ReadTengxunOneValue(pTengxunWebRTData, llTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���󣺳ɽ����"));
			return false;
		}
		// ������
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���󣺻�����"));
			return false;
		}
		// ��ӯ��
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������ӯ��"));
			return false;
		}
		// ����
		if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData��������"));
			return false;
		}
		// ��߼�
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������߼�"));
			return false;
		}
		// ��ͼ�
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������ͼ�"));
			return false;
		}
		// ���
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData�������"));
			return false;
		}
		// ��ͨ��ֵ����λΪ����Ԫ��
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������ͨ��ֵ"));
			return false;
		}
		m_llCurrentValue = static_cast<INT64>(dTemp * 100000000);
		// ����ֵ����λΪ����Ԫ��
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData��������ֵ"));
			return false;
		}
		m_llTotalValue = static_cast<INT64>(dTemp * 100000000);
		// �о���
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData�����о���"));
			return false;
		}
		// ��ͣ��
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData������ͣ��"));
			return false;
		}
		if (dTemp > 0.01) m_lHighLimit = static_cast<long>((dTemp + 0.000001) * 1000);
		// ��ͣ��
		if (!ReadTengxunOneValue(pTengxunWebRTData, dTemp)) {
			gl_systemMessage.PushErrorMessage(_T("ReadTengxunData���󣺵�ͣ��"));
			return false;
		}
		if (dTemp > 0.01) m_lLowLimit = static_cast<long>((dTemp + 0.000001) * 1000);

		// ��������ݾ������ݲ��������ʱ�������롣
		while (pTengxunWebRTData->GetCurrentPosData() != 0x00a) {
			pTengxunWebRTData->IncreaseCurrentPos();
			if (pTengxunWebRTData->OutOfRange()) {
				return false;
			}
		}
		pTengxunWebRTData->IncreaseCurrentPos();
		CheckTengxunRTDataActive();
		SetDataSource(__TENGXUN_RT_WEB_DATA__);
		return true;
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadTengxunData�쳣 "), e);
		return false;
	}
}

bool CWebRTData::CheckTengxunRTDataActive() {
	if (!IsValidTime(14)) { // �������ʱ����14��ǰ
		m_fActive = false;
	}
	else if ((m_lOpen == 0) && (m_llVolume == 0) && (m_lHigh == 0) && (m_lLow == 0)) { // ��Ѷ�ǻ�Ծ��Ʊ��m_lNew��Ϊ�㣬�ʶ�����ʹ������Ϊ�ж�����
		m_fActive = false; // ��Ѷ�ǻ�Ծ��Ʊ��ʵʱ����Ҳ�������е��ֶΣ��ʶ��ڴ�ȷ����Ϊ�ǻ�Ծ
	}
	else m_fActive = true;

	return m_fActive;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ��INT64ֵ������~�Ž���������ֵ��llReturnValue��
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunOneValue(CWebDataPtr pTengxunWebRTData, INT64& llReturnValue) {
	INT64 llTemp;
	char buffer3[200];

	if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
		return false;
	}
	llTemp = atoll(buffer3);
	if (llTemp < 0) return false;
	if (llTemp > 0) llReturnValue = llTemp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ��������ֵ������~�Ž���������ֵ��dReturnValue��
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunOneValue(CWebDataPtr pTengxunWebRTData, double& dReturnValue) {
	double dTemp;
	char buffer3[200];

	if (!ReadTengxunOneValue(pTengxunWebRTData, buffer3)) {
		return false;
	}
	dTemp = atof(buffer3);
	dReturnValue = dTemp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ��������ֵ������~�Ž���������ֵ��lReturnValue��
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunOneValue(CWebDataPtr pWebDataReceived, long& lReturnValue) {
	long lTemp;
	char buffer3[200];

	if (!ReadTengxunOneValue(pWebDataReceived, buffer3)) {
		return false;
	}
	lTemp = atol(buffer3);
	if (lTemp < 0) return false;
	if (lTemp > 0) lReturnValue = lTemp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����һ���ַ���������~�Ž����������buffer�С�
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadTengxunOneValue(CWebDataPtr pWebDataReceived, char* buffer) {
	int i = 0;
	try {
		while (pWebDataReceived->GetCurrentPosData() != '~') {
			if ((pWebDataReceived->GetCurrentPosData() == 0x00a) || pWebDataReceived->OutOfRange()) return false;
			buffer[i++] = pWebDataReceived->GetCurrentPosData();
			pWebDataReceived->IncreaseCurrentPos();
		}
		buffer[i] = 0x000;
		pWebDataReceived->IncreaseCurrentPos();
		return true;
	}
	catch (exception& e) {
		buffer[i] = 0x000;
		CString errorMessage = pWebDataReceived->GetStockCode() + _T(" ReadTengxunOnValue�쳣:");
		errorMessage += buffer;
		ReportErrorToSystemMessage(errorMessage, e);
		return false;
	}
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
// ����ʵʱ����ȱ�ٹؼ��Եĳɽ����һ��ʶ��޷���Ϊ�������ݣ�ֻ����Ϊ�����á�
// ��turnover��Ϊ�ɽ�������ʹ��֮��05/12/2020��
//
//  �������Ĺ�Ʊ���Ƶ���ʽ��������ʱ��ʹ�ã�����boost ptree�����ĵ�֧�ֲ��㣬��ֻ֧��utf8��ʽ��������ȡ�����ַ�ʱ�������룩��
// ���ڲ���wstring��CStringW���ι��ɣ��Ϳ���������ʾ�ˡ�
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::ReadNeteaseData(ptree::iterator& it) {
	ptree pt1, pt2;
	bool fSucceed = true;
	string strSymbol, strSymbol2, strTime, strUpdateTime, strName;
	double dHigh, dLow, dNew, dOpen, dLastClose;
	array<double, 5> aAsk{ 0,0,0,0,0 }, aBid{ 0,0,0,0,0 };
	CString strSymbol4, str1, strName3;
	string sName;

	try {
		pt1 = it->second;
		strSymbol = it->first;
		strSymbol4 = XferNeteaseToStandred(strSymbol.c_str());
		SetSymbol(strSymbol4);
		sName = pt1.get<string>(_T("name")); // �˴������Ĺ�Ʊ����Ϊ���루ϵͳʹ�ö��ֽ��ַ������˴�Ϊutf-8�ַ�����
		SetStockName(sName); // ��utf-8�ַ���ת��Ϊ���ֽ��ַ���
		strTime = pt1.get<string>(_T("time"));
		strSymbol2 = pt1.get<string>(_T("code"));
		m_time = ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str());
	}
	catch (ptree_error& e) { // �ṹ������
		// do nothing
		CString strError2 = strSymbol4;
		strError2 += _T(" ");
		strError2 += e.what();
		gl_systemMessage.PushErrorMessage(strError2);
		fSucceed = false;
	}
	try {
		SetVolume(ptreeGetLongLong(pt1, _T("volume")));
		m_llAmount = ptreeGetLongLong(pt1, _T("turnover"));
		dHigh = ptreeGetDouble(pt1, _T("high"));
		SetHigh(dHigh * 1000);
		dLow = ptreeGetDouble(pt1, _T("low"));
		SetLow(dLow * 1000);
		dNew = ptreeGetDouble(pt1, _T("price"));
		SetNew(dNew * 1000);
		dLastClose = ptreeGetDouble(pt1, _T("yestclose"));
		SetLastClose(dLastClose * 1000);
		dOpen = ptreeGetDouble(pt1, _T("open"));
		SetOpen(dOpen * 1000);

		SetVBuy(0, ptreeGetLong(pt1, _T("bidvol1")));
		SetVBuy(1, ptreeGetLong(pt1, _T("bidvol2")));
		SetVBuy(2, ptreeGetLong(pt1, _T("bidvol3")));
		SetVBuy(3, ptreeGetLong(pt1, _T("bidvol4")));
		SetVBuy(4, ptreeGetLong(pt1, _T("bidvol5")));
		SetVSell(0, ptreeGetLong(pt1, _T("askvol1")));
		SetVSell(1, ptreeGetLong(pt1, _T("askvol2")));
		SetVSell(2, ptreeGetLong(pt1, _T("askvol3")));
		SetVSell(3, ptreeGetLong(pt1, _T("askvol4")));
		SetVSell(4, ptreeGetLong(pt1, _T("askvol5")));
		aAsk[0] = ptreeGetDouble(pt1, _T("ask1"));
		SetPSell(0, aAsk[0] * 1000);
		aAsk[1] = ptreeGetDouble(pt1, _T("ask2"));
		SetPSell(1, aAsk[1] * 1000);
		aAsk[2] = ptreeGetDouble(pt1, _T("ask3"));
		SetPSell(2, aAsk[2] * 1000);
		aAsk[3] = ptreeGetDouble(pt1, _T("ask4"));
		SetPSell(3, aAsk[3] * 1000);
		aAsk[4] = ptreeGetDouble(pt1, _T("ask5"));
		SetPSell(4, aAsk[4] * 1000);
		aBid[0] = ptreeGetDouble(pt1, _T("bid1"));
		SetPBuy(0, aBid[0] * 1000);
		aBid[1] = ptreeGetDouble(pt1, _T("bid2"));
		SetPBuy(1, aBid[1] * 1000);
		aBid[2] = ptreeGetDouble(pt1, _T("bid3"));
		SetPBuy(2, aBid[2] * 1000);
		aBid[3] = ptreeGetDouble(pt1, _T("bid4"));
		SetPBuy(3, aBid[3] * 1000);
		aBid[4] = ptreeGetDouble(pt1, _T("bid5"));
		SetPBuy(4, aBid[4] * 1000);

		CheckNeteaseRTDataActive();
		fSucceed = true;
	}
	catch (ptree_error&) { // �ǻ�Ծ��Ʊ�������еȣ�
		SetActive(false);
		fSucceed = true;
	}
	SetActive(true);

	return fSucceed;
}

bool CWebRTData::CheckNeteaseRTDataActive(void) {
	m_fActive = false;
	if (!IsValidTime(14)) { // �ǻ�Ծ��Ʊ��updateʱ��Ϊ0��ת��Ϊtime_tʱΪ-1.
		return m_fActive;
	}
	if ((m_lOpen == 0) || (m_lNew == 0)) {// ���׷ǻ�Ծ��Ʊ��ʵʱ����Ҳ�������е��ֶΣ��ʶ��ڴ�ȷ����Ϊ�ǻ�Ծ
		return m_fActive;
	}
	m_fActive = true;

	return m_fActive;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// ʵʱ���ݵ���Чʱ�䷶ΧΪ��������ڡ������зż�ʱ�������������Ƿż�ǰ�����һ�����ݡ����ڷż�ʱ�������ʮһ��ʱ�䣬�������츻ԣ��
// �ʶ�ʮ�����ڵ����ݶ�����Ϊ����Чʱ�����ݣ������ܹ���֤���ɵ��ջ��Ʊ����
//
//////////////////////////////////////////////////////////////////////////////////////////////
bool CWebRTData::IsValidTime(long lDays) const {
	if (m_time < (gl_pChinaMarket->GetUTCTime() - lDays * 24 * 3600)) { // ȷ��ʵʱ���ݲ����ڵ�ǰʱ���14��ǰ�����ڷż��Ϊ7�죬����ǰ�����Ϣ�գ���ʮһ�죩
		return false;
	}
	else if (m_time > gl_pChinaMarket->GetUTCTime()) {
		return false;
	}
	else  return true;
}

void CWebRTData::SaveData(CSetRealTimeData& setRTData) {
	ASSERT(setRTData.IsOpen());

	setRTData.m_Time = ConvertValueToString(m_time);
	setRTData.m_Symbol = GetSymbol();
	setRTData.m_StockName = GetStockName();
	setRTData.m_New = ConvertValueToString(GetNew(), 1000);
	setRTData.m_High = ConvertValueToString(GetHigh(), 1000);
	setRTData.m_Low = ConvertValueToString(GetLow(), 1000);
	setRTData.m_LastClose = ConvertValueToString(GetLastClose(), 1000);
	setRTData.m_Open = ConvertValueToString(GetOpen(), 1000);
	setRTData.m_Volume = ConvertValueToString(GetVolume());
	setRTData.m_Amount = ConvertValueToString(GetAmount());
	setRTData.m_Stroke = _T("0");
	setRTData.m_PBuy1 = ConvertValueToString(GetPBuy(0), 1000);
	setRTData.m_VBuy1 = ConvertValueToString(GetVBuy(0));
	setRTData.m_PSell1 = ConvertValueToString(GetPSell(0), 1000);
	setRTData.m_VSell1 = ConvertValueToString(GetVSell(0));

	setRTData.m_PBuy2 = ConvertValueToString(GetPBuy(1), 1000);
	setRTData.m_VBuy2 = ConvertValueToString(GetVBuy(1));
	setRTData.m_PSell2 = ConvertValueToString(GetPSell(1), 1000);
	setRTData.m_VSell2 = ConvertValueToString(GetVSell(1));

	setRTData.m_PBuy3 = ConvertValueToString(GetPBuy(2), 1000);
	setRTData.m_VBuy3 = ConvertValueToString(GetVBuy(2));
	setRTData.m_PSell3 = ConvertValueToString(GetPSell(2), 1000);
	setRTData.m_VSell3 = ConvertValueToString(GetVSell(2));

	setRTData.m_PBuy4 = ConvertValueToString(GetPBuy(3), 1000);
	setRTData.m_VBuy4 = ConvertValueToString(GetVBuy(3));
	setRTData.m_PSell4 = ConvertValueToString(GetPSell(3), 1000);
	setRTData.m_VSell4 = ConvertValueToString(GetVSell(3));

	setRTData.m_PBuy5 = ConvertValueToString(GetPBuy(4), 1000);
	setRTData.m_VBuy5 = ConvertValueToString(GetVBuy(4));
	setRTData.m_PSell5 = ConvertValueToString(GetPSell(4), 1000);
	setRTData.m_VSell5 = ConvertValueToString(GetVSell(4));
}

void CWebRTData::AppendData(CSetRealTimeData& setRTData) {
	ASSERT(setRTData.IsOpen());
	setRTData.AddNew();
	SaveData(setRTData);
	setRTData.Update();
}

void CWebRTData::LoadData(CSetRealTimeData& setRTData) {
	ASSERT(setRTData.IsOpen());

	m_time = atoll(setRTData.m_Time);
	m_strSymbol = setRTData.m_Symbol;
	m_strStockName = setRTData.m_StockName;
	m_lLastClose = static_cast<long>(atof(setRTData.m_LastClose) * 1000);
	m_lOpen = static_cast<long>(atof(setRTData.m_Open) * 1000);
	m_lNew = static_cast<long>(atof(setRTData.m_New) * 1000);
	m_lHigh = static_cast<long>(atof(setRTData.m_High) * 1000);
	m_lLow = static_cast<long>(atof(setRTData.m_Low) * 1000);
	m_llVolume = atoll(setRTData.m_Volume);
	m_llAmount = atoll(setRTData.m_Amount);
	m_lPBuy.at(0) = static_cast<long>(atof(setRTData.m_PBuy1) * 1000);
	m_lVBuy.at(0) = atol(setRTData.m_VBuy1);
	m_lPBuy.at(1) = static_cast<long>(atof(setRTData.m_PBuy2) * 1000);
	m_lVBuy.at(1) = atol(setRTData.m_VBuy2);
	m_lPBuy.at(2) = static_cast<long>(atof(setRTData.m_PBuy3) * 1000);
	m_lVBuy.at(2) = atol(setRTData.m_VBuy3);
	m_lPBuy.at(3) = static_cast<long>(atof(setRTData.m_PBuy4) * 1000);
	m_lVBuy.at(3) = atol(setRTData.m_VBuy4);
	m_lPBuy.at(4) = static_cast<long>(atof(setRTData.m_PBuy5) * 1000);
	m_lVBuy.at(4) = atol(setRTData.m_VBuy5);
	m_lPSell.at(0) = static_cast<long>(atof(setRTData.m_PSell1) * 1000);
	m_lVSell.at(0) = atol(setRTData.m_VSell1);
	m_lPSell.at(1) = static_cast<long>(atof(setRTData.m_PSell2) * 1000);
	m_lVSell.at(1) = atol(setRTData.m_VSell2);
	m_lPSell.at(2) = static_cast<long>(atof(setRTData.m_PSell3) * 1000);
	m_lVSell.at(2) = atol(setRTData.m_VSell3);
	m_lPSell.at(3) = static_cast<long>(atof(setRTData.m_PSell4) * 1000);
	m_lVSell.at(3) = atol(setRTData.m_VSell4);
	m_lPSell.at(4) = static_cast<long>(atof(setRTData.m_PSell5) * 1000);
	m_lVSell.at(4) = atol(setRTData.m_VSell5);
}