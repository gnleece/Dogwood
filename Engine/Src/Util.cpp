#include "Util.h"

#include <climits>
#include <ctime>
#include <fstream>
#include <ostream>
#include <sstream>

unsigned int MakeGuid(string str)
{
    // Get timestamp
    time_t timer;
    time(&timer);
    string timestamp = std::to_string(timer);

    // Generate a random number as well, in case this function was called so recently
    // that we have the same timestamp as before
    int random = rand() % INT_MAX;

    // Hash string + timestamp + random number to create unique ID
    unsigned int guid = std::hash<string>()(str + timestamp + std::to_string(random));

    return guid;
}

Vector3 ReadVector3FromString(string str)
{
    Vector3 vec;
    size_t pos = 0;
    int index = 0;
    string token;
    string delimiter = "; ";

    while ((pos = str.find(delimiter)) != std::string::npos && index < 2)
    {
        token = str.substr(0, pos);
        vec[index] = std::stof(token);
        str.erase(0, pos + delimiter.length());
        index++;
    }
    if (index == 2)
    {
        vec[index] = std::stof(str);
    }

    return vec;
}

string WriteVector3ToString(Vector3 v)
{
    std::ostringstream stream;
    stream << v.x() << "; " << v.y() << "; " << v.z();
    return stream.str();
}

void FileCopy(string source, string destination)
{
    std::ifstream src(source.c_str(), std::ios::binary);
    std::ofstream dst(destination.c_str(), std::ios::binary);

    dst << src.rdbuf();
}

string GetFriendlyAssetNameFromPath(string path)
{
    std::size_t found = path.find_last_of("/\\");
    string filename = path.substr(found + 1);

    found = filename.find_first_of(".");
    string friendlyName = filename.substr(0, found);

    return friendlyName;
}