#ifndef MH4U_PE_HPP
#define MH4U_PE_HPP

struct Palico_text
{
    int64_t offsets[3];
    std::vector<uint8_t> texts[3];
};

struct Palico
{
    uint32_t unknown0;
    uint32_t unknown1;
    uint8_t level;
    uint8_t unknown2;
    uint8_t unknown3;
    uint8_t forte;
    uint8_t quest_skill[3];
    uint8_t signature_skill;
    uint8_t meownster_skill;
    uint8_t casting_skill;
    uint8_t team_skill;
    uint8_t coat;
    uint8_t clothing;
    uint8_t voice;
    uint8_t eyes;
    uint8_t ears;
    uint8_t tail;
    uint8_t id;
    uint8_t unknown4;
    uint8_t unknown5;
    uint8_t coat_rgb[3];
    uint8_t terminator1;
    uint8_t cloth_rgb[3];
    uint8_t terminator2;
    uint32_t text_offset;
    Palico_text text;
};

class MH4U_PE
{
    public:
        MH4U_PE();
        ~MH4U_PE();
        bool load(std::wstring source, int32_t region = -2);
        bool save();
        bool save(std::wstring output);
        bool encrypt(std::wstring output, int region);
        Palico* palico;
    protected:
        bool writeBuffer();
        void editBuffer(uint32_t pos, uint8_t* ptr, uint8_t size);
        std::vector<uint8_t> buffer;
        std::wstring filename;
};

#endif // MH4U_PE_HPP
