CC		= clang++
CFLAGS	= -Wall -Wextra -Werror

test: test.cc
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f test

re: clean test
