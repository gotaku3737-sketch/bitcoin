
## 2026-08-28 - Optimize C++ small string generation via integer packing
**Learning:** In C++ performance-critical paths, mapping values to small multi-byte sequences (like converting a byte to two hex characters) can be optimized by packing the characters into a single native integer (e.g., `uint16_t`) and handling byte order using `std::endian::native` from `<bit>`.
**Action:** Always prefer packing small constant-sized character outputs into integers over `memcpy` or multiple 8-bit stores to allow compilers to use a single scalar store instruction, significantly reducing execution time.
## 2026-08-30 - Inefficient string replacement in loops
**Learning:** In C++, calling `std::string::replace` or `std::regex_replace` inside a loop or string for multiple substitutions causes an O(N^2) performance bottleneck or high regex overhead. To achieve O(N) complexity and fast string operations, pre-allocate a new string with `reserve()` and construct the result using `append()`.
**Action:** Replace `std::regex_replace` with a manual search-and-replace algorithm using `find()`, `reserve()`, and `append()` for simple string substitutions.
