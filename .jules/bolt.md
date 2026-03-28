
## 2026-03-20 - Batching RPC calls in functional tests
**Learning:** Functional tests that make sequential `getblock` or `getblockhash` RPC calls inside loops or comprehensions suffer from N+1 query bottlenecks, leading to slow execution times.
**Action:** Use `node.batch([node.getblock.get_request(b) for b in blocks])` to batch RPC requests instead of iterating and calling them sequentially.

## 2024-03-25 - Avoid Full Block Downloads for Metadata
**Learning:** In functional tests, calling `node.getblock(hash)` defaults to returning the entire block including all transactions (verbosity=1). If the test only requires metadata from the header, like the block time or height, this introduces a performance bottleneck due to unnecessary JSON-RPC parsing and payload size.
**Action:** Use `node.getblockheader(hash)` (or `node.getblockcount()`, `node.getbestblockhash()`) when only header fields are required to minimize test execution time.
## 2024-03-28 - Optimize functional test block RPC calls
**Learning:** The `getblock()` RPC call downloads and serializes the full block including all transactions. When only header fields like `time`, `height`, or `hash` are needed, using `getblock()` causes unnecessary overhead.
**Action:** Prefer `getblockheader()` when retrieving only block metadata (e.g. `time`, `height`, `hash`) to minimize RPC call overhead and prevent downloading unnecessary payload data.
