#ifndef MH4U_QE_H
#define MH4U_QE_H

#define FLAG_OFFSET 0xa0
#define MAGIC_NUMBER 0x35303076

#define MAX_BOSS_GROUP 5
#define MAX_INTRUDER 5
#define MAX_SUPPLY 40
#define MAX_DELIVERY 8

struct Quest_supply_items
{
    uint16_t item_id;
    uint16_t count;
};

struct Quest_reward_items
{
    uint16_t chance;
    uint16_t item_id;
    uint16_t count;
};

struct Quest_reward_box
{
    uint8_t unknown0;
    uint8_t unknown1;
    uint16_t unknown2;
    uint32_t reward_items_offset;
    std::vector<Quest_reward_items> reward_items;
};

struct Quest_boss_group
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

struct Quest_intruder // 44
{
    uint32_t probability;
    uint32_t boss_type;
    uint32_t spawn_quantity;
    uint8_t spawn_condition;
    uint8_t spawn_area;
    uint8_t unknown0;
    uint8_t form;
    uint8_t size_related2;
    uint16_t unknown1;
    uint8_t frenzy;
    float coordinates[3];
    uint32_t angles[3];
};


struct Quest_monster_stats // and bosses
{
    uint16_t size_percentage;
    uint8_t size_variation;
    uint8_t health;
    uint8_t attack;
    uint8_t defense;
    uint8_t stamina;
    uint8_t unknown;
};

struct Quest_monster_group // 40
{
    uint32_t monster_type;
    uint32_t spawn_quantity;
    uint8_t spawn_condition;
    uint8_t spawn_area;
    uint8_t crash_flag; // 0x0A
    uint8_t form;
    uint8_t size_related1;
    uint8_t size_related2;
    uint16_t unknown1;
    float coordinates[3]; // 12
    uint32_t angles[3]; // 12
};

struct Quest_text_offset
{
    uint32_t language_offset[5];
};

struct Quest_language_offset
{
    uint32_t title_offset;
    std::vector<uint8_t> title;
    uint32_t success_offset;
    std::vector<uint8_t> success;
    uint32_t failure_offset;
    std::vector<uint8_t> failure;
    uint32_t summary_offset;
    std::vector<uint8_t> summary;
    uint32_t main_monsters_offset;
    std::vector<uint8_t> main_monsters;
    uint32_t client_offset;
    std::vector<uint8_t> client;
    uint32_t sub_quest_offset;
    std::vector<uint8_t> sub_quest;
};

struct Skill_preset
{
    uint16_t id;
    uint16_t amount;
};

struct Challenge_weapon_preset // 28
{
    uint8_t category;
    uint8_t kinsect_lvl;
    uint16_t id;
    uint8_t attribute_value;
    uint8_t attribute_id;
    uint8_t decoration_id1;
    uint8_t decoration_fixed1;
    uint8_t decoration_id2;
    uint8_t decoration_fixed2;
    uint8_t decoration_id3;
    uint8_t decoration_fixed3;
    uint8_t sharpness;
    uint8_t attack;
    uint8_t level;
    uint8_t kinsect_id;
    uint8_t polished_state;
    uint8_t rarity;
    uint8_t excavated_flag;
    uint8_t honing;
    uint8_t kinsect_stat_power;
    uint8_t kinsect_stat_stamina;
    uint8_t kinsect_stat_speed;
    uint8_t kinsect_element_fire;
    uint8_t kinsect_element_water;
    uint8_t kinsect_element_thunder;
    uint8_t kinsect_element_ice;
    uint8_t kinsect_element_dragon;
}; // 27 elements

struct Challenge_armor_preset // 28
{
    uint8_t category;
    uint8_t excavated_level;
    uint16_t id;
    uint16_t color;
    uint8_t decoration_id1;
    uint8_t decoration_fixed1;
    uint8_t decoration_id2;
    uint8_t decoration_fixed2;
    uint8_t decoration_id3;
    uint8_t decoration_fixed3;
    uint8_t resistance;
    uint8_t defense;
    uint16_t unknown0;
    uint8_t polished_state;
    uint8_t rarity;
    uint8_t excavated_flag;
    uint8_t color_setting;
    uint32_t unknown1;
    uint32_t unknown2;
}; // 16 elements

struct Challenge_talisman_preset // 28
{
    uint8_t category;
    uint8_t slot_count;
    uint8_t id;
    uint16_t unknown1;
    uint8_t unknown2;
    uint8_t decoration_id1;
    uint8_t decoration_fixed1;
    uint8_t decoration_id2;
    uint8_t decoration_fixed2;
    uint8_t decoration_id3;
    uint8_t decoration_fixed3;
    uint16_t skill_id1;
    int16_t skill_value1;
    uint16_t skill_id2;
    int16_t skill_value2;
    uint32_t unknown3;
    uint32_t unknown4;
}; // 12 elements

#define C_ITEMS 24

struct Quest_challenge // 352
{
    uint32_t unknown0[7];
    Challenge_weapon_preset weapon; // 256-32 total
    Challenge_armor_preset armor[5];
    Challenge_talisman_preset talisman;
    uint32_t unknown_equip[7];
    uint32_t unknown1;
    Quest_supply_items items[24]; // 96
};

struct Boss_group_offset
{
    uint32_t offset;
    uint32_t count;
    uint32_t start;
};

struct Supply_item_list
{
    uint32_t start_offset;
    std::vector<Quest_supply_items> items;
};

struct Supply_box_group
{
    uint8_t id;
    uint8_t count;
    uint16_t unknown;
    uint32_t offset;
    uint32_t start;
};

struct Small_monster_group
{
    std::vector<Quest_monster_group> mon;
    uint32_t offset;
    uint32_t nUse;
};

struct Small_monster_sequence
{
    int32_t isCopyOf;
    std::vector<Small_monster_group*> group;
};

//------------------------------------------------
struct Quest
{
    uint32_t flags_offset;
    uint32_t magic;
    uint32_t supply_box_offset;
    uint8_t supply_condition1[8];
    uint8_t supply_condition2[8];
    uint32_t reward_box_offset[3];
    uint32_t boss_offset;
    uint32_t monster_offset;
    uint32_t intruder_offset;
    Quest_monster_stats boss_stats[5];
    Quest_monster_stats monster_stats;
    uint8_t monster_condition[16];
    uint32_t hr_points;
    uint32_t hr_points_penalty;
    uint32_t sub_hr_points;

    uint8_t intruder_condition[4];
    uint32_t spawn_timer;
    uint8_t unknown2c;
    uint8_t collect_type;
    uint8_t carve_type;
    uint8_t monster_ai;
    uint8_t player_spawn;

    uint8_t arena_gate;
    uint8_t gate_init;
    uint8_t grid_time[2];
    uint8_t unknown2e[15];

    uint32_t flags;
    uint32_t fee;
    uint32_t reward;
    uint32_t penalty;
    uint32_t sub_reward;
    uint32_t time;
    uint32_t intruder_probability;
    uint32_t text_offset;
    uint16_t id;
    uint16_t stars;
    uint8_t locale;
    uint8_t terms1;
    uint8_t terms2;
    uint8_t unknown3;
    uint8_t main_quest_count[4];
    uint32_t main_quest1_type;
    uint16_t main_quest1_id;
    uint16_t main_quest1_count;
    uint32_t main_quest2_type;
    uint16_t main_quest2_id;
    uint16_t main_quest2_count;
    uint32_t sub_quest_type;
    uint16_t sub_quest_id;
    uint16_t sub_quest_count;
    uint32_t challenge_quest_offset;
    uint16_t boss_icons[5];
    uint16_t unknown5;
    uint32_t unknown6[3];

    Quest_reward_box reward_box[3];

    std::vector<Boss_group_offset> boss_group_offset;
    Quest_boss_group boss_group[5];
    uint32_t boss_count;

    uint32_t possible_monster_offset[3];
    Small_monster_sequence possible_monster[3];
    std::vector<Small_monster_group*> monster_group_list;

    std::vector<Quest_intruder> intruder;
    Quest_text_offset quest_text_offset;
    Quest_language_offset language_offset[5];
    Quest_challenge challenge[5];
    int32_t preset_count;

    Supply_item_list supply_list;
    std::vector<Supply_box_group> supply_box;

    ~Quest()
    {
        for(uint32_t i = 0; i < monster_group_list.size(); i++)
            if(monster_group_list[i] != NULL) cdelete(monster_group_list[i]);
    };
};

class MH4U_QE
{
    public:
        MH4U_QE();
        ~MH4U_QE();
        bool load(std::wstring source, int32_t region = -2);
        bool save();
        bool save(std::wstring output);
        bool encrypt(std::wstring output, int region);

        bool modifyBoss(int32_t n, bool add, int32_t bid = 0);
        bool modifyBossGroup(int32_t n, bool add);
        bool moveBossToGroup(int32_t n, int32_t b, int32_t g);
        bool modifyIntruder(int32_t n, bool add);

        bool modifyMonster(int32_t n, bool add, int32_t bid = 0);
        bool modifyMonsterGroup(int32_t g, int32_t n, bool add);
        bool moveMonsterToGroup(int32_t n, int32_t b, int32_t g);
        Small_monster_group* searchMonsterGroup(uint32_t off);
        int32_t getMonsterGroupPos(Small_monster_group* ptr);
        bool pasteMonsterGroup(Small_monster_group* ptr, int32_t g);
        bool unbindMonsterGroup(int32_t g);

        bool modifyReward(int32_t n, bool add);
        bool unbindReward(int32_t n);
        bool addReward(int32_t n);

        bool modifySupplyGroup(bool add);
        bool modifySupplyItem(bool add);

        bool modifyChallenge(bool add);

        Quest* quest;
    protected:
        bool writeBuffer();
        void editBuffer(uint32_t pos, uint8_t* ptr, uint8_t size);
        void insertBuffer(uint32_t pos, uint8_t* ptr, uint8_t size);
        void deleteBuffer(uint32_t pos, uint8_t size);
        void updateOffset(uint32_t start, int32_t value);
        std::vector<uint8_t> buffer;
        std::wstring filename;
};

#endif // MH4U_QE_H
