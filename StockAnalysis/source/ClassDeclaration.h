#pragma once

using namespace std;
#include<memory>

class CChinaStock;
typedef shared_ptr<CChinaStock> CChinaStockPtr;
class CChinaMarket;
typedef shared_ptr<CChinaMarket> CChinaMarketPtr;

class CSystemMessage;
class CThreadStatus;
class CWebRTDataContainer;
