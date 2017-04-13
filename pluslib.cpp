#include "main.hpp"
#include "pluslib.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#if (defined(ENGINE_OS) && ENGINE_OS == 1)
    #include <direct.h>
#endif

#include <wx/wx.h>

#include <iomanip>
#define F_PRECISION 9

uint8_t PlusLib::log_level;
uint64_t log_startup_timestamp;

std::string PlusLib::int2hex(int i)
{
    std::stringstream o;
    o << std::hex << i;
    return o.str();
}

int PlusLib::hex2int(std::string i)
{
    std::stringstream c(i);
    int o;
    c >> std::hex >> o;
    return o;
}

bool PlusLib::str2bool(std::string i)
{
    std::stringstream c(i);
    bool o;
    c >> o;
    return o;
}

short PlusLib::str2short(std::string i)
{
    std::stringstream c(i);
    short o;
    c >> o;
    return o;
}

int PlusLib::str2int(std::string i)
{
    std::stringstream c(i);
    int o;
    c >> o;
    return o;
}

long PlusLib::str2long(std::string i)
{
    std::stringstream c(i);
    long o;
    c >> o;
    return o;
}

float PlusLib::str2float(std::string i)
{
    std::stringstream c(i);
    float o;
    c >> o;
    return o;
}

double PlusLib::str2double(std::string i)
{
    std::stringstream c(i);
    double o;
    c >> o;
    return o;
}

bool PlusLib::str2bool(std::wstring i)
{
    std::wstringstream c(i);
    bool o;
    c >> o;
    return o;
}

short PlusLib::str2short(std::wstring i)
{
    std::wstringstream c(i);
    short o;
    c >> o;
    return o;
}

int PlusLib::str2int(std::wstring i)
{
    std::wstringstream c(i);
    int o;
    c >> o;
    return o;
}

long PlusLib::str2long(std::wstring i)
{
    std::wstringstream c(i);
    long o;
    c >> o;
    return o;
}

float PlusLib::str2float(std::wstring i)
{
    std::wstringstream c(i);
    float o;
    c >> o;
    return o;
}

double PlusLib::str2double(std::wstring i)
{
    std::wstringstream c(i);
    double o;
    c >> o;
    return o;
}

std::string PlusLib::bool2str(bool i)
{
    std::stringstream o;
    o << i;
    return o.str();
}

std::string PlusLib::short2str(short i)
{
    std::stringstream o;
    o << i;
    return o.str();
}

std::string PlusLib::int2str(int i)
{
    std::stringstream o;
    o << i;
    return o.str();
}

std::string PlusLib::long2str(long i)
{
    std::stringstream o;
    o << i;
    return o.str();
}

std::string PlusLib::float2str(float i)
{
    std::stringstream o;
    o << std::setprecision(F_PRECISION) << i;
    return o.str();
}

std::string PlusLib::double2str(double i)
{
    std::stringstream o;
    o << std::setprecision(F_PRECISION) << i;
    return o.str();
}

void PlusLib::init_log()
{
    log_level = 2; // 2 = all, 1 = disk only, 0 = none
    log_startup_timestamp = time(NULL);
}

void PlusLib::write_log( const std::string &text, bool timestamp)
{
    std::ofstream log_file(
        LOGFILE_NAME, std::ios_base::out | std::ios_base::app );
    if(timestamp) log_file << (uint64_t)(time(NULL) - log_startup_timestamp) << ": " << text;
    else log_file << text;
}

void PlusLib::write_console( const std::string &text, bool timestamp)
{
    if(timestamp) std::cout << (uint64_t)(time(NULL) - log_startup_timestamp) << ": " << text;
    else std::cout << text;
}

void PlusLib::write_all( const std::string &text, bool timestamp)
{
    if(log_level > 0)
    {
        write_log(text, timestamp);
        if(log_level > 1)
        {
            write_console(text, timestamp);
        }
    }
}

uint64_t PlusLib::getStartupTimestamp()
{
    return log_startup_timestamp;
}

void PlusLib::clear_log()
{
    std::ofstream log_file(
        LOGFILE_NAME, std::ios_base::out | std::ios_base::trunc);
}

std::vector<std::string> &PlusLib::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> PlusLib::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

bool PlusLib::is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    if(s[0] == '-') ++it; // nevative number
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool PlusLib::is_number_f(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    if(s[0] == '-') ++it; // nevative number
    bool dot = false;
    while (it != s.end() && (std::isdigit(*it) || *it == '.' || *it == ','))
    {
        if(*it == '.' || *it == ',')
        {
            if(dot) return false;
            dot = true;
        }
        ++it;
    }
    return !s.empty() && it == s.end();
}

std::string PlusLib::replaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

std::istream& PlusLib::getline(std::istream& stream, std::string& str)
{
    std::getline(stream, str);
    if(str.length() > 0 && str[str.length()-1] == '\r')
    {
        str.erase(str.length()-1);
    }
    return stream;
}

bool PlusLib::checkIfDirectoryExist(std::string path)
{
    struct stat info;

    if( stat( path.c_str(), &info ) != 0 )
    {
        write_all("PlusLib: cannot access path:" + path + "\n");
        return false;
    }
    else if( info.st_mode & S_IFDIR )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PlusLib::makeDirectory(std::string path)
{
    std::vector<std::string> elems = PlusLib::split(path, '/');
    std::string current = "";
    for(int i = 0; i < (int)elems.size(); i++)
    {
        if(!checkIfDirectoryExist(current + elems[i]))
        {
            std::string str = current + elems[i];
            #if (defined(ENGINE_OS) && ENGINE_OS == 1)
                if(mkdir(str.c_str()) != -1) write_all("PlusLib: " + str + " created\n");
                else write_all("PlusLib: cannot create " + str + ", error\n");
            #elif (defined(ENGINE_OS) && (ENGINE_OS == 2 || ENGINE_OS == 3))
                if(mkdir(str.c_str(), 0755) != -1) PlusLib::write_all("PlusLib: " + str + " created\n");
                else PlusLib::write_all("PlusLib: cannot create " + str + ", error\n");
            #else
                PlusLib::write_all("PlusLib: cannot make directory, unknown OS\n");
            #endif
        }
        current += elems[i];
        current += '/';
    }
}

void PlusLib::deleteDirectory(std::string path)
{
    #if (defined(ENGINE_OS) && ENGINE_OS == 1)
        system(std::string("rmdir \"" + path + "\" /s /q" ).c_str());
        PlusLib::write_all(path + " deleted\n");
    #elif (defined(ENGINE_OS) && (ENGINE_OS == 2 || ENGINE_OS == 3))
        PlusLib::write_all("DISABLED\n");
    #else
        PlusLib::write_all("DISABLED\n");
    #endif
}

bool PlusLib::checkIfFileExist(std::string path)
{
    struct stat buffer;
    return (stat (path.c_str(), &buffer) == 0);
}

bool PlusLib::copyFile(std::string src, std::string dst)
{
    std::ifstream source(src.c_str(), std::ios::binary);
    std::ofstream dest(dst.c_str(), std::ios::binary);

    if(!source || !dest) return false;

    dest << source.rdbuf();

    source.close();
    dest.close();
    return true;
}

bool PlusLib::validate32(std::string str, uint32_t &result, uint32_t min, uint32_t max, std::string name)
{
    if(is_number(str))
    {
        int64_t tmp = str2int(str);
        if(tmp >= min && tmp <= max)
        {
            result = tmp % 0x100000000;
            return true;
        }
        else wxMessageBox(wxString(std::string("Error : " + name + " range is " + PlusLib::int2str(min) + "-" + PlusLib::int2str(max)).c_str(), wxConvUTF8));
    }
    else wxMessageBox(wxString(std::string("Error : " + name + " is not a number").c_str(), wxConvUTF8));
    return false;
}

bool PlusLib::validate16(std::string str, uint16_t &result, uint32_t min, uint32_t max, std::string name)
{
    if(is_number(str))
    {
        int64_t tmp = str2int(str);
        if(tmp >= min && tmp <= max)
        {
            result = tmp % 0x10000;
            return true;
        }
        else wxMessageBox(wxString(std::string("Error : " + name + " range is " + PlusLib::int2str(min) + "-" + PlusLib::int2str(max)).c_str(), wxConvUTF8));
    }
    else wxMessageBox(wxString(std::string("Error : " + name + " is not a number").c_str(), wxConvUTF8));
    return false;
}

bool PlusLib::validate8(std::string str, uint8_t &result, uint32_t min, uint32_t max, std::string name)
{
    if(is_number(str))
    {
        int64_t tmp = str2int(str);
        if(tmp >= min && tmp <= max)
        {
            result = tmp % 0x100;
            return true;
        }
        else wxMessageBox(wxString(std::string("Error : " + name + " range is " + PlusLib::int2str(min) + "-" + PlusLib::int2str(max)).c_str(), wxConvUTF8));
    }
    else wxMessageBox(wxString(std::string("Error : " + name + " is not a number").c_str(), wxConvUTF8));
    return false;
}
