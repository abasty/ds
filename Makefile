tests : tests.c ds_btree.c *.h
	$(CC) $(CFLAGS) -g -O -Wall -Werror -o $@ tests.c ds_btree.c

clean :
	@rm tests 2>/dev/null || true
