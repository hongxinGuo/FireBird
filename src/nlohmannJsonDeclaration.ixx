module;

#include"nlohmann/json.hpp"

export module NlohmannJsonDeclaration;

using std::shared_ptr;

export {
	using nlohmannJson = nlohmann::ordered_json;
	using nlohmannJsonPtr = shared_ptr<nlohmannJson>;
}
