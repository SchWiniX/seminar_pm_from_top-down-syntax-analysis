#include "../src/function_handler.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	char* t1 = "R | (B)";
	char* t2 = "E=E";
	char* t3 = "a | b | (E+E)";
	rule* r[26];
	for(int i = 0; i < 26; i++) r[i] = NULL;
	printf(
			"B: %d, R: %d, E: %d\n",
			uppercase_char_to_index('B'),
			uppercase_char_to_index('R'),
			uppercase_char_to_index('E')
		);
	r[uppercase_char_to_index('B')] = create_rule('B', t1);
	r[uppercase_char_to_index('R')] = create_rule('R', t2);
	r[uppercase_char_to_index('E')] = create_rule('E', t3);

	for(int i = 0; i < r[uppercase_char_to_index('R')]->remainder_count; i++)
		printf("'%c'\n", r[uppercase_char_to_index('R')]->remainder[i]);

	for(int j = 0; j < 26; j++) {
		if (r[j] == NULL) continue;
		printf("%c -> ", r[j]->name);

		if (r[j]->singleton_count != 0)
			printf("%c ", r[j]->singletons[0]);
		for(int i = 1; i < r[j]->singleton_count; i++) {
			printf("| %c ", r[j]->singletons[i]);
		}
		if (r[j]->singleton_count != 0) printf("| %s\n", r[j]->remainder);
		else printf("%s\n", r[j]->remainder);
		
	}

	printf("%d\n", apply_rule("(a=(b+a))", 'B', r));
	return 0;
}
