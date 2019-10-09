#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <elf.h>

#include "libloader.h"
#include "perror.h"
#include "recursive2.h"

/*
void md5(const char* filename) {
	int bytes;
  	unsigned char c[MD5_DIGEST_LENGTH];
	unsigned char data[CHUNK_SIZE];
	FILE *fp = fopen(filename, "rb");
    	MD5_CTX mdContext;
	
	if (fp == NULL) {
		print_error("opening file handler");
		return;
	}

  	MD5_Init (&mdContext);
	while ((bytes = fread(data, 1, CHUNK_SIZE, fp)) != 0) {
		MD5_Update(&mdContext, data, bytes);
	}

	MD5_Final(c, &mdContext);
	
	for(int i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", c[i]);
    	printf (" %s\n", filename);
    	fclose (fp);
    	return;
} */

void set_modified_time() {
	struct stat file_stat;
    	if (stat(lib_dir, &file_stat) != 0) {
		print_error("loading modified stat");
   	}
    	last_modified = file_stat.st_mtime;
}

int is_modified() {
	struct stat file_stat;
    	if (stat(lib_dir, &file_stat) != 0) {
		print_error("loading modified stat");
   	}
    	return (file_stat.st_mtime > last_modified);	
}

void config() {
	modules = 0;
	getcwd(shell_dir, sizeof(shell_dir));
	strcpy(lib_dir, shell_dir);
	strcat(lib_dir, "/libs");	
}

void reset_flags() {
	only_name = 0;
	show_hidden = 1;
	is_recursive = 0;
}

int get_name_from_path(const char* path) {
	int lastindex = 0;
	for (int i = 0; i < strlen(path); i++) {
		if (path[i]=='/') lastindex = (i+1);
	}

	return lastindex;
}

int get_extension_from_path(const char* path) {

	int lastindex = 0;

	int size = strlen(path);

	for (int i = 0; i < size; i++) {
		if (path[i]=='.') lastindex = (i+1);
	}

	char tmp[3];
	strncpy(tmp, path+lastindex, 3);
	if (strcmp(tmp, "so")) {
		return -1;
	}
	
	return lastindex;
}

void search_new_libs() {
	recursive(&add_lib, lib_dir, 0);
	set_modified_time();
}

void add_lib(const char* libpath) {
	char tmp[256];
	int to = get_extension_from_path(libpath);
	if (to==-1) {return;}

	int from = get_name_from_path(libpath);

	strncpy(tmp, libpath+from, strlen(libpath)-from);

	
	for (int i = 0; i < modules; i++) {
		if (!strcmp(lib[i].id, tmp)) {
			return;		
		}
	}
	load_lib(libpath);
}

void load_lib(const char* libpath) {

	int to = get_extension_from_path(libpath);
	if (to==-1) {return;}

	int from = get_name_from_path(libpath);

	strncpy(lib[modules].id, libpath+from, strlen(libpath)-from);

	//strncpy(lib[modules].id, id, strlen(name)-3);

	lib[modules].handle = dlopen(libpath, RTLD_LAZY);
	if (!lib[modules].handle) {  fprintf(stderr, "Error: %s\n", dlerror()); return;}
	lib[modules].function = dlsym(lib[modules].handle, "__start");
	if (!lib[modules].function) { fprintf(stderr, "Error: %s\n", dlerror()); return;}
	
	modules++;
	return;
}

int unload_lib() {
	dlclose(lib[modules].handle);
	//lib[modules].id = NULL;
	lib[modules].function = NULL;
	lib[modules].handle = NULL;
	return (!lib[modules].handle);
}

void unload_all_libs() {
	while (modules > 0) {
		modules--;
		unload_lib(); 
	}
}

void load_all_libs() {
	unload_all_libs();
	recursive(&load_lib, lib_dir, 0);
	set_modified_time();
}
