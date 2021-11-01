#include "arg.h"
#include "parser.h"

int main(int argc, char **argv) 
{
	argument a("output");
	a.set_flags("-v", "--verbose");
	a.set_help_message("generate full output");
	a.set_action(STORE);

	argument b("encrypt");
	b.set_flags("-e", "--encrypt");
	b.set_action(STORE_TRUE);

	parser p;
	p.add_arguments(a,b);
	p.set_description("this program does many many important things");
	p.parse_args(argc, argv);
}


