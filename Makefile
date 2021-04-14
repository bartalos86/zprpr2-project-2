TASK := main.c

current:
	clear && gcc $(TASK) -std=c99 -pedantic -Wall -Werror -Wextra -o compiled && ./compiled

memcheck:
	clear && gcc $(TASK) -std=c99 -pedantic -Wall -Werror -Wextra -o compiled && valgrind -v --leak-check=full --show-leak-kinds=all ./compiled