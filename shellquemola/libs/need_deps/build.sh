gcc -Wall -shared -fPIC -o list.so list.c ../../sys/recursive2.c
gcc -Wall -shared -fPIC -o allocate.so allocate.c ../../sys/perror.c ../../sys/time.c ../../sys/mem_list.c
gcc -Wall -shared -fPIC -o deallocate.so deallocate.c ../../sys/libloader.c ../../sys/perror.c ../../sys/time.c ../../sys/mem_list.c
gcc -Wall -shared -fPIC -o work.so work.c ../../sys/symbol_table.c
gcc -Wall -shared -fPIC -o runf.so runf.c ../../sys/symbol_table.c
