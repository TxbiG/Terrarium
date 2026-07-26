#include <terra_acpi.h>

#if defined(TERRARIUM_USE_UACPI)
#include <uacpi/uacpi.h>
#include <uacpi/tables.h>
#endif

static int acpi_ready;

void terra_acpi_init(const terra_boot_info_t *boot_info) {
    acpi_ready = 0;

    if (!boot_info || boot_info->rsdp == 0)
        return;

#if defined(TERRARIUM_USE_UACPI)
    /*
     * uACPI platform hooks still need to be implemented before full table
     * loading is enabled. This boundary keeps all uACPI usage in one place.
     */
    acpi_ready = 1;
#else
    acpi_ready = 1;
#endif
}

int terra_acpi_is_ready(void) {
    return acpi_ready;
}
