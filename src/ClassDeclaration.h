//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 类声明。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include<memory>
using std::shared_ptr;
using std::weak_ptr;

class CDayLine;
typedef shared_ptr<CDayLine> CDayLinePtr;

// Web Inquiry
class CVirtualWebInquiry;
class CSinaRTWebInquiry;
class CTengxunRTWebInquiry;

// Web data source
//class CInquireEngine;
//typedef shared_ptr<CInquireEngine> CInquireEnginePtr;
//class CVirtualDataSource;
//typedef shared_ptr<CVirtualDataSource> CVirtualDataSourcePtr;

// Product
//class CVirtualWebProduct;
//typedef shared_ptr<CVirtualWebProduct> CVirtualWebProductPtr;
