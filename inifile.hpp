#ifndef _INIFILE_HPP_
#define _INIFILE_HPP_

class IniFile
{
    public:
        IniFile();
        ~IniFile();
        bool load(std::string name);
        bool save(std::string name);
        bool save();
        void clear();

        bool isLoaded();
        void editStrData(std::string name, std::string data, std::string category = "");
        void editBoolData(std::string name, bool data, std::string category = "");
        void editIntData(std::string name, uint32_t data, std::string category = "");
        void editFloatData(std::string name, float data, std::string category = "");

        std::string getStrData(std::string name, std::string category = "");
        bool getBoolData(std::string name, std::string category = "");
        uint32_t getIntData(std::string name, std::string category = "");
        float getFloatData(std::string name, std::string category = "");

        bool exist(std::string name, std::string category = "");
    protected:
        void init();
        std::string filename; // name of the Loaded File
        bool loaded; // keep track if the load function has been used successfuly
        std::map<std::string, std::map<std::string, std::string> > datas; // file Data
        // [category]
        // name=data
};

#endif
