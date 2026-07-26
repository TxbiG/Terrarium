#ifndef TERRARIUM_SYSTEM_PACKAGE_INSTALL_H
#define TERRARIUM_SYSTEM_PACKAGE_INSTALL_H

#include <stdint.h>

#define TERRA_PACKAGE_TX_MAX 64u

typedef enum terra_package_transaction_action {
    TERRA_PACKAGE_TX_INSTALL = 0,
    TERRA_PACKAGE_TX_REMOVE,
    TERRA_PACKAGE_TX_UPGRADE,
    TERRA_PACKAGE_TX_ROLLBACK
} terra_package_transaction_action_t;

typedef enum terra_package_transaction_state {
    TERRA_PACKAGE_TX_PLANNED = 0,
    TERRA_PACKAGE_TX_PREPARED,
    TERRA_PACKAGE_TX_APPLIED,
    TERRA_PACKAGE_TX_ROLLED_BACK,
    TERRA_PACKAGE_TX_FAILED
} terra_package_transaction_state_t;

typedef struct terra_package_transaction {
    char id[TERRA_PACKAGE_TX_MAX];
    char package_name[64];
    char from_version[32];
    char to_version[32];
    terra_package_transaction_action_t action;
    terra_package_transaction_state_t state;
    uint32_t requires_reboot;
} terra_package_transaction_t;

typedef struct terra_package_solve_result {
    uint32_t install_count;
    uint32_t remove_count;
    uint32_t upgrade_count;
    uint32_t conflict_count;
    uint64_t download_bytes;
    uint64_t installed_bytes;
} terra_package_solve_result_t;

int terra_package_install(const char *package_name);
int terra_package_remove(const char *package_name);
int terra_package_install_file(const char *path);
int terra_package_solve(const char *package_name, terra_package_solve_result_t *out_result);
int terra_package_prepare_transaction(const terra_package_transaction_t *transaction);
int terra_package_commit_transaction(const char *transaction_id);
int terra_package_rollback_transaction(const char *transaction_id);

#endif
