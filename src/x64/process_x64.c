#include "utils.h"

static void print_symtab_x64(Elf64_Symtab_Element *symtab, Elf64_Xword i) {
	char buff[512];
    size_t len;


    for (Elf64_Xword j = 0; j < i; j++) {
		if (symtab[j].name != NULL) {
			ft_memset(buff, ' ', sizeof(char) * 512);
			if (symtab[j].sym_element->st_value != 0)
                address_to_hex(symtab[j].sym_element->st_value, buff, 16);

            buff[17] = symtab[j].type;
            buff[19] = 0;
            strlcat(buff, symtab[j].name, 512);
            len = strlcat(buff, "\n", 512);
            write(STDOUT_FILENO, buff, len);
		}
	}

}

static Elf64_Shdr *get_an_type_in_symtab(Elf64_Shdr *shdr, int size, Elf64_Word type){
	for (int i = 0; i < size; i++)
		if (shdr[i].sh_type == type)
			return shdr + i;
	return NULL;
}

static char get_section_data(unsigned char bind, Elf64_Word section_type, Elf64_Xword section_flags){
	if (section_type == SHT_NOBITS && (section_flags & SHF_ALLOC)) {
		if (section_flags & SHF_IA_64_SHORT) {
			return bind == STB_LOCAL ? 's' : 'S'; // Small uninitialized data section
		} else {
			return bind == STB_LOCAL ? 'b' : 'B'; // Regular BSS section
		}
	} else if (section_flags & SHF_ALLOC) {
		if (section_flags & SHF_WRITE) {
			if (section_flags & SHF_IA_64_SHORT) { // Assuming is_small_data_section is defined elsewhere
				return bind == STB_LOCAL ? 'g' : 'G'; // Small data section
			} else {
				return bind == STB_LOCAL ? 'd' : 'D'; // Initialized data section
			}
		} else {
			return bind == STB_LOCAL ? 'r' : 'R'; // Read-only data section
		}
	}
	return '?';
}

static char get_symbol_type(Elf64_Sym *sym, Elf64_Shdr *shdr, Elf64_Ehdr *ehdr) {
	unsigned char type;
	unsigned char bind;
	Elf64_Word section_type;
	Elf64_Xword section_flags;

	type = ELF64_ST_TYPE(sym->st_info);
	bind = ELF64_ST_BIND(sym->st_info);
	section_type = sym->st_shndx >= ehdr->e_shnum  ? 0 : shdr[sym->st_shndx].sh_type;
	section_flags = sym->st_shndx >= ehdr->e_shnum ? 0 : shdr[sym->st_shndx].sh_flags;

	if (sym->st_shndx == SHN_ABS) {
		return 'A';
	}

	if (bind == STB_WEAK) {
		return sym->st_shndx == SHN_UNDEF ? 'w' : 'W';
	}

	if (bind == STB_GNU_UNIQUE) {
		return 'u';
	}

	switch (type) {
		case STT_OBJECT:
			if (sym->st_shndx == SHN_COMMON)
				return 'C';
			return get_section_data(
					bind, section_type, section_flags);
		case STT_FUNC:
			if (section_flags & SHF_EXECINSTR)
				return bind == STB_LOCAL ? 't' : 'T'; // Code section
			else if (sym->st_shndx == SHN_UNDEF)
				return 'U';
			break;
		case STT_SECTION:
			return 'S';
		case STT_FILE:
			return 'F';
		case STT_COMMON:
			return 'C';
		case STT_TLS:
			return 'T';
		case STT_NOTYPE:
			if (sym->st_shndx == SHN_UNDEF)
				return 'U';
			return get_section_data(
					bind, section_type, section_flags);
		default:
			if (type == STT_GNU_IFUNC) {
				return 'i';
			}
			break;
	}

	return '?'; // Unknown symbol type
}


static int is_element(Elf64_Sym *sym){
	unsigned char type;

	type = ELF64_ST_TYPE(sym->st_info);
	return type != STT_SECTION && type != STT_FILE && sym->st_name != 0;

}

static void fill_symtab(Elf64_Symtab_Element *symtab_element, Elf64_Shdr *shdr_sym, Elf64_Shdr *shdr, const unsigned char *file)
{
	Elf64_Sym *sym_tab;
	char *str_table;

	sym_tab = (Elf64_Sym *)(file + shdr_sym->sh_offset);
	str_table = (char *)(file + shdr[shdr_sym->sh_link].sh_offset);

	for (Elf64_Xword i = 1; i < shdr_sym->sh_size / shdr_sym->sh_entsize; i++){
		if (is_element(sym_tab + i)){
			symtab_element->sym_element = sym_tab + i;
			symtab_element->name = str_table + sym_tab[i].st_name;
			symtab_element->type = get_symbol_type(
					sym_tab + i, shdr, (Elf64_Ehdr *)file);
			symtab_element++;
		}
	}
}

void swap_symtab_element(Elf64_Symtab_Element *a, Elf64_Symtab_Element *b)
{
	Elf64_Symtab_Element tmp;

	ft_memcpy(&tmp, a, sizeof(Elf64_Symtab_Element));
	ft_memcpy(a, b, sizeof(Elf64_Symtab_Element));
	ft_memcpy(b, &tmp, sizeof(Elf64_Symtab_Element));
}

static Elf64_Xword partition(Elf64_Symtab_Element *arr, long long low, long long high) {
	char *pivot = arr[high].name;

	long long i = (low - 1);
	for (long long j=low; j <= high; j++)
	{
		if (ft_strcmp(arr[j].name, pivot) < 0)
		{
			i++;
			swap_symtab_element(arr + i, arr + j);
		}
	}
	swap_symtab_element(arr + i + 1, arr + high);
	return (i + 1);
}

static void quickSort(Elf64_Symtab_Element *arr, long long low, long long high) {
	if (low < high)
	{
		long long pi = partition(arr, low, high);
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

static Elf64_Xword count_nbr(int (*f)(Elf64_Sym *), const unsigned char *file, Elf64_Shdr  *shdr_sym)
{
	Elf64_Sym *sym_tab;
	Elf64_Xword num_sym;
	long long size;

	sym_tab = (Elf64_Sym *)(file + shdr_sym->sh_offset);
	size = shdr_sym->sh_size / shdr_sym->sh_entsize;
	num_sym = 0;
	for (int i = 0; i < size; i++)
		if (f(sym_tab + i))
			num_sym++;
	return num_sym;
}

void	process_x64(const unsigned char *file, size_t length_file)
{
	Elf64_Ehdr	*ehdr;
	Elf64_Shdr 	*shdr;
	Elf64_Shdr  *shdr_sym;

	Elf64_Xword num_sym;
	Elf64_Symtab_Element *symtab;

	ehdr = (Elf64_Ehdr *)file;
	shdr = (Elf64_Shdr *)(file + ehdr->e_shoff);
	shdr_sym = get_an_type_in_symtab(shdr, ehdr->e_shnum, SHT_SYMTAB);
	if (!shdr_sym)
	{
		munmap((void *)file, length_file);
		handle_error("no symbols");
	}

	num_sym = count_nbr(&is_element, file, shdr_sym);
	symtab = ft_calloc(num_sym, sizeof(Elf64_Symtab_Element));

	fill_symtab(symtab, shdr_sym, shdr, file);
	quickSort(symtab, (Elf64_Xword)0, num_sym - 1);
	print_symtab_x64(symtab, num_sym);
	free(symtab);
}
