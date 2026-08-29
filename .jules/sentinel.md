## 2026-08-29 - Prevent Fail-Open Vulnerabilities in Enum Checks
**Vulnerability:** Explicit equality checks for enum error conditions (e.g., `status == READ_STATUS_INVALID`) caused a fail-open vulnerability if the enum expanded over time or received an unrecognized value.
**Learning:** Checking for specific failure modes rather than negating the success mode allows new, unexpected status codes to slip through and be treated as success or ignored.
**Prevention:** Use a negated check against the success state (e.g., `if (status != READ_STATUS_OK)`) and ensure the inner logic uses a default `else` block to securely handle all unrecognized failure modes by treating them as severe errors (e.g., triggering `Misbehaving()`).
