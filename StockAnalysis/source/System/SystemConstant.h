#pragma once

// �й���Ʊ�г���ʼ��
constexpr auto __CHINA_MARKET_BEGIN_DATE__ = 19900101;

enum {
	__STOCK_NOT_CHECKED__ = 128, // ��δ�����Ĺ�Ʊ����
	__STOCK_IPOED__ = 255, // ������Ʊ��ʶ
	__STOCK_NULL__ = 127, // ��Ч���루�˴���Ĺ�Ʊ�����ڣ�
	__STOCK_DELISTED__ = 1, // �����У���ͣ�ƣ��Ĺ�Ʊ��ʶ
	__STOCK_NOT_YET_LIST__ = 2, // �Ѵ��ڹ�Ʊ���룬��δ���У���δ���еĹ�Ʊ���Ĺ�Ʊ��ʶ
};

enum {
	__SHANGHAI_MARKET__ = 1, // �Ϻ��г�
	__SHENZHEN_MARKET__ = 2, // �����г�
	__SHANGHAI_INDEX__ = 3, // �Ϻ�ָ��
	__SHENZHEN_INDEX__ = 4, // ����ָ��
	__SHANGHAI_MAIN__ = 5, // �Ϻ�����
	__SHENZHEN_MAIN__ = 6, // ��������
	__SHANGHAI_3BAN__ = 7, // �Ϻ�3��
	__SHENZHEN_3BAN__ = 8, //������С��
	__SHANGHAI_B_SHARE__ = 9, //�Ϻ�B��
	__SHENZHEN_B_SHARE__ = 10, // ����B��
	__SHANGHAI_KECHUANG__ = 11, // �Ϻ��ƴ���
	__SHENZHEN_CHUANGYE__ = 12, // ���ڴ�ҵ��
};
