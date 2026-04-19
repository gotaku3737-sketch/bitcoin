
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
