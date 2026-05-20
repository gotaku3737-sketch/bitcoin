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
## 2024-04-14 - Fix fail-open vulnerability in compact block initialization
**Vulnerability:** Unrecognized or new `ReadStatus` enum values in `PartiallyDownloadedBlock::InitData` caused a fail-open state where initialization failures were ignored, processing potentially uninitialized compact blocks.
**Learning:** Generic error handlers must use an outer `if (status != READ_STATUS_OK)` fallback rather than explicitly checking for specific known error conditions, to ensure safe fail-closed behavior if new errors are added.
**Prevention:** Always maintain a secure fail-closed pattern (`if (status != OK) return;`) outside of specific status checks.
## 2026-04-18 - Fix fail-open error handling in net_processing
**Vulnerability:** The error handling for compact block construction in `src/net_processing.cpp` used an explicit equality check `if (status == READ_STATUS_INVALID)` and `if (status == READ_STATUS_FAILED)`. If a new `ReadStatus` value was added, it would fall through to the block success path, improperly parsing and applying invalid/malicious data.
**Learning:** Checking for equality of explicit failure enums causes insecure, fail-open vulnerabilities if the enum expands over time.
**Prevention:** Instead of explicit comparisons, default to securely closing the error scope using a negated check: `if (status != READ_STATUS_OK)`.
## 2024-05-24 - SQLite PRAGMA Parameterization
**Vulnerability:** SQL injection potential due to using `strprintf` to construct PRAGMA statements with unsanitized user inputs.
**Learning:** SQLite PRAGMA statements do not support standard `?` parameter binding. This requires manual string construction which is prone to injection if not done safely.
**Prevention:** Construct PRAGMA statements using `sqlite3_mprintf` with `%w` for identifiers and `%Q` for values, then free the resulting pointer with `sqlite3_free`.
## 2025-05-23 - [Harden Notification Command Execution]
**Vulnerability:** Command Injection in shell-executed notifications.
**Learning:** Functions like AlertNotify were manually sanitizing and quoting interpolated strings, which is brittle and potentially bypassable compared to standard escaping utilities.
**Prevention:** Use ShellEscape for all data interpolation in shell commands on supported platforms, and explicitly warn against unescaped interpolation in any code path that executes commands via the shell.

## 2026-05-16 - Prevent Argument Splitting in Windows Shell Executions
**Vulnerability:** Windows cmd.exe single-quote argument injection. The safeStatus interpolation used single quotes (`'`), which cmd.exe treats as literal characters rather than string delimiters. This causes spaces in the sanitized string to be parsed as argument separators.
**Learning:** Unlike POSIX shells, Windows cmd.exe does not support single quotes for string escaping or grouping. When using `_wsystem()` on Windows, single-quoted strings containing spaces will be split into multiple arguments, potentially altering command behavior.
**Prevention:** Always use double quotes (`"`) when interpolating strings into Windows shell commands, or omit quoting entirely if the input is guaranteed to be safe and without spaces (like hex strings).
