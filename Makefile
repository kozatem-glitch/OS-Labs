CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGETS = parent child1 child2

.PHONY: all clean run help

all: $(TARGETS)
	@echo ""
	@echo "Build complete!"
	@echo "Run: make run"
	@echo "Clean: make clean"
	@echo "Help: make help"
	@echo ""

parent: parent.c
	$(CC) $(CFLAGS) -o parent parent.c

child1: child1.c
	$(CC) $(CFLAGS) -o child1 child1.c

child2: child2.c
	$(CC) $(CFLAGS) -o child2 child2.c

clean:
	rm -f $(TARGETS)

run: all
	./parent

help:
	@echo "Доступные команды:"
	@echo "  make        - скомпилировать все программы"
	@echo "  make clean  - удалить скомпилированные файлы"
	@echo "  make run    - скомпилировать и запустить"
	@echo "  make help   - показать эту справку"
