#include <string>
#include <string_view>
#include <iostream>
#include <chrono>

constexpr char ToUpper(char c)
{
    return (c >= 'a' && c <= 'z' ? (c - 'a') + 'A' : c);
}

std::string ToUpper_original(std::string_view str)
{
    std::string r;
    r.reserve(str.size());
    for (auto ch : str) r += ToUpper(ch);
    return r;
}

std::string ToUpper_optimized(std::string_view str)
{
    std::string r;
    r.resize(str.size());
    for (size_t i = 0; i < str.size(); ++i) r[i] = ToUpper(str[i]);
    return r;
}

int main() {
    std::string test_str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for(int i=0; i<10; ++i) test_str += test_str; // Make it big

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i) {
        volatile auto res = ToUpper_original(test_str);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Original: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i) {
        volatile auto res = ToUpper_optimized(test_str);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Optimized: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << std::endl;

    return 0;
}
