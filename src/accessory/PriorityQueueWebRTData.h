#pragma once

#include <queue>
using std::shared_ptr;

class CWebRTData;

class CPriorityQueueWebRTData final {
public:
	CPriorityQueueWebRTData() = default;
	CPriorityQueueWebRTData(const CPriorityQueueWebRTData&) = delete;
	CPriorityQueueWebRTData& operator=(const CPriorityQueueWebRTData&) = delete;
	CPriorityQueueWebRTData(const CPriorityQueueWebRTData&&) noexcept = delete;
	CPriorityQueueWebRTData& operator=(const CPriorityQueueWebRTData&&) noexcept = delete;
	~CPriorityQueueWebRTData() = default;

	void Reset();

	// 通用接口函数
	void PushData(const shared_ptr<CWebRTData>& pData);
	shared_ptr<CWebRTData> PopData();
	[[nodiscard]] shared_ptr<CWebRTData> GetHead() const noexcept { return m_priorityQueueWebRTData.top(); }
	[[nodiscard]] size_t Size();

protected:
	// 需要定义下述结构，结构中重载（）运算符，定义如何确定指针的大小（按时间顺序从小到大排列,相同时间的按先后放入的顺序排列）。
	struct cmpRTData {
		bool operator()(const shared_ptr<CWebRTData>& p1, const shared_ptr<CWebRTData>& p2) const;
	};

protected:
	// 当系统更新到采集多元实时数据后，此队列需要改为有优先级的队列，优先级为实时数据的交易时间，
	// 这样就能够将多处采集到的实时数据按照时间排列为一个队列。
	// 大致为这样： priority_queue<CStockRTDataPtr, vector<CStockRTDataDataPtr>, cmpRTData> m_qRTStockData;
	// 结构cmpRTData定义为：
	// struct cmpRTData {
	//   bool operator () (const CWebRTDataPtr p1, const CWebRTDataPtr p2) const {
	//     return(p1->GetMarketTime() > p2->GetMarketTime());
	//   }
	// };
	std::priority_queue<shared_ptr<CWebRTData>, std::vector<shared_ptr<CWebRTData>>, cmpRTData> m_priorityQueueWebRTData;
	std::mutex m_MutexAccessData; // 互斥。
};
