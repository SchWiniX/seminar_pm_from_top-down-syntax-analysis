#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_handler.h"

void process_str_remainder(rule* r, char* rule_remainder) {
	int indx;
	char curr;
	while((curr = *(rule_remainder++)) != '\0') {

	}

}

rule* create_rule(char name, char* rule_input) {

	rule* ret = (rule*) malloc(sizeof(rule));
	if (ret == NULL) return NULL;

	ret->name = name;
	ret->singeltons = (char*) malloc(512);
	ret->str = (char*) malloc(512);

	int32_t indx = 0;
	printf("%s\n", rule_input);
	char curr;
	char last = '|';
	while((curr = *(rule_input++)) != '\0') {
		if (indx >= 512) return NULL;
		if (curr == ' ') continue;
		if (curr != '|' && last != '|') {
			//swap from reading Y_1...Y_m to reading Z_1...Z_m
			ret->str[0] = last;
			ret->str[1] = curr;
			break;
		} else if (curr == '|' && last != '|') {
			last = '|';
			continue;
		} else if (curr == '|' && last == '|') ret->singeltons[indx++] = '\0';
		else ret->singeltons[indx++] = curr;

		last = curr;
	}
	ret->singleton_count = indx - 2;
	return ret;
}

int apply_rule(rule* current) {
	return 0;
}
