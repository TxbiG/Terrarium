# Terrarium Login Accounts

`terrarium-login` currently reads a simple developer account database before the
full auth/user database exists.

Install path:

```text
/etc/terrarium/login/users.conf
```

Source fallback for hosted testing:

```text
services/login/users.conf
```

Format:

```text
name:uid:gid:home:shell:password
```

This is intentionally not the final security model. Phase 6 still needs real
password hashing, shadow-file separation, account lockout policy, capabilities,
session tokens, and privilege transitions through the system auth APIs.