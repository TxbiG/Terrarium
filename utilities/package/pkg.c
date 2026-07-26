#include "../common.h"

#include <package/install.h>
#include <package/package.h>
#include <package/repo.h>
#include <package/signature.h>
#include <status.h>

static const char *status_name(int status) {
    switch (status) {
    case TERRA_STATUS_OK: return "ok";
    case TERRA_STATUS_NOTREADY: return "backend not ready";
    case TERRA_STATUS_NOSYS: return "not implemented";
    case TERRA_STATUS_ACCES: return "policy denied";
    case TERRA_STATUS_INVAL: return "invalid argument";
    default: return status < 0 ? "error" : "ok";
    }
}

int main(int argc, char **argv) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("pkg", "[list|info <name>|install <name>|remove <name>|solve <name>|repos|verify <pkg> <sig>]");
        return argc < 2 ? 1 : 0;
    }

    int status = TERRA_STATUS_INVAL;
    if (strcmp(argv[1], "list") == 0) {
        terra_package_info_t packages[32];
        status = terra_package_list(packages, 32);
        if (status >= 0) {
            for (int i = 0; i < status && i < 32; ++i)
                printf("%s %s %s\n", packages[i].name, packages[i].version, packages[i].architecture);
            return 0;
        }
    } else if (strcmp(argv[1], "info") == 0 && argc > 2) {
        terra_package_info_t info;
        status = terra_package_query(argv[2], &info);
        if (status == TERRA_STATUS_OK) {
            printf("%s %s %s\n%s\n", info.name, info.version, info.architecture, info.description);
            return 0;
        }
    } else if (strcmp(argv[1], "install") == 0 && argc > 2) {
        status = terra_package_install(argv[2]);
    } else if (strcmp(argv[1], "remove") == 0 && argc > 2) {
        status = terra_package_remove(argv[2]);
    } else if (strcmp(argv[1], "solve") == 0 && argc > 2) {
        terra_package_solve_result_t result;
        status = terra_package_solve(argv[2], &result);
        if (status == TERRA_STATUS_OK) {
            printf("install=%u remove=%u upgrade=%u conflicts=%u download=%llu installed=%llu\n",
                   result.install_count, result.remove_count, result.upgrade_count, result.conflict_count,
                   (unsigned long long)result.download_bytes, (unsigned long long)result.installed_bytes);
            return 0;
        }
    } else if (strcmp(argv[1], "repos") == 0) {
        terra_package_repo_t repos[16];
        status = terra_repo_list(repos, 16);
        if (status >= 0) {
            for (int i = 0; i < status && i < 16; ++i)
                printf("%s %s channel=%s enabled=%d tls=%d sig=%d\n", repos[i].name, repos[i].url, repos[i].channel, repos[i].enabled, repos[i].require_tls, repos[i].require_signature);
            return 0;
        }
    } else if (strcmp(argv[1], "verify") == 0 && argc > 3) {
        status = terra_package_verify_signature(argv[2], argv[3]);
    }

    if (status == TERRA_STATUS_INVAL) {
        util_usage("pkg", "[list|info <name>|install <name>|remove <name>|solve <name>|repos|verify <pkg> <sig>]");
        return 1;
    }

    fprintf(stderr, "pkg: %s (%d)\n", status_name(status), status);
    return status < 0 ? 2 : 0;
}
