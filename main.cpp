#include "args.h"
#include "parser.h"

int main() {
	argument a("name");
	a.set_flags("-v", "--verbose", "bobby", "joe");
	a.set_help_message("hello there");
	a.set_action(STORE_TRUE);
	a.set_nargs(VARIABLE);
	a.set_default("42");
	a.set_variable_type(BOOL);


	argument b;
	b.set_default("blah blah blah");

	parser p;
	p.add_arguments(a, b);
	p.print_arguments();
}

