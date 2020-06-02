#include <string>
#include <vector>




//Base argument class
//Optional Argument Class
//Positional Argument Class


class argument{
 protected:
  std::vector<std::string> names;
};

class positional_argument : public argument {
 private:
};


class optional_argument : public argument {
 private:

  
};
