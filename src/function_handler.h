#include <stdint.h>

typedef struct {
	// Entry Non-Terminal
	char name;
	// List of Terminals and non-Terminals
	char* singeltons;
	uint32_t singleton_count;
	// string containing terminals and non-terminals
	char* str;
} rule;

//expect rule input of format X -> Y_1 | Y_2 | ... | Y_n | Z_1, Z_2 ... Z_m where name = X and rule_unput = Y_1...
rule* create_rule(char name, char* rule_input);

int apply_rule(rule* current);
