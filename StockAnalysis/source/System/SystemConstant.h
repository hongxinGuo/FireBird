#pragma once

// �й���Ʊ�г���ʼ��
constexpr auto _CHINA_MARKET_BEGIN_DATE_ = 19900101;

enum {
	_STOCK_NOT_CHECKED_ = 128, // ��δ�����Ĺ�Ʊ����
	_STOCK_IPOED_ = 255, // ������Ʊ��ʶ
	_STOCK_NULL_ = 127, // ��Ч���루�˴���Ĺ�Ʊ�����ڣ�
	_STOCK_DELISTED_ = 1, // �����У���ͣ�ƣ��Ĺ�Ʊ��ʶ
	_STOCK_NOT_YET_LIST_ = 2, // �Ѵ��ڹ�Ʊ���룬��δ���У���δ���еĹ�Ʊ���Ĺ�Ʊ��ʶ
};

enum {
	_SHANGHAI_MARKET_ = 1, // �Ϻ��г�
	_SHENZHEN_MARKET_ = 2, // �����г�
	_SHANGHAI_INDEX_ = 3, // �Ϻ�ָ��
	_SHENZHEN_INDEX_ = 4, // ����ָ��
	_SHANGHAI_MAIN_ = 5, // �Ϻ�����
	_SHENZHEN_MAIN_ = 6, // ��������
	_SHANGHAI_3BAN_ = 7, // �Ϻ�3��
	_SHENZHEN_3BAN_ = 8, //������С��
	_SHANGHAI_B_SHARE_ = 9, //�Ϻ�B��
	_SHENZHEN_B_SHARE_ = 10, // ����B��
	_SHANGHAI_KECHUANG_ = 11, // �Ϻ��ƴ���
	_SHENZHEN_CHUANGYE_ = 12, // ���ڴ�ҵ��
};
