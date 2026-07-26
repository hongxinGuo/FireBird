#pragma once

using std::shared_ptr;

#include"nlohmann/json_fwd.hpp"
using nlohmannJson = nlohmann::ordered_json;

using nlohmannJsonPtr = shared_ptr<nlohmannJson>;
