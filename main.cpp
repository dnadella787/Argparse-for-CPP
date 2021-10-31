#include "arg.h"
#include "parser.h"

int main(int argc, char **argv) 
{
	argument a("output");
	a.set_flags("-v", "-verbose");
	a.set_help_message("generate full output");

	argument b("encrypt");
	b.set_flags("-e", "--encrypt");

	parser p;
	p.add_arguments(a,b);
	p.set_description("this program does many many important things");
	p.parse_args(argc, argv);
}


