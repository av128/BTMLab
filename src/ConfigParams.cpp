#include "ConfigParams.hpp"
#include <fstream>
#include <iostream>

using namespace std;

void fixLine(string& line, string cc)
{
    const auto first = line.find_first_not_of(cc);
    const auto last = line.find_last_not_of(cc);

    line = line.substr(first, last - first + 1);
}

Param::Param(string line)
{
    fixLine(line, "-");
    fixLine(line, " ");
    const auto first = line.find_first_of(":");
    name = line.substr(0, first);
    val = line.substr(first + 2);
}

void Param::print() const
{
    cout << "Param: " << name << " " << val << "\n";
}

Param ModuleParams::getParam(string name) const
{
    for (auto& param : params)
        if (param.name == name) return param;

    std::runtime_error("Module: " + name + " Not Found");
}

void ModuleParams::print() const
{
    cout << "Module: " << name << "\n";
    for (auto& param : params)
        param.print();
}

ConfigParams::ConfigParams(string filename)
{
    vector<string> lines;
    ifstream myfile(filename.c_str());
    if (myfile.is_open())
    {
        string line;
        while (getline(myfile, line))
        {
            fixLine(line, " ");
            if (!line.empty() && line[0] != '#') lines.push_back(line);
        }
        myfile.close();
    }
    else
        std::runtime_error("Unable to open file: " + filename);

    size_t i = 0;
    while (i < lines.size())
    {
        auto line = lines[i];
        if (line[0] != '-' && line.back() == ':')
        {
            ModuleParams mp;
            mp.name = line;
            mp.name.pop_back();
            size_t j = i + 1;
            while (j < lines.size())
            {
                auto paramLine = lines[j];
                if (paramLine[0] != '-')
                {
                    break;
                }
                else
                {
                    mp.params.push_back(Param(paramLine));
                }
                j++;
            }
            modules.push_back(mp);
            i = j;
        }
    }
}

bool ConfigParams::isModuleMember(string name) const
{
    for (auto& module : modules)
        if (module.name == name) return true;

    return false;
}

ModuleParams ConfigParams::getModuleParams(string name) const
{
    for (auto& module : modules)
        if (module.name == name) return module;

    std::runtime_error("Module: " + name + " Not Found");
}

void ConfigParams::print() const
{
    for (auto& module : modules)
        module.print();
}