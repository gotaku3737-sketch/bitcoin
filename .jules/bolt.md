
## 2026-03-20 - Batching RPC calls in functional tests
**Learning:** Functional tests that make sequential `getblock` or `getblockhash` RPC calls inside loops or comprehensions suffer from N+1 query bottlenecks, leading to slow execution times.
**Action:** Use `node.batch([node.getblock.get_request(b) for b in blocks])` to batch RPC requests instead of iterating and calling them sequentially.

## 2024-03-24 - Do not blindly replace getblock with getblockheader in functional tests
**Learning:** Bitcoin Core does not prune block headers. Some functional tests (like `feature_pruning.py`, `rpc_getblockfrompeer.py`, and `p2p_unrequested_blocks.py`) explicitly use `getblock` to assert that the *full block data* has been downloaded or is available on disk, not just the header. Blindly replacing `getblock` with `getblockheader` in these specific cases bypasses the actual test condition, allowing broken pruning logic to pass CI.
**Action:** When optimizing `getblock` calls in tests, only apply it where it's explicitly fetching header metadata (like `['time']`, `['height']`, `['previousblockhash']`) for constructing subsequent blocks, and exclude tests that assert the presence of full block data.
