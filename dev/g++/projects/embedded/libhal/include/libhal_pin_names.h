/**
 * @file      libhal_pin_names.h
 * @brief     It provides the basic features all supported hardware shall provide.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 * @see       libhal README.md
 */
#pragma once


#if (SELECTED_HW == ti_msp_exp432p401r)
#include "hardware/ti_msp_exp432p401r/include/libhal_pin_names.raspberry_pi.h"
#elif (SELECTED_HW == raspberry_pi)
#include "hardware/raspberry_pi/include/libhal_pin_names.raspberry_pi.h"
#else
#error "Then env variable SELECTED_HW is not properly defined"
#endif // SELECTED_HW
