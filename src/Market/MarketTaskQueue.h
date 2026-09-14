#pragma once

#include <queue>

#include"MarketTask.h"

using std::shared_ptr;
using std::chrono::local_seconds;
using std::chrono::hh_mm_ss;
using std::chrono::seconds;
using std::priority_queue;
using std::vector;

class CMarketTaskQueue {
public:
	CMarketTaskQueue();
	~CMarketTaskQueue() = default;

	void AddTask(const CMarketTaskPtr& pTask);
	void AddTask(long lTaskType, const hh_mm_ss<seconds>& lExecuteTime);
	void AddTask(long lTaskType, local_seconds executeTime);
	CMarketTaskPtr GetTask() const { return m_vMarketTask.top(); }
	void DiscardCurrentTask() { m_vMarketTask.pop(); }
	void DeleteTask(long lTaskType); // 删除指定类型的任务
	size_t Size() const { return m_vMarketTask.size(); }
	bool Empty() const { return m_vMarketTask.empty(); }

	vector<CMarketTaskPtr> GetTasks();

private:
	static void CreateIndexMap();

protected:
	// 需要定义下述结构，结构中重载()运算符，定义如何确定指针的大小（按时间顺序从小到大排列,相同时间的按先后放入的顺序排列）。
	struct cmpMarketTaskData {
		bool operator()(const CMarketTaskPtr& p1, const CMarketTaskPtr& p2) const noexcept;
	};

protected:
	std::priority_queue<CMarketTaskPtr, vector<CMarketTaskPtr>, cmpMarketTaskData> m_vMarketTask;
};
