#ifndef NM_UTILS_H
# define NM_UTILS_H

# include <stdio.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <strings.h>
# include <bsd/string.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <elf.h>
# include <errno.h>

# define DEFAULT_FILE "a.out"

typedef struct s_symtab {
	Elf64_Sym 		*sym_element;
	char 			type;
	char 			*name;
} Elf_Symtab_Element;

void	process_x64(const unsigned char *file, size_t length_file);
void	process_x32(const unsigned char *file, size_t length_file);

void	handle_error(char *msg);

#endif //NM_UTILS_H
