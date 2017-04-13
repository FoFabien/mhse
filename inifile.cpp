#include "main.hpp"
#include "pluslib.hpp"
#include "inifile.hpp"

IniFile::IniFile()
{
    loaded = false;
}

IniFile::~IniFile()
{

}

bool IniFile::load(std::string name)
{
    loaded = false;
    clear();
    filename = name;
    std::ifstream f;
    try {
        f.open(name.c_str(), std::ios::in);

        if(!f)
        {
            init();
            return false;
        }
        std::string tmp_str;
        std::string current_category = "";
        if(datas.find("") == datas.end()) datas[""] = std::map<std::string, std::string>();
        while(f.good())
        {
            PlusLib::getline(f, tmp_str);
            if(tmp_str.size() > 2 && tmp_str[0] == '[' && tmp_str[tmp_str.size()-1] == ']')
            {
                tmp_str = tmp_str.substr(1, tmp_str.size()-2);
                if(datas.find(tmp_str) == datas.end()) datas[tmp_str] = std::map<std::string, std::string>();
                current_category = tmp_str;
            }
            else if(tmp_str.size() > 1)
            {
                std::vector<std::string> elems;
                PlusLib::split(tmp_str, '=', elems);
                if(elems.size() == 1) datas[current_category][elems[0]] = std::string("");
                else
                {
                    for(unsigned int i = 2; i < elems.size(); i++) elems[1] = elems[1] + "=" + elems[i];
                    datas[current_category][elems[0]] = elems[1];
                }

            }
        }
        loaded = true;
        PlusLib::write_all("IniFile: " + filename + " loaded\n");
        return true;
    }
    catch (std::ifstream::failure e) {
        PlusLib::write_all("IniFile: Exception opening/reading " + name + "\n");
        PlusLib::write_all("IniFile: Explanatory string: " + std::string(e.what()) + "\n");
        clear();
        return false;
    }
}

bool IniFile::save(std::string name)
{
    std::ofstream f;
    try {
        f.open(name.c_str(), std::ios::out | std::ios::trunc);
        if(!f)
        {
            PlusLib::write_all("IniFile: Failed to open " + name + "\n");
        }
        for(std::map<std::string, std::map<std::string, std::string> >::iterator it1 = datas.begin(); it1 != datas.end(); ++it1)
        {
            if(!it1->first.empty()) f << "[" << it1->first << "]\n";
            for(std::map<std::string, std::string>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
                f << it2->first << "=" << it2->second << "\n";
            if(!it1->second.empty()) f << "\n";
        }
        return true;
    }
    catch (std::ofstream::failure e) {
        PlusLib::write_all("IniFile: Exception opening/writing " + name + "\n");
        PlusLib::write_all("IniFile: Explanatory string: " + std::string(e.what()) + "\n");
        return false;
    }
}

bool IniFile::save()
{
    return save(filename);
}

void IniFile::clear()
{
    datas.clear();
    loaded = false;
}

bool IniFile::isLoaded()
{
    return loaded;
}

void IniFile::editStrData(std::string name, std::string data, std::string category)
{
    if(datas.find(category) == datas.end()) datas[category] = std::map<std::string, std::string>();
    datas[category][name] = data;
}

void IniFile::editBoolData(std::string name, bool data, std::string category)
{
    if(datas.find(category) == datas.end()) datas[category] = std::map<std::string, std::string>();
    datas[category][name] = PlusLib::bool2str(data);
}

void IniFile::editIntData(std::string name, uint32_t data, std::string category)
{
    if(datas.find(category) == datas.end()) datas[category] = std::map<std::string, std::string>();
    datas[category][name] = PlusLib::int2str(data);
}

void IniFile::editFloatData(std::string name, float data, std::string category)
{
    if(datas.find(category) == datas.end()) datas[category] = std::map<std::string, std::string>();
    datas[category][name] = PlusLib::float2str(data);
}

std::string IniFile::getStrData(std::string name, std::string category)
{
    if(datas.find(category) == datas.end() || datas[category].find(name) == datas[category].end())
    {
        //PlusLib::write_all("IniFile: Cannot find entry " + name + " in category [" + category + "]\n");
        return std::string("");
    }
    return datas[category][name];
}

bool IniFile::getBoolData(std::string name, std::string category)
{
    if(datas.find(category) == datas.end() || datas[category].find(name) == datas[category].end())
    {
        //PlusLib::write_all("IniFile: Cannot find entry " + name + " in category [" + category + "]\n");
        return false;
    }
    return PlusLib::str2bool(datas[category][name]);
}

uint32_t IniFile::getIntData(std::string name, std::string category)
{
    if(datas.find(category) == datas.end() || datas[category].find(name) == datas[category].end())
    {
        //PlusLib::write_all("IniFile: Cannot find entry " + name + " in category [" + category + "]\n");
        return 0;
    }
    return PlusLib::str2int(datas[category][name]);
}

float IniFile::getFloatData(std::string name, std::string category)
{
    if(datas.find(category) == datas.end() || datas[category].find(name) == datas[category].end())
    {
        //PlusLib::write_all("IniFile: Cannot find entry " + name + " in category [" + category + "]\n");
        return 0;
    }
    return PlusLib::str2float(datas[category][name]);
}

bool IniFile::exist(std::string name, std::string category)
{
    if(datas.find(category) == datas.end() || datas[category].find(name) == datas[category].end())
    {
        return false;
    }
    return true;
}

void IniFile::init()
{
    clear();
    loaded = true;
    filename = "config.ini";

    editStrData("ENCRYPT_REGION", "1", "GENERAL");
    editStrData("TOOL_REGION", "1", "GENERAL");
    editStrData("TOOL_PROXY_REGION", "1", "GENERAL");
    editStrData("TOOL_QUEST_FOLDER", "", "GENERAL");
    editStrData("ENCRYPT_PALICO_REGION", "1", "GENERAL");
    editStrData("CREATE_REGION", "1", "GENERAL");
    editStrData("DEV_PASSWORD", "", "GENERAL");

    save();
}
