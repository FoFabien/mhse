#include "main.hpp"
#include "pluslib.hpp"
#include "mh4u_se.hpp"
#include "mh4u_crypto.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <wx/wx.h>

MH4U_SE::MH4U_SE()
{
    isProtected = true;
    savedata = NULL;
	PlusLib::write_all("MH4U_SE:: Ready\n");
}

MH4U_SE::~MH4U_SE()
{
    cdelete(savedata);
	PlusLib::write_all("MH4U_SE:: Terminated\n");
}

void MH4U_SE::setProtection(bool bo)
{
    isProtected = bo;
}

bool MH4U_SE::getProtection()
{
    return isProtected;
}

bool MH4U_SE::load(std::wstring source, bool decrypt)
{
    boost::filesystem::ifstream fp;
    std::stringstream p;
	try
	{
	    if(!decrypt)
        {
            boost::filesystem::path path = source;
            fp.open(path, std::ios::in | std::ios::binary);
            if(!fp)
            {
                PlusLib::write_all("MH4U_SE:: Failed to open the file\n");
                return false;
            }
            char tmp_c;
            while(fp.good())
            {
                fp.read(&tmp_c, 1);
                p.write(&tmp_c, 1);
            }
            fp.close();
        }
        else
        {
            std::vector<uint8_t> p_array;
            MH4U_Crypto crypto;
            if(crypto.decryptSave(source, p_array) != 0)
            {
                PlusLib::write_all("MH4U_SE:: Failed to decrypt the file\n");
                return false;
            }
            for(size_t i = 0; i < p_array.size(); i++)
                p.write((char*)&p_array[i], 1);
            p.write((char*)&p_array[p_array.size()-1], 1);
        }
        filename = source;
        uint8_t tmp_char;
        uint32_t tmp_value;

		cdelete(savedata);
		savedata = new Save();


        p.read((char*)(savedata->name), 20);
        p.read((char*)&tmp_value, 4);
        if(tmp_value != 0)
        {
            PlusLib::write_all("MH4U_SE:: Invalid savedata\n");
            cdelete(savedata);
            if(!decrypt)
            {
                PlusLib::write_all("MH4U_SE:: Trying to decrypt...\n");
                return load(source, true);
            }
            return false;
        }

        p.seekg(0x18, p.beg);
        p.read((char*)&(savedata->sex), 1);

        p.seekg(0x15E, p.beg);
        for(uint32_t i = 0; i < 14; i++)
        {
            for(uint32_t j = 0; j < 100; j++)
            {
                p.read((char*)&(savedata->box[i][j].item_id), 2);
                p.read((char*)&(savedata->box[i][j].count), 2);
            }
        }

        p.seekg(0x173E, p.beg);
        for(uint32_t i = 0; i < 15; i++)
        {
            for(uint32_t j = 0; j < 100; j++)
            {
                p.read((char*)savedata->equipment[i][j], 28);
            }
        }

        p.seekg(0xCB14, p.beg);
        for(uint32_t i = 0; i < 32; i++)
        {
            p.read((char*)&(savedata->inventory[i].item_id), 2);
            p.read((char*)&(savedata->inventory[i].count), 2);
        }

        p.seekg(0xDCA8, p.beg);
        for(uint32_t i = 0; i < 10; i++)
        {
            p.read((char*)savedata->gq[i].creator, 20);
            p.read((char*)savedata->gq[i].unknowna, 4);
            p.read((char*)savedata->gq[i].unique_ID, 8);
            p.read((char*)savedata->gq[i].unknownb, 5);
            p.read((char*)&(savedata->gq[i].weapon_bias), 1);
            p.read((char*)&(savedata->gq[i].armor_serie), 1);
            p.read((char*)&(savedata->gq[i].armor_piece), 1);
            for(uint32_t j = 0; j < 5; j++)
            {
                p.read((char*)&(savedata->gq[i].boss[j].boss_type), 4);
                p.read((char*)&(savedata->gq[i].boss[j].spawn_quantity), 4);
                p.read((char*)&(savedata->gq[i].boss[j].spawn_condition), 1);
                p.read((char*)&(savedata->gq[i].boss[j].spawn_area), 1);
                p.read((char*)&(savedata->gq[i].boss[j].unknown0), 1);
                p.read((char*)&(savedata->gq[i].boss[j].form), 1);
                p.read((char*)&(savedata->gq[i].boss[j].size_related2), 1);
                p.read((char*)&(savedata->gq[i].boss[j].unknown1), 2);
                p.read((char*)&(savedata->gq[i].boss[j].frenzy), 1);
                p.read((char*)savedata->gq[i].boss[j].coordinates, 12);
                p.read((char*)savedata->gq[i].boss[j].angles, 12);
            }
            for(uint32_t j = 0; j < 5; j++)
            {
                p.read((char*)&(savedata->gq[i].info[j].other_area), 1);
                p.read((char*)&(savedata->gq[i].info[j].sleep_area), 1);
                p.read((char*)savedata->gq[i].info[j].spawn_area, 5);
            }
            p.read((char*)&(savedata->gq[i].unknownc), 1);
            for(uint32_t j = 0; j < 5; j++)
                p.read((char*)&(savedata->gq[i].area[j]), 4);

            p.read((char*)&(savedata->gq[i].initial_lvl), 1);
            p.read((char*)&(savedata->gq[i].lvl), 1);
            p.read((char*)&(savedata->gq[i].treasure), 1);
            p.read((char*)savedata->gq[i].boss_frenzy, 2);
            p.read((char*)savedata->gq[i].unknownd, 3);
        }

        p.seekg(0xE8A8, p.beg);
        for(uint32_t i = 10; i < 13; i++)
        {
            p.read((char*)savedata->gq[i].unknownb, 5);
            p.read((char*)&(savedata->gq[i].weapon_bias), 1);
            p.read((char*)&(savedata->gq[i].armor_serie), 1);
            p.read((char*)&(savedata->gq[i].armor_piece), 1);
            for(uint32_t j = 0; j < 5; j++)
            {
                p.read((char*)&(savedata->gq[i].boss[j].boss_type), 4);
                p.read((char*)&(savedata->gq[i].boss[j].spawn_quantity), 4);
                p.read((char*)&(savedata->gq[i].boss[j].spawn_condition), 1);
                p.read((char*)&(savedata->gq[i].boss[j].spawn_area), 1);
                p.read((char*)&(savedata->gq[i].boss[j].unknown0), 1);
                p.read((char*)&(savedata->gq[i].boss[j].form), 1);
                p.read((char*)&(savedata->gq[i].boss[j].size_related2), 1);
                p.read((char*)&(savedata->gq[i].boss[j].unknown1), 2);
                p.read((char*)&(savedata->gq[i].boss[j].frenzy), 1);
                p.read((char*)savedata->gq[i].boss[j].coordinates, 12);
                p.read((char*)savedata->gq[i].boss[j].angles, 12);
            }
            for(uint32_t j = 0; j < 5; j++)
            {
                p.read((char*)&(savedata->gq[i].info[j].other_area), 1);
                p.read((char*)&(savedata->gq[i].info[j].sleep_area), 1);
                p.read((char*)savedata->gq[i].info[j].spawn_area, 5);
            }
            p.read((char*)&(savedata->gq[i].unknownc), 1);
            for(uint32_t j = 0; j < 5; j++)
                p.read((char*)&(savedata->gq[i].area[j]), 4);
        }

        // fill buffer with the file for editing
		p.seekg(0, p.beg);
		buffer.clear();
		while(p.good())
		{
			p.read((char*)&tmp_char, 1);
			if(p.good()) buffer.push_back(tmp_char);
		}
		buffer.pop_back();

		PlusLib::write_all("MH4U_SE:: Loading successful\n");
		return true;
	}
	catch(std::exception e)
	{
		cdelete(savedata);
		PlusLib::write_all("MH4U_SE:: Exception opening/reading\n");
		return false;
	}
}

bool MH4U_SE::save()
{
    return save(filename);
}

bool MH4U_SE::save(std::wstring output)
{
    boost::filesystem::path path = output;
	boost::filesystem::ofstream f;
	try
	{
	    if(!writeBuffer()) return false;
		f.open(path, std::ios::out | std::ios::trunc | std::ios::binary);
		if(!f)
		{
			PlusLib::write_all("MH4U_SE:: Failed to open the file\n");
			return false;
		}

		for(uint32_t i = 0; i < buffer.size(); i++)
			f.write((char*)&buffer[i], 1);

		f.close();

		PlusLib::write_all("MH4U_SE:: Saving successful\n");
		return true;
	}
	catch(std::exception e)
	{
		PlusLib::write_all("MH4U_SE:: Exception opening/writing\n");
		return false;
	}
}

bool MH4U_SE::encrypt(std::wstring output)
{
    if(!writeBuffer()) return false;
    MH4U_Crypto crypto;

    std::streampos fsize = buffer.size();
    unsigned char *array = new unsigned char[fsize+8];
    for(size_t i = 0; i < fsize; i++) array[i+4] = buffer[i];

    switch(crypto.encryptSave(output, array, fsize))
    {
        case 1: wxMessageBox(_T("Error : Failed to open the input file")); delete [] array; return false;
        case 2: wxMessageBox(_T("Error : Failed to open the ouput file")); delete [] array; return false;
        default: delete [] array; return true;
    }
    return false;
}

bool MH4U_SE::writeBuffer()
{
    if(!savedata)
	{
		PlusLib::write_all("MH4U_SE:: No savedata loaded \n");
		return false;
	}
	editBuffer(0, (uint8_t*)(savedata->name), 20);
	editBuffer(0x18, (uint8_t*)&(savedata->sex), 1);

    if(!isProtected)
    {
        for(uint32_t i = 0; i < 14; i++)
        {
            for(uint32_t j = 0; j < 100; j++)
            {
                editBuffer(0x15E +  i*400 + j*4, (uint8_t*)&(savedata->box[i][j].item_id), 2);
                editBuffer(0x15E +2+i*400 + j*4, (uint8_t*)&(savedata->box[i][j].count), 2);
            }
        }

        for(uint32_t i = 0; i < 15; i++)
        {
            for(uint32_t j = 0; j < 100; j++)
            {
                editBuffer(0x173E + i*2800 + j*28, (uint8_t*)savedata->equipment[i][j], 28);
            }
        }

        for(uint32_t i = 0; i < 32; i++)
        {
            editBuffer(0xCB14 + i*4, (uint8_t*)&(savedata->inventory[i].item_id), 2);
            editBuffer(0xCB14 +2+i*4, (uint8_t*)&(savedata->inventory[i].count), 2);
        }
    }

    for(uint32_t i = 0; i < 10; i++)
    {
        editBuffer(0xDCA8 + i*304, (uint8_t*)savedata->gq[i].creator, 20);
        editBuffer(0xDCA8 +20 + i*304, (uint8_t*)savedata->gq[i].unknowna, 4);
        editBuffer(0xDCA8 +24 + i*304, (uint8_t*)savedata->gq[i].unique_ID, 8);
        editBuffer(0xDCA8 +32 + i*304, (uint8_t*)savedata->gq[i].unknownb, 5);
        editBuffer(0xDCA8 +37 + i*304, (uint8_t*)&(savedata->gq[i].weapon_bias), 1);
        editBuffer(0xDCA8 +38 + i*304, (uint8_t*)&(savedata->gq[i].armor_serie), 1);
        editBuffer(0xDCA8 +39 + i*304, (uint8_t*)&(savedata->gq[i].armor_piece), 1);
        for(uint32_t j = 0; j < 5; j++)
        {
            editBuffer(0xDCA8 +40 +40*j + i*304, (uint8_t*)&(savedata->gq[i].boss[j].boss_type), 4);
            editBuffer(0xDCA8 +44 +40*j + i*304, (uint8_t*)&(savedata->gq[i].boss[j].spawn_quantity), 4);
            editBuffer(0xDCA8 +48 +40*j + i*304, (uint8_t*)&(savedata->gq[i].boss[j].spawn_condition), 1);
            editBuffer(0xDCA8 +49 +40*j + i*304, (uint8_t*)&(savedata->gq[i].boss[j].spawn_area), 1);
            editBuffer(0xDCA8 +50 +40*j + i*304, (uint8_t*)&(savedata->gq[i].boss[j].unknown0), 1);
            editBuffer(0xDCA8 +51 +40*j + i*304, (uint8_t*)&(savedata->gq[i].boss[j].form), 1);
            editBuffer(0xDCA8 +52 +40*j + i*304, (uint8_t*)&(savedata->gq[i].boss[j].size_related2), 1);
            editBuffer(0xDCA8 +53 +40*j + i*304, (uint8_t*)&(savedata->gq[i].boss[j].unknown1), 2);
            editBuffer(0xDCA8 +55 +40*j + i*304, (uint8_t*)&(savedata->gq[i].boss[j].frenzy), 1);
            editBuffer(0xDCA8 +56 +40*j + i*304, (uint8_t*)savedata->gq[i].boss[j].coordinates, 12);
            editBuffer(0xDCA8 +68 +40*j + i*304, (uint8_t*)savedata->gq[i].boss[j].angles, 12);
        }
        for(uint32_t j = 0; j < 5; j++)
        {
            editBuffer(0xDCA8 +240 +7*j + i*304, (uint8_t*)&(savedata->gq[i].info[j].other_area), 1);
            editBuffer(0xDCA8 +241 +7*j + i*304, (uint8_t*)&(savedata->gq[i].info[j].sleep_area), 1);
            editBuffer(0xDCA8 +242 +7*j + i*304, (uint8_t*)savedata->gq[i].info[j].spawn_area, 5);
        }
        editBuffer(0xDCA8 +275 + i*304, (uint8_t*)&(savedata->gq[i].unknownc), 1);
        for(uint32_t j = 0; j < 5; j++)
            editBuffer(0xDCA8 +276 +j*4 + i*304, (uint8_t*)&(savedata->gq[i].area[j]), 4);

        editBuffer(0xDCA8 +296 + i*304, (uint8_t*)&(savedata->gq[i].initial_lvl), 1);
        editBuffer(0xDCA8 +297 + i*304, (uint8_t*)&(savedata->gq[i].lvl), 1);
        editBuffer(0xDCA8 +298 + i*304, (uint8_t*)&(savedata->gq[i].treasure), 1);
        editBuffer(0xDCA8 +299 + i*304, (uint8_t*)savedata->gq[i].boss_frenzy, 2);
        editBuffer(0xDCA8 +301 + i*304, (uint8_t*)savedata->gq[i].unknownd, 3);
    }

    for(uint32_t i = 0; i < 3; i++)
    {
        editBuffer(0xE8A8 + i*264, (uint8_t*)savedata->gq[i+10].unknownb, 5);
        editBuffer(0xE8A8 +5 + i*264, (uint8_t*)&(savedata->gq[i+10].weapon_bias), 1);
        editBuffer(0xE8A8 +6 + i*264, (uint8_t*)&(savedata->gq[i+10].armor_serie), 1);
        editBuffer(0xE8A8 +7 + i*264, (uint8_t*)&(savedata->gq[i+10].armor_piece), 1);
        for(uint32_t j = 0; j < 5; j++)
        {
            editBuffer(0xE8A8 +8 +40*j + i*264, (uint8_t*)&(savedata->gq[i+10].boss[j].boss_type), 4);
            editBuffer(0xE8A8 +12 +40*j + i*264, (uint8_t*)&(savedata->gq[i+10].boss[j].spawn_quantity), 4);
            editBuffer(0xE8A8 +16 +40*j + i*264, (uint8_t*)&(savedata->gq[i+10].boss[j].spawn_condition), 1);
            editBuffer(0xE8A8 +17 +40*j + i*264, (uint8_t*)&(savedata->gq[i+10].boss[j].spawn_area), 1);
            editBuffer(0xE8A8 +18 +40*j + i*264, (uint8_t*)&(savedata->gq[i+10].boss[j].unknown0), 1);
            editBuffer(0xE8A8 +19 +40*j + i*264, (uint8_t*)&(savedata->gq[i+10].boss[j].form), 1);
            editBuffer(0xE8A8 +20 +40*j + i*264, (uint8_t*)&(savedata->gq[i+10].boss[j].size_related2), 1);
            editBuffer(0xE8A8 +21 +40*j + i*264, (uint8_t*)&(savedata->gq[i+10].boss[j].unknown1), 2);
            editBuffer(0xE8A8 +23 +40*j + i*264, (uint8_t*)&(savedata->gq[i+10].boss[j].frenzy), 1);
            editBuffer(0xE8A8 +24 +40*j + i*264, (uint8_t*)savedata->gq[i+10].boss[j].coordinates, 12);
            editBuffer(0xE8A8 +36 +40*j + i*264, (uint8_t*)savedata->gq[i+10].boss[j].angles, 12);
        }
        for(uint32_t j = 0; j < 5; j++)
        {
            editBuffer(0xE8A8 +208 +7*j + i*264, (uint8_t*)&(savedata->gq[i+10].info[j].other_area), 1);
            editBuffer(0xE8A8 +209 +7*j + i*264, (uint8_t*)&(savedata->gq[i+10].info[j].sleep_area), 1);
            editBuffer(0xE8A8 +210 +7*j + i*264, (uint8_t*)savedata->gq[i+10].info[j].spawn_area, 5);
        }
        editBuffer(0xE8A8 +243 + i*264, (uint8_t*)&(savedata->gq[i+10].unknownc), 1);
        for(uint32_t j = 0; j < 5; j++)
            editBuffer(0xE8A8 +244 +j*4 + i*264, (uint8_t*)&(savedata->gq[i+10].area[j]), 4);
    }

    return true;
}

void MH4U_SE::editBuffer(uint32_t pos, uint8_t* ptr, uint8_t size)
{
    if(!savedata) return;
	for(uint32_t i = pos; i < buffer.size() && pos >= 0 && i < pos + size; i++)
        buffer[i] = ptr[i-pos];
}

