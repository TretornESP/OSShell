#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "symbol_table.h"
#include "utility.h"

#include <linux/elf.h>

struct symbol * get_symbol(const char* filename, const char* constant) {

	struct symbol_table *sym = symbol_table_create(filename);
	if (sym) {
		printf("SIZE: %d\n", sym->num_symbols);
		printf("NAME: %s\n", sym->name);

		for (int i = 0; i < sym->num_symbols; i++) {
			printf("SYM %d: %s [%x]\n", i, (sym->symbols[i]).name, (sym->symbols[i]).addr);
			if (!strcmp(sym->symbols[i]).name, constant)) {
				return sym->symbols[i];
			}
		}
	} else {
		printf("ERROR\n");
	}
	return NULL;
}
