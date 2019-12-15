/*
 * Copyright (C) 2019 Jakub Kaderka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file    app/gui.h
 * @brief   Graphical interface for user
 *
 * @addtogroup app
 * @{
 */

#ifndef __APP_GUI_H_
#define __APP_GUI_H_

#include <types.h>
#include "drivers/gps.h"

typedef enum {
    GUI_EVT_SHORT_NEXT,     /**< Short next button press */
    GUI_EVT_SHORT_ENTER,    /**< Short enter button press */
    GUI_EVT_LONG_NEXT,      /**< Long next button press */
    GUI_EVT_LONG_ENTER,     /**< Long enter button press */
    GUI_EVT_REDRAW,         /**< Redraw current screen */
    /* internal events */
    GUI_EVT_ENTERED,        /**< Window was just entered from outside */
} gui_event_t;

/**
 * System menu handling
 *
 * @param event     Event to be processed
 * @return True if still in menu, false if exited from menu
 */
extern bool Gui_Menu(gui_event_t event);

/**
 * Main screens handling
 *
 * @param event     Event to be processed
 * @return  True if still in screens, false if menu entered
 */
extern bool Gui_Screens(gui_event_t event);

/**
 * Draw popup window
 *
 * @param str   String to be shown inside popup
 */
extern void Gui_Popup(const char *str);

/**
 * Sent event to GUI
 *
 * @param event     Event to be processed
 */
extern void Gui_Event(gui_event_t event);

/**
 * Initialize GUI and draw main screen
 */
extern void Gui_Init(void);

#endif

/** @} */

