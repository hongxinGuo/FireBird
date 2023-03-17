#pragma once

#include<queue>
using std::priority_queue;

#include"MarketTask.h"

class CMarketTaskQueue {
public:
	CMarketTaskQueue();
	~CMarketTaskQueue() = default;

	void StoreTask(CMarketTaskPtr pTask) { m_vMarketTask.push(pTask); }
	CMarketTaskPtr GetTask() const { return m_vMarketTask.top(); }
	void DiscardTask() { m_vMarketTask.pop(); }
	size_t Size() const { return m_vMarketTask.size(); }

	bool IsEmpty() const {
		if (Size() == 0) return true;
		else return false;
	}

	vector<CMarketTaskPtr> GetTaskVector();

protected:
	void CreateIndexMap();

protected:
	// 需要定义下述结构，结构中重载（）运算符，定义如何确定指针的大小（按时间顺序从小到大排列,相同时间的按先后放入的顺序排列）。
	struct cmpMarketTaskData {
		bool operator()(const CMarketTaskPtr& p1, const CMarketTaskPtr& p2) const noexcept {
			// 有优先级的队列默认排列顺序是从大到小，故而从小到大排列需要使用 > 符号。
			// 相同时间的数据，先放入的数据位于后放入的数据前面。
			// 不同时间的数据，时间较早的数据位于时间较晚的数据前面。
			return (p1->GetTime() > p2->GetTime());
		}
	};

protected:
	priority_queue<CMarketTaskPtr, vector<CMarketTaskPtr>, cmpMarketTaskData> m_vMarketTask;
};
