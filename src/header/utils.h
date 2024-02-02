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
# include <unistd.h>

# define DEFAULT_FILE "a.out"

typedef struct {
	Elf64_Sym 		*sym_element;
	char 			type;
	char 			*name;
} Elf64_Symtab_Element;


typedef struct {
    Elf32_Sym 		*sym_element;
    char 			type;
    char 			*name;
} Elf32_Symtab_Element;


void	process_x64(const unsigned char *file, size_t length_file);
void	process_x32(const unsigned char *file, size_t length_file);

void	handle_error(char *msg);

void    *ft_memset(void *b, int c, size_t len);
void    *ft_memcpy(void *_dest, const void *_src, size_t _n);
void    *ft_calloc(size_t count, size_t size);
int     ft_strcmp(const char *_s1, const char *_s2);
size_t  ft_strlcat(char *dst, const char *src, size_t siz);

void    address_to_hex(uint64_t addr, char *buffer, int buffer_len);

#endif //NM_UTILS_H
