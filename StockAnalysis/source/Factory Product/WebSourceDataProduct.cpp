#include"pch.h"

#include"WebSourceDataProduct.h"

IMPLEMENT_DYNCREATE(CWebSourceDataProduct, CObject)

CWebSourceDataProduct::CWebSourceDataProduct() : CObject() {
	m_pMarket = nullptr;
	m_lIndex = -1; // 设置为越界
}