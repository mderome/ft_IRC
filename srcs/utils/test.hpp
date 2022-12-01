#ifndef TEST_HPP
# define TEST_HPP

# include "parsing.hpp"

class test : public parsing
{
    private :
        std::string name;

    public :
        test() : parsing() {};
        test(std::string name) : name(name) {};
        ~test() {};
        void    set_name(std::string name) { this->name = name; };
        std::string get_name() { return (this->name); };

        bool    valid_server_name(std::string server_name) { return (parsing::valid_server_name(server_name)); };
        bool    valid_user_name(std::string user_name) { return (parsing::valid_user_name(user_name)); };
        bool    valid_channel_name(std::string channel_name) { return (parsing::valid_channel_name(channel_name)); };
        bool    is_local(std::string server_name) { return (parsing::is_local(server_name)); };
        bool    is_regular(std::string server_name) { return (parsing::is_regular(server_name)); };
};

#endif
