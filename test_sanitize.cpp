#include <string>
#include <string_view>
#include <iostream>
#include <chrono>
#include <array>

static const std::string CHARS_ALPHA_NUM = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

static std::array<bool, 256> BuildSafeCharsArray(std::string_view safe_chars) {
    std::array<bool, 256> arr{};
    for (char c : safe_chars) {
        arr[static_cast<unsigned char>(c)] = true;
    }
    return arr;
}

static const std::array<bool, 256> SAFE_CHARS[] =
{
    BuildSafeCharsArray(CHARS_ALPHA_NUM + " .,;-_/:?@()"), // SAFE_CHARS_DEFAULT
    BuildSafeCharsArray(CHARS_ALPHA_NUM + " .,;-_?@"), // SAFE_CHARS_UA_COMMENT
    BuildSafeCharsArray(CHARS_ALPHA_NUM + ".-_"), // SAFE_CHARS_FILENAME
    BuildSafeCharsArray(CHARS_ALPHA_NUM + "!*'();:@&=+$,/?#[]-_.~%"), // SAFE_CHARS_URI
};

std::string SanitizeString_original(std::string_view str, int rule)
{
    std::string result;
    result.reserve(str.size());
    for (char c : str) {
        if (SAFE_CHARS[rule][static_cast<unsigned char>(c)]) {
            result.push_back(c);
        }
    }
    return result;
}

std::string SanitizeString_optimized(std::string_view str, int rule)
{
    std::string result;
    result.resize(str.size());
    size_t len = 0;
    for (char c : str) {
        if (SAFE_CHARS[rule][static_cast<unsigned char>(c)]) {
            result[len++] = c;
        }
    }
    result.resize(len);
    return result;
}

int main() {
    std::string test_str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+";
    for(int i=0; i<8; ++i) test_str += test_str; // Make it big

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i) {
        volatile auto res = SanitizeString_original(test_str, 0);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Original: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i) {
        volatile auto res = SanitizeString_optimized(test_str, 0);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Optimized: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << std::endl;

    return 0;
}
