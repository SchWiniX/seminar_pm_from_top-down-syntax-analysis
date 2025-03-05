build:
	gcc -o builds/dpm src/main.c

clean:
	rm builds/*

test:
	gcc -o builds/create_rule_test tests/function_handler_tests.c src/function_handler.c
