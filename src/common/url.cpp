// Copyright (c) 2015-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <common/url.h>

#include <charconv>
#include <string>
#include <string_view>
#include <system_error>

std::string UrlDecode(std::string_view url_encoded)
{
    size_t percent_pos = url_encoded.find('%');
    if (percent_pos == std::string_view::npos) {
        return std::string(url_encoded);
    }

    std::string res;
    res.resize(url_encoded.size());
    size_t out_len = 0;

    // Fast path: copy until first '%'
    for (size_t i = 0; i < percent_pos; ++i) {
        res[out_len++] = url_encoded[i];
    }

    for (size_t i = percent_pos; i < url_encoded.size(); ++i) {
        char c = url_encoded[i];
        // Special handling for percent which should be followed by two hex digits
        // representing an octet values, see RFC 3986, Section 2.1 Percent-Encoding
        if (c == '%' && i + 2 < url_encoded.size()) {
            unsigned int decoded_value{0};
            auto [p, ec] = std::from_chars(url_encoded.data() + i + 1, url_encoded.data() + i + 3, decoded_value, 16);

            // Only if there is no error and the pointer is set to the end of
            // the string, we can be sure both characters were valid hex
            if (ec == std::errc{} && p == url_encoded.data() + i + 3) {
                res[out_len++] = static_cast<char>(decoded_value);
                // Next two characters are part of the percent encoding
                i += 2;
                continue;
            }
            // In case of invalid percent encoding, add the '%' and continue
        }
        res[out_len++] = c;
    }

    res.resize(out_len);
    return res;
}
