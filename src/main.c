#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_handler.h"

extern int verbose;
extern int skip;

int setup_initial_grammer(rule* grammer[]) {
	char* B = "R | (B)";
	char* R = "E=E";
	char* E = "a | b | c | (E+E)";

	for(int i = 0; i < 26; i++) {
		free_rule(grammer[i]);
		grammer[i] = NULL;
	}
	
	grammer[uppercase_char_to_index('B')] = create_rule('B', B);
	grammer[uppercase_char_to_index('R')] = create_rule('R', R);
	grammer[uppercase_char_to_index('E')] = create_rule('E', E);

	return 0;
}

int clean_string(char* str, int offset) {
	char* str_off = str + offset;
	int len = strlen(str_off);
	int indx = 0;
	char curr;
	char cpy[len];
	while((curr = *(str_off++)) != '\0') {
		if (curr == ' ') continue;
		cpy[indx++] = curr;
	}
	cpy[indx] = '\0';
	strcpy(str_off, cpy);
	return 0;
}

int add_rule(char rule_name, char* rule_body, rule* grammer[]) {
	if (grammer[uppercase_char_to_index(rule_name)] != NULL) {
		printf("rule already exists\n");
		return 1;
	}
	grammer[uppercase_char_to_index(rule_name)] = create_rule(rule_name, rule_body);
	return 0;
}

int list_rules(rule* grammer[]) {
	for(int j = 0; j < 26; j++) {
		if (grammer[j] == NULL) continue;
		printf("%c -> ", grammer[j]->name);

		if (grammer[j]->singleton_count != 0)
			printf("%c ", grammer[j]->singletons[0]);
		for(int i = 1; i < grammer[j]->singleton_count; i++) {
			printf("| %c ", grammer[j]->singletons[i]);
		}
		if (grammer[j]->singleton_count != 0 && grammer[j]->remainder_count != 0) printf("| %s\n", grammer[j]->remainder);
		else printf("%s\n", grammer[j]->remainder);
	}
	return 0;
}

int remove_all_rules(rule* grammer[]) {
	for(int i = 0; i < 26; i++) {
		free_rule(grammer[i]);
		grammer[i] = NULL;
	}
	return 0;
}

int remove_rule(char rule_name, rule* grammer[]) {
	int indx = uppercase_char_to_index(rule_name);
	free_rule(grammer[indx]);
	grammer[indx] = NULL;
	return 0;
}

int run(char start_rule, char* input_str, rule* grammer[]) {
	int skip_temp = skip;
	int verbose_temp = verbose;
	int ret = apply_rule(input_str, start_rule, grammer, 0, 0);
	apply_rule(NULL, '\0', NULL, 0, 1);
	skip = 1;
	printf("Parsing returned: %d\n\nThe full callstack is:\n", ret);
	apply_rule(input_str, start_rule, grammer, 0, 0);
	skip = skip_temp;
	verbose = verbose_temp;
	apply_rule(NULL, '\0', NULL, 0, 1);
	return ret;
}

int main(int argc, char *argv[]) {
	char* inbuf = (char*) calloc(512, 1);
	char* commandbuf = (char*) malloc(32);
	rule** grammer = (rule**) calloc(26, sizeof(rule*));

	setup_initial_grammer(grammer);
	char start_char = 'B';

	while (1) {
		printf("DPM> ");
		scanf("%s", commandbuf);
		if (commandbuf[0] != ':') {
			printf("not a command\n");
			continue;
		} else if (commandbuf[1] == 'q') break;
		else if (commandbuf[1] == 'c' && commandbuf[2] == 'a') {
			remove_all_rules(grammer);
			continue;
		} else if (commandbuf[1] == 'c') {
			scanf("%s", inbuf);
			remove_rule(*inbuf, grammer);
			continue;
		} else if (commandbuf[1] == 'a') {
			if (scanf("%s %*s %[^\n]s\n", inbuf, inbuf+1) != 2) {
				printf("reading rule failed\n");
				continue;
			}
			clean_string(inbuf + 1, 0);
			add_rule(*inbuf, inbuf+1, grammer);
			continue;
		} else if (commandbuf[1] == 's') {
			scanf("%s", inbuf);
			start_char = *inbuf;
		} else if (commandbuf[1] == 'r') {
			scanf("%s", inbuf);
			run(start_char, inbuf, grammer);
			continue;
		} else if (commandbuf[1] == 'R') {
			setup_initial_grammer(grammer);
			continue;
		} else if (commandbuf[1] == 'l') {
			list_rules(grammer);
			continue;
		} else if (commandbuf[1] == 'v') {
			scanf("%d", &verbose);
		} else if (commandbuf[1] == 'S') {
			scanf("%d", &skip);
		}

	}

	free(commandbuf);
	remove_all_rules(grammer);
	free(grammer);
	commandbuf = NULL;
	return 0;
}
