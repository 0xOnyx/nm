NAME = ft_nm

COLOR_ESC			= \033
COLOR_WHITE			= $(COLOR_ESC)[97m
COLOR_CYAN			= $(COLOR_ESC)[96m
COLOR_GREEN			= $(COLOR_ESC)[32m
COLOR_BLUE			= $(COLOR_ESC)[34m
COLOR_YELLOW		= $(COLOR_ESC)[33m
COLOR_MAGENTA 		= $(COLOR_ESC)[35m
COLOR_BOLD			= $(COLOR_ESC)[1m
COLOR_RESET			= $(COLOR_ESC)[0m
COLOR_RESET_BOLD	= $(COLOR_ESC)[21m

PATH_HEADER			= src/header/
PATH_x64			= src/x64/
PATH_x86			= src/x86/
PATH_UTIL			= src/utils/
PATH_MAIN			= src/
PATH_OBJ			= objs/

HEADER				= utils.h
SRC_x64				= process_x64.c
SRC_x86				= process_x86.c
SRC_UTIL			= utils.c
SRC_MAIN			= main.c

SRC_x64S			= $(addprefix $(PATH_x64),$(SRC_x64))
SRC_x86S			= $(addprefix $(PATH_x86),$(SRC_x86))
SRC_UTILS			= $(addprefix $(PATH_UTIL),$(SRC_UTIL))
SRC_MAINS			= $(addprefix $(PATH_MAIN),$(SRC_MAIN))

SRCS 				= $(SRC_x64S) $(SRC_x86S) $(SRC_UTILS) $(SRC_MAINS)

OBJ					= $(SRCS:.c=.o)

OBJS				= $(addprefix $(PATH_OBJ),$(OBJ))

HEADERS				= $(addprefix $(PATH_HEADER),$(HEADER))


DEBUG				= 1

DEBUGING			= -g3 -fsanitize=address -fsanitize=undefined
CFLAGS				= -Wall -Wextra -Werror

OPTIONS				= -I$(PATH_HEADER)

LIB					= 

CC					= gcc
RM					= rm -rf

ifeq ($(DEBUG), 1)
	CFLAGS += $(DEBUGING)
endif

OPTIMISATION_LEVEL = 0

ifdef OPTIMISATION_LEVEL
	ifeq ($(OPTIMISATION_LEVEL),0)
		# Nothing
	else
		CFLAGS += -O$(OPTIMISATION_LEVEL)
	endif
else
	CFLAGS += -Ofast
endif

all			: $(NAME)

$(PATH_OBJ)$(PATH_MAIN)%.o	: $(PATH_MAIN)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_MAIN)
	@$(CC) $(CFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_BLUE)DEBUG => [%s] $(COLOR_BOLD)MAIN\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_x64)%.o	: $(PATH_x64)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_x64)
	@$(CC) $(CFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_MAGENTA)DEBUG => [%s] $(COLOR_BOLD)X64\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_x86)%.o	: $(PATH_x86)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_x86)
	@$(CC) $(CFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_YELLOW)DEBUG => [%s] $(COLOR_BOLD)X64\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(PATH_OBJ)$(PATH_UTIL)%.o		: $(PATH_UTIL)%.c $(HEADERS)
	@mkdir -p $(PATH_OBJ)$(PATH_UTIL)
	@$(CC) $(CFLAGS) $(OPTIONS) -o $(@) -c $(<)
	@printf "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION $(COLOR_MAGENTA)DEBUG => [%s] $(COLOR_BOLD)UTIL\t\t=>\t$(COLOR_WHITE)%s$(COLOR_RESET)\n" $(DEBUG) $<

$(NAME)		: $(OBJS)
	@$(CC) $(CFLAGS) $(OPTIONS) -o $(@) $(OBJS) $(LIB)
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] LINKAGE $(COLOR_BOLD)ALL OBJS FILE =>\n\t $(COLOR_WHITE)$(OBJS:.o=.o\n\t)"
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] COMPILATION FINISH !$(COLOR_WHITE)$(COLOR_RESET_BOLD)"

clean		:
	@$(RM) $(OBJS)
	@$(RM) $(PATH_OBJ)
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] DELETE $(COLOR_BOLD)ALL OBJS FILE $(NAME) =>\n\t $(COLOR_WHITE)$(OBJS:.o=.o\n\t)"
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] CLEAN FINISH !$(COLOR_RESET)"

fclean		: clean
	@$(RM) $(NAME)
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] DELETE $(COLOR_BOLD)PROGRAMME =>\n\t $(COLOR_WHITE)$(NAME)"
	@echo "$(COLOR_GREEN)[$(COLOR_WHITE)INFO$(COLOR_GREEN)] FCLEAN FINISH !$(COLOR_RESET)"

re			: fclean all

.PHONY: all fclean clean re