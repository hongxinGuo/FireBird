module;

export module FireBirdLib.Accessory.SaveAndLoad;

import std;
using std::string;

export {
	void SaveToFile(const string& strFileName, string s);
	bool LoadFromFile(const string& strFileName, string& s);
}
