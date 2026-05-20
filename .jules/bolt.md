
## 2026-03-20 - Batching RPC calls in functional tests
**Learning:** Functional tests that make sequential `getblock` or `getblockhash` RPC calls inside loops or comprehensions suffer from N+1 query bottlenecks, leading to slow execution times.
**Action:** Use `node.batch([node.getblock.get_request(b) for b in blocks])` to batch RPC requests instead of iterating and calling them sequentially.

## 2024-03-25 - Avoid Full Block Downloads for Metadata
**Learning:** In functional tests, calling `node.getblock(hash)` defaults to returning the entire block including all transactions (verbosity=1). If the test only requires metadata from the header, like the block time or height, this introduces a performance bottleneck due to unnecessary JSON-RPC parsing and payload size.
**Action:** Use `node.getblockheader(hash)` (or `node.getblockcount()`, `node.getbestblockhash()`) when only header fields are required to minimize test execution time.
## 2024-11-20 - Optimize String Mutation and Searching
**Learning:** String mutations using `+=` character-by-character, even with `reserve()`, are significantly slower than copying the string and mutating in-place due to bounds-checking overhead. Additionally, standard library functions like `.find()` on `std::string_view` are heavily optimized (often vectorized) compared to manual character iteration.
**Action:** When transforming string cases, construct a copy and mutate in-place rather than building a new string character by character. Prefer standard library search functions like `.find()` over manual loops.
## 2025-05-15 - [External Signer Fingerprint]
**Learning:** Multiple external signers can be handled by filtering with their master key fingerprint.
**Action:** Always provide fingerprint when multiple signers might be present.

## 2024-04-22 - Optimize all() in functional tests
**Learning:** Using list comprehensions within `all()` (e.g., `all([x in y for x in z])`) creates an intermediate list in memory before evaluating `all()`, which defeats the short-circuiting behavior of `all()`.
**Action:** Use generator expressions instead of list comprehensions within `all()` (e.g., `all(x in y for x in z)`) to leverage short-circuiting and reduce memory allocations, as noted in the memory context.
## 2026-05-02 - Optimize all() and any() in Python Functional Tests
**Learning:** Using list comprehensions within `all()` or `any()` (e.g., `all([x in y for x in z])`) forces Python to evaluate the entire list and keep it in memory before applying the short-circuiting logic. This defeats the purpose of early exits and increases memory allocations unnecessarily.
**Action:** Always use generator expressions instead (e.g., `all(x in y for x in z)`) to allow Python to evaluate items lazily, immediately exiting on the first failure/success and reducing memory overhead.
## 2024-05-06 - Replace GetStrongRandBytes with GetRandBytes for transient values
**Learning:** `GetStrongRandBytes` performs slow OS-level entropy gathering and is meant for long-term secure keys. Using it for transient values like network nonces or unique prefixes unnecessarily drains OS entropy and blocks execution, negatively impacting performance. `GetRandBytes` is a fast CSPRNG and is the correct choice for these transient values.
**Action:** Always prefer `GetRandBytes` for fast, transient network values, and reserve `GetStrongRandBytes` for long-term keys or high-security persistent secrets.
## 2024-05-14 - Pre-resizing String Encodings Avoids Reallocation Overhead
**Learning:** When building strings with known or calculable upper-bound sizes, pre-resizing (`str.resize()`) and mutating by index (`str[pos++] = char`) avoids bounds-checking and reallocation overhead, leading to significant CPU efficiency gains compared to repeated character appending (`str += char`), providing an observed 25-35% speedup for string encoders.
**Action:** For string construction with a predictable max size in performance-critical areas, prefer `resize()` over `reserve()` paired with index-based mutation over `+=` concatenation.
## 2024-05-18 - Optimize std::regex_replace with string find and replace
**Learning:** `std::regex_replace` can be extremely slow compared to a manual loop using `std::string::find` and `std::string::replace` when dealing with simple plain-text search and substitution.
**Action:** Replace `std::regex_replace` with a manual string find and replace loop for plain string substitution where regex matching is not actually needed.
## 2024-05-14 - Optimize UrlDecode
**Learning:** The URL decoding path was doing character-by-character string append `+=` inside a loop which requires bounds checking and reallocation. In scenarios where URL encoding is absent or light, pre-sizing the buffer and mutating via index, combined with a `string_view::find()` fast path, provides a measurable CPU efficiency gain (3-4x faster for unencoded URLs).
**Action:** For string building of known upper-bound size, pre-resize them and mutate via index (`str[pos++] = char`) instead of appending character-by-character.
