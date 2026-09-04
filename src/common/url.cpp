// Copyright (c) 2015-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <common/url.h>

#include <array>
#include <bit>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <system_error>

// Pack the two hex chars of a byte into a single uint16_t for fast assignment
constexpr auto HexPacked = []() {
    std::array<uint16_t, 256> arr{};
    constexpr char hex_chars[] = "0123456789ABCDEF";
    for (int i = 0; i < 256; ++i) {
        char c1 = hex_chars[(i >> 4) & 0xF];
        char c2 = hex_chars[i & 0xF];
        if constexpr (std::endian::native == std::endian::little) {
            arr[i] = (static_cast<uint16_t>(c2) << 8) | static_cast<uint16_t>(c1);
        } else {
            arr[i] = (static_cast<uint16_t>(c1) << 8) | static_cast<uint16_t>(c2);
        }
    }
    return arr;
}();

std::string UrlDecode(std::string_view url_encoded)
{
    std::string res;
    res.resize(url_encoded.size());
    size_t pos = 0;

    for (size_t i = 0; i < url_encoded.size(); ++i) {
        char c = url_encoded[i];
        // Special handling for percent which should be followed by two hex digits
        // representing an octet values, see RFC 3986, Section 2.1 Percent-Encoding
        if (c == '%' && i + 2 < url_encoded.size()) {
            unsigned int decoded_value{0};
            auto [p, ec] = std::from_chars(url_encoded.data() + i + 1, url_encoded.data() + i + 3, decoded_value, 16);

            // Only if there is no error and the pointer is set to the end of
            // the string, we can be sure both characters were valid hex
            if (ec == std::errc{} && p == url_encoded.data() + i + 3) {
                res[pos++] = static_cast<char>(decoded_value);
                // Next two characters are part of the percent encoding
                i += 2;
                continue;
            }
            // In case of invalid percent encoding, add the '%' and continue
        }
        res[pos++] = c;
    }
    res.resize(pos);
    return res;
}

std::string UrlEncode(std::string_view str)
{
    std::string res;
    res.resize(str.size() * 3); // worst case: every char needs encoding
    size_t pos = 0;

    for (char ch : str) {
        auto c = static_cast<unsigned char>(ch);
        // Unreserved characters per RFC 3986, Section 2.3
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') {
            res[pos++] = ch;
        } else {
            // Percent-encode all other characters
            res[pos++] = '%';
            uint16_t packed = HexPacked[c];
            std::memcpy(&res[pos], &packed, sizeof(packed));
            pos += 2;
        }
    }
    res.resize(pos);
    return res;
}
