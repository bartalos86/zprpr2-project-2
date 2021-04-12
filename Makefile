TASK := main.c

current:
	clear && gcc $(TASK) -std=c99 -pedantic -Wall -Werror -Wextra -o compiled && ./compiled