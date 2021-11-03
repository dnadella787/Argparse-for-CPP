#include "arg.h"
#include "parser.h"

int main(int argc, char **argv) 
{
	argument a("OUTPUT");
	a.set_flags("-v", "--verbose");
	a.set_help_message("generate full output");
	a.set_action(STORE);

	argument b("ENCRYPT");
	b.set_flags("-e", "--encrypt");
	b.set_action(STORE_TRUE);

	argument c("DOTTED");
	c.set_flags("-d", "--dotted");
	c.set_action(APPEND);
	c.set_nargs(3);

	argument d("ELROND");
	d.set_flags("-lord", "--lrond");
	d.set_action(COUNT);

	parser p;
	p.add_arguments(&a,&b,&c,&d);
	p.set_description("this program does many many important things");
	p.parse_args(argc, argv);

	std::cout << a.get_val() << std::endl;
	std::cout << b.get_val() << std::endl;
	std::cout << "vector: " << std::endl;
	for (std::string s : c.get_vals())
	{
		std::cout << s << std::endl;
	}
	std::cout << d.get_val() << std::endl;
}


