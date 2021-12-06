#include "kmain.h"
#include <arvern/utils.h>
#include <core/arm/atag.h>
#include <drivers/ccu.h>
#include <drivers/uart0.h>
#include <kmain.h>
#include <mmu/alloc.h>
#include <sys/k_syscall.h>

void kmain(uint32_t r0, uint32_t r1, uint32_t r2)
{
  UNUSED(r0);
  UNUSED(r1);

  ccu_init();
  uart0_init();
  kmain_early_start();

  atag_init((atag_header_t*)r2);

  print_step("initializing heap allocator");
  alloc_init();
  print_ok();

  print_step("initializing syscalls");
  syscall_init();
  print_ok();

  kmain_init_fs((uintptr_t)NULL);

  kmain_start(atag_get_cmdline());
}
