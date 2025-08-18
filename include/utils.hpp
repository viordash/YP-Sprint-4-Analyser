#pragma once

#include <charconv>
#include <stdexcept>
#include <string>
#include <string_view>

inline int ToInt(std::string_view value) {
    int result{};
    auto it_begin = value.begin();
    while (*it_begin == ' ' && it_begin != value.end()) {
        it_begin++;
    }
    auto [parse_end_ptr, error_code] = std::from_chars(it_begin, value.end(), result);
    if (error_code != std::errc{} || parse_end_ptr != value.data() + value.size()) {
        throw std::invalid_argument("Cannot convert '" + std::string(value) + "' to integral");
    }
    return result;
}
