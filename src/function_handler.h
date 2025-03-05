#include <stdint.h>

typedef struct {
	// Entry Non-Terminal
	char name;
	// List of Terminals and non-Terminals
	char* singletons;
	int32_t singleton_count;
	// string containing terminals and non-terminals
	char* remainder;
	int32_t remainder_count;
} rule;

//expect rule input of format X -> Y_1 | Y_2 | ... | Y_n | Z_1, Z_2 ... Z_m where name = X and rule_unput = Y_1...
rule* create_rule(char name, char* rule_input);

int apply_rule(char* input_string, uint8_t rule_index, rule* grammer[]);
