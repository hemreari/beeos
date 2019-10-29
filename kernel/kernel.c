#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"

void main() {
    isr_install();
	irq_install();

	clear_screen();

	kprint("Type something\n"
		   "Type END to halt the CPU\n>");
}

void user_input(char *input) {
  if (strcomp(input, "END") == 0) {
	kprint("Stopping the CPU. Bye\n");
	asm volatile("hlt");
  }

  kprint("You typed: ");
  kprint(input);
  kprint("\n> ");
}
