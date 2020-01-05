#ifndef __CRWEBER_INDEX_WEB_DATA_H__
#define __CRWEBER_INDEX_WEB_DATA_H__

#include"WebData.h"

class CCrweberIndexWebData final : public CWebData
{
public:
  CCrweberIndexWebData();
  ~CCrweberIndexWebData();

  virtual void InquireNextWebData(void) override;
  virtual void StartReadingThread(void) override;
};

#endif
