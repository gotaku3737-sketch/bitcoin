1. Add `CHARSET_DICT` in `test/functional/test_framework/segwit_addr.py`.
   - Add `CHARSET_DICT = {c: i for i, c in enumerate(CHARSET)}` below the `CHARSET` definition.
2. Optimize character searches in `bech32_decode` using the precomputed dictionary `CHARSET_DICT`.
   - Update `not all(x in CHARSET for x in bech[pos+1:])` to `not all(x in CHARSET_DICT for x in bech[pos+1:])`.
   - Update `data = [CHARSET.find(x) for x in bech[pos+1:]]` to `data = [CHARSET_DICT[x] for x in bech[pos+1:]]`.
   - Update `not all(x in CHARSET for x in bech[pos+1:])` to use `CHARSET_DICT`.
3. Complete pre-commit steps to ensure proper testing, verification, review, and reflection are done.
   - Run `test/lint/test_runner/Cargo.toml` and pytest/python functional tests.
4. Submit the pull request with a detailed title and description covering What, Why, Impact, and Measurement.
