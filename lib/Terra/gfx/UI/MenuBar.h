#pragma once

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

// Items > SubItem > FlyoutsItem

typedef struct 
{

} MENU;

typedef struct 
{
    MENU menu;

} MENUITEM;

typedef struct 
{
    MENUITEM menuitem;
    // Connect to MenuItem
} MENUFLYOUT;

MENU Menu();
MENUITEM MenuItem(MENU menu);
MENUFLYOUT MenuFlyout(MENUITEM menuitem);

#ifdef __cplusplus
}
#endif  // __cplusplus