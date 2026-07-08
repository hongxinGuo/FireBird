#pragma once

using std::string;

void SaveToFile(const string& strFileName, string s);
bool LoadFromFile(const string& strFileName, string& s);
