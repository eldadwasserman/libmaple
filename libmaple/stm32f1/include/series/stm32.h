/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010, 2011 LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file libmaple/stm32f1/stm32.h
 * @brief STM32F1 chip- and series-specific definitions.
 */

#ifndef _LIBMAPLE_STM32F1_H_
#define _LIBMAPLE_STM32F1_H_

#ifdef __cplusplus
extern "C" {
#endif

#define STM32_MCU_SERIES                STM32_SERIES_F1

/* The STM32F1 series is subdivided into "lines". libmaple currently
 * officially supports STM32F103 performance line MCUs (see the
 * MCU-specific value section below).
 *
 * You can use these F1 line defines if porting libmaple to support
 * MCUs on other lines. */
#define STM32_F1_LINE_PERFORMANCE       0
#define STM32_F1_LINE_VALUE             1
#define STM32_F1_LINE_ACCESS            2
#define STM32_F1_LINE_CONNECTIVITY      3

/*
 * MCU-specific values.
 *
 * You can use this section to override any of the below settings on a
 * per-MCU basis. For example, if your MCU has different STM32_PCLK1
 * or STM32_PCLK2 values, you can set them here and the values for
 * STM32F103 microcontrollers set below won't take effect.
 */

#if defined(MCU_STM32F103RB)
#   define STM32_F1_LINE                STM32_F1_LINE_PERFORMANCE
#   define STM32_NR_GPIO_PORTS          4
#   define STM32_SRAM_END               ((void*)0x20005000)
#   define STM32_MEDIUM_DENSITY

#elif defined(MCU_STM32F103ZE)
#   define STM32_F1_LINE                STM32_F1_LINE_PERFORMANCE
#   define STM32_NR_GPIO_PORTS          7
#   define STM32_SRAM_END               ((void*)0x20010000)
#   define STM32_HIGH_DENSITY

#elif defined(MCU_STM32F103CB)
#   define STM32_F1_LINE                STM32_F1_LINE_PERFORMANCE
    /* This STM32_NR_GPIO_PORTS is not strictly true, but only pins 0
     * and exist, and they're used for OSC (e.g. on e.g. LeafLabs
     * Maple Mini), so we'll live with this for now. */
#   define STM32_NR_GPIO_PORTS          3
#   define STM32_SRAM_END               ((void*)0x20005000)
#   define STM32_MEDIUM_DENSITY

#elif defined(MCU_STM32F103RE)
#   define STM32_F1_LINE                STM32_F1_LINE_PERFORMANCE
#   define STM32_NR_GPIO_PORTS          4
#   define STM32_SRAM_END               ((void*)0x20010000)
#   define STM32_HIGH_DENSITY

#elif defined(MCU_STM32F100RB)
#   define STM32_F1_LINE                STM32_F1_LINE_VALUE
#   define STM32_NR_GPIO_PORTS          4
#   define STM32_SRAM_END               ((void*)0x20002000)
#   define STM32_MEDIUM_DENSITY

#else
#error "Unrecognized STM32F1 MCU, or no MCU specified. Add something like " \
       "-DMCU_STM32F103RB to your compiler arguments."
#endif

/*
 * Derived values.
 */

#if STM32_F1_LINE == STM32_F1_LINE_PERFORMANCE
     /* All supported performance line MCUs have a USB peripheral */
#    define STM32_HAVE_USB              1

#    ifdef STM32_MEDIUM_DENSITY
#       define STM32_NR_INTERRUPTS      43
#       define STM32_HAVE_FSMC          0
#    elif defined(STM32_HIGH_DENSITY)
#       define STM32_NR_INTERRUPTS      60
#       define STM32_HAVE_FSMC          1
#    endif

#elif STM32_F1_LINE == STM32_F1_LINE_VALUE
     /* Value line MCUs don't have USB peripherals. */
#    define STM32_HAVE_USB              0

#    ifdef STM32_MEDIUM_DENSITY
#        define STM32_NR_INTERRUPTS     56
#        define STM32_HAVE_FSMC         0
#    elif defined(STM32_HIGH_DENSITY)
         /* 61 interrupts here counts the possibility for a remapped
          * DMA2 channel 5 IRQ occurring at NVIC index 60.  */
#        define STM32_NR_INTERRUPTS     61
#        define STM32_HAVE_FSMC         1
#    endif

#endif

/*
 * Clock configuration.
 *
 * You can patch these for your line, MCU, clock configuration,
 * etc. here or by setting cflags when compiling libmaple.
 */

#if STM32_F1_LINE == STM32_F1_LINE_PERFORMANCE
#    ifndef STM32_PCLK1
#    define STM32_PCLK1                     36000000U
#    endif
#    ifndef STM32_PCLK2
#    define STM32_PCLK2                     72000000U
#    endif
#    ifndef STM32_DELAY_US_MULT
#    define STM32_DELAY_US_MULT             12 /* FIXME: value is incorrect. */
#    endif
#elif STM32_F1_LINE == STM32_F1_LINE_VALUE        /* TODO */
#    ifndef STM32_PCLK1
#    define STM32_PCLK1                     12000000U
#    endif
#    ifndef STM32_PCLK2
#    define STM32_PCLK2                     24000000U
#    endif
#    ifndef STM32_DELAY_US_MULT
#    define STM32_DELAY_US_MULT             8 /* FIXME: value is incorrect. */
#    endif
#elif STM32_F1_LINE == STM32_F1_LINE_ACCESS       /* TODO */
#elif STM32_F1_LINE == STM32_F1_LINE_CONNECTIVITY /* TODO */
#endif

/* Make sure we have the F1-specific defines we need. */
#if !defined(STM32_F1_LINE)
#error "Bad STM32F1 configuration. Check STM32F1 <series/stm32.h> header."
#endif

#ifdef __cplusplus
}
#endif

#endif
