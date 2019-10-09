#ifndef LIBLOADER_H_INCLUDED
#define LIBLOADER_H_INCLUDED
#include "./time.h"
#endif

#define MAX_MODULES 128
#define DIR_SIZE 1024

char shell_dir[DIR_SIZE];
char lib_dir[DIR_SIZE];

time_t last_modified;

int show_hidden, only_name, is_recursive;

int modules;

struct module {
	char id[256];
	void* handle;
	void (*function)(int, char**);
};
struct module lib[MAX_MODULES];

//void md5(const char*);
void set_modified_time();
int is_modified();
void config();
void reset_flags();
int get_name_from_path(const char*);
int get_extension_from_path(const char*);
void search_new_libs();
void add_lib(const char*);
void load_lib(const char*);
int unload_lib();
void unload_all_libs();
void load_all_libs();
