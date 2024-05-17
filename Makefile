name = minishell
cc = cc
build_dir = build
src = builtins pipex minishell my_split helper_functions1 helper_functions2
obj = $(addprefix $(build_dir)/, $(addsuffix .o, $(src)))
command_handler_src = executor quotes_parser
command_handler_dir = command_handler
obj += $(addprefix $(build_dir)/$(command_handler_dir)/, $(addsuffix .o, $(command_handler_src)))
readline_dir = readline-8.2
libft_dir = libft
lflags = -Llibft -L$(readline_dir)/lib
iflags = -Iincs -I$(libft_dir) -I$(readline_dir)/include
cflags = -g3 #-fsanitize=address#-Wall -Wextra -Werror

all : $(name)

$(name) : $(build_dir) $(build_dir)/$(command_handler_dir) $(readline_dir)/lib/libreadline.a $(libft_dir)/libft.a $(obj)
	$(cc) $(cflags) $(lflags) -o $(name) $(obj) -lreadline -lcurses -lft

$(build_dir)/%.o: %.c Makefile incs/minishell.h
	$(cc) $(cflags) $(iflags) -c $< -o $@

$(libft_dir)/libft.a:
	make -C $(libft_dir)

$(readline_dir)/lib/libreadline.a:
	cd $(readline_dir); ./configure --prefix=$(shell pwd)/$(readline_dir) --enable-shared=no
	make -C $(readline_dir) install

$(build_dir)/$(command_handler_dir): | $(build_dir)
	mkdir $@

$(build_dir):
	mkdir $@

clean :
	rm -rf $(build_dir)
	make -C $(libft_dir) clean
	-make -C $(readline_dir) clean

fclean : clean
	make -C $(libft_dir) fclean
	rm -f $(name)
	-make -C $(readline_dir) uninstall
	-make -C $(readline_dir) distclean

re : fclean all

.PHONY: all clean fclean re
