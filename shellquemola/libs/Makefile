OBJS := authors.so bye.so chdir.so close.so create.so date.so delete.so exit.so hola.so hour.so list.so pid.so query.so quit.so


.c.o:
	gcc -Wall -shared -fPIC $< -o $@

clean: 
	@rm -f $(OBJS)
