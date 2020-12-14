#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#ifdef ENABLE_LOGS_FOR_TESTS

#define LOG(level, level_color, format, ...)                                   \
  printf("%-8s | %s:%d:%s(): " format "\n",                                    \
         level,                                                                \
         __FILE__,                                                             \
         __LINE__,                                                             \
         __func__,                                                             \
         __VA_ARGS__)

#else // ENABLE_LOGS_FOR_TEST

#include <drivers/serial.h>

static const uint16_t serial_com1 = SERIAL_COM1;

#ifdef LOGS_WITH_COLORS
// See: https://github.com/shiena/ansicolor
#define ANSICOLOR_RESET        "\x1b[0m"
#define ANSICOLOR_FG_CYAN      "\x1b[36m"
#define ANSICOLOR_FG_LIGHTGRAY "\x1b[90m"
#define ANSICOLOR_FG_YELLOW    "\x1b[33m"

// We use colors because the serial port is bound to a file in debug mode (see
// qemu config in `Makefile`). That would not work otherwise, I think.
#define LOG(level, level_color, format, ...)                                   \
  fctprintf(&serial_stream_output,                                             \
            (void*)&serial_com1,                                               \
            "%s%-8s%s | %s%s:%ld:%s():%s " format "\n",                        \
            level_color,                                                       \
            level,                                                             \
            ANSICOLOR_RESET,                                                   \
            ANSICOLOR_FG_LIGHTGRAY,                                            \
            __FILE__,                                                          \
            __LINE__,                                                          \
            __func__,                                                          \
            ANSICOLOR_RESET,                                                   \
            __VA_ARGS__)

#else // LOGS_WITH_COLORS

#define LOG(level, level_color, format, ...)                                   \
  fctprintf(&serial_stream_output,                                             \
            (void*)&serial_com1,                                               \
            "%-8s | %s:%ld:%s(): " format "\n",                                \
            level,                                                             \
            __FILE__,                                                          \
            __LINE__,                                                          \
            __func__,                                                          \
            __VA_ARGS__)

#endif // LOGS_WITH_COLORS

#endif // ENABLE_LOGS_FOR_TEST

#ifdef ENABLE_KERNEL_DEBUG

#define DEBUG(format, ...) LOG("DEBUG", ANSICOLOR_FG_CYAN, format, __VA_ARGS__)

#define HEX_DEBUG(data, len)                                                   \
  DEBUG("%s", "(hexdump)");                                                    \
  for (uint64_t i = 0; i < len; i++) {                                         \
    if (i > 0 && i % 8 == 0) {                                                 \
      fctprintf(&serial_stream_output, (void*)&serial_com1, "\n");             \
    }                                                                          \
    fctprintf(                                                                 \
      &serial_stream_output, (void*)&serial_com1, "%02x ", (uint8_t)data[i]);  \
  }                                                                            \
  fctprintf(&serial_stream_output, (void*)&serial_com1, "\n");

#else // ENABLE_KERNEL_DEBUG

#define DEBUG(format, ...)

#define HEX_DEBUG(data, len)

#endif // ENABLE_KERNEL_DEBUG

#define INFO(format, ...) LOG("INFO", ANSICOLOR_FG_YELLOW, format, __VA_ARGS__)

#endif
