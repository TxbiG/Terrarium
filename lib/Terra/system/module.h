/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Dynamic loading of modules into the kernel.
 *
 * Rewritten by Richard Henderson <rth@tamu.edu> Dec 1996
 * Rewritten again by Rusty Russell, 2002
 */

#ifndef TERRARIUM_DRIVER_MODULE_H
#define TERRARIUM_DRIVER_MODULE_H




#define MODULE_AUTHOR(_author) 					_author			// Author(s), use "Name <email>" or "Name".
#define MODULE_DESCRIPTION(_description) 		_description	// What your module does.
#define MODULE_LICENSE(_license) 				_license		// The following license for free software modules (e.g. GPL v2, MIT etc)
#define MODULE_PERMISSIONS(_permissions) _permissions  // Modules permissions such as read, write and or delete. devices and privilege



void driver_module_init(void);
void driver_module_exit(void);

#endif // TERRARIUM_DRIVER_MODULE_H
