
## 2026-08-28 - Optimize C++ small string generation via integer packing
**Learning:** In C++ performance-critical paths, mapping values to small multi-byte sequences (like converting a byte to two hex characters) can be optimized by packing the characters into a single native integer (e.g., `uint16_t`) and handling byte order using `std::endian::native` from `<bit>`.
**Action:** Always prefer packing small constant-sized character outputs into integers over `memcpy` or multiple 8-bit stores to allow compilers to use a single scalar store instruction, significantly reducing execution time.
## 2026-08-30 - Inefficient string replacement in loops
**Learning:** In C++, calling `std::string::replace` or `std::regex_replace` inside a loop or string for multiple substitutions causes an O(N^2) performance bottleneck or high regex overhead. To achieve O(N) complexity and fast string operations, pre-allocate a new string with `reserve()` and construct the result using `append()`.
**Action:** Replace `std::regex_replace` with a manual search-and-replace algorithm using `find()`, `reserve()`, and `append()` for simple string substitutions.

## 2026-08-30 - Optimize character searches using precomputed boolean arrays
**Learning:** In C++ core code, when filtering or validating strings against fixed character sets in hot paths, replacing linear string searches (like `std::string::find`) with a precomputed static boolean array lookup (e.g., `std::array<bool, 256>`) converts O(M) lookups to O(1) and significantly improves performance.
**Action:** Use a precomputed static boolean array lookup for string character searches against fixed character sets. Ensure characters are cast to `unsigned char` before indexing to prevent out-of-bounds access for negative values. Provide a specialized overload or conditional branch for single-character lookups (e.g., direct equality check `*it == sep`) to avoid the array initialization overhead for trivial cases.

## 2024-09-04 - Optimize C++ string construction in URL encoding
**Learning:** Using `+=` to append characters one by one to a `std::string` incurs noticeable overhead due to continuous size checks and potential reallocations, even when `reserve()` is called. Furthermore, appending hex characters sequentially requires two store operations.
**Action:** Pre-allocate the upper-bound size using `resize()`, build the string using direct index assignment (e.g., `str[pos++] = c`), and then `resize()` down to the final length. Use a precomputed `constexpr std::array<uint16_t, 256>` packed with native endianness to write two hex chars via a single 16-bit scalar store.
## 2026-08-31 - Optimize Python character searches in Bech32 decoding
**Learning:** In Python-based performance-critical code (e.g., 'test/functional/test_framework/segwit_addr.py'), linear character lookups using '.find()' or membership checks against strings within list comprehensions have significant overhead. Replacing these with precomputed dictionary lookups converts O(M) searches to O(1) and typically provides a measurable CPU efficiency gain (over 50% for Bech32 checksum logic).
**Action:** Replace linear character lookups using '.find()' or membership checks with precomputed dictionary lookups in hot paths.

## 2024-09-13 - Optimize string conversion in C++
**Learning:** Using `+=` to append characters one by one to a `std::string` incurs noticeable overhead due to continuous size checks and potential reallocations, even when `reserve()` is called. This was observed in `ToLower` and `ToUpper`.
**Action:** Pre-allocate the upper-bound size using `resize()` and build the string using direct index assignment (e.g., `str[pos++] = c`). This is over 2.5x faster in tight loops.

## 2024-09-15 - Optimize string conversion in C++ ToLower/ToUpper
**Learning:** Using `r[i] = ...` inside a loop for string modification incurs overhead due to bounds checking and other internal checks in `std::string::operator[]`.
**Action:** When modifying a pre-allocated `std::string` in a hot loop, access the raw character buffer via `data()` and use pointer arithmetic or raw indexing to bypass `std::string` bounds checks, yielding an ~80% reduction in execution time for large strings.
