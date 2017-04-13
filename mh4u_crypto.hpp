#ifndef MH4U_CRYPTO_H
#define MH4U_CRYPTO_H

#include <string>
#include "blowfish.h"
#include <fstream>
#include <vector>

class MH4U_Crypto
{
    public:
        MH4U_Crypto();
        ~MH4U_Crypto();
        void setRegion(int reg);
        int decrypt(std::wstring input, std::wstring output);
        int decrypt(std::wstring input, std::vector<uint8_t> &p_array);
        int encrypt(std::wstring input, std::wstring output);
        int encrypt(std::wstring output, unsigned char *array, std::streampos fsize);

        int decryptSave(std::wstring input, std::wstring output);
        int decryptSave(std::wstring input, std::vector<uint8_t> &p_array);
        int encryptSave(std::wstring input, std::wstring output);
        int encryptSave(std::wstring output, unsigned char *array, std::streampos fsize);
    protected:
        int region;
        Blowfish bf;
};

#endif // MH4U_CRYPTO_H
