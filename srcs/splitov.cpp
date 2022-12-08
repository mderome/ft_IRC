//devine avec le nom

#include "../inc/utils.hpp"

std::vector<std::string>    splitov(std::string str, char delim)
{
    std::vector<std::string>    res;
    std::string                 tmp;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == delim) {
            res.push_back(tmp);
            tmp.clear();
        } else
            tmp += str[i];
    }
    res.push_back(tmp);
    return res;
}