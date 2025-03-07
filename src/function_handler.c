#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "function_handler.h"

int verbose = 0;
int skip = 0;

int uppercase_char_to_index(char C) {
	return (int) C - 65;
}

int tab_gen(char* buff, int reccursion_depth) {
	for (int i = 0; i < reccursion_depth * 2; i += 2) {
		buff[i] = '|';
		buff[i + 1] = ' ';
	}
	buff[reccursion_depth * 2] = '\0';
	return 0;
}

int free_rule(rule* rule_to_free) {
	if(!rule_to_free) return -1;
	free(rule_to_free->singletons);
	free(rule_to_free->remainder);
	free(rule_to_free);
	return 0;
}

void process_remainder(rule* r, char* rule_remainder, uint32_t start_from) {
	int indx = start_from;
	char curr;
	while ((curr = *(rule_remainder++)) != '\0') {
		if (curr == ' ') continue;
		r->remainder[indx++] = curr;
	}
	r->remainder_count = indx;
	r->remainder[indx] = '\0';

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

int apply_rule(char* input_string, char name_char, rule* grammer[], int depth, int reset) {
	static int h = 0;
	int h_old = h;

	if (reset) {
		h = 0;
		return 0;
	}

	char tabs[64];
	tab_gen(tabs, depth);

	uint32_t rule_index = uppercase_char_to_index(name_char);
	printf("%sCall %c (h = %d)\n", tabs, name_char, h);
	rule* curr_rule = grammer[rule_index];


	for(int i = 0; i < curr_rule->singleton_count; i++) {
		int pot_index = uppercase_char_to_index(curr_rule->singletons[i]);

		if (0 <= pot_index && pot_index <= 25) {
			if(apply_rule(input_string, curr_rule->singletons[i], grammer, depth + 1, 0)) {
				printf("%s%c returned True (h = %d)\n", tabs, name_char, h);
				return 1;
			}
			h = h_old;
			continue;
		}

		if(curr_rule->singletons[i] == '~') {
			if (verbose) printf("| %ssingletons: %c: %d matched the empty string\n", tabs, curr_rule->singletons[i], pot_index);
			printf("%s%c returned True (h = %d)\n", tabs, name_char, h);
			return 1;
		}

		if(input_string[h] == curr_rule->singletons[i]) {
			if (verbose) printf("| %ssingletons: %c: %d matched with input_string[h]: %c\n", tabs, curr_rule->singletons[i], pot_index, input_string[h]);
			printf("%s%c returned True (h = %d)\n", tabs, name_char, h);
			h++;
			return 1;
		}
		if (verbose) printf("| %ssingletons: %c: %d NOT matched with input_string[h]: %c\n", tabs, curr_rule->singletons[i], pot_index, input_string[h]);
		h = h_old;
	}


	for(int i = 0; i < curr_rule->remainder_count - 1; i++) {
		int pot_index = uppercase_char_to_index(curr_rule->remainder[i]);

		if (0 <= pot_index && pot_index <= 25) {
			if(!apply_rule(input_string, curr_rule->remainder[i], grammer, depth + 1, 0)) {
				h = h_old;
				printf("%s%c returned False (h = %d)\n", tabs, name_char, h);
				return 0;
			}
			continue;
		}

		if(input_string[h] != curr_rule->remainder[i]) {
			if (verbose) printf("| %sremainder: %c: %d NOT matched with input_string[h]: %c\n", tabs, curr_rule->remainder[i], pot_index, input_string[h]);
			printf("%s%c returned False (h = %d)\n", tabs, name_char, h);
			h = h_old;
			return 0;
		}

		if (verbose) printf("| %sremainder: %c: %d matched with input_string[h]: %c\n", tabs, curr_rule->remainder[i], pot_index, input_string[h]);
		h++;
	}


	int pot_index = uppercase_char_to_index(curr_rule->remainder[curr_rule->remainder_count - 1]);

		if (0 <= pot_index && pot_index <= 25) {
			if(!apply_rule(input_string, curr_rule->remainder[curr_rule->remainder_count - 1], grammer, depth + 1, 0)) {
				printf("%s%c returned False (h = %d)\n", tabs, name_char, h);
				h = h_old;
				return 0;
			}
			printf("%s%c returned True (h = %d)\n", tabs, name_char, h);
			return 1;
		}

		if(input_string[h] != curr_rule->remainder[curr_rule->remainder_count - 1]) {
		if (verbose) printf("| %sremainder: %c: %d NOT matched with input_string[h]: %c\n", tabs, curr_rule->remainder[curr_rule->remainder_count - 1], pot_index, input_string[h]);
			printf("%s%c returned False (h = %d)\n", tabs, name_char, h);
			h = h_old;
			return 0;
		}
		if (verbose) printf("| %sremainder: %c: %d matched with input_string[h]: %c\n", tabs, curr_rule->remainder[curr_rule->remainder_count - 1], pot_index, input_string[h]);
		printf("%s%c returned True (h = %d)\n", tabs, name_char, h);
		h++;
		return 1;
}
