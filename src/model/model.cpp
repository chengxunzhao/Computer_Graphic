#include "model.h"
#include "log.h"
#include <sstream>
Model::Model(std::string filename)
{
    Logger::Instance().set_log_level(LogLevel::INFO);
    std::ifstream input;
    input.open(filename, std::ifstream::in);
    if (!input.is_open())
    {
        LOGE("open file failed, filename : %s", filename);
        return;
    }
    if (input.fail())
    {
        LOGE("stream is error");
        return;
    }
    std::string line;
    while (std::getline(input, line))
    {
        std::istringstream iss(line);
        char trash;
        if (line.compare(0, 2, "v ") == 0)
        {
           iss >> trash;
           
           vertex vet;
           iss >> vet.x >> vet.y >> vet.z;
           vertexs.push_back(vet);
        }
        else if (line.compare(0, 2, "f ") == 0)
        {
            iss >> trash;
            fragment fragm;
            std::string a, b, c;
            iss >> a >> b >> c;
            fragm.a = std::stoi(a.substr(0, a.find("/")));
            fragm.b = std::stoi(b.substr(0, b.find("/")));
            fragm.c = std::stoi(c.substr(0, c.find("/")));
            fragments_index.push_back(fragm);
        }
    }
    LOGI("vertex_num is {}", vertexs.size());
    LOGI("fragment num is {}", fragments_index.size());
}

int Model::fragment_size() const
{
    return fragments_index.size();
}

int Model::vertex_size() const
{
    return vertexs.size();
}

vertex Model::vetx(const int i) const
{
    return vertexs[i];
}

vertex Model::fragment_vert(const int i_fragment, const int i_index)
{
    return vertexs[fragments_index[i_fragment][i_index]];
}
