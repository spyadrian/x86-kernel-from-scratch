#ifndef _MODULE_H_
#define _MODULE_H_

#include <stdint.h>

// --- DEFINE ---

// ...

// --- FUNCTION ---

/**
 * @brief Initializes the module system.
 */
void module_init();

/**
 * @brief Retrieves the total number of modules currently loaded or initialized.
 *
 * @return The number of available modules.
 */
uint32_t module_count();

/**
 * @brief Gets the memory address of a module based on its index.
 *
 * @param idx  The index of the module.
 * @return A pointer to the memory address of the specified module.
 */
void *module_get_addr(uint16_t idx);

/**
 * @brief Retrieves the size of a module in bytes.
 *
 * @param idx  The index of the module.
 * @return The size of the specified module in bytes.
 */
uint32_t module_size(uint16_t idx);

/**
 * @brief Retrieves the command line string used to launch or configure a module.
 *
 * @param idx  The index of the module.
 * @return A pointer to the command line string of the module.
 */
char *module_cmdline(uint16_t idx);

/**
 * @brief Retrieves a specific argument passed to the module's command line.
 *
 * @param idx     The index of the module.
 * @param nb_arg  The index of the argument to retrieve.
 * @param arg     Pointer to a buffer where the argument will be stored.
 */
void module_get_arg(uint16_t idx, uint8_t nb_arg, char *arg);

/**
 * @brief Retrieves the memory address of the last module.
 *
 * @return A pointer to the memory address of the last loaded module.
 */
void *module_last_addr();

#endif // _MODULE_H_
