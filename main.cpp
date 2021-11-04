#include "arg.h"
#include "parser.h"

int main(int argc, char **argv) 
{
	argument a("FIRST ARGUMENET");
	a.set_flags("-a", "--austin");
	a.set_help_message("generate full output");
	a.set_action(STORE);

	argument b("SECOND ARGUMENT");
	b.set_flags("-b", "--book");
	b.set_action(STORE_TRUE);

	argument c("THIRD ARGUMENT");
	c.set_flags("-c", "--cutlet");
	c.set_action(STORE_FALSE);

	argument d("FOURTH ARGUMENT");
	d.set_flags("-d", "--dracula");
	d.set_action(COUNT);

	argument e("FIFTH ARGUMENT");
	e.set_flags("-e", "--epsilon");
	e.set_action(APPEND);
	e.set_nargs(3);


	parser p;
	p.add_arguments(&a,&b,&c,&d, &e);
	p.set_description("this program does many many important things");
	p.parse_args(argc, argv);

	std::cout << "FIRST ARGUMENT : " << a.get_store() << std::endl;
	std::cout << "SECOND ARGUMENT : " << b.get_store_tf() << std::endl;
	std::cout << "THIRD ARUGMENT : " << c.get_store_tf() << std::endl;
	std::cout << "FOURTH ARGUMENT : " << d.get_count() << std::endl;
	if (!e.is_empty())
	{
		std::cout << "FIFTH ARGUMENT: " << d.get_count() << std::endl;
		for (std::string s : e.get_append())
		{
			std::cout << s << std::endl;
		}
	}
	else 
	{
		std::cout << "FIFTH ARGUMENT is empty" << std::endl;
	}
}


