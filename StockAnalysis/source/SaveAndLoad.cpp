#include "pch.h"

using namespace std;
#include<fstream>

#include "SaveAndLoad.h"

void SaveToFile(CString strFileName, string s) {
	fstream f(strFileName, ios::out);
	f << s;
	f.close();
}

bool LoadFromFile(CString strFileName, string& s) {
	string sLine;
	fstream f(strFileName, ios::in);
	if (f.is_open()) {
		while (f.good()) {
			f >> sLine;
			s += sLine;
		}
		f.close();
		return true;
	}
	return false;
}