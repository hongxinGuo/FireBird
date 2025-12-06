#pragma once

[[nodiscard]] CString ConvertValueToCString(long lValue, int iDividend = 1);
[[nodiscard]] CString ConvertValueToCString(INT64 iValue, int iDividend = 1);
[[nodiscard]] CString ConvertValueToCString(int iValue, int iDividend = 1);
[[nodiscard]] CString ConvertValueToCString(double dValue, int iDividend = 1);

[[nodiscard]] string ConvertValueToString(long lValue, int iDividend = 1);
[[nodiscard]] string ConvertValueToString(INT64 iValue, int iDividend = 1);
[[nodiscard]] string ConvertValueToString(int iValue, int iDividend = 1);
[[nodiscard]] string ConvertValueToString(double dValue, int iDividend = 1);

[[nodiscard]] string FormatToMK(int64_t iNumber);
