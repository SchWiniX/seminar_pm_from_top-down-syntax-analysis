#include "../src/function_handler.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	char* t1 = "A | B|C |d | (a+b)";
	rule* r = create_rule('X', t1);
	printf("%c -> ", r->name);

	for(int i = 0; i < r->singleton_count; i++) {
		printf("| %c ", r->singeltons[i]);
	}
	printf("| %s\n", r->str);
	return 0;
}
