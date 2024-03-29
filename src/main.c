#include "utils.h"

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

void nm(char *file){
	int 				arch;
	struct stat			sb;
	unsigned char		*buff_file;
	size_t				length_file;
	int					fd_file;

	if ((fd_file = open(file, O_RDONLY)) < 0
		||	fstat(fd_file, &sb) < 0) {
		handle_error(file);
		return ;
	}
	length_file = (size_t)sb.st_size;
	if ((buff_file = mmap(NULL, length_file, PROT_READ, MAP_PRIVATE, fd_file, 0)) == (void *)-1 ) {
		handle_error("memory");
		return ;
	}
	if (check_elf(buff_file, &arch))
	{
		munmap(buff_file, length_file);
		handle_error("not an elf file");
		return ;
	}
	if (arch == ELFCLASS64)
		process_x64(buff_file, length_file);
	else
		process_x32(buff_file, length_file);
	munmap(buff_file, length_file);
}


int	main(int argc, char **argv)
{
	char	buff[512];
	size_t	len;

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++){
			if (argc != 2){
				ft_memset(buff,0, sizeof(char) * 512);
				ft_strlcat(buff, argv[i], 512);
				len = ft_strlcat(buff, ":\n", 512);
                ft_print(buff, len);
			}
			nm(argv[i]);
			if (i + 1 != argc)
                ft_print("\n", 1);
		}
		return (argc - 1);
	}
	else
		nm(DEFAULT_FILE);
	return (1);
}
