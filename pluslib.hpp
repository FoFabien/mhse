#ifndef PLUSLIB_HPP_INCLUDED
#define PLUSLIB_HPP_INCLUDED

namespace PlusLib
{
    std::string int2hex(int i);
    int hex2int(std::string i);
    //string conversion
    bool str2bool(std::string i);
    short str2short(std::string i);
    int str2int(std::string i);
    long str2long(std::string i);
    float str2float(std::string i);
    double str2double(std::string i);

    bool str2bool(std::wstring i);
    short str2short(std::wstring i);
    int str2int(std::wstring i);
    long str2long(std::wstring i);
    float str2float(std::wstring i);
    double str2double(std::wstring i);
    //number conversion
    std::string bool2str(bool i);
    std::string short2str(short i);
    std::string int2str(int i);
    std::string long2str(long i);
    std::string float2str(float i);
    std::string double2str(double i);
    //log function
    void init_log();
    void write_log( const std::string &text, bool timestamp = true);
    void write_console( const std::string &text, bool timestamp = true);
    void write_all( const std::string &text, bool timestamp = true);
    uint64_t getStartupTimestamp();
    void clear_log();
    //string manipulation
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split(const std::string &s, char delim);

    std::string replaceString(std::string subject, const std::string& search,const std::string& replace);
    //check if number
    bool is_number(const std::string& s);
    bool is_number_f(const std::string& s);
    //stream manipulation
    std::istream& getline(std::istream& stream, std::string& str);
    int getStreamSize(std::istream &f);
    //basic filesystem function
    bool checkIfDirectoryExist(std::string path);
    void makeDirectory(std::string path);
    void deleteDirectory(std::string path);
    bool checkIfFileExist(std::string path);
    bool copyFile(std::string src, std::string dst);
    // validator
    bool validate32(std::string str, uint32_t &result, uint32_t min, uint32_t max, std::string name);
    bool validate16(std::string str, uint16_t &result, uint32_t min, uint32_t max, std::string name);
    bool validate8(std::string str, uint8_t &result, uint32_t min, uint32_t max, std::string name);

    //others
    extern uint8_t log_level;
};

#endif // PLUSLIB_HPP_INCLUDED
