
## 2026-03-20 - Batching RPC calls in functional tests
**Learning:** Functional tests that make sequential `getblock` or `getblockhash` RPC calls inside loops or comprehensions suffer from N+1 query bottlenecks, leading to slow execution times.
**Action:** Use `node.batch([node.getblock.get_request(b) for b in blocks])` to batch RPC requests instead of iterating and calling them sequentially.

## 2024-03-25 - Avoid Full Block Downloads for Metadata
**Learning:** In functional tests, calling `node.getblock(hash)` defaults to returning the entire block including all transactions (verbosity=1). If the test only requires metadata from the header, like the block time or height, this introduces a performance bottleneck due to unnecessary JSON-RPC parsing and payload size.
**Action:** Use `node.getblockheader(hash)` (or `node.getblockcount()`, `node.getbestblockhash()`) when only header fields are required to minimize test execution time.
## 2024-04-01 - Optimize CLI Report String Building
**Learning:** Using `std::ostringstream` combined with `tfm::format` directly appending to the stream is a clean and performant pattern for building large multi-line strings incrementally in C++. This avoids the overhead of repeated `std::string` concatenations (and associated temporary memory allocations/reallocations) commonly seen with `result += strprintf(...)`.
**Action:** Always prefer `std::ostringstream` and `tfm::format(stream, ...)` when constructing large textual output, particularly in formatting-heavy tools like the RPC/CLI interfaces, to reduce allocation overhead.
