## 2026-03-19 - N+1 Queries in Functional Tests
**Learning:** Python functional tests making sequential RPC calls in list comprehensions (e.g., `[node.getblock(b) for b in blocks]`) cause a significant N+1 query performance bottleneck due to excessive HTTP round-trips to the `bitcoind` node.
**Action:** Always use the `node.batch()` method provided by the test framework to group multiple RPC requests into a single JSON-RPC batch call (e.g., `node.batch([node.getblock.get_request(b) for b in blocks])`) when fetching multiple resources in functional tests.
