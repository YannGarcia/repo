/**
 * \file      libhal.h
 * \brief     Header file for the Hardware Abstract Layer library.
 *            It provides the basic features all supported hardware shall provide
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 * \see       libhal README.md
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

#if defined(ti_msp_exp432p401r)
#include "libhal.ti_msp_exp432p401r.h"
#elif defined(raspberry_pi)
#include "libhal.raspberry_pi.h"
#else
#error "Then env variables for hardwares are not properly defined"
#endif
