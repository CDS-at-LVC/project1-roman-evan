#pragma once
#include <vector>
#include <unordered_map>
#include <wx/wx.h>


template<typename KeyType, typename ValueType>
void get_keys(const std::unordered_map<KeyType, ValueType>& map, wxArrayString& keysArray) {
    keysArray.reserve(map.size());  // Reserve space to avoid multiple allocations

    for (const auto& pair : map) {
        keysArray.Add(pair.first);
    }
}