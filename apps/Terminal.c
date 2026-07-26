#include <Terra/app/app.h>
#include <shell/shell.h>
#include <shell/history.h>
#include <shell/job_control.h>
#include <status.h>

#include <stdio.h>
#include <string.h>

#define TERMINAL_COLS 80
#define TERMINAL_ROWS 24
#define TERMINAL_SCROLLBACK 256
#define TERMINAL_LINE_MAX 160

typedef struct terminal_line {
    char text[TERMINAL_LINE_MAX];
} terminal_line_t;

typedef struct terminal_app {
    terminal_line_t lines[TERMINAL_SCROLLBACK];
    unsigned line_count;
    unsigned scroll_head;
    unsigned cursor_x;
    unsigned cursor_y;
    int shell_ready;
    int pty_ready;
} terminal_app_t;

static void terminal_append_line(terminal_app_t *terminal, const char *line) {
    terminal_line_t *slot;
    if (!terminal || !line)
        return;

    slot = &terminal->lines[terminal->scroll_head % TERMINAL_SCROLLBACK];
    snprintf(slot->text, sizeof(slot->text), "%s", line);
    terminal->scroll_head++;
    if (terminal->line_count < TERMINAL_SCROLLBACK)
        terminal->line_count++;
    terminal->cursor_x = 0;
    terminal->cursor_y = terminal->line_count < TERMINAL_ROWS ? terminal->line_count : TERMINAL_ROWS - 1u;
}

static void terminal_init(terminal_app_t *terminal) {
    if (!terminal)
        return;
    memset(terminal, 0, sizeof(*terminal));
    terminal->shell_ready = terra_shell_execute_line("echo terminal-probe") != TERRA_STATUS_NOSYS;
    terminal->pty_ready = 0;
    terminal_append_line(terminal, "Terrarium Terminal");
    terminal_append_line(terminal, terminal->shell_ready ? "shell service: available" : "shell service: not ready");
    terminal_append_line(terminal, terminal->pty_ready ? "pty backend: available" : "pty backend: not implemented");
    terminal_append_line(terminal, "$ ");
}

static void terminal_render(const terminal_app_t *terminal, const terra_app_t *app) {
    unsigned visible = terminal && terminal->line_count < TERMINAL_ROWS ? terminal->line_count : TERMINAL_ROWS;
    printf("Terminal [%dx%d] lines=%u frame=%u\n",
           app->window.view.rect.width,
           app->window.view.rect.height,
           terminal ? terminal->line_count : 0,
           app->frames);

    if (!terminal)
        return;

    for (unsigned i = 0; i < visible; ++i) {
        unsigned index = (terminal->scroll_head + TERMINAL_SCROLLBACK - visible + i) % TERMINAL_SCROLLBACK;
        printf("  %s\n", terminal->lines[index].text);
    }
}

static int terminal_execute_line(terminal_app_t *terminal, const char *line) {
    int status;
    if (!terminal || !line)
        return TERRA_STATUS_INVAL;

    terminal_append_line(terminal, line);
    terra_shell_history_add(line);
    status = terra_shell_execute_line(line);
    if (status == TERRA_STATUS_NOSYS || status == TERRA_STATUS_NOTREADY)
        terminal_append_line(terminal, "terminal: shell execution backend is not implemented yet");
    return status;
}

int main(int argc, char **argv) {
    terminal_app_t terminal;
    terra_app_t app;
    terra_app_desc_t desc = {
        .id = "org.terrarium.Terminal",
        .name = "Terminal",
        .kind = TERRA_APP_KIND_SYSTEM,
        .width = 900,
        .height = 640,
    };

    if (terra_app_init(&app, &desc) != 0) {
        fprintf(stderr, "Terminal: failed to initialize Terra app runtime\n");
        return 1;
    }

    terminal_init(&terminal);
    if (argc > 1)
        terminal_execute_line(&terminal, argv[1]);

    terra_app_present(&app);
    terminal_render(&terminal, &app);
    terra_app_shutdown(&app);
    return 0;
}
