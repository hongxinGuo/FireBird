#include "VirtualMarket.h"

CVirtualMarket::CVirtualMarket(void) {
  m_fReadyToRun = true;
}

CVirtualMarket::~CVirtualMarket(void) {
}

bool CVirtualMarket::SchedulingTask(void) {
  return false;
}