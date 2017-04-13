#include <iostream>
#include "sha1.h"
#include "mh4u_crypto.hpp"


#include "main.hpp"
#include "pluslib.hpp"
#include <time.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

char japan[] = "AgK2DYheaCjyHGP8";
char others[] = "AgK2DYheaCjyHGPB";
char korea[] = "AgK2DYheaOjyHGP8";
char taiwan[] = "Capcom123 ";
char save_key[] = "blowfish key iorajegqmrna4itjeangmb agmwgtobjteowhv9mope";

void byteswap(unsigned int* array, int len)
{
    for(int i = 0; i < len; i++)
    {
        array[i] = ((array[i] << 24) & 0xFF000000)
                +  ((array[i] << 8)  & 0x00FF0000)
                +  ((array[i] >> 8)  & 0x0000FF00)
                +  ((array[i] >> 24) & 0x000000FF);
    }
}

std::vector<unsigned char> xorSave(unsigned short* ptr, int len, int key)
{
    std::vector<unsigned char> r;
    for(int i = 0; i < len; i++)
    {
        if(!key) key = 1;
        key = (key * 0xb0) % 0xff53;
        ptr[i] ^= key;
        r.push_back(ptr[i] % 0x100);
        r.push_back(ptr[i] / 0x100);
    }
    return r;
}

int checksum(unsigned char* array, int len) {
   int sum = 0;
   for(int i = 0; i < len; i++) {
      sum += array[i];
   }
   return sum;
}

bool compareHash(unsigned char* a, unsigned char* b)
{
    for(int i = 0; i < 20; i++)
    {
        if(a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}

MH4U_Crypto::MH4U_Crypto()
{
    region = false;
}

MH4U_Crypto::~MH4U_Crypto()
{
    //dtor
}

void MH4U_Crypto::setRegion(int reg)
{
    region = reg;
}

int MH4U_Crypto::decrypt(std::wstring input, std::wstring output)
{
    switch(region)
    {
        case 0: bf.Set_Passwd(japan); break;
        case 1: bf.Set_Passwd(others); break;
        case 2: bf.Set_Passwd(korea); break;
        case 3: bf.Set_Passwd(taiwan); break;
        default: return -1;
    }

    boost::filesystem::path path = input;
    boost::filesystem::ifstream f(path, std::ios::in | std::ios::binary);
    if(!f) return 1;

    std::streampos fsize = f.tellg();
    f.seekg( 0, std::ios::end );
    fsize = f.tellg() - fsize;

    f.seekg( 0, std::ios::beg );

    unsigned char *array = new unsigned char[fsize];

    for(size_t i = 0; i < fsize; i++) f.read((char*)&array[i], 1);
    f.close();

    unsigned int size = (((((array[fsize-4] << 8) + array[fsize-3]) << 8) + array[fsize-2]) <<8) + array[fsize-1];

    if(size > fsize * 4)
    {
        delete [] array;
        return 2;
    }

    bf.Decrypt(array, fsize-4);

    unsigned char md[20];
    for(size_t i = 0; i < 20; i++) md[i] = array[size-20+i];
    size -= 20;

    unsigned char tmp[20];
    sha1::calc(array, size, tmp);
    if(!compareHash(md, tmp))
    {
        delete [] array;
        return 3;
    }

    path = output;
    boost::filesystem::ofstream o(path, std::ios::out | std::ios::trunc | std::ios::binary);
    if(!o)
    {
        delete [] array;
        return 4;
    }

    o.write((char*)array, size);
    o.close();

    delete [] array;
    return 0;
}

int MH4U_Crypto::decrypt(std::wstring input, std::vector<uint8_t> &p_array)
{
    switch(region)
    {
        case 0: bf.Set_Passwd(japan); break;
        case 1: bf.Set_Passwd(others); break;
        case 2: bf.Set_Passwd(korea); break;
        case 3: bf.Set_Passwd(taiwan); break;
        default: return -1;
    }

    boost::filesystem::path path = input;
    boost::filesystem::ifstream f(path, std::ios::in | std::ios::binary);
    if(!f) return 1;

    std::streampos fsize = f.tellg();
    f.seekg( 0, std::ios::end );
    fsize = f.tellg() - fsize;

    f.seekg( 0, std::ios::beg );

    unsigned char *array = new unsigned char[fsize];

    for(size_t i = 0; i < fsize; i++) f.read((char*)&array[i], 1);
    f.close();

    unsigned int size = (((((array[fsize-4] << 8) + array[fsize-3]) << 8) + array[fsize-2]) <<8) + array[fsize-1];

    if(size > fsize * 4)
    {
        delete [] array;
        return 2;
    }

    bf.Decrypt(array, fsize-4);

    unsigned char md[20];
    for(size_t i = 0; i < 20; i++) md[i] = array[size-20+i];
    size -= 20;

    unsigned char tmp[20];
    sha1::calc(array, size, tmp);
    if(!compareHash(md, tmp))
    {
        delete [] array;
        return 3;
    }


    p_array.clear();
    for(size_t i = 0; i < size; i++) p_array.push_back(array[i]);

    delete [] array;
    return 0;
}

int MH4U_Crypto::encrypt(std::wstring input, std::wstring output)
{
    switch(region)
    {
        case 0: bf.Set_Passwd(japan); break;
        case 1: bf.Set_Passwd(others); break;
        case 2: bf.Set_Passwd(korea); break;
        case 3: bf.Set_Passwd(taiwan); break;
        default: return -1;
    }

    boost::filesystem::path path = input;
    boost::filesystem::ifstream f(path, std::ios::in | std::ios::binary);
    if(!f) return 1;

    std::streampos fsize = f.tellg();
    f.seekg( 0, std::ios::end );
    fsize = f.tellg() - fsize;

    unsigned char *array = new unsigned char[fsize+20+(8-((fsize + 20) % 8))];

    f.seekg(0, std::ios::beg);

    for(size_t i = 0; i < fsize; i++) f.read((char*)&array[i], 1);
    f.close();

    int tmp = encrypt(output, array, fsize);

    delete [] array;
    return tmp;
}

int MH4U_Crypto::encrypt(std::wstring output, unsigned char *array, std::streampos fsize)
{
    switch(region)
    {
        case 0: bf.Set_Passwd(japan); break;
        case 1: bf.Set_Passwd(others); break;
        case 2: bf.Set_Passwd(korea); break;
        case 3: bf.Set_Passwd(taiwan); break;
        default: return -1;
    }

    unsigned char tmp[20];
    sha1::calc(array, fsize, tmp);

    for(int i = 0; i < 20; i++) array[fsize+i] = tmp[i];
    fsize+=20;

    if(fsize % 8 != 0)
        for(int i = 0; i < 8 - (fsize % 8); i++) array[fsize+i] = 0x00;

    bf.Encrypt(array, fsize+(8-(fsize % 8)));

    boost::filesystem::path path = output;
    boost::filesystem::ofstream o(path, std::ios::out | std::ios::trunc | std::ios::binary);
    if(!o) return 2;

    o.write((char*)array, fsize+(8-(fsize % 8)));

    fsize = ((((((fsize % 0x100) << 8) + ((fsize % 0x10000) >> 8)) << 8) + ((fsize % 0x1000000) >> 16)) << 8) + (fsize >> 24);

    o.write((char*)&fsize, 4);
    o.close();

    return 0;
}

// SAVE ============================================

int MH4U_Crypto::decryptSave(std::wstring input, std::wstring output)
{
    boost::filesystem::path path = input;
    boost::filesystem::ifstream f(path, std::ios::in | std::ios::binary);
    if(!f) return 1;

    std::streampos fsize = f.tellg();
    f.seekg( 0, std::ios::end );
    fsize = f.tellg() - fsize;

    f.seekg( 0, std::ios::beg );

    unsigned char *array = new unsigned char[fsize];

    for(size_t i = 0; i < fsize; i++) f.read((char*)&array[i], 1);
    f.close();

    bf.Set_Passwd(save_key);
    bf.Decrypt(array, fsize);
    uint16_t seed = ((uint32_t*)array)[0] >> 16;
    std::vector<unsigned char> xor_buf = xorSave((unsigned short*)(array+4), (fsize-4)/2, seed);
    uint32_t csum = xor_buf[0] + (xor_buf[1] + (xor_buf[2] + (xor_buf[3] << 8) << 8) << 8);
    for(size_t i = 0; i < fsize-8; i++)
        array[i] = xor_buf[i+4];
    if((checksum(array, fsize-8) & 0xffffffff) != csum)
    {
        delete [] array;
        return 3;
    }

    path = output;
    boost::filesystem::ofstream o(path, std::ios::out | std::ios::trunc | std::ios::binary);
    if(!o) return 4;

    o.write((char*)array, fsize-8);
    o.close();

    delete [] array;
    return 0;
}

int MH4U_Crypto::encryptSave(std::wstring input, std::wstring output)
{
    boost::filesystem::path path = input;
    boost::filesystem::ifstream f(path, std::ios::in | std::ios::binary);
    if(!f) return 1;

    std::streampos fsize = f.tellg();
    f.seekg( 0, std::ios::end );
    fsize = f.tellg() - fsize;

    f.seekg( 0, std::ios::beg );

    unsigned char *array = new unsigned char[fsize+8];

    for(size_t i = 0; i < fsize; i++) f.read((char*)&array[i+4], 1); // let 4 bytes empty at the start
    f.close();

    int tmp = encrypt(output, array, fsize);

    delete [] array;
    return tmp;
}

int MH4U_Crypto::decryptSave(std::wstring input, std::vector<uint8_t> &p_array)
{
    boost::filesystem::path path = input;
    boost::filesystem::ifstream f(path, std::ios::in | std::ios::binary);
    if(!f) return 1;

    std::streampos fsize = f.tellg();
    f.seekg( 0, std::ios::end );
    fsize = f.tellg() - fsize;

    f.seekg( 0, std::ios::beg );

    unsigned char *array = new unsigned char[fsize];

    for(size_t i = 0; i < fsize; i++) f.read((char*)&array[i], 1);
    f.close();

    bf.Set_Passwd(save_key);
    bf.Decrypt(array, fsize);
    uint16_t seed = ((uint32_t*)array)[0] >> 16;
    std::vector<unsigned char> xor_buf = xorSave((unsigned short*)(array+4), (fsize-4)/2, seed);
    uint32_t csum = xor_buf[0] + (xor_buf[1] + (xor_buf[2] + (xor_buf[3] << 8) << 8) << 8);
    for(size_t i = 0; i < fsize-8; i++)
        array[i] = xor_buf[i+4];
    if((checksum(array, fsize-8) & 0xffffffff) != csum)
    {
        return 3;
        delete [] array;
    }

    p_array.clear();
    for(size_t i = 0; i < fsize-8; i++) p_array.push_back(array[i]);

    delete [] array;
    return 0;
}

int MH4U_Crypto::encryptSave(std::wstring output, unsigned char *array, std::streampos fsize)
{
    uint32_t csum = checksum(array+4, fsize) & 0xffffffff;
    ((uint32_t*)array)[0] = csum;

    uint16_t seed = rand() % 0x10000; // random seed
    std::vector<unsigned char> xor_buf = xorSave((unsigned short*)array, (fsize+4)/2, seed); // apply xor

    uint32_t tmp = (seed << 16) + 0x10;
    ((uint32_t*)array)[0] = tmp;
    for(size_t i = 0; i < fsize+4; i++)
        array[i+4] = xor_buf[i];
    bf.Set_Passwd(save_key);
    bf.Encrypt(array, fsize+8);

    boost::filesystem::path path = output;
    boost::filesystem::ofstream o(path, std::ios::out | std::ios::trunc | std::ios::binary);
    if(!o) return 2;

    o.write((char*)array, fsize+8);
    o.close();

    return 0;
}
