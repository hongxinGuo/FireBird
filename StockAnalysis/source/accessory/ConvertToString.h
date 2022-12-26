#pragma once

[[nodiscard]] CString ConvertValueToString(const long lValue, const int iDividend = 1);
[[nodiscard]] CString ConvertValueToString(const INT64 iValue, const int iDividend = 1);
[[nodiscard]] CString ConvertValueToString(const int iValue, const int iDividend = 1);
[[nodiscard]] CString ConvertValueToString(const double dValue, const int iDividend = 1);
