#include "SectionIndex.h"

CSectionIndex::CSectionIndex() : CObject() {
  m_fActive = false;
  m_wMarket = 0;
  m_strComment = _T("");
  m_lIndexNumber = 0;
}

CSectionIndex::~CSectionIndex(void) {
}