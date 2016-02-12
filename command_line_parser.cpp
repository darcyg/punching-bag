#include "command_line_parser.hpp"

bool command_line_parser::run(int argn, const char *argv[])
{
    std::shared_ptr<abstract_option> current;

    for (int i=1; i<argn; ++i)
    {
        std::string parameter = argv[i];
        if (parameter.empty())
            continue;

        if (parameter.front() == '-')
        {
            if (parameter.size() < 2)
                throw std::runtime_error("Parameter introducer, but no parameter given");

            if (parameter[1] == '-')
            {
                current = find_long(parameter.substr(2));
            }
            else
            {
                current = find_short(parameter)
            }
        }
    }
}
