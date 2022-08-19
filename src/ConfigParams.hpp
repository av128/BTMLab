#pragma once

#include <string>
#include <vector>

struct Param
{
    Param(std::string line);

    double valAsDouble() const { return std::stod(val); }
    double valAsInt() const { return std::stoi(val); }
    bool valAsBool() const { return val == "true"; }

    std::string name, val;
    void print() const;
};

struct ModuleParams
{
    std::string name;
    std::vector<Param> params;

    Param getParam(std::string name) const;
    void print() const;
};

struct ConfigParams
{
    std::vector<ModuleParams> modules;

    ConfigParams(std::string filename);

    bool isModuleMember(std::string name) const;

    ModuleParams getModuleParams(std::string name) const;
    void print() const;
};