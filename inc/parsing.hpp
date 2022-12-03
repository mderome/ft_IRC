# ifndef PARSING_HPP
# define PARSING_HPP

# include <string>
# include <vector>
# include <iostream>
# include <sstream>
# include <stdlib.h>

class parsing
{
    public :

        parsing();
        ~parsing();


        //validty name of user and channel
        bool    valid_server_name(std::string server_name);
        bool    valid_user_name(std::string user_name);
        bool    valid_channel_name(std::string channel_name);
        bool    is_local(std::string server_name);
        bool    is_regular(std::string server_name);

        //parsing command
        

};

# endif
