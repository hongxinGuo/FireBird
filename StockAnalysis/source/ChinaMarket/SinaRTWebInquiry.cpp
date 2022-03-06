#include"pch.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "SinaRTWebInquiry.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CSinaRTWebInquiry::CSinaRTWebInquiry() : CVirtualWebInquiry() {
	// 2022年1月20日后，新浪实时数据服务器需要添加报头验证数据，格式为Referer:https://finance.sina.com.cn
	// User - Agent部分只用于说明格式,即报头皆以\r\n（CRLF)结束
	m_strHeaders = _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n");

	m_strWebDataInquirePrefix = _T("https://hq.sinajs.cn/list="); // 新浪实时数据服务器已使用https格式
	m_strWebDataInquireSuffix = _T("");
	m_strConnectionName = _T("SinaRT");
	m_lShortestInquiringInterval = 400; // 新浪实时数据每3秒服务器更新一次，故而每400毫秒查询一次即可在3秒内完成轮询一次的任务
	m_lInquiringNumber = 850; // 新浪实时数据查询数量默认值
}

CSinaRTWebInquiry::~CSinaRTWebInquiry() {
}

bool CSinaRTWebInquiry::TransferData(CWebDataPtr pWebData) {
	auto byteReaded = GetByteReaded();
	if (m_lContentLength > 0) {
		if (m_lContentLength != byteReaded) gl_systemMessage.PushErrorMessage(_T("网络数据长度不符：") + m_strInquire.Left(120));
	}
	m_sBuffer.resize(byteReaded);
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // 使用std::move以加速执行速度
	m_sBuffer.resize(1024 * 1024); // 重新分配内存
	pWebData->SetBufferLength(byteReaded);
	pWebData->ResetCurrentPos();
	return true;
}

bool CSinaRTWebInquiry::ProcessData(CWebDataPtr pWebData) {
	INT64 llTotal = 0;
	bool fSucceed = true;
	pWebData->ResetCurrentPos();
	while (!pWebData->IsProcessedAllTheData()) {
		if (gl_fExitingSystem) return fSucceed;
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		if (pRTData->ReadSinaData(pWebData)) {
			llTotal++;
			gl_WebRTDataContainer.PushSinaData(pRTData); // 将此实时数据指针存入实时数据队列
		}
		else {
			fSucceed = false;
			gl_systemMessage.PushErrorMessage(_T("新浪实时数据解析返回失败信息"));
			break;  // 后面的数据出问题，抛掉不用。
		}
	}
	gl_pChinaMarket->IncreaseRTDataReceived(llTotal);

	return fSucceed;
}

bool CSinaRTWebInquiry::PrepareNextInquiringStr(void) {
	CString strMiddle = _T("");
	CString strSinaStockCode;

	// 申请下一批次股票实时数据
	// 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
	// 开市时使用今日活跃股票池
	strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock());
	strSinaStockCode = strMiddle.Left(8); // 只提取第一个股票代码。新浪代码格式为：sh000001，共八个字符。
	gl_systemMessage.SetStockCodeForInquiringRTData(XferSinaToStandred(strSinaStockCode));
	CreateTotalInquiringString(strMiddle);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 新浪网络实时数据提取函数。
// 目前只提取前12000个股票的实时数据。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CSinaRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet) {
	return gl_pChinaMarket->GetSinaStockInquiringStr(lTotalNumber, fUsingTotalStockSet);
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个新浪实时数据\n", lNumberOfData);
	return true;
}