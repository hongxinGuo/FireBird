#include"pch.h"

#include"ProductWebData.h"

IMPLEMENT_DYNCREATE(CProductWebSourceData, CObject)

CProductWebSourceData::CProductWebSourceData() : CObject() {
	m_pMarket = nullptr;
	m_lIndex = -1; // 设置为越界
}