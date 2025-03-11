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

extern int uppercase_char_to_index(char name_char);

int apply_rule(char* input_string, char name_char, rule* grammer[]) {
	static int h = 0;
	int h_old = h;

	uint32_t rule_index = uppercase_char_to_index(name_char);
	rule* curr_rule = grammer[rule_index];
	
	//##################################################################################################################
	
	for(int i = 0; i < curr_rule->singleton_count; i++) {
		int pot_index = uppercase_char_to_index(curr_rule->singletons[i]);

		if (0 <= pot_index && pot_index <= 25) {
			if(apply_rule(input_string, curr_rule->singletons[i], grammer)) {
				return 1;
			}
			h = h_old;
			continue;
		}

		if(curr_rule->singletons[i] == '~') {
			return 1;
		}

		if(input_string[h] == curr_rule->singletons[i]) {
			h++;
			return 1;
		}
		h = h_old;
	}

	//##################################################################################################################

	for(int i = 0; i < curr_rule->remainder_count - 1; i++) {
		int pot_index = uppercase_char_to_index(curr_rule->remainder[i]);

		if (0 <= pot_index && pot_index <= 25) {
			if(!apply_rule(input_string, curr_rule->remainder[i], grammer)) {
				h = h_old;
				return 0;
			}
			continue;
		}
		if(input_string[h] != curr_rule->remainder[i]) {
			h = h_old;
			return 0;
		}
		h++;
	}

	//##################################################################################################################


	if (curr_rule->remainder_count > 0) {
		int pot_index = uppercase_char_to_index(curr_rule->remainder[curr_rule->remainder_count - 1]);
		if (0 <= pot_index && pot_index <= 25) {
			if(!apply_rule(input_string, curr_rule->remainder[curr_rule->remainder_count - 1], grammer)) {
				h = h_old;
				return 0;
			}
		}

		if(input_string[h] != curr_rule->remainder[curr_rule->remainder_count - 1]) {
			h = h_old;
			return 0;
		}
		h++;
	}
	return 1;
}
