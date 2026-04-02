## 2024-05-14 - [Auth] Use Strong Randomness for RPC Cookies
**Vulnerability:** Weak randomness used for generating RPC authentication cookies.
**Learning:** `GetRandBytes` is designed for 'fast' seeding and does not guarantee strong OS entropy, which is required for cryptographic tokens. `GetStrongRandBytes` must be used.
**Prevention:** Audit all generation of authentication tokens or secret keys to ensure they use `GetStrongRandBytes`.

## 2024-05-18 - Missing OS-level entropy for secp256k1 blinding seed
**Vulnerability:** The random blinding seed passed to the `secp256k1` context in `ECC_Start` used `GetRandBytes()` instead of `GetStrongRandBytes()`.
**Learning:** Using `GetRandBytes()` instead of `GetStrongRandBytes()` for cryptographically secure random values fails to provide OS-level entropy, potentially exposing blinding scalar multiplications to side-channel attacks.
**Prevention:** Always use `GetStrongRandBytes()` when generating cryptographically secure random values (such as authentication cookies or keys) to ensure OS-level entropy.

## 2024-05-18 - Missing OS-level entropy for network and proxy nonces
**Vulnerability:** Weak randomness used for generating PCP mapping nonces (`mapport.cpp`) and proxy credential prefixes (`netbase.cpp`).
**Learning:** `GetRandBytes()` only provides fast seeding and does not guarantee strong OS-level entropy. Using it for protocol nonces or proxy isolation credentials can lead to predictability, potentially allowing attackers to interfere with mappings or link proxy circuits.
**Prevention:** Always use `GetStrongRandBytes()` when generating tokens, nonces, or credentials that require unpredictability for security or isolation purposes.
