#pragma once

#include"WebRTData.h"
#include"PriorityQueueWebRTData.h"

#include<queue>
using std::priority_queue;

class CPriorityQueueWebRTData final {
public:
	CPriorityQueueWebRTData();
	~CPriorityQueueWebRTData();
	void Reset(void);

	// ͨ�ýӿں���
	void PushData(CWebRTDataPtr pData);
	[[nodiscard]] CWebRTDataPtr PopData(void);
	[[nodiscard]] CWebRTDataPtr GetHead(void) const noexcept { return m_priorityQueueWebRTData.top(); }
	[[nodiscard]] size_t Size(void);

protected:
	// ��Ҫ���������ṹ���ṹ�����أ�����������������ȷ��ָ��Ĵ�С����ʱ��˳���С��������,��ͬʱ��İ��Ⱥ�����˳�����У���
	struct cmpRTData {
		bool operator()(const CWebRTDataPtr& p1, const CWebRTDataPtr& p2) const noexcept {
			// �����ȼ��Ķ���Ĭ������˳���ǴӴ�С���ʶ���С����������Ҫʹ�� > ���š�
			// ��ͬʱ������ݣ��ȷ��������λ�ں���������ǰ�档
			// ��ͬʱ������ݣ�ʱ����������λ��ʱ�����������ǰ�档
			return (p1->GetTransactionTime() > p2->GetTransactionTime());
		}
	};

protected:
	// ��ϵͳ���µ��ɼ���Ԫʵʱ���ݺ󣬴˶�����Ҫ��Ϊ�����ȼ��Ķ��У����ȼ�Ϊʵʱ���ݵĽ���ʱ�䣬
	// �������ܹ����ദ�ɼ�����ʵʱ���ݰ���ʱ������Ϊһ�����С�
	// ����Ϊ������ priority_queue<CStockRTDataPtr, vector<CStockRTDataDataPtr>, cmpRTData> m_qRTStockData;
	// �ṹcmpRTData����Ϊ��
	// struct cmpRTData {
	//   bool operator () (const CWebRTDataPtr p1, const CWebRTDataPtr p2) const {
	//     return(p1->GetMarketTime() > p2->GetMarketTime());
	//   }
	// };
	priority_queue<CWebRTDataPtr, vector<CWebRTDataPtr>, cmpRTData> m_priorityQueueWebRTData;
	mutex m_MutexAccessData; // ���⡣
};