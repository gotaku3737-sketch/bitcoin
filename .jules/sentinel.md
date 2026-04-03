## 2024-05-14 - [Auth] Use Strong Randomness for RPC Cookies
**Vulnerability:** Weak randomness used for generating RPC authentication cookies.
**Learning:** `GetRandBytes` is designed for 'fast' seeding and does not guarantee strong OS entropy, which is required for cryptographic tokens. `GetStrongRandBytes` must be used.
**Prevention:** Audit all generation of authentication tokens or secret keys to ensure they use `GetStrongRandBytes`.

## 2024-05-18 - Missing OS-level entropy for secp256k1 blinding seed
**Vulnerability:** The random blinding seed passed to the `secp256k1` context in `ECC_Start` used `GetRandBytes()` instead of `GetStrongRandBytes()`.
**Learning:** Using `GetRandBytes()` instead of `GetStrongRandBytes()` for cryptographically secure random values fails to provide OS-level entropy, potentially exposing blinding scalar multiplications to side-channel attacks.
**Prevention:** Always use `GetStrongRandBytes()` when generating cryptographically secure random values (such as authentication cookies or keys) to ensure OS-level entropy.
## 2024-11-20 - Add security headers to HTTP responses
**Vulnerability:** Missing HTTP security headers (X-Frame-Options, X-Content-Type-Options, Content-Security-Policy) on HTTP/RPC endpoints.
**Learning:** Security headers should be applied centrally in the HTTP server response writer (`HTTPRequest::WriteReply`) to ensure all endpoints are protected, rather than duplicating code across individual handlers.
**Prevention:** Always consider the centralized response writer when adding global response headers.
