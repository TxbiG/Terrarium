#include "../common.h"

#include <auth/sandbox.h>
#include <status.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("sandboxctl", "[status <subject>|apply <subject> <name>|release <subject>]");
        return argc < 2 ? 1 : 0;
    }

    if (strcmp(argv[1], "status") == 0 && argc > 2) {
        terra_sandbox_policy_t policy;
        int rc = terra_sandbox_get((uint32_t)strtoul(argv[2], 0, 0), &policy);
        if (rc == TERRA_STATUS_OK) {
            printf("sandbox=%s caps=%llu net=%u fs_write=%u debug=%u\n", policy.name, (unsigned long long)policy.allowed_caps, policy.allow_network, policy.allow_filesystem_write, policy.allow_debug);
            return 0;
        }
        fprintf(stderr, "sandboxctl: no sandbox for subject (%d)\n", rc);
        return 2;
    }
    if (strcmp(argv[1], "apply") == 0 && argc > 3) {
        terra_sandbox_policy_t policy = {0};
        snprintf(policy.name, sizeof(policy.name), "%s", argv[3]);
        policy.allowed_caps = 0;
        policy.allow_network = 0;
        policy.allow_filesystem_write = 0;
        policy.allow_debug = 0;
        int rc = terra_sandbox_apply((uint32_t)strtoul(argv[2], 0, 0), &policy);
        return rc == TERRA_STATUS_OK ? 0 : 2;
    }
    if (strcmp(argv[1], "release") == 0 && argc > 2) {
        int rc = terra_sandbox_release((uint32_t)strtoul(argv[2], 0, 0));
        return rc == TERRA_STATUS_OK ? 0 : 2;
    }

    util_usage("sandboxctl", "[status <subject>|apply <subject> <name>|release <subject>]");
    return 1;
}
