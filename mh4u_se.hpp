#ifndef MH4U_SE_HPP
#define MH4U_SE_HPP

struct Save_item
{
    uint16_t item_id;
    uint16_t count;
};

struct GQ_boss
{
    uint32_t boss_type;
    uint32_t spawn_quantity;
    uint8_t spawn_condition;
    uint8_t spawn_area;
    uint8_t unknown0;
    uint8_t form;
    uint8_t size_related2;
    uint16_t unknown1;
    uint8_t frenzy;
    float coordinates[3]; // float is 4 bytes
    uint32_t angles[3];
};

struct GQ_boss_info
{
    uint8_t other_area;
    uint8_t sleep_area;
    uint8_t spawn_area[5];
};

struct GQ_area
{
    uint8_t area_map;
    uint8_t area_entrance;
    uint8_t area_exit;
    uint8_t area_end;
};

struct Save_GQ
{
    uint16_t creator[10];
    uint8_t unknowna[4];
    uint8_t unique_ID[8];
    uint8_t unknownb[5];
    uint8_t weapon_bias;
    uint8_t armor_serie;
    uint8_t armor_piece;
    GQ_boss boss[5];
    GQ_boss_info info[5];
    uint8_t unknownc;
    GQ_area area[5];
    uint8_t initial_lvl;
    uint8_t lvl;
    uint8_t treasure;
    uint8_t boss_frenzy[2];
    uint8_t unknownd[3];
};

struct Save
{
    uint16_t name[10];
    uint8_t sex;
    uint32_t money;
    Save_item box[14][100];
    Save_item inventory[32];
    uint8_t equipment[15][100][28];
    uint32_t caravan_pts;
    Save_GQ gq[13];
};

class MH4U_SE
{
    public:
        MH4U_SE();
        ~MH4U_SE();
        void setProtection(bool bo);
        bool getProtection();
        bool load(std::wstring source, bool decrypt = false);
        bool save();
        bool save(std::wstring output);
        bool encrypt(std::wstring output);
        Save* savedata;
    protected:
        bool writeBuffer();
        void editBuffer(uint32_t pos, uint8_t* ptr, uint8_t size);
        std::vector<uint8_t> buffer;
        std::wstring filename;
        bool isProtected;
};

#endif // MH4U_SE_HPP
