#include "main.hpp"
#include "pluslib.hpp"
#include "mh4u_pe.hpp"
#include "mh4u_crypto.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <wx/wx.h>

MH4U_PE::MH4U_PE()
{
    palico = NULL;
	PlusLib::write_all("MH4U_PE:: Ready\n");
}

MH4U_PE::~MH4U_PE()
{
    cdelete(palico);
	PlusLib::write_all("MH4U_PE:: Terminated\n");
}

bool MH4U_PE::load(std::wstring source, int32_t region)
{
    boost::filesystem::ifstream fp;
    std::stringstream p;
	try
	{
	    if(region == -2)
        {
            boost::filesystem::path path = source;
            fp.open(path, std::ios::in | std::ios::binary);
            if(!fp)
            {
                PlusLib::write_all("MH4U_PE:: Failed to open the file\n");
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
            bool ok = false;
            MH4U_Crypto crypto;
            if(region == -1)
            {
                for(size_t i = 0; i < 4 && !ok; i++)
                {
                    crypto.setRegion(i);
                    if(crypto.decrypt(source, p_array) == 0) ok = true;
                }
            }
            else
            {
                crypto.setRegion(region);
                if(crypto.decrypt(source, p_array) == 0) ok = true;
            }

            if(!ok)
            {
                PlusLib::write_all("MH4U_PE:: Failed to decrypt the file\n");
                return false;
            }
            for(size_t i = 0; i < p_array.size(); i++)
                p.write((char*)&p_array[i], 1);
            p.write((char*)&p_array[p_array.size()-1], 1);
        }
        filename = source;
        uint8_t tmp_char;

		cdelete(palico);
		palico = new Palico();

        p.read((char*)&(palico->unknown0), 4);
        p.read((char*)&(palico->unknown1), 4);
        if(palico->unknown0 != 0 || palico->unknown1 != 0)
        {
            PlusLib::write_all("MH4U_PE:: Invalid palico\n");
            cdelete(palico);
            if(region == -2)
            {
                PlusLib::write_all("MH4U_PE:: Trying to decrypt...\n");
                return load(source, -1);
            }
            return false;
        }

        p.read((char*)&(palico->level), 1);
        p.read((char*)&(palico->unknown2), 1);
        p.read((char*)&(palico->unknown3), 1);
        p.read((char*)&(palico->forte), 1);
        p.read((char*)palico->quest_skill, 3);
        p.read((char*)&(palico->signature_skill), 1);
        p.read((char*)&(palico->meownster_skill), 1);
        p.read((char*)&(palico->casting_skill), 1);
        p.read((char*)&(palico->team_skill), 1);
        p.read((char*)&(palico->coat), 1);
        p.read((char*)&(palico->clothing), 1);
        p.read((char*)&(palico->voice), 1);
        p.read((char*)&(palico->eyes), 1);
        p.read((char*)&(palico->ears), 1);
        p.read((char*)&(palico->tail), 1);
        p.read((char*)&(palico->id), 1);
        p.read((char*)&(palico->unknown4), 1);
        p.read((char*)&(palico->unknown5), 1);
        p.read((char*)palico->coat_rgb, 3);
        p.read((char*)&(palico->terminator1), 1);
        p.read((char*)palico->cloth_rgb, 3);
        p.read((char*)&(palico->terminator2), 1);
        p.read((char*)&(palico->text_offset), 4);

        PlusLib::write_all("Text offset: " + PlusLib::int2hex(palico->text_offset) + "\n");
        if(palico->text_offset != 0)
        {
            p.seekg(palico->text_offset, p.beg);
            p.read((char*)&(palico->text.offsets[0]), 4);
            p.read((char*)&(palico->text.offsets[1]), 4);
            p.read((char*)&(palico->text.offsets[2]), 4);
            for(uint8_t i = 0; i < 3; i++)
            {
                PlusLib::write_all("Text suboffset " + PlusLib::int2str(i+1) + ": " + PlusLib::int2hex(palico->text.offsets[i]) + "\n");
                if(palico->text.offsets[i] != 0)
                {
                    int64_t max;
                    if(i == 2 && palico->text_offset > palico->text.offsets[2]) max = palico->text_offset - palico->text.offsets[2];
                    else if(i == 2) max = 127 - palico->text.offsets[2];
                    else max = palico->text.offsets[i+1] - palico->text.offsets[i];

                    p.seekg(palico->text.offsets[i], p.beg);
                    for(uint8_t j = 0; j < max; j++)
                    {
                        p.read((char*)&tmp_char, 1);
                        palico->text.texts[i].push_back(tmp_char);
                    }
                }
            }
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

		PlusLib::write_all("MH4U_PE:: Loading successful - Palico ID : " + PlusLib::int2str(palico->id) + "\n");
		return true;
	}
	catch(std::exception e)
	{
		cdelete(palico);
		PlusLib::write_all("MH4U_PE:: Exception opening/reading\n");
		return false;
	}
}

bool MH4U_PE::save()
{
    return save(filename);
}

bool MH4U_PE::save(std::wstring output)
{
    boost::filesystem::path path = output;
	boost::filesystem::ofstream f;
	try
	{
	    if(!writeBuffer()) return false;
		f.open(path, std::ios::out | std::ios::trunc | std::ios::binary);
		if(!f)
		{
			PlusLib::write_all("MH4U_PE:: Failed to open the file\n");
			return false;
		}

		for(uint32_t i = 0; i < buffer.size(); i++)
			f.write((char*)&buffer[i], 1);

		f.close();

		PlusLib::write_all("MH4U_PE:: Saving successful\n");
		return true;
	}
	catch(std::exception e)
	{
		PlusLib::write_all("MH4U_PE:: Exception opening/writing\n");
		return false;
	}
}

bool MH4U_PE::encrypt(std::wstring output, int region)
{
    if(!writeBuffer()) return false;

    MH4U_Crypto crypto;
    crypto.setRegion(region);

    std::streampos fsize = buffer.size();
    unsigned char *array = new unsigned char[fsize+20+(8-((fsize + 20) % 8))];
    for(size_t i = 0; i < fsize; i++) array[i] = buffer[i];

    switch(crypto.encrypt(output, array, fsize))
    {
        case 1: wxMessageBox(_T("Error : Failed to open the input file")); delete [] array; return false;
        case 2: wxMessageBox(_T("Error : Failed to open the ouput file")); delete [] array; return false;
        case -1: wxMessageBox(_T("Error : Invalid region selected")); delete [] array; return false;
        default: delete [] array; return true;
    }
}

bool MH4U_PE::writeBuffer()
{
    if(!palico)
	{
		PlusLib::write_all("MH4U_PE:: No palico loaded \n");
		return false;
	}
	editBuffer(0, (uint8_t*)&(palico->unknown0), 4);
    editBuffer(4, (uint8_t*)&(palico->unknown1), 4);
    editBuffer(8, (uint8_t*)&(palico->level), 1);
    editBuffer(9, (uint8_t*)&(palico->unknown2), 1);
    editBuffer(10, (uint8_t*)&(palico->unknown3), 1);
    editBuffer(11, (uint8_t*)&(palico->forte), 1);
    editBuffer(12, (uint8_t*)palico->quest_skill, 3);
    editBuffer(15, (uint8_t*)&(palico->signature_skill), 1);
    editBuffer(16, (uint8_t*)&(palico->meownster_skill), 1);
    editBuffer(17, (uint8_t*)&(palico->casting_skill), 1);
    editBuffer(18, (uint8_t*)&(palico->team_skill), 1);
    editBuffer(19, (uint8_t*)&(palico->coat), 1);
    editBuffer(20, (uint8_t*)&(palico->clothing), 1);
    editBuffer(21, (uint8_t*)&(palico->voice), 1);
    editBuffer(22, (uint8_t*)&(palico->eyes), 1);
    editBuffer(23, (uint8_t*)&(palico->ears), 1);
    editBuffer(24, (uint8_t*)&(palico->tail), 1);
    editBuffer(25, (uint8_t*)&(palico->id), 1);
    editBuffer(26, (uint8_t*)&(palico->unknown4), 1);
    editBuffer(27, (uint8_t*)&(palico->unknown5), 1);
    editBuffer(28, (uint8_t*)palico->coat_rgb, 3);
    editBuffer(31, (uint8_t*)&(palico->terminator1), 1);
    editBuffer(32, (uint8_t*)palico->cloth_rgb, 3);
    editBuffer(35, (uint8_t*)&(palico->terminator2), 1);
    editBuffer(36, (uint8_t*)&(palico->text_offset), 4);

    if(palico->text_offset != 0)
    {
        editBuffer(palico->text_offset  , (uint8_t*)&(palico->text.offsets[0]), 4);
        editBuffer(palico->text_offset+4, (uint8_t*)&(palico->text.offsets[1]), 4);
        editBuffer(palico->text_offset+8, (uint8_t*)&(palico->text.offsets[2]), 4);
        for(uint8_t i = 0; i < 3; i++)
        {
            if(palico->text.offsets[i] != 0)
            {
                for(uint8_t j = 0; j < palico->text.texts[i].size(); j++)
                {
                    editBuffer(palico->text.offsets[i]+j, (uint8_t*)&palico->text.texts[i][j], 1);
                }
            }
        }
    }
    return true;
}

void MH4U_PE::editBuffer(uint32_t pos, uint8_t* ptr, uint8_t size)
{
    if(!palico) return;
	for(uint32_t i = pos; i < buffer.size() && pos >= 0 && i < pos + size; i++)
        buffer[i] = ptr[i-pos];
}
