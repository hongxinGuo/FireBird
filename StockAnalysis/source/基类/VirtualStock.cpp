#include"globedef.h"
#include"accessory.h"
#include "VirtualStock.h"

CVirtualStock::CVirtualStock() : CObject() {
  Reset();
}

CVirtualStock::~CVirtualStock() {
}

void CVirtualStock::Reset(void) {
  m_fTodayNewStock = false;
  m_fUpdateStockProfileDB = false;
  m_fActive = false;
}
