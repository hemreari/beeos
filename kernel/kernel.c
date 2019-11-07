#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../fs/initrd.h"
#include <stdint.h>


void kernel_main() {
	isr_install();
	irq_install();

	asm("int $2");
	asm("int $3");

	clear_screen();

	kprint("Type something\n"
		   "Type END to halt the CPU\n>");

	uint32_t initrd_location;
	kmalloc(1000, 1, &initrd_location);
	char initrd_location_str[16] = "";
	hex_to_ascii(initrd_location, initrd_location_str);
	kprint("initrd location: ");
	kprint(initrd_location_str);

	fs_root = initialise_initrd(initrd_location);

	int i = 0;
	struct dirent *node = 0;

	while ((node = readdir_fs(fs_root, i)) != 0) {
		kprint("Found File ");
		kprint(node->name);
		fs_node_t *fsnode = finddir_fs(fs_root, node->name);

		if ((fsnode->flags&0x7) == FS_DIRECTORY) {
			kprint("\n\t(directory)\n");
		} else {
			kprint("\n\t contents: \"");
			char buf[256];
			uint32_t sz = read_fs(fsnode, 0, 256, buf);
			int j;
			for (j = 0; j < sz; j++) {
				kprint(buf[j]);
			}
			kprint("\"\n");
		}
		i++;
	}
}

void user_input(char *input) {
	if (strcmp(input, "END") == 0) {
		kprint("Stopping the CPU. Bye\n");
		asm volatile("hlt");
	} else if (strcmp(input, "PAGE") == 0) {
		uint32_t phys_addr;
		uint32_t page = kmalloc(1000, 1, &phys_addr);
		char page_str[16] = "";
		hex_to_ascii(page, page_str);
		char phys_str[16] = "";
		hex_to_ascii(phys_addr, phys_str);
		kprint("Page: ");
		kprint(page_str);
		kprint(", physical address: ");
		kprint(phys_str);
		kprint("\n");
	}

	kprint("You typed: ");
	kprint(input);
	kprint("\n> ");
}

