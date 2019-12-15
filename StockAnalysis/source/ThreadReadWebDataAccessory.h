#ifndef __THREAD_READ_WEB_DATA_ACCESSORY_H__
#define __THREAD_READ_WEB_DATA_ACCESSORY_H__

#include"WebData.h"
#include"WebDataReceived.h"

CWebDataReceivedPtr TransferWebDataToBuffer(CWebData* pWebData);
void ThreadReadWebData(CWebData* pWebData, long lStartDelayTime, long lSecondDelayTime);

#endif
