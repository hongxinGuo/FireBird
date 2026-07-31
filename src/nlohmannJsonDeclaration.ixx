module;

#include"nlohmann/json.hpp"

export module FireBirdLib.Accessory.NlohmannJson.Declaration;

import std;
using std::shared_ptr;

export {
	using nlohmannJson = nlohmann::ordered_json;
	using nlohmannJsonPtr = shared_ptr<nlohmannJson>;
}
