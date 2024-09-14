#pragma once
#include <vector>
#include <unordered_map>
#include <wx/wx.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


template<typename KeyType, typename ValueType>
void get_keys(const std::unordered_map<KeyType, ValueType>& map, wxArrayString& keysArray) {
    keysArray.clear();
    keysArray.reserve(map.size());  // Reserve space to avoid multiple allocations

    for (const auto& pair : map) {
        keysArray.Add(pair.first);
    }
}

inline std::string GenerateGUID()
{
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    return boost::uuids::to_string(uuid);
}