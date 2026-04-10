## 2024-05-14 - [Auth] Use Strong Randomness for RPC Cookies
**Vulnerability:** Weak randomness used for generating RPC authentication cookies.
**Learning:** `GetRandBytes` is designed for 'fast' seeding and does not guarantee strong OS entropy, which is required for cryptographic tokens. `GetStrongRandBytes` must be used.
**Prevention:** Audit all generation of authentication tokens or secret keys to ensure they use `GetStrongRandBytes`.

## 2024-05-18 - Missing OS-level entropy for secp256k1 blinding seed
**Vulnerability:** The random blinding seed passed to the `secp256k1` context in `ECC_Start` used `GetRandBytes()` instead of `GetStrongRandBytes()`.
**Learning:** Using `GetRandBytes()` instead of `GetStrongRandBytes()` for cryptographically secure random values fails to provide OS-level entropy, potentially exposing blinding scalar multiplications to side-channel attacks.
**Prevention:** Always use `GetStrongRandBytes()` when generating cryptographically secure random values (such as authentication cookies or keys) to ensure OS-level entropy.

## 2024-05-18 - [HTTP] Add Security Headers
**Vulnerability:** Missing HTTP security headers (X-Frame-Options, X-Content-Type-Options, Content-Security-Policy).
**Learning:** The HTTP RPC server was vulnerable to clickjacking, MIME-sniffing, and potentially XSS. These headers must be set globally on all responses.
**Prevention:** Always set global security headers for all responses in the core HTTP handling logic (`HTTPRequest::WriteReply`).
## 2023-10-24 - Handle InitData failures for PartiallyDownloadedBlock correctly
**Vulnerability:** Ignored initialization failures for partially downloaded blocks in P2P net processing logic.
**Learning:** Returning without logging or acting upon `READ_STATUS_INVALID` when handling invalid compact blocks allowed peers to spam malformed protocol messages silently without consequence (DoS vector).
**Prevention:** Always penalize explicitly invalid peer data using `Misbehaving(peer, ...)` instead of `// TODO: don't ignore failures`.
