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

 @file semihosting.c
 @brief Semihosting interface source file
 @author Thomas Reidemeister

 Adapted from: https://github.com/tomverbeure/vexriscv_ocd_blog/blob/437263116729a1b317fa7b7723c10c721be8a805/sw_semihosting/semihosting.c
*/

#include <stdio.h>
#include <stdarg.h>

#include "semihosting.h"

#define SEMIHOSTING_SYS_OPEN            0x01
#define SEMIHOSTING_SYS_CLOSE           0x02
#define SEMIHOSTING_SYS_WRITEC          0x03
#define SEMIHOSTING_SYS_WRITE0          0x04
#define SEMIHOSTING_SYS_WRITE           0x05
#define SEMIHOSTING_SYS_READ            0x06
#define SEMIHOSTING_SYS_READC           0x07
#define SEMIHOSTING_SYS_ISERROR         0x08
#define SEMIHOSTING_SYS_ISTTY           0x09
#define SEMIHOSTING_SYS_SEEK            0x0A
#define SEMIHOSTING_SYS_FLEN            0x0C
#define SEMIHOSTING_SYS_TMPNAM          0x0D
#define SEMIHOSTING_SYS_REMOVE          0x0E
#define SEMIHOSTING_SYS_RENAME          0x0F
#define SEMIHOSTING_SYS_CLOCK           0x10
#define SEMIHOSTING_SYS_TIME            0x11
#define SEMIHOSTING_SYS_SYSTEM          0x12
#define SEMIHOSTING_SYS_ERRNO           0x13
#define SEMIHOSTING_SYS_GET_CMDLINE     0x15
#define SEMIHOSTING_SYS_HEAPINFO        0x16
#define SEMIHOSTING_EnterSVC            0x17
#define SEMIHOSTING_ReportException     0x18
#define SEMIHOSTING_SYS_ELAPSED         0x30
#define SEMIHOSTING_SYS_TICKFREQ        0x31

#define RISCV_SEMIHOSTING_CALL_NUMBER 7

static inline int __attribute__ ((always_inline)) call_host(int reason, void* arg) {
#if 1
    register int value asm ("a0") = reason;
    register void* ptr asm ("a1") = arg;
    asm volatile (
        // Note: align 4 means, align by 2 to the power of 4!
        " .align 4 \n"
        // Generate compressed instructions
        " .option push \n"
        " .option norvc \n"
        // the debugger fetching the instructions could lead to a page fault.
        " slli x0, x0, 0x1f \n"
        " ebreak \n"
        " srai x0, x0, %[swi] \n"
        " .option pop \n"

        : "=r" (value) /* Outputs */
        : "0" (value), "r" (ptr), [swi] "i" (RISCV_SEMIHOSTING_CALL_NUMBER) /* Inputs */
        : "memory" /* Clobbers */
    );
    return value;
#else
    return 0;    
#endif    
}

static void sh_write0(const char* buf)
{
    // Print zero-terminated string
    call_host(SEMIHOSTING_SYS_WRITE0, (void*) buf);
}

static void sh_writec(char c)
{
    // Print single character
    call_host(SEMIHOSTING_SYS_WRITEC, (void*)&c);
}

__attribute__((used))
int _write(int fd, char *buf, int size)
{
  int i;

  for(i = 0; i < size; i++) {
    sh_writec(buf[i]);
  }

  return size;
}

__attribute__((used))
void *_sbrk(ptrdiff_t incr)
{
  extern char _end[];
  extern char _heap_end[];
  static char *curbrk = _end;

  if ((curbrk + incr < _end) || (curbrk + incr > _heap_end))
    return (void *) -1;

  curbrk += incr;
  return curbrk - incr;
}

int printf_(const char* format, ...)
{
    char buffer[128];

    va_list va;
    va_start(va, format);
    const int ret = vsnprintf(buffer, sizeof(buffer)-1, format, va);
    va_end(va);

    sh_write0(buffer);
    return ret;
}
