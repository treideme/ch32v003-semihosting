/*******************************************************************************
 * Copyright 2023 Thomas Reidemeister                                          *
 *                                                                             *
 * Licensed under the Apache License, Version 2.0 (the "License");             *
 * you may not use this file except in compliance with the License.            *
 * You may obtain a copy of the License at                                     *
 *                                                                             *
 *     http://www.apache.org/licenses/LICENSE-2.0                              *
 *                                                                             *
 * Unless required by applicable law or agreed to in writing, software         *
 * distributed under the License is distributed on an "AS IS" BASIS,           *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    *
 * See the License for the specific language governing permissions and         *
 * limitations under the License.                                              *
 *******************************************************************************

 @file main.c
 @author Thomas Reidemeister
*/

#include "semihosting.h"
#include "ch32v00x_dbgmcu.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    printf_("SystemClk:%lu\r\n", SystemCoreClock );
    printf_( "ChipID:%08lx\r\n", DBGMCU_GetCHIPID() );

    while(1)
    {
    }
}
