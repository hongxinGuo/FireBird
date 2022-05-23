/////////////////////////////////////////////////////////////////////////////////////
//
// 存储实时数据的聚合类。只有头文件。
//
// 此队列中存储的是经过处理的实时数据，每条数据为一个股票的即时交易状态。
//
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"PriorityQueueWebRTData.h"

class CWebRTDataContainer final : public CObject {
public:
	CWebRTDataContainer();
	// 不允许赋值。
	CWebRTDataContainer(const CWebRTDataContainer&) = delete;
	CWebRTDataContainer& operator=(const CWebRTDataContainer&) = delete;
	CWebRTDataContainer(const CWebRTDataContainer&&) noexcept = delete;
	CWebRTDataContainer& operator=(const CWebRTDataContainer&&) noexcept = delete;
	~CWebRTDataContainer();

	void ResetSinaQueue(void) { m_qSinaRTData.Reset(); }
	size_t SinaDataSize(void) { return m_qSinaRTData.Size(); }
	void PushSinaData(CWebRTDataPtr pData) { m_qSinaRTData.PushData(pData); }
	CWebRTDataPtr PopSinaData(void) { return m_qSinaRTData.PopData(); }
	size_t TengxunDataSize(void) { return m_qTengxunRTData.Size(); }
	void PushTengxunData(CWebRTDataPtr pData) { m_qTengxunRTData.PushData(pData); }
	CWebRTDataPtr PopTengxunData(void) { return m_qTengxunRTData.PopData(); }
	size_t NeteaseDataSize(void) { return m_qNeteaseRTData.Size(); }
	void PushNeteaseData(CWebRTDataPtr pData) { m_qNeteaseRTData.PushData(pData); }
	CWebRTDataPtr PopNeteaseData(void) { return m_qNeteaseRTData.PopData(); }

protected:
	CPriorityQueueWebRTData m_qSinaRTData; // 新浪实时数据队列。
	CPriorityQueueWebRTData m_qTengxunRTData; // 腾讯实时数据队列。
	CPriorityQueueWebRTData m_qNeteaseRTData; // 网易实时数据队列。
};
