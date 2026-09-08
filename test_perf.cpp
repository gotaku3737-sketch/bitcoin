#include <string>
#include <string_view>
#include <iostream>
#include <chrono>

constexpr char ToLower(char c)
{
    return (c >= 'A' && c <= 'Z' ? (c - 'A') + 'a' : c);
}

std::string ToLower_original(std::string_view str)
{
    std::string r;
    r.reserve(str.size());
    for (auto ch : str) r += ToLower(ch);
    return r;
}

std::string ToLower_optimized(std::string_view str)
{
    std::string r;
    r.resize(str.size());
    for (size_t i = 0; i < str.size(); ++i) r[i] = ToLower(str[i]);
    return r;
}

int main() {
    std::string test_str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for(int i=0; i<10; ++i) test_str += test_str; // Make it big

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i) {
        volatile auto res = ToLower_original(test_str);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Original: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i) {
        volatile auto res = ToLower_optimized(test_str);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Optimized: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << std::endl;

    return 0;
}
