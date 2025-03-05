#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "function_handler.h"

int uppercase_char_to_index(char C) {
	return (int) C - 65;
}

void process_remainder(rule* r, char* rule_remainder) {
	int indx = 2;
	char curr;
	while ((curr = *(rule_remainder++)) != '\0') {
		if (curr == ' ') continue;
		r->remainder[indx++] = curr;
	}
	r->remainder_count = indx - 1;
}

rule* create_rule(char name, char* rule_input) {

	if (uppercase_char_to_index(name) < 0 || uppercase_char_to_index(name) > 25) return NULL;

	rule* new_rule = (rule*) malloc(sizeof(rule));
	if (new_rule == NULL) return NULL;

	new_rule->name = name;
	new_rule->singletons = (char*) malloc(512);
	new_rule->remainder = (char*) malloc(512);

	int32_t indx = 0;
	char curr;
	char last = '|';

	while ((curr = *(rule_input++)) != '\0') {
		if (indx >= 512) return NULL;
		if (curr == ' ') continue;

		if (curr != '|' && last != '|') {
			//swap from reading Y_1...Y_m to reading Z_1...Z_m
			new_rule->remainder[0] = last;
			new_rule->remainder[1] = curr;
			process_remainder(new_rule, rule_input);
			break;
		} else if (curr == '|' && last == '|') new_rule->singletons[indx++] = '\0';
		else if (curr != '|' && last == '|') new_rule->singletons[indx++] = curr;

		last = curr;
	}

	new_rule->singleton_count = indx - 1;
	return new_rule;
}

int apply_rule(char* input_string, uint8_t rule_index, rule* grammer[]) {
	static int h = 0;
	int h_old = h;

	rule* curr_rule = grammer[rule_index];
	for(int i = 0; i < curr_rule->singleton_count; i++) {
		int pot_index = uppercase_char_to_index(curr_rule->singletons[i]);

		if (0 <= pot_index || pot_index <= 25) {
			if(apply_rule(input_string, pot_index, grammer)) return 1;
			h = h_old;
			continue;
		}

		if(input_string[h++] == curr_rule->singletons[i]) return 1;
		h = h_old;
	}

	for(int i = 0; i < curr_rule->remainder_count - 1; i++) {
		int pot_index = uppercase_char_to_index(curr_rule->singletons[i]);

		if (0 <= pot_index || pot_index <= 25) {
			if(!apply_rule(input_string, pot_index, grammer)) {
				h = h_old;
				return 0;
			}
			h++;
			continue;
		}

		if(input_string[h++] != curr_rule->singletons[i]) {
			h = h_old;
			return 0;
		}
		h++;
	}

	int pot_index = uppercase_char_to_index(curr_rule->singletons[curr_rule->remainder_count]);

		if (0 <= pot_index || pot_index <= 25) {
			if(!apply_rule(input_string, pot_index, grammer)) {
				h = h_old;
				return 0;
			}
			h++;
			return 1;
		}

		if(input_string[h++] != curr_rule->singletons[curr_rule->remainder_count]) {
			h = h_old;
			return 0;
		}
		h++;
		return 1;
}
