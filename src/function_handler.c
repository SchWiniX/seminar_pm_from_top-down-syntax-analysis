#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "function_handler.h"

int uppercase_char_to_index(char C) {
	return (int) C - 65;
}

void process_remainder(rule* r, char* rule_remainder, uint32_t start_from) {
	int indx = start_from;
	char curr;
	while ((curr = *(rule_remainder++)) != '\0') {
		if (curr == ' ') continue;
		r->remainder[indx++] = curr;
	}
	r->remainder_count = indx;
}

rule* create_rule(char name, char* rule_input) {

	if (uppercase_char_to_index(name) < 0 || uppercase_char_to_index(name) > 25) return NULL;

	rule* new_rule = (rule*) malloc(sizeof(rule));
	if (new_rule == NULL) return NULL;

	new_rule->name = name;
	new_rule->singletons = (char*) malloc(512);
	new_rule->remainder = (char*) malloc(512);
	new_rule->remainder[0] = '\0';
	new_rule->remainder_count = 0;
	new_rule->singleton_count = 0;


	int32_t indx = -1;
	char curr;
	char last = '|';

	char* rule_input_2 = rule_input;
	while ((curr = *(rule_input_2++)) != '\0') {
		if (curr == '|') {
			indx = 0;
			break;
		}
	}
	if (indx == -1) {
		process_remainder(new_rule, rule_input, 0);
		return new_rule;
	}

	while ((curr = *(rule_input++)) != '\0') {
		if (indx >= 512) return NULL;
		if (curr == ' ') continue;

		if (curr != '|' && last != '|') {
			//swap from reading Y_1...Y_m to reading Z_1...Z_m
			new_rule->remainder[0] = last;
			new_rule->remainder[1] = curr;
			process_remainder(new_rule, rule_input, 2);
			break;
		} else if (curr != '|' && last == '|') new_rule->singletons[indx++] = curr;

		last = curr;
	}
	if (new_rule->remainder[0] == '\0')	{
		new_rule->singletons[indx++] = *(rule_input - 2);
	}
	new_rule->singleton_count = indx - 1;
	return new_rule;
}

int apply_rule(char* input_string, char name_char, rule* grammer[]) {
	uint32_t rule_index = uppercase_char_to_index(name_char);
	static int h = 0;
	int h_old = h;

	printf("Call %c (h = %d)\n", name_char, h);
	rule* curr_rule = grammer[rule_index];


	for(int i = 0; i < curr_rule->singleton_count; i++) {
		int pot_index = uppercase_char_to_index(curr_rule->singletons[i]);

		if (0 <= pot_index && pot_index <= 25) {
			if(apply_rule(input_string, curr_rule->singletons[i], grammer)) {
				printf("%c returned True (h = %d)\n", name_char, h);
				return 1;
			}
			h = h_old;
			continue;
		}

		if(curr_rule->singletons[i] == '~') {
			printf("singletons: %c: %d matched the empty string\n", curr_rule->singletons[i], pot_index);
			printf("%c returned True (h = %d)\n", name_char, h);
			return 1;
		}

		if(input_string[h] == curr_rule->singletons[i]) {
			printf("singletons: %c: %d matched with input_string[h]: %c\n", curr_rule->singletons[i], pot_index, input_string[h]);
			printf("%c returned True (h = %d)\n", name_char, h);
			h++;
			return 1;
		}
		printf("singletons: %c: %d NOT matched with input_string[h]: %c\n", curr_rule->singletons[i], pot_index, input_string[h]);
		h = h_old;
	}


	for(int i = 0; i < curr_rule->remainder_count - 1; i++) {
		int pot_index = uppercase_char_to_index(curr_rule->remainder[i]);

		if (0 <= pot_index && pot_index <= 25) {
			if(!apply_rule(input_string, curr_rule->remainder[i], grammer)) {
				h = h_old;
				printf("%c returned False (h = %d)\n", name_char, h);
				return 0;
			}
			continue;
		}

		if(input_string[h] != curr_rule->remainder[i]) {
			printf("remainder: %c: %d NOT matched with input_string[h]: %c\n", curr_rule->remainder[i], pot_index, input_string[h]);
			printf("%c returned False (h = %d)\n", name_char, h);
			h = h_old;
			return 0;
		}

		printf("remainder: %c: %d matched with input_string[h]: %c\n", curr_rule->remainder[i], pot_index, input_string[h]);
		h++;
	}


	int pot_index = uppercase_char_to_index(curr_rule->remainder[curr_rule->remainder_count - 1]);

		if (0 <= pot_index && pot_index <= 25) {
			if(!apply_rule(input_string, curr_rule->remainder[curr_rule->remainder_count - 1], grammer)) {
				printf("%c returned False (h = %d)\n", name_char, h);
				h = h_old;
				return 0;
			}
			return 1;
		}

		if(input_string[h] != curr_rule->remainder[curr_rule->remainder_count - 1]) {
		printf("remainder: %c: %d NOT matched with input_string[h]: %c\n", curr_rule->remainder[curr_rule->remainder_count - 1], pot_index, input_string[h]);
			printf("%c returned False (h = %d)\n", name_char, h);
			h = h_old;
			return 0;
		}
		printf("remainder: %c: %d matched with input_string[h]: %c\n", curr_rule->remainder[curr_rule->remainder_count - 1], pot_index, input_string[h]);
		h++;
		return 1;
}
