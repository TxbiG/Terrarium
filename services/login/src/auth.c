#include "auth.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOGIN_ACCOUNT_DB "/etc/terrarium/login/users.conf"
#define LOGIN_ACCOUNT_FALLBACK "services/login/users.conf"
#define LINE_MAX_CHARS 768u

static void copy_text(char *dst, size_t cap, const char *src) {
    size_t i = 0;

    if (!dst || cap == 0)
        return;
    if (!src)
        src = "";

    while (src[i] && i + 1 < cap) {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

static char *trim(char *text) {
    char *end;

    while (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n')
        ++text;

    end = text + strlen(text);
    while (end > text && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\r' || end[-1] == '\n'))
        *--end = '\0';

    return text;
}

static int parse_account_line(char *line, login_account_t *out_account) {
    char *fields[6] = {0};
    char *cursor = line;

    if (!line || !out_account)
        return -1;

    for (size_t i = 0; i < 6; ++i) {
        char *sep;
        fields[i] = cursor;
        sep = cursor ? strchr(cursor, ':') : 0;
        if (sep) {
            *sep = '\0';
            cursor = sep + 1;
        } else {
            cursor = 0;
        }
    }

    if (!fields[0] || !fields[1] || !fields[2] || !fields[3] || !fields[4] || !fields[5])
        return -1;

    memset(out_account, 0, sizeof(*out_account));
    copy_text(out_account->name, sizeof(out_account->name), trim(fields[0]));
    out_account->uid = (uint32_t)strtoul(trim(fields[1]), 0, 10);
    out_account->gid = (uint32_t)strtoul(trim(fields[2]), 0, 10);
    copy_text(out_account->home, sizeof(out_account->home), trim(fields[3]));
    copy_text(out_account->shell, sizeof(out_account->shell), trim(fields[4]));
    copy_text(out_account->password, sizeof(out_account->password), trim(fields[5]));

    return out_account->name[0] && out_account->home[0] && out_account->shell[0] ? 0 : -1;
}

static int lookup_in_file(const char *path, const char *user, login_account_t *out_account) {
    FILE *file;
    char line[LINE_MAX_CHARS];

    file = fopen(path, "r");
    if (!file)
        return -1;

    while (fgets(line, sizeof(line), file)) {
        login_account_t account;
        char *entry = trim(line);

        if (!*entry || *entry == '#')
            continue;

        if (parse_account_line(entry, &account) != 0)
            continue;

        if (strcmp(account.name, user) == 0) {
            if (out_account)
                *out_account = account;
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return -1;
}

int login_lookup_account(const char *user, login_account_t *out_account) {
    if (!user || !*user)
        return -1;

    if (lookup_in_file(LOGIN_ACCOUNT_DB, user, out_account) == 0)
        return 0;

    return lookup_in_file(LOGIN_ACCOUNT_FALLBACK, user, out_account);
}

int login_authenticate(const char *user, const char *password, login_account_t *out_account) {
    login_account_t account;

    if (!user || !password)
        return 0;
    if (login_lookup_account(user, &account) != 0)
        return 0;
    if (strcmp(account.password, password) != 0)
        return 0;

    if (out_account)
        *out_account = account;
    return 1;
}