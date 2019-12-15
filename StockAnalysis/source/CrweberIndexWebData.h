#ifndef __CRWEBER_INDEX_WEB_DATA_H__
#define __CRWEBER_INDEX_WEB_DATA_H__

#include"WebData.h"

class CCrweberIndexWebData final : public CWebData
{
public:
  CCrweberIndexWebData();
  ~CCrweberIndexWebData();

  virtual bool GetWebData(void) override;
  virtual void InquireNextWebData(void) override;
  virtual int GetInquiringStr(CString& strInquire, long lTotalNumber = 900, bool fSkipUnactiveStock = true) override;
  virtual void StartReadingThread(void) override;
};

#endif
