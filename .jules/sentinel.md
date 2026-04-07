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
## 2024-06-03 - Fail-Closed Status Handling

**Vulnerability:** A `TODO` in `net_processing.cpp` ignored `InitData` failures on compact blocks, which could enable DoS resource exhaustion from malicious peers. An initial fix attempted to explicitly handle known failure statuses but dropped the generic `if (status != READ_STATUS_OK)` check.
**Learning:** Removing a generic catch-all check in favor of specific enum matches creates a "fail-open" vulnerability. If new error enum variants are added later, they will bypass the checks entirely, leading to undefined behavior or processing of uninitialized data.
**Prevention:** Always maintain a safe outer fail-closed perimeter `if (status != SUCCESS_STATUS)` when handling specific error sub-cases, guaranteeing the code aborts on any unexpected outcome.
