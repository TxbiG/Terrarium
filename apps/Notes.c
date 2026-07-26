#include <Terra/app/app.h>
#include <fs/vfs.h>
#include <status.h>
#include <text/utf8.h>
#include <utils/fs_util.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define NOTES_DEFAULT_PATH "/home/user/Notes/quick-note.txt"
#define NOTES_INITIAL_TEXT "Welcome to Terrarium Notes.\n\nText editing, persistence, clipboard, and rich rendering are wired as OS contracts here instead of local fallbacks.\n"

typedef struct notes_document {
    char path[256];
    char text[1024];
    size_t bytes;
    unsigned long characters;
    int valid_utf8;
    int storage_ready;
    int file_ready;
    int dirty;
} notes_document_t;

static void notes_document_init(notes_document_t *doc, const char *path) {
    if (!doc)
        return;

    memset(doc, 0, sizeof(*doc));
    snprintf(doc->path, sizeof(doc->path), "%s", path && path[0] ? path : NOTES_DEFAULT_PATH);
    snprintf(doc->text, sizeof(doc->text), "%s", NOTES_INITIAL_TEXT);
    doc->bytes = strlen(doc->text);
    doc->valid_utf8 = terra_utf8_validate(doc->text);
    doc->characters = doc->valid_utf8 ? terra_utf8_length(doc->text) : 0;
    doc->dirty = 0;
}

static void notes_probe_storage(notes_document_t *doc) {
    if (!doc)
        return;

    terra_vfs_stat_t stat;
    doc->storage_ready = terra_fs_ensure_dir("/home/user/Notes") == TERRA_STATUS_OK;
    doc->file_ready = terra_vfs_stat(doc->path, &stat) == TERRA_STATUS_OK;
}

static void notes_present_state(const terra_app_t *app, const notes_document_t *doc) {
    if (!app || !doc)
        return;

    printf("%s\n", app->desc.name);
    printf("  document: %s\n", doc->path);
    printf("  text: %zu bytes, %lu characters, utf8=%s\n",
           doc->bytes,
           doc->characters,
           doc->valid_utf8 ? "valid" : "invalid");
    printf("  storage backend: %s\n", doc->storage_ready ? "ready" : "not implemented");
    printf("  existing file: %s\n", doc->file_ready ? "found" : "not available");
    printf("  editor backend: pending Terra UI multiline text widget and input focus routing\n");
    printf("  persistence backend: pending VFS open/read/write/fsync contracts\n");
    printf("  clipboard backend: pending compositor clipboard protocol client support\n");
}

int main(int argc, char **argv) {
    notes_document_t doc;
    notes_document_init(&doc, argc > 1 ? argv[1] : NOTES_DEFAULT_PATH);
    notes_probe_storage(&doc);

    terra_app_t app;
    const terra_app_desc_t desc = {
        .id = "org.terrarium.Notes",
        .name = "Notes",
        .kind = TERRA_APP_KIND_UTILITY,
        .width = 720,
        .height = 520,
    };

    if (terra_app_init(&app, &desc) != 0) {
        fprintf(stderr, "Notes: compositor connection unavailable; running storage/text readiness checks only.\n");
        memset(&app, 0, sizeof(app));
        app.desc = desc;
    }

    terra_app_present(&app);
    notes_present_state(&app, &doc);
    terra_app_request_close(&app);
    terra_app_shutdown(&app);

    return doc.valid_utf8 ? 0 : 1;
}
