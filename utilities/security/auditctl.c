#include "../common.h"

#include <auth/audit.h>
#include <status.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("auditctl", "[status|enable|disable|log <subject> <action> <allow|deny> <message>|list]");
        return argc < 2 ? 1 : 0;
    }

    if (strcmp(argv[1], "status") == 0) {
        printf("audit=%s records=%d\n", terra_audit_is_enabled() ? "enabled" : "disabled", terra_audit_count());
        return 0;
    }
    if (strcmp(argv[1], "enable") == 0)
        return terra_audit_set_enabled(1) == TERRA_STATUS_OK ? 0 : 2;
    if (strcmp(argv[1], "disable") == 0)
        return terra_audit_set_enabled(0) == TERRA_STATUS_OK ? 0 : 2;
    if (strcmp(argv[1], "log") == 0 && argc > 5) {
        terra_audit_decision_t decision = strcmp(argv[4], "allow") == 0 ? TERRA_AUDIT_ALLOW : TERRA_AUDIT_DENY;
        int rc = terra_audit_write((uint32_t)strtoul(argv[2], 0, 0), (uint32_t)strtoul(argv[3], 0, 0), decision, argv[5]);
        return rc == TERRA_STATUS_OK ? 0 : 2;
    }
    if (strcmp(argv[1], "list") == 0) {
        int count = terra_audit_count();
        for (int i = 0; i < count; ++i) {
            terra_audit_record_t record;
            if (terra_audit_read((unsigned long)i, &record) == TERRA_STATUS_OK)
                printf("%llu subject=%u action=%u decision=%s %s\n", (unsigned long long)record.sequence, record.subject, record.action, record.decision == TERRA_AUDIT_ALLOW ? "allow" : "deny", record.message);
        }
        return 0;
    }

    util_usage("auditctl", "[status|enable|disable|log <subject> <action> <allow|deny> <message>|list]");
    return 1;
}
