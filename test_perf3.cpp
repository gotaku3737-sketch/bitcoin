#include <string>
#include <string_view>
#include <iostream>
#include <chrono>

std::string ToLower_optimized(std::string_view str)
{
    std::string r;
    r.resize(str.size());
    for (size_t i = 0; i < str.size(); ++i) r[i] = (str[i] >= 'A' && str[i] <= 'Z' ? (str[i] - 'A') + 'a' : str[i]);
    return r;
}
