## 2024-05-14 - [Auth] Use Strong Randomness for RPC Cookies
**Vulnerability:** Weak randomness used for generating RPC authentication cookies.
**Learning:** `GetRandBytes` is designed for 'fast' seeding and does not guarantee strong OS entropy, which is required for cryptographic tokens. `GetStrongRandBytes` must be used.
**Prevention:** Audit all generation of authentication tokens or secret keys to ensure they use `GetStrongRandBytes`.

## 2024-05-18 - Missing OS-level entropy for secp256k1 blinding seed
**Vulnerability:** The random blinding seed passed to the `secp256k1` context in `ECC_Start` used `GetRandBytes()` instead of `GetStrongRandBytes()`.
**Learning:** Using `GetRandBytes()` instead of `GetStrongRandBytes()` for cryptographically secure random values fails to provide OS-level entropy, potentially exposing blinding scalar multiplications to side-channel attacks.
**Prevention:** Always use `GetStrongRandBytes()` when generating cryptographically secure random values (such as authentication cookies or keys) to ensure OS-level entropy.
## 2024-05-18 - Missing Defense-in-Depth HTTP Security Headers
**Vulnerability:** The HTTP RPC server did not set standard security headers (X-Frame-Options, X-Content-Type-Options, Content-Security-Policy), slightly increasing exposure to client-side or browser-based attacks against the RPC interface.
**Learning:** Security headers should be applied globally at the framework level (e.g., inside `HTTPRequest::WriteReply`) rather than duplicated across individual endpoint handlers.
**Prevention:** Ensure all HTTP response generation mechanisms include standard security headers by default.
