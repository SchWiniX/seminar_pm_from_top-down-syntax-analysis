#include "../src/function_handler.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	//char* B = "R | (B)";
	//char* R = "E=E";
	//char* E = "a | b | c | S | M | D | (E+E)";
	//char* S = "(E-E)";
	//char* M = "(E*E)";
	//char* D = "(E/E)";
	
	char* B = "b | ~";
	char* R = "aB";
	rule* r[26];
	for(int i = 0; i < 26; i++) r[i] = NULL;
	printf(
			"B: %d, R: %d, E: %d, S: %d, M: %d, D: %d\n",
			uppercase_char_to_index('B'),
			uppercase_char_to_index('R'),
			uppercase_char_to_index('E'),
			uppercase_char_to_index('S'),
			uppercase_char_to_index('M'),
			uppercase_char_to_index('D')
		);
	r[uppercase_char_to_index('B')] = create_rule('B', B);
	r[uppercase_char_to_index('R')] = create_rule('R', R);
//r[uppercase_char_to_index('E')] = create_rule('E', E);
//r[uppercase_char_to_index('S')] = create_rule('S', S);
//r[uppercase_char_to_index('M')] = create_rule('M', M);
//r[uppercase_char_to_index('D')] = create_rule('D', D);

	for(int j = 0; j < 26; j++) {
		if (r[j] == NULL) continue;
		printf("%c -> ", r[j]->name);

		if (r[j]->singleton_count != 0)
			printf("%c ", r[j]->singletons[0]);
		for(int i = 1; i < r[j]->singleton_count; i++) {
			printf("| %c ", r[j]->singletons[i]);
		}
		if (r[j]->singleton_count != 0 && r[j]->remainder_count != 0) printf("| %s\n", r[j]->remainder);
		else printf("%s\n", r[j]->remainder);
		
	}

	char* str = "a";
	printf("calling apply function on %s with starting rule %c\n", str, 'R');
	printf("which returned: %d\n", apply_rule(str, 'R', r));
	return 0;
}
