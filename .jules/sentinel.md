## 2026-08-29 - Prevent Fail-Open Vulnerabilities in Enum Checks
**Vulnerability:** Explicit equality checks for enum error conditions (e.g., `status == READ_STATUS_INVALID`) caused a fail-open vulnerability if the enum expanded over time or received an unrecognized value.
**Learning:** Checking for specific failure modes rather than negating the success mode allows new, unexpected status codes to slip through and be treated as success or ignored.
**Prevention:** Use a negated check against the success state (e.g., `if (status != READ_STATUS_OK)`) and ensure the inner logic uses a default `else` block to securely handle all unrecognized failure modes by treating them as severe errors (e.g., triggering `Misbehaving()`).
## 2024-05-18 - [Fix fail-open enum checking in net_processing]
**Vulnerability:** Optimistic block reconstruction was ignoring READ_STATUS_INVALID errors rather than penalizing the peer, creating a fail-open issue and potential DoS vector.
**Learning:** Failing to handle explicit error enums securely leads to peers being able to send bogus data without punishment.
**Prevention:** Use an explicit `else` branch for non-OK, non-FAILED statuses to catch unrecognized enum values and explicitly invoke `Misbehaving()`.
## 2024-05-24 - SQL Injection Risk in SQLite PRAGMA execution
**Vulnerability:** In `src/wallet/sqlite.cpp`, the `SetPragma` and `ReadPragmaInteger` functions constructed SQL PRAGMA statements by formatting strings using `strprintf("PRAGMA %s = %s", key, value)` and `strprintf("PRAGMA %s", key)`.
**Learning:** SQLite PRAGMA statements do not support the standard '?' parameter binding for prepared statements. Consequently, `sqlite3_bind_*` cannot be used to safely bind parameters in PRAGMA queries.
**Prevention:** To construct PRAGMA statements safely without risking SQL injection, use `sqlite3_mprintf` with the `%w` format specifier for identifiers (which safely escapes them) and `%Q` for values (which escapes and quotes them). The resulting pointer must be freed with `sqlite3_free`.
## 2024-09-04 - Prevent Fail-Open Vulnerabilities in DisconnectResult Enum Checks
**Vulnerability:** Explicit equality checks for `res == DISCONNECT_FAILED` enum error conditions caused a fail-open vulnerability if the enum expanded over time or received an unrecognized value.
**Learning:** Checking for specific failure modes rather than negating the success mode allows new, unexpected status codes to slip through and be treated as success or ignored.
**Prevention:** Use a negated check against the success/expected states (e.g., `if (res != DISCONNECT_OK && res != DISCONNECT_UNCLEAN)`) to securely route all unrecognized or new failure modes to the failure path.
