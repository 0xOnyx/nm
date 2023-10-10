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



void	handle_error(char *msg)
{
	char	buff_str[255];

	if (strlcat(buff_str, "ft_nm: ", 255) > 255
		|| strlcat(buff_str, buff_str, 255) > 255)
		exit(1);
	perror(msg);
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
	)
}

void	proces_x64(const unsigned char *file, size_t length_file)
{
	Elf64_Ehdr	*ehdr;
	Elf64_Shdr 	*shdr;

	ehdr = (Elf64_Ehdr *)file;
	shdr = (Elf64_Shdr *)file + ehdr->e_shoff;
	for (int i = 0; i < ehdr->e_shnum; i++)
	{
		if (shdr[i].sh_type = SHT_SYMTAB)
        {

        }
	}

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
	 	process_x64();
//	else
//		process_x32();

	return (0);
}