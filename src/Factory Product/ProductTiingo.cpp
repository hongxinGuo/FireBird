#include"pch.h"

#include"ProductTiingo.h"

bool CProductTiingo::IsValidData(const CWebDataPtr& pWebData) {
	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return false;
	if (pWebData->IsVoidJson()) return false;

	return true;
}
