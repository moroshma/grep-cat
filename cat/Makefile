GCC = gcc
FLAGS = -Wall -Werror -Wextra
CatSrc = cat.c handler.c


cat:
	$(GCC) $(FLAGS) $(CatSrc) -o cat



commit:
	git add .
	git commit -m "update"
	git push origin develop

clear:
	rm cat
