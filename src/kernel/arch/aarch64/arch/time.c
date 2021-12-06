#include <arch/time.h>

#include <time/fake_clock.h>

void arch_timer_init()
{
  // TODO:
}

uint64_t arch_timer_uptime_microseconds()
{
  // TODO:
  return 0;
}

void arch_clock_init()
{
  fake_clock_init();
}

uint64_t arch_clock_get_current_microseconds()
{
  return fake_clock_get_current_microseconds();
}
