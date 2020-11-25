#include "StakeSection.h"

CStakeSection::CStakeSection() : CObject() {
  m_fActive = false;
  m_wMarket = 0;
  m_strComment = _T("");
  m_lIndexNumber = 0;

  m_fBuildStakePtr = false;
}

CStakeSection::~CStakeSection(void) {
}