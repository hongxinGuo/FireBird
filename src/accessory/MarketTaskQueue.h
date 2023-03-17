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
	// ��Ҫ���������ṹ���ṹ�����أ�����������������ȷ��ָ��Ĵ�С����ʱ��˳���С��������,��ͬʱ��İ��Ⱥ�����˳�����У���
	struct cmpMarketTaskData {
		bool operator()(const CMarketTaskPtr& p1, const CMarketTaskPtr& p2) const noexcept {
			// �����ȼ��Ķ���Ĭ������˳���ǴӴ�С���ʶ���С����������Ҫʹ�� > ���š�
			// ��ͬʱ������ݣ��ȷ��������λ�ں���������ǰ�档
			// ��ͬʱ������ݣ�ʱ����������λ��ʱ����������ǰ�档
			return (p1->GetTime() > p2->GetTime());
		}
	};

protected:
	priority_queue<CMarketTaskPtr, vector<CMarketTaskPtr>, cmpMarketTaskData> m_vMarketTask;
};
