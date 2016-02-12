
class command_line_parser
{
public:
    class abstract_option
    {

    };

    template <typename T>
    class option
    : public abstract_option
    {
    public:
        T const& get();
    };

    enum class type
    {
        optional,
        required
    };

    command_line_parser();

    template <typename T>
    std::shared_ptr<option<T> const> option(type option_type, char short_name, std::string long_name, std::string description);

    bool run(int argn, char const* argv[]);

private:

    std::shared_ptr<abstract_option> find_short(char short_name);
    std::shared_ptr<abstract_option> find_long(std::string const& long_name);

};
