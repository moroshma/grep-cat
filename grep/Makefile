GCC = gcc
FLAGS = -Wall -Werror -Wextra
GREP_SRC = grep.c handler.c

grep:
	$(GCC) $(FLAGS) $(GREP_SRC) -o grep

commit:
	git add .
	git commit -m "update"
	git push origin develop

clear:
	rm grep
