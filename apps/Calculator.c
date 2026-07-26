#include <Terra/app/app.h>

#include <stdio.h>

typedef struct calculator {
    double current_value;
    double memory_value;
    char current_operator;
    int reset_next;
} calculator_t;

static void calc_init(calculator_t *calc) {
    if (!calc)
        return;
    calc->current_value = 0.0;
    calc->memory_value = 0.0;
    calc->current_operator = 0;
    calc->reset_next = 0;
}

static void calc_input_digit(calculator_t *calc, char digit) {
    if (!calc || digit < '0' || digit > '9')
        return;
    if (calc->reset_next) {
        calc->current_value = 0.0;
        calc->reset_next = 0;
    }
    calc->current_value = calc->current_value * 10.0 + (double)(digit - '0');
}

static void calc_input_operator(calculator_t *calc, char op) {
    if (!calc)
        return;
    calc->memory_value = calc->current_value;
    calc->current_operator = op;
    calc->reset_next = 1;
}

static void calc_input_equals(calculator_t *calc) {
    if (!calc)
        return;
    switch (calc->current_operator) {
    case '+':
        calc->current_value = calc->memory_value + calc->current_value;
        break;
    case '-':
        calc->current_value = calc->memory_value - calc->current_value;
        break;
    case '*':
        calc->current_value = calc->memory_value * calc->current_value;
        break;
    case '/':
        if (calc->current_value != 0.0)
            calc->current_value = calc->memory_value / calc->current_value;
        break;
    default:
        break;
    }
    calc->current_operator = 0;
    calc->reset_next = 1;
}

static void calc_render(const calculator_t *calc, const terra_app_t *app) {
    printf("Calculator [%ux%u] value=%.2f frame=%u\n",
           app->window.view.rect.width,
           app->window.view.rect.height,
           calc ? calc->current_value : 0.0,
           app->frames);
}

int main(int argc, char **argv) {
    calculator_t calc;
    terra_app_t app;
    terra_app_desc_t desc = {
        .id = "org.terrarium.Calculator",
        .name = "Calculator",
        .kind = TERRA_APP_KIND_DESKTOP,
        .width = 360,
        .height = 520,
    };
    (void)argc;
    (void)argv;

    calc_init(&calc);
    if (terra_app_init(&app, &desc) != 0) {
        fprintf(stderr, "Calculator: failed to initialize Terra app runtime\n");
        return 1;
    }

    calc_input_digit(&calc, '4');
    calc_input_operator(&calc, '+');
    calc_input_digit(&calc, '2');
    calc_input_equals(&calc);
    terra_app_present(&app);
    calc_render(&calc, &app);
    terra_app_shutdown(&app);
    return 0;
}

