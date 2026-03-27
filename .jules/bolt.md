
## 2026-03-20 - Batching RPC calls in functional tests
**Learning:** Functional tests that make sequential `getblock` or `getblockhash` RPC calls inside loops or comprehensions suffer from N+1 query bottlenecks, leading to slow execution times.
**Action:** Use `node.batch([node.getblock.get_request(b) for b in blocks])` to batch RPC requests instead of iterating and calling them sequentially.

## 2024-03-25 - Avoid Full Block Downloads for Metadata
**Learning:** In functional tests, calling `node.getblock(hash)` defaults to returning the entire block including all transactions (verbosity=1). If the test only requires metadata from the header, like the block time or height, this introduces a performance bottleneck due to unnecessary JSON-RPC parsing and payload size.
**Action:** Use `node.getblockheader(hash)` (or `node.getblockcount()`, `node.getbestblockhash()`) when only header fields are required to minimize test execution time.

## 2026-03-27 - Batching RPC calls in test framework
**Learning:** Bitcoin Core's `BitcoinTestFramework` allows batching JSON-RPC requests using the `node.batch()` method. This is extremely beneficial in functional tests containing loops over sequential blocks or transactions. Sequential calls (e.g. `getblockhash` then `getblock`) cause N+1 query problems overhead.
**Action:** When iterating over ranges of block heights or hashes in Python functional tests, use `node.batch()` with list comprehensions of `.get_request()` methods to execute the commands in bulk. Ensure no scratch files like patch scripts or temporary performance benchmarking files are left in the repository directory before committing.
