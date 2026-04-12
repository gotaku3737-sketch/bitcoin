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
## 2024-05-15 - [P2P Denial of Service via Unhandled Statuses]
**Vulnerability:** A generic catch-all `if (status != READ_STATUS_OK)` implicitly ignored specific error states (`READ_STATUS_INVALID`) returned by `PartiallyDownloadedBlock::InitData`, missing the opportunity to penalize malicious peers sending invalid compact blocks via `Misbehaving`.
**Learning:** Code blocks annotated with "TODO: don't ignore failures" often mask real DoS vectors. Security by disconnection relies on active policing of protocol anomalies.
**Prevention:** Always handle specific return codes representing explicitly malicious data or protocol violations (e.g., `READ_STATUS_INVALID`) before relying on generic error exits to ensure robust network defense.
