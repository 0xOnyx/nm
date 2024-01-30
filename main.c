#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <strings.h>
#include <bsd/string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <elf.h>
#include <errno.h>

#define DEFAULT_FILE "a.out"

enum SYM_TYPE {
'A';
'B';
'b';
'C';
'D';
'd';
'G';
'g';
'i';
'I';
'N';
'n';
'p';
'R';
'r';
'S';
's';
'T';
't';
'U';
'u';
'V';
'v';
'W';
'w';
'-';
};


typedef enum {
	SYM_TYPE_ABSOLUTE = 'A',
	SYM_TYPE_BSS = 'B',
	SYM_TYPE_COMMON = 'C',
	SYM_TYPE_DATA = 'D',
	SYM_TYPE_SMALL_DATA = 'G',
	SYM_TYPE_INDIRECT_FUNC = 'i',
	SYM_TYPE_INDIRECT = 'I',
	SYM_TYPE_DEBUG = 'N',
	SYM_TYPE_READONLY = 'R',
	SYM_TYPE_SMALL_BSS = 'S',
	SYM_TYPE_TEXT = 'T',
	SYM_TYPE_UNDEFINED = 'U',
	SYM_TYPE_UNIQUE_GLOBAL = 'u',
	SYM_TYPE_WEAK_OBJECT = 'V',
	SYM_TYPE_WEAK = 'W',
	SYM_TYPE_UNKNOWN = '?',
} Elf_Symbol_Type;

typedef struct s_symtab {
	Elf64_Sym 		*sym_element;
	Elf_Symbol_Type *type;
	char 			*name;
} Elf_Symtab_Element;


void	handle_error(char *msg)
{
	char	buff_str[255];

	if (strlcat(buff_str, "ft_nm: ", 255) > 255
		|| strlcat(buff_str, msg, 255) > 255)
		exit(1);
	perror(buff_str);
	exit(EXIT_FAILURE);
}

int		check_elf(const unsigned char *file, int *arch)
{
	*arch = file[EI_CLASS];
	return (*arch <= ELFCLASSNONE
		 || *arch > ELFCLASS64
		 || file[EI_DATA] <= ELFDATANONE
		 || file[EI_DATA] > ELFDATA2MSB
		 || file[EI_VERSION] != EV_CURRENT
		 || file[EI_MAG0] != ELFMAG0
		 || file[EI_MAG1] != ELFMAG1
		 || file[EI_MAG2] != ELFMAG2
		 || file[EI_MAG3] != ELFMAG3
	);
}

Elf64_Shdr *get_an_type_in_symtab(Elf64_Shdr *shdr, const unsigned  char *file, int size, Elf64_Word type)
{
	for (int i = 0; i < size; i++)
		if (shdr[i].sh_type == type)
			return shdr + i;
}

Elf_Symbol_Type get_type_in_symtab()
{



void fill_symtab(Elf_Symtab_Element *symtab_element, Elf64_Shdr *shdr_sym, Elf64_Shdr *shdr, const unsigned char *file)
{
	Elf64_Sym *sym_tab;
	char *str_table;

	sym_tab = (Elf64_Sym *)(file + shdr_sym->sh_offset);
	str_table = (char *)(file + shdr[shdr_sym->sh_link].sh_offset);

	for (int i = 0; i < shdr_sym->sh_size / shdr_sym->sh_entsize; i++){
		int index_str = sym_tab[i].st_name;
		if (index_str != 0){
			symtab_element[i].sym_element = sym_tab + i;
			symtab_element[i].name = str_table + index_str;
			symtab_element[i].type = get_an_type_in_symtab()
		}
	}
}

void	process_x64(const unsigned char *file, size_t length_file)
{
	Elf64_Ehdr	*ehdr;
	Elf64_Shdr 	*shdr;
	Elf64_Shdr  *shdr_sym;

	ehdr = (Elf64_Ehdr *)file;
	shdr = (Elf64_Shdr *)(file + ehdr->e_shoff);
	shdr_sym = get_an_type_in_symtab(shdr, file, ehdr->e_shnum, SHT_SYMTAB);


	Elf_Symtab_Element *symtab = calloc(shdr_sym->sh_size / shdr_sym->sh_entsize, sizeof(Elf_Symtab_Element));
	fill_symtab(symtab, shdr_sym, shdr, file);
}

int	main(int argc, char **argv)
{
	int 				arch;
	struct stat			sb;
	unsigned char		*buff_file;
	size_t				length_file;
	int					fd_file;
	char 				*file;

	file = argc <= 1 ? DEFAULT_FILE : argv[1];
	if ((fd_file = open(file, O_RDONLY)) < 0
	||	fstat(fd_file, &sb) < 0) {
		handle_error(file);
	}
	length_file = (size_t)sb.st_size;
	if ((buff_file = mmap(NULL, length_file, PROT_READ, MAP_PRIVATE, fd_file, 0)) == (void *)-1 ) {
		handle_error("memory");
	}
	if (check_elf(buff_file, &arch) < 0)
	{
		munmap(buff_file, length_file);
		handle_error(file);
	}
	if (arch == ELFCLASS64)
	 	process_x64(buff_file, length_file);
//	else
//		process_x32();

	return (0);
}