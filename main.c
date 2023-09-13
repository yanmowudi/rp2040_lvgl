/****************************************************************************
*  Copyright 2021 Gorgon Meducer (Email:embedded_zhuoran@hotmail.com)       *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/
/*============================ INCLUDES ======================================*/
#include "pico/stdlib.h"
#include "perf_counter.h"
#include "RP2040.h"
#include "bsp.h"

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/porting/lv_port_disp.h"

#include <stdio.h>

#include "RTE_Components.h"
#if defined(RTE_Compiler_EventRecorder) && defined(RTE_Compiler_IO_STDOUT_EVR)
#   include <EventRecorder.h>
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

void SysTick_Handler(void)
{
    lv_tick_inc(1);
}

static void system_init(void)
{
    extern void SystemCoreClockUpdate();

    SystemCoreClockUpdate();
    /*! \note if you do want to use SysTick in your application, please use 
     *!       init_cycle_counter(true); 
     *!       instead of 
     *!       init_cycle_counter(false); 
     */
    SysTick_Config(SystemCoreClock/1000);
    init_cycle_counter(true);

#if defined(RTE_Compiler_EventRecorder) && defined(RTE_Compiler_IO_STDOUT_EVR)
    EventRecorderInitialize(0, 1);
#endif
    stdio_init_all();

    led_init();
    spilcd_init();
    //i80lcd_init();
}

int main(void) 
{
    system_init();

    __cycleof__("printf") {
        printf("Hello Pico-Template\r\n");
    }
    
#if defined( __PERF_COUNTER_COREMARK__ ) && __PERF_COUNTER_COREMARK__
    printf("\r\nRun Coremark 1.0...\r\n");
    coremark_main();
#endif
    
    lv_init();
    lv_port_disp_init();
    
    lv_demo_benchmark();
    
    while (true) {
        led_breath();
        lv_timer_handler();
    }
    //return 0;
}
