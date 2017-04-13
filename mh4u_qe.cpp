#include "main.hpp"
#include "pluslib.hpp"
#include "mh4u_qe.hpp"
#include "mh4u_crypto.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <sstream>
#include <wx/wx.h>

MH4U_QE::MH4U_QE()
{
	quest = NULL;
	PlusLib::write_all("MH4U_QE:: Ready\n");
}

MH4U_QE::~MH4U_QE()
{
	cdelete(quest);
	PlusLib::write_all("MH4U_QE:: Terminated\n");
}

bool MH4U_QE::load(std::wstring source, int32_t region)
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
                PlusLib::write_all("MH4U_QE:: Failed to open the file\n");
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
                PlusLib::write_all("MH4U_QE:: Failed to decrypt the file\n");
                return false;
            }
            for(size_t i = 0; i < p_array.size(); i++)
                p.write((char*)&p_array[i], 1);
            p.write((char*)&p_array[p_array.size()-1], 1);
        }


		filename = source;
		uint8_t tmp_char;
		uint32_t tmp_value;
		bool boolean;

		cdelete(quest);
		quest = new Quest();

		p.read((char*)&(quest->flags_offset), 4);
		p.read((char*)&(quest->magic), 4);

		if(quest->magic != MAGIC_NUMBER)
		{
			PlusLib::write_all("MH4U_QE:: Invalid quest\n");
            cdelete(quest);
            if(region == -2)
            {
                PlusLib::write_all("MH4U_QE:: Trying to decrypt...\n");
                return load(source, -1);
            }
            return false;
		}

		p.read((char*)&(quest->supply_box_offset), 4);
		p.read((char*)quest->supply_condition1, 8);
		p.read((char*)quest->supply_condition2, 8);
		p.read((char*)quest->reward_box_offset, 12);
		p.read((char*)&(quest->boss_offset), 4);
		p.read((char*)&(quest->monster_offset), 4);
		p.read((char*)&(quest->intruder_offset), 4);
		p.read((char*)quest->boss_stats, 40);
		p.read((char*)&(quest->monster_stats), 8);
		p.read((char*)quest->monster_condition, 16);
		p.read((char*)&(quest->hr_points), 4);
		p.read((char*)&(quest->hr_points_penalty), 4);
		p.read((char*)&(quest->sub_hr_points), 4);

		p.read((char*)quest->intruder_condition, 4);
		p.read((char*)&quest->spawn_timer, 4);
		p.read((char*)&quest->unknown2c, 1);
		p.read((char*)&quest->collect_type, 1);
		p.read((char*)&quest->carve_type, 1);
		p.read((char*)&quest->monster_ai, 1);
		p.read((char*)&quest->player_spawn, 1);
		p.read((char*)&quest->arena_gate, 1);
        p.read((char*)&quest->gate_init, 1);
        p.read((char*)quest->grid_time, 2);
        p.read((char*)quest->unknown2e, 15);

        PlusLib::write_all("Flags offset: " + PlusLib::int2hex(quest->flags_offset) + "\n");
		p.seekg(quest->flags_offset, p.beg); // @ flags offset
		p.read((char*)&(quest->flags), 4);
		p.read((char*)&(quest->fee), 4);
		p.read((char*)&(quest->reward), 4);
		p.read((char*)&(quest->penalty), 4);
		p.read((char*)&(quest->sub_reward), 4);
		p.read((char*)&(quest->time), 4);
		p.read((char*)&(quest->intruder_probability), 4);
		p.read((char*)&(quest->text_offset), 4);
		p.read((char*)&(quest->id), 2);
		p.read((char*)&(quest->stars), 2);
		p.read((char*)&(quest->locale), 1);
		p.read((char*)&(quest->terms1), 1);
		p.read((char*)&(quest->terms2), 1);
		p.read((char*)&(quest->unknown3), 1);
		p.read((char*)quest->main_quest_count, 4);
		p.read((char*)&(quest->main_quest1_type), 4);
		p.read((char*)&(quest->main_quest1_id), 2);
		p.read((char*)&(quest->main_quest1_count), 2);
		p.read((char*)&(quest->main_quest2_type), 4);
		p.read((char*)&(quest->main_quest2_id), 2);
		p.read((char*)&(quest->main_quest2_count), 2);
		p.read((char*)&(quest->sub_quest_type), 4);
		p.read((char*)&(quest->sub_quest_id), 2);
		p.read((char*)&(quest->sub_quest_count), 2);
		p.read((char*)&(quest->challenge_quest_offset), 4);
		p.read((char*)quest->boss_icons, 10);
		p.read((char*)&(quest->unknown5), 2);
		p.read((char*)quest->unknown6, 12);

        PlusLib::write_all("Supply Box offset: " + PlusLib::int2hex(quest->supply_box_offset) + "\n");
        if(quest->supply_box_offset != 0)
        {
            p.seekg(quest->supply_box_offset, p.beg); // @ supply box offset
            quest->supply_list.start_offset = 0;
            Supply_box_group tmp_supply;
            do
            {
                p.read((char*)&(tmp_supply.id), 1);
                if(tmp_supply.id != 0xFF)
                {
                    p.read((char*)&(tmp_supply.count), 1);
                    p.read((char*)&(tmp_supply.unknown), 2);
                    p.read((char*)&(tmp_supply.offset), 4);
                    if(quest->supply_list.start_offset == 0 || tmp_supply.offset < quest->supply_list.start_offset) quest->supply_list.start_offset = tmp_supply.offset;
                    quest->supply_box.push_back(tmp_supply);
                }
            }while(tmp_supply.id != 0xFF);
            PlusLib::write_all("Supply Box count: " + PlusLib::int2str(quest->supply_box.size()) + "\n");

            uint32_t item_space = (quest->supply_box_offset - quest->supply_list.start_offset) / 4;
            if(item_space > 40) item_space = 40;

            PlusLib::write_all("Supply Items start: " + PlusLib::int2hex(quest->supply_list.start_offset) + "\n");
            p.seekg(quest->supply_list.start_offset, p.beg); // @ supply items start
            for(uint32_t i = 0; i < item_space; i++)
            {
                Quest_supply_items tmp;
                p.read((char*)&(tmp.item_id), 2);
                p.read((char*)&(tmp.count), 2);
                quest->supply_list.items.push_back(tmp);
            }
            PlusLib::write_all("Supply Items count: " + PlusLib::int2str(quest->supply_list.items.size()) + "\n");

            for(uint32_t i = 0; i < quest->supply_box.size(); i++)
                quest->supply_box[i].start = (quest->supply_box[i].offset - quest->supply_list.start_offset) / 4;
        }

		PlusLib::write_all("Challenge quest offset: " + PlusLib::int2hex(quest->challenge_quest_offset) + "\n");
		quest->preset_count = 0;
		if(quest->challenge_quest_offset != 0)
		{
			p.seekg(quest->challenge_quest_offset, p.beg); // @ challenge quest offset
			int32_t c = 0;
			for(; c < 5 && quest->challenge_quest_offset + (c * 352) < quest->supply_list.start_offset; c++)
			{
				p.read((char*)quest->challenge[c].unknown0, 28); // 7x4
				p.read((char*)&(quest->challenge[c].weapon.category), 1); // weapon, 28 bytes
                p.read((char*)&(quest->challenge[c].weapon.kinsect_lvl), 1);
                p.read((char*)&(quest->challenge[c].weapon.id), 2);
                p.read((char*)&(quest->challenge[c].weapon.attribute_value), 1);
                p.read((char*)&(quest->challenge[c].weapon.attribute_id), 1);
                p.read((char*)&(quest->challenge[c].weapon.decoration_id1), 1);
                p.read((char*)&(quest->challenge[c].weapon.decoration_fixed1), 1);
                p.read((char*)&(quest->challenge[c].weapon.decoration_id2), 1);
                p.read((char*)&(quest->challenge[c].weapon.decoration_fixed2), 1);
                p.read((char*)&(quest->challenge[c].weapon.decoration_id3), 1);
                p.read((char*)&(quest->challenge[c].weapon.decoration_fixed3), 1);
                p.read((char*)&(quest->challenge[c].weapon.sharpness), 1);
                p.read((char*)&(quest->challenge[c].weapon.attack), 1);
                p.read((char*)&(quest->challenge[c].weapon.level), 1);
                p.read((char*)&(quest->challenge[c].weapon.kinsect_id), 1);
                p.read((char*)&(quest->challenge[c].weapon.polished_state), 1);
                p.read((char*)&(quest->challenge[c].weapon.rarity), 1);
                p.read((char*)&(quest->challenge[c].weapon.excavated_flag), 1);
                p.read((char*)&(quest->challenge[c].weapon.honing), 1);
                p.read((char*)&(quest->challenge[c].weapon.kinsect_stat_power), 1);
                p.read((char*)&(quest->challenge[c].weapon.kinsect_stat_stamina), 1);
                p.read((char*)&(quest->challenge[c].weapon.kinsect_stat_speed), 1);
                p.read((char*)&(quest->challenge[c].weapon.kinsect_element_fire), 1);
                p.read((char*)&(quest->challenge[c].weapon.kinsect_element_water), 1);
                p.read((char*)&(quest->challenge[c].weapon.kinsect_element_thunder), 1);
                p.read((char*)&(quest->challenge[c].weapon.kinsect_element_ice), 1);
                p.read((char*)&(quest->challenge[c].weapon.kinsect_element_dragon), 1);

				for(uint32_t i = 0; i <= 4; i++) // 5x28
                {
                    p.read((char*)&(quest->challenge[c].armor[i].category), 1); // armor, 28 bytes
                    p.read((char*)&(quest->challenge[c].armor[i].excavated_level), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].id), 2);
                    p.read((char*)&(quest->challenge[c].armor[i].color), 2);
                    p.read((char*)&(quest->challenge[c].armor[i].decoration_id1), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].decoration_fixed1), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].decoration_id2), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].decoration_fixed2), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].decoration_id3), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].decoration_fixed3), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].resistance), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].defense), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].unknown0), 2);
                    p.read((char*)&(quest->challenge[c].armor[i].polished_state), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].rarity), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].excavated_flag), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].color_setting), 1);
                    p.read((char*)&(quest->challenge[c].armor[i].unknown1), 4);
                    p.read((char*)&(quest->challenge[c].armor[i].unknown2), 4);
                }
                p.read((char*)&(quest->challenge[c].talisman.category), 1); // talisman, 28 bytes
                p.read((char*)&(quest->challenge[c].talisman.slot_count), 1);
                p.read((char*)&(quest->challenge[c].talisman.id), 1);
                p.read((char*)&(quest->challenge[c].talisman.unknown1), 2);
                p.read((char*)&(quest->challenge[c].talisman.unknown2), 1);
                p.read((char*)&(quest->challenge[c].talisman.decoration_id1), 1);
                p.read((char*)&(quest->challenge[c].talisman.decoration_fixed1), 1);
                p.read((char*)&(quest->challenge[c].talisman.decoration_id2), 1);
                p.read((char*)&(quest->challenge[c].talisman.decoration_fixed2), 1);
                p.read((char*)&(quest->challenge[c].talisman.decoration_id3), 1);
                p.read((char*)&(quest->challenge[c].talisman.decoration_fixed3), 1);
                p.read((char*)&(quest->challenge[c].talisman.skill_id1), 2);
                p.read((char*)&(quest->challenge[c].talisman.skill_value1), 2);
                p.read((char*)&(quest->challenge[c].talisman.skill_id2), 2);
                p.read((char*)&(quest->challenge[c].talisman.skill_value2), 2);
                p.read((char*)&(quest->challenge[c].talisman.unknown3), 4);
                p.read((char*)&(quest->challenge[c].talisman.unknown4), 4);

                p.read((char*)&quest->challenge[c].unknown_equip, 28); // 28
				p.read((char*)&quest->challenge[c].unknown1, 4);

				for(uint32_t i = 0; i < C_ITEMS; i++)
				{
					Quest_supply_items tmp;
					p.read((char*)&tmp.item_id, 2);
					p.read((char*)&tmp.count, 2);
					quest->challenge[c].items[i] = tmp;
				}
			}
			quest->preset_count = c;
			PlusLib::write_all("Challenge preset count: " + PlusLib::int2str(quest->preset_count) + "\n");
		}

		for(uint8_t i = 0; i < 3; i++)
		{
			PlusLib::write_all("Reward Box offset " + PlusLib::int2str(i+1) + ": " + PlusLib::int2hex(quest->reward_box_offset[i]) + "\n");
			if(quest->reward_box_offset[i] != 0 && (i == 0 || (i == 1 && quest->reward_box_offset[i] != quest->reward_box_offset[i-1])
            || (i == 2 && quest->reward_box_offset[i] != quest->reward_box_offset[1] && quest->reward_box_offset[i] != quest->reward_box_offset[0])))
			{
				p.seekg(quest->reward_box_offset[i], p.beg);// @ reward box offset
				p.read((char*)&(quest->reward_box[i].unknown0), 1);
				p.read((char*)&(quest->reward_box[i].unknown1), 1);
				p.read((char*)&(quest->reward_box[i].unknown2), 2);
				p.read((char*)&(quest->reward_box[i].reward_items_offset), 4);

				PlusLib::write_all("Reward Items offset " + PlusLib::int2str(i+1) + ": " + PlusLib::int2hex(quest->reward_box[i].reward_items_offset) + "\n");
				if(quest->reward_box[i].reward_items_offset != 0)
                {
                    p.seekg(quest->reward_box[i].reward_items_offset, p.beg); // @ reward items offset
                    boolean = true;
                    do
                    {
                        Quest_reward_items tmp;
                        p.read((char*)&(tmp.chance), 2);
                        if(tmp.chance == 0xFFFF) boolean = false;// note : à tester plus tard
                        else
                        {
                            p.read((char*)&(tmp.item_id), 2);
                            p.read((char*)&(tmp.count), 2);
                            quest->reward_box[i].reward_items.push_back(tmp);
                        }
                    }
                    while(boolean);
                }
				PlusLib::write_all("Reward items. Line " + PlusLib::int2str(i+1) + " - Count: " + PlusLib::int2str(quest->reward_box[i].reward_items.size()) + "\n");

			}
			else if(quest->reward_box_offset[i] != 0)
			{
				p.seekg(quest->reward_box_offset[i], p.beg);// @ reward box offset
				p.read((char*)&(quest->reward_box[i].unknown0), 1);
				p.read((char*)&(quest->reward_box[i].unknown1), 1);
				p.read((char*)&(quest->reward_box[i].unknown2), 2);
				p.read((char*)&(quest->reward_box[i].reward_items_offset), 4);
			}
		}
		if(quest->reward_box_offset[2] == quest->reward_box_offset[0] && quest->reward_box_offset[2] != 0)
            quest->reward_box_offset[2] = 0xFFFFFFFE;
        else if(quest->reward_box_offset[2] == quest->reward_box_offset[1] && quest->reward_box_offset[2] != 0)
			quest->reward_box_offset[2] = 0xFFFFFFFF;
		if(quest->reward_box_offset[1] == quest->reward_box_offset[0] && quest->reward_box_offset[1] != 0)
			quest->reward_box_offset[1] = 0xFFFFFFFE;

		PlusLib::write_all("Boss offset: " + PlusLib::int2hex(quest->boss_offset) + "\n");
		if(quest->boss_offset != 0)
		{
			p.seekg(quest->boss_offset, p.beg); // @ boss offset
			do
			{
				p.read((char*)(&tmp_value), 4);
				if(tmp_value != 0 && tmp_value < 0xFFFF)
				{
					Boss_group_offset tmp;
					tmp.offset = tmp_value;
					tmp.count = 0;
					quest->boss_group_offset.push_back(tmp);
				}
			}
			while(tmp_value != 0 && tmp_value < 0xFFFF);
		}
		PlusLib::write_all("Boss offset count: " + PlusLib::int2str(quest->boss_group_offset.size()) + "\n");
		if(quest->boss_group_offset.size() == 0) return false;

		quest->boss_count = 0;
		tmp_char = 0;
		for(uint32_t i = 0; i < quest->boss_group_offset.size(); i++)
		{
			PlusLib::write_all("Boss Group offset " + PlusLib::int2str(i+1) + ": " + PlusLib::int2hex(quest->boss_group_offset[i].offset) + "\n");
			if(quest->boss_group_offset[i].offset != 0)
			{
				p.seekg(quest->boss_group_offset[i].offset, p.beg); // @ boss group offset
				Quest_boss_group tmp;
				do
				{
					p.read((char*)&(tmp.boss_type), 4);
					if(tmp.boss_type < 0xFFFF)
					{
						p.read((char*)&(tmp.spawn_quantity), 4);
						p.read((char*)&(tmp.spawn_condition), 1);
						p.read((char*)&(tmp.spawn_area), 1);
						p.read((char*)&(tmp.unknown0), 1);
						p.read((char*)&(tmp.form), 1);
						p.read((char*)&(tmp.size_related2), 1);
						p.read((char*)&(tmp.unknown1), 2);
						p.read((char*)&(tmp.frenzy), 1);
						p.read((char*)tmp.coordinates, 12);
						p.read((char*)tmp.angles, 12);
						quest->boss_group[tmp_char] = tmp;
						quest->boss_count++;
						tmp_char++;
						quest->boss_group_offset[i].count++;
					}
				}
				while(tmp.boss_type < 0xFFFF && tmp_char < 5);
				PlusLib::write_all("Group " + PlusLib::int2str(i+1) + " Boss count: " + PlusLib::int2str(quest->boss_group_offset[i].count) + "\n");
			}
		}
		PlusLib::write_all("Total boss count: " + PlusLib::int2str(quest->boss_count) + "\n");

		quest->boss_group_offset[0].start = 0;
		for(uint32_t i = 1; i < quest->boss_group_offset.size(); i++)
			quest->boss_group_offset[i].start = quest->boss_group_offset[i-1].count + quest->boss_group_offset[i-1].start;

        PlusLib::write_all("Intruder offset: " + PlusLib::int2hex(quest->intruder_offset) + "\n");
        if(quest->intruder_offset != 0)
        {
            p.seekg(quest->intruder_offset, p.beg); // @ intruder offset
            Quest_intruder tmp;
            do
            {
                p.read((char*)&(tmp.probability), 4);
                if(tmp.probability < 0xFFFF)
                {
                    p.read((char*)&(tmp.boss_type), 4);
                    p.read((char*)&(tmp.spawn_quantity), 4);
                    p.read((char*)&(tmp.spawn_condition), 1);
                    p.read((char*)&(tmp.spawn_area), 1);
                    p.read((char*)&(tmp.unknown0), 1);
                    p.read((char*)&(tmp.form), 1);
                    p.read((char*)&(tmp.size_related2), 1);
                    p.read((char*)&(tmp.unknown1), 2);
                    p.read((char*)&(tmp.frenzy), 1);
                    p.read((char*)tmp.coordinates, 12);
                    p.read((char*)tmp.angles, 12);
                    quest->intruder.push_back(tmp);
                }
            }while(tmp.probability < 0xFFFF);
        }

		PlusLib::write_all("Monster offset: " + PlusLib::int2hex(quest->monster_offset) + "\n");
		if(quest->monster_offset != 0)
        {
            p.seekg(quest->monster_offset, p.beg); // @ monster offset
            p.read((char*)quest->possible_monster_offset, 12);
            for(uint32_t m = 0; m < 3; m++)
            {
                PlusLib::write_all("Areas offset " + PlusLib::int2str(m) + " : " + PlusLib::int2hex(quest->possible_monster_offset[m]) + "\n");
                p.seekg(quest->possible_monster_offset[m], p.beg); // @ areas offset
                quest->possible_monster[m].isCopyOf = -1;
                if(m > 0 && quest->possible_monster_offset[m] == quest->possible_monster_offset[m-1])
                    quest->possible_monster[m].isCopyOf = m-1;
                if(m > 1 && quest->possible_monster_offset[m] == quest->possible_monster_offset[m-2])
                    quest->possible_monster[m].isCopyOf = m-2;
                if(quest->possible_monster[m].isCopyOf == -1)
                {
                    do
                    {
                        p.read((char*)&tmp_value, 4);
                        if(tmp_value != 0)
                        {
                            Small_monster_group* tmp_ptr = searchMonsterGroup(tmp_value);
                            if(tmp_ptr == NULL)
                            {
                                tmp_ptr = new Small_monster_group();
                                tmp_ptr->offset = tmp_value;
                                tmp_ptr->nUse = 0;
                                quest->monster_group_list.push_back(tmp_ptr);
                            }
                            tmp_ptr->nUse++;
                            quest->possible_monster[m].group.push_back(tmp_ptr);
                        }
                    }while(tmp_value != 0);
                }
            }

            for(uint32_t o = 0; o < quest->monster_group_list.size(); o++)
            {
                PlusLib::write_all("Monster Group offset: " + PlusLib::int2hex(quest->monster_group_list[o]->offset) + "\n");
                p.seekg(quest->monster_group_list[o]->offset, p.beg);
                do
                {
                    p.read((char*)&tmp_value, 4);
                    if(tmp_value != 0xFFFFFFFF)
                    {
                        Quest_monster_group tmp;
                        tmp.monster_type = tmp_value;
                        p.read((char*)&tmp.spawn_quantity, 4);
                        p.read((char*)&tmp.spawn_condition, 1);
                        p.read((char*)&tmp.spawn_area, 1);
                        p.read((char*)&tmp.crash_flag, 1);
                        p.read((char*)&tmp.form, 1);
                        p.read((char*)&tmp.size_related1, 1);
                        p.read((char*)&tmp.size_related2, 1);
                        p.read((char*)&tmp.unknown1, 2);
                        p.read((char*)tmp.coordinates, 12);
                        p.read((char*)tmp.angles, 12);
                        quest->monster_group_list[o]->mon.push_back(tmp);
                    }
                }
                while(tmp_value != 0xFFFFFFFF);
            }
        }

		PlusLib::write_all("Text offset: " + PlusLib::int2hex(quest->text_offset) + "\n");
		if(quest->text_offset != 0)
        {
            p.seekg(quest->text_offset, p.beg); // @ text offset
            p.read((char*)quest->quest_text_offset.language_offset, 20);

            for(uint8_t i = 0; i < 5; i++)
            {
                if(quest->quest_text_offset.language_offset[i] >= quest->text_offset)
                    quest->quest_text_offset.language_offset[i] = 0;
            }

            for(uint8_t i = 0; i < 5; i++)
            {
                if(quest->quest_text_offset.language_offset[i] != 0)
                {
                    PlusLib::write_all("Language offset " + PlusLib::int2str(i) + ": " + PlusLib::int2hex(quest->quest_text_offset.language_offset[i]) + "\n");
                    p.seekg(quest->quest_text_offset.language_offset[i], p.beg);

                    p.read((char*)&(quest->language_offset[i].title_offset), 4);
                    p.read((char*)&(quest->language_offset[i].success_offset), 4);
                    p.read((char*)&(quest->language_offset[i].failure_offset), 4);
                    p.read((char*)&(quest->language_offset[i].summary_offset), 4);
                    p.read((char*)&(quest->language_offset[i].main_monsters_offset), 4);
                    p.read((char*)&(quest->language_offset[i].client_offset), 4);
                    p.read((char*)&(quest->language_offset[i].sub_quest_offset), 4);

                    PlusLib::write_all("Language Title offset " + PlusLib::int2str(i) + ": " + PlusLib::int2hex(quest->language_offset[i].title_offset) + "\n");
                    p.seekg(quest->language_offset[i].title_offset, p.beg);
                    for(uint32_t j = 0; j < quest->language_offset[i].success_offset - quest->language_offset[i].title_offset; j++)
                    {
                        uint8_t tmp;
                        p.read((char*)&tmp, 1);
                        quest->language_offset[i].title.push_back(tmp);
                    }

                    PlusLib::write_all("Language Success offset " + PlusLib::int2str(i) + ": " + PlusLib::int2hex(quest->language_offset[i].success_offset) + "\n");
                    p.seekg(quest->language_offset[i].success_offset, p.beg);
                    for(uint32_t j = 0; j < quest->language_offset[i].failure_offset - quest->language_offset[i].success_offset; j++)
                    {
                        uint8_t tmp;
                        p.read((char*)&tmp, 1);
                        quest->language_offset[i].success.push_back(tmp);
                    }

                    PlusLib::write_all("Language Failure offset " + PlusLib::int2str(i) + ": " + PlusLib::int2hex(quest->language_offset[i].failure_offset) + "\n");
                    p.seekg(quest->language_offset[i].failure_offset, p.beg);
                    for(uint32_t j = 0; j < quest->language_offset[i].summary_offset - quest->language_offset[i].failure_offset; j++)
                    {
                        uint8_t tmp;
                        p.read((char*)&tmp, 1);
                        quest->language_offset[i].failure.push_back(tmp);
                    }

                    PlusLib::write_all("Language Summary offset " + PlusLib::int2str(i) + ": " + PlusLib::int2hex(quest->language_offset[i].summary_offset) + "\n");
                    p.seekg(quest->language_offset[i].summary_offset, p.beg);
                    for(uint32_t j = 0; j < quest->language_offset[i].main_monsters_offset - quest->language_offset[i].summary_offset; j++)
                    {
                        uint8_t tmp;
                        p.read((char*)&tmp, 1);
                        quest->language_offset[i].summary.push_back(tmp);
                    }

                    PlusLib::write_all("Language Main Monsters offset " + PlusLib::int2str(i) + ": " + PlusLib::int2hex(quest->language_offset[i].main_monsters_offset) + "\n");
                    p.seekg(quest->language_offset[i].main_monsters_offset, p.beg);
                    for(uint32_t j = 0; j < quest->language_offset[i].client_offset - quest->language_offset[i].main_monsters_offset; j++)
                    {
                        uint8_t tmp;
                        p.read((char*)&tmp, 1);
                        quest->language_offset[i].main_monsters.push_back(tmp);
                    }

                    PlusLib::write_all("Language Client offset " + PlusLib::int2str(i) + ": " + PlusLib::int2hex(quest->language_offset[i].client_offset) + "\n");
                    p.seekg(quest->language_offset[i].client_offset, p.beg);
                    for(uint32_t j = 0; j < quest->language_offset[i].sub_quest_offset - quest->language_offset[i].client_offset; j++)
                    {
                        uint8_t tmp;
                        p.read((char*)&tmp, 1);
                        quest->language_offset[i].client.push_back(tmp);
                    }

                    PlusLib::write_all("Language Subquest offset " + PlusLib::int2str(i) + ": " + PlusLib::int2hex(quest->language_offset[i].sub_quest_offset) + "\n");
                    p.seekg(quest->language_offset[i].sub_quest_offset, p.beg);
                    for(uint32_t j = 0; j < quest->quest_text_offset.language_offset[i] - quest->language_offset[i].sub_quest_offset; j++)
                    {
                        uint8_t tmp;
                        p.read((char*)&tmp, 1);
                        quest->language_offset[i].sub_quest.push_back(tmp);
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

		PlusLib::write_all("MH4U_QE:: Loading successful - Quest ID : " + PlusLib::int2str(quest->id) + "\n");
		return true;
	}
	catch(std::exception e)
	{
		cdelete(quest);
		PlusLib::write_all("MH4U_QE:: Exception opening/reading\n");
		return false;
	}
}

bool MH4U_QE::save()
{
	return save(filename);
}


bool MH4U_QE::save(std::wstring output)
{
    boost::filesystem::path path = output;
	boost::filesystem::ofstream f;
	try
	{
	    if(!writeBuffer()) return false;
		f.open(path, std::ios::out | std::ios::trunc | std::ios::binary);
		if(!f)
		{
			PlusLib::write_all("MH4U_QE:: Failed to open the file\n");
			return false;
		}

		for(uint32_t i = 0; i < buffer.size() - buffer.size() % 16; i++)
			f.write((char*)&buffer[i], 1);

		f.close();

        if(quest->reward_box_offset[2] == quest->reward_box_offset[0] && quest->reward_box_offset[2] != 0)
            quest->reward_box_offset[2] = 0xFFFFFFFE;
        else if(quest->reward_box_offset[2] == quest->reward_box_offset[1] && quest->reward_box_offset[2] != 0)
			quest->reward_box_offset[2] = 0xFFFFFFFF;
		if(quest->reward_box_offset[1] == quest->reward_box_offset[0] && quest->reward_box_offset[1] != 0)
			quest->reward_box_offset[1] = 0xFFFFFFFE;

		PlusLib::write_all("MH4U_QE:: Saving successful\n");
		return true;
	}
	catch(std::exception e)
	{
		PlusLib::write_all("MH4U_QE:: Exception opening/writing\n");
		return false;
	}
}

bool MH4U_QE::encrypt(std::wstring output, int region)
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

bool MH4U_QE::writeBuffer()
{
	if(!quest)
	{
		PlusLib::write_all("MH4U_QE:: No quest loaded \n");
		return false;
	}
	uint32_t tmp_value;
	std::string tmp_str;

	editBuffer(0, (uint8_t*)&quest->flags_offset, 4);
	tmp_value = MAGIC_NUMBER;
	editBuffer(4, (uint8_t*)&tmp_value, 4);
	editBuffer(8, (uint8_t*)&quest->supply_box_offset, 4);
	editBuffer(12, (uint8_t*)quest->supply_condition1, 8);
	editBuffer(20, (uint8_t*)quest->supply_condition2, 8);

	for(uint32_t i = 1; i < 3; i++)
		if(quest->reward_box_offset[i] == 0xFFFFFFFF) quest->reward_box_offset[i] = quest->reward_box_offset[1];
		else if(quest->reward_box_offset[i] == 0xFFFFFFFE) quest->reward_box_offset[i] = quest->reward_box_offset[0];

	editBuffer(28, (uint8_t*)quest->reward_box_offset, 12);
	editBuffer(40, (uint8_t*)&quest->boss_offset, 4);
	editBuffer(44, (uint8_t*)&quest->monster_offset, 4);
	editBuffer(48, (uint8_t*)&quest->intruder_offset, 4);
	editBuffer(52, (uint8_t*)quest->boss_stats, 40);
	editBuffer(92, (uint8_t*)&quest->monster_stats, 8);
	editBuffer(100, (uint8_t*)quest->monster_condition, 16);
	editBuffer(116, (uint8_t*)&quest->hr_points, 4);
	editBuffer(120, (uint8_t*)&quest->hr_points_penalty, 4);
	editBuffer(124, (uint8_t*)&quest->sub_hr_points, 4);

	editBuffer(128, (uint8_t*)quest->intruder_condition, 4);
	editBuffer(132, (uint8_t*)&quest->spawn_timer, 4);
	editBuffer(136, (uint8_t*)&quest->unknown2c, 1);
	editBuffer(137, (uint8_t*)&quest->collect_type, 1);
	editBuffer(138, (uint8_t*)&quest->carve_type, 1);
	editBuffer(139, (uint8_t*)&quest->monster_ai, 1);
	editBuffer(140, (uint8_t*)&quest->player_spawn, 1);

	editBuffer(141, (uint8_t*)&quest->arena_gate, 1);
    editBuffer(142, (uint8_t*)&quest->gate_init, 1);
    editBuffer(143, (uint8_t*)quest->grid_time, 2);
    editBuffer(145, (uint8_t*)quest->unknown2e, 15);

	editBuffer(quest->flags_offset, (uint8_t*)&quest->flags, 4);
	editBuffer(quest->flags_offset+4, (uint8_t*)&quest->fee, 4);
	editBuffer(quest->flags_offset+8, (uint8_t*)&quest->reward, 4);
	editBuffer(quest->flags_offset+12, (uint8_t*)&quest->penalty, 4);
	editBuffer(quest->flags_offset+16, (uint8_t*)&quest->sub_reward, 4);
	editBuffer(quest->flags_offset+20, (uint8_t*)&quest->time, 4);
	editBuffer(quest->flags_offset+24, (uint8_t*)&quest->intruder_probability, 4);
	editBuffer(quest->flags_offset+28, (uint8_t*)&quest->text_offset, 4);
	editBuffer(quest->flags_offset+32, (uint8_t*)&quest->id, 2);
	editBuffer(quest->flags_offset+34, (uint8_t*)&quest->stars, 2);
	editBuffer(quest->flags_offset+36, (uint8_t*)&quest->locale, 1);
	editBuffer(quest->flags_offset+37, (uint8_t*)&quest->terms1, 1);
	editBuffer(quest->flags_offset+38, (uint8_t*)&quest->terms2, 1);
	editBuffer(quest->flags_offset+39, (uint8_t*)&quest->unknown3, 1);
	editBuffer(quest->flags_offset+40, (uint8_t*)quest->main_quest_count, 4);
	editBuffer(quest->flags_offset+44, (uint8_t*)&quest->main_quest1_type, 4);
	editBuffer(quest->flags_offset+48, (uint8_t*)&quest->main_quest1_id, 2);
	editBuffer(quest->flags_offset+50, (uint8_t*)&quest->main_quest1_count, 2);
	editBuffer(quest->flags_offset+52, (uint8_t*)&quest->main_quest2_type, 4);
	editBuffer(quest->flags_offset+56, (uint8_t*)&quest->main_quest2_id, 2);
	editBuffer(quest->flags_offset+58, (uint8_t*)&quest->main_quest2_count, 2);
	editBuffer(quest->flags_offset+60, (uint8_t*)&quest->sub_quest_type, 4);
	editBuffer(quest->flags_offset+64, (uint8_t*)&quest->sub_quest_id, 2);
	editBuffer(quest->flags_offset+66, (uint8_t*)&quest->sub_quest_count, 2);
	editBuffer(quest->flags_offset+68, (uint8_t*)&quest->challenge_quest_offset, 4);
	editBuffer(quest->flags_offset+72, (uint8_t*)quest->boss_icons, 10);
	editBuffer(quest->flags_offset+82, (uint8_t*)&quest->unknown5, 2);
	editBuffer(quest->flags_offset+84, (uint8_t*)quest->unknown6, 12);

    if(quest->supply_box_offset != 0)
    {
        tmp_value = quest->supply_box_offset; // @ supply box offset
        for(uint32_t i = 0; i < quest->supply_box.size(); i++)
        {
            editBuffer(tmp_value  +8*i, (uint8_t*)&quest->supply_box[i].id, 1);
            editBuffer(tmp_value+1+8*i, (uint8_t*)&quest->supply_box[i].count, 1);
            editBuffer(tmp_value+2+8*i, (uint8_t*)&quest->supply_box[i].unknown, 2);
            quest->supply_box[i].offset = quest->supply_list.start_offset + (4 * quest->supply_box[i].start);
            editBuffer(tmp_value+4+8*i, (uint8_t*)&(quest->supply_box[i].offset), 4);
        }

        tmp_value = quest->supply_list.start_offset;
        for(uint32_t i = 0; i < quest->supply_list.items.size(); i++)
        {
            editBuffer(tmp_value   + 4*i, (uint8_t*)&quest->supply_list.items[i].item_id, 2);
            editBuffer(tmp_value+2 + 4*i, (uint8_t*)&quest->supply_list.items[i].count, 2);
        }
    }

	tmp_value = quest->challenge_quest_offset; // @ challenge quest offset
	if(tmp_value != 0)
	{
		for(int32_t c = 0; c < quest->preset_count; c++)
		{
			editBuffer(tmp_value, (uint8_t*)quest->challenge[c].unknown0, 28); // 7x4

            editBuffer(tmp_value+28, (uint8_t*)&(quest->challenge[c].weapon.category), 1); // weapon, 28 bytes
            editBuffer(tmp_value+29, (uint8_t*)&(quest->challenge[c].weapon.kinsect_lvl), 1);
            editBuffer(tmp_value+30, (uint8_t*)&(quest->challenge[c].weapon.id), 2);
            editBuffer(tmp_value+32, (uint8_t*)&(quest->challenge[c].weapon.attribute_value), 1);
            editBuffer(tmp_value+33, (uint8_t*)&(quest->challenge[c].weapon.attribute_id), 1);
            editBuffer(tmp_value+34, (uint8_t*)&(quest->challenge[c].weapon.decoration_id1), 1);
            editBuffer(tmp_value+35, (uint8_t*)&(quest->challenge[c].weapon.decoration_fixed1), 1);
            editBuffer(tmp_value+36, (uint8_t*)&(quest->challenge[c].weapon.decoration_id2), 1);
            editBuffer(tmp_value+37, (uint8_t*)&(quest->challenge[c].weapon.decoration_fixed2), 1);
            editBuffer(tmp_value+38, (uint8_t*)&(quest->challenge[c].weapon.decoration_id3), 1);
            editBuffer(tmp_value+39, (uint8_t*)&(quest->challenge[c].weapon.decoration_fixed3), 1);
            editBuffer(tmp_value+40, (uint8_t*)&(quest->challenge[c].weapon.sharpness), 1);
            editBuffer(tmp_value+41, (uint8_t*)&(quest->challenge[c].weapon.attack), 1);
            editBuffer(tmp_value+42, (uint8_t*)&(quest->challenge[c].weapon.level), 1);
            editBuffer(tmp_value+43, (uint8_t*)&(quest->challenge[c].weapon.kinsect_id), 1);
            editBuffer(tmp_value+44, (uint8_t*)&(quest->challenge[c].weapon.polished_state), 1);
            editBuffer(tmp_value+45, (uint8_t*)&(quest->challenge[c].weapon.rarity), 1);
            editBuffer(tmp_value+46, (uint8_t*)&(quest->challenge[c].weapon.excavated_flag), 1);
            editBuffer(tmp_value+47, (uint8_t*)&(quest->challenge[c].weapon.honing), 1);
            editBuffer(tmp_value+48, (uint8_t*)&(quest->challenge[c].weapon.kinsect_stat_power), 1);
            editBuffer(tmp_value+49, (uint8_t*)&(quest->challenge[c].weapon.kinsect_stat_stamina), 1);
            editBuffer(tmp_value+50, (uint8_t*)&(quest->challenge[c].weapon.kinsect_stat_speed), 1);
            editBuffer(tmp_value+51, (uint8_t*)&(quest->challenge[c].weapon.kinsect_element_fire), 1);
            editBuffer(tmp_value+52, (uint8_t*)&(quest->challenge[c].weapon.kinsect_element_water), 1);
            editBuffer(tmp_value+53, (uint8_t*)&(quest->challenge[c].weapon.kinsect_element_thunder), 1);
            editBuffer(tmp_value+54, (uint8_t*)&(quest->challenge[c].weapon.kinsect_element_ice), 1);
            editBuffer(tmp_value+55, (uint8_t*)&(quest->challenge[c].weapon.kinsect_element_dragon), 1);

            for(uint32_t i = 0; i <= 4; i++) // 5x28
            {
                editBuffer(tmp_value+(i*28)+56, (uint8_t*)&(quest->challenge[c].armor[i].category), 1); // armor, 28 bytes
                editBuffer(tmp_value+(i*28)+57, (uint8_t*)&(quest->challenge[c].armor[i].excavated_level), 1);
                editBuffer(tmp_value+(i*28)+58, (uint8_t*)&(quest->challenge[c].armor[i].id), 2);
                editBuffer(tmp_value+(i*28)+60, (uint8_t*)&(quest->challenge[c].armor[i].color), 2);
                editBuffer(tmp_value+(i*28)+62, (uint8_t*)&(quest->challenge[c].armor[i].decoration_id1), 1);
                editBuffer(tmp_value+(i*28)+63, (uint8_t*)&(quest->challenge[c].armor[i].decoration_fixed1), 1);
                editBuffer(tmp_value+(i*28)+64, (uint8_t*)&(quest->challenge[c].armor[i].decoration_id2), 1);
                editBuffer(tmp_value+(i*28)+65, (uint8_t*)&(quest->challenge[c].armor[i].decoration_fixed2), 1);
                editBuffer(tmp_value+(i*28)+66, (uint8_t*)&(quest->challenge[c].armor[i].decoration_id3), 1);
                editBuffer(tmp_value+(i*28)+67, (uint8_t*)&(quest->challenge[c].armor[i].decoration_fixed3), 1);
                editBuffer(tmp_value+(i*28)+68, (uint8_t*)&(quest->challenge[c].armor[i].resistance), 1);
                editBuffer(tmp_value+(i*28)+69, (uint8_t*)&(quest->challenge[c].armor[i].defense), 1);
                editBuffer(tmp_value+(i*28)+70, (uint8_t*)&(quest->challenge[c].armor[i].unknown0), 2);
                editBuffer(tmp_value+(i*28)+72, (uint8_t*)&(quest->challenge[c].armor[i].polished_state), 1);
                editBuffer(tmp_value+(i*28)+73, (uint8_t*)&(quest->challenge[c].armor[i].rarity), 1);
                editBuffer(tmp_value+(i*28)+74, (uint8_t*)&(quest->challenge[c].armor[i].excavated_flag), 1);
                editBuffer(tmp_value+(i*28)+75, (uint8_t*)&(quest->challenge[c].armor[i].color_setting), 1);
                editBuffer(tmp_value+(i*28)+76, (uint8_t*)&(quest->challenge[c].armor[i].unknown1), 4);
                editBuffer(tmp_value+(i*28)+80, (uint8_t*)&(quest->challenge[c].armor[i].unknown2), 4);
            }
            editBuffer(tmp_value+196, (uint8_t*)&(quest->challenge[c].talisman.category), 1); // talisman, 28 bytes
            editBuffer(tmp_value+197, (uint8_t*)&(quest->challenge[c].talisman.slot_count), 1);
            editBuffer(tmp_value+198, (uint8_t*)&(quest->challenge[c].talisman.id), 1);
            editBuffer(tmp_value+199, (uint8_t*)&(quest->challenge[c].talisman.unknown1), 2);
            editBuffer(tmp_value+201, (uint8_t*)&(quest->challenge[c].talisman.unknown2), 1);
            editBuffer(tmp_value+202, (uint8_t*)&(quest->challenge[c].talisman.decoration_id1), 1);
            editBuffer(tmp_value+203, (uint8_t*)&(quest->challenge[c].talisman.decoration_fixed1), 1);
            editBuffer(tmp_value+204, (uint8_t*)&(quest->challenge[c].talisman.decoration_id2), 1);
            editBuffer(tmp_value+205, (uint8_t*)&(quest->challenge[c].talisman.decoration_fixed2), 1);
            editBuffer(tmp_value+206, (uint8_t*)&(quest->challenge[c].talisman.decoration_id3), 1);
            editBuffer(tmp_value+207, (uint8_t*)&(quest->challenge[c].talisman.decoration_fixed3), 1);
            editBuffer(tmp_value+208, (uint8_t*)&(quest->challenge[c].talisman.skill_id1), 2);
            editBuffer(tmp_value+210, (uint8_t*)&(quest->challenge[c].talisman.skill_value1), 2);
            editBuffer(tmp_value+212, (uint8_t*)&(quest->challenge[c].talisman.skill_id2), 2);
            editBuffer(tmp_value+214, (uint8_t*)&(quest->challenge[c].talisman.skill_value2), 2);
            editBuffer(tmp_value+216, (uint8_t*)&(quest->challenge[c].talisman.unknown3), 4);
            editBuffer(tmp_value+220, (uint8_t*)&(quest->challenge[c].talisman.unknown4), 4);

            editBuffer(tmp_value+224, (uint8_t*)quest->challenge[c].unknown_equip, 28); // 28
			editBuffer(tmp_value+252    , (uint8_t*)&quest->challenge[c].unknown1, 4);
			for(uint32_t i = 0; i < C_ITEMS; i++)
			{
				editBuffer(tmp_value+256+i*4   , (uint8_t*)&quest->challenge[c].items[i].item_id, 2);
				editBuffer(tmp_value+258+i*4   , (uint8_t*)&quest->challenge[c].items[i].count, 2);
			}
			tmp_value += 352;
		}
	}

	for(uint8_t i = 0; i < 3; i++)
	{
		tmp_value = quest->reward_box_offset[i];// @ reward box offset
		if(quest->reward_box_offset[i] != 0)
		{
			editBuffer(tmp_value, (uint8_t*)&quest->reward_box[i].unknown0, 1);
			editBuffer(tmp_value+1, (uint8_t*)&quest->reward_box[i].unknown1, 1);
			editBuffer(tmp_value+2, (uint8_t*)&quest->reward_box[i].unknown2, 2);
			editBuffer(tmp_value+4, (uint8_t*)&quest->reward_box[i].reward_items_offset, 4);

			tmp_value = quest->reward_box[i].reward_items_offset; // @ reward items offset
			for(uint32_t j = 0; j < quest->reward_box[i].reward_items.size(); j++) // note : à corriger plus tard ?
			{
				editBuffer(tmp_value   + 6 * j, (uint8_t*)&quest->reward_box[i].reward_items[j].chance, 2);
				editBuffer(tmp_value+2 + 6 * j, (uint8_t*)&quest->reward_box[i].reward_items[j].item_id, 2);
				editBuffer(tmp_value+4 + 6 * j, (uint8_t*)&quest->reward_box[i].reward_items[j].count, 2);
			}
		}
	}

	if(quest->boss_offset != 0)
	{
		tmp_value = quest->boss_offset; // @ boss offset
		for(uint32_t i = 0; i < quest->boss_group_offset.size(); i++)
			editBuffer(tmp_value+4*i, (uint8_t*)&quest->boss_group_offset[i].offset, 4);
	}

	for(uint32_t i = 0; i < quest->boss_group_offset.size(); i++)
	{
		if(quest->boss_group_offset[i].offset != 0)
		{
			tmp_value = quest->boss_group_offset[i].offset; // @ boss group offset
			for(uint32_t j = quest->boss_group_offset[i].start; j < 5 && j < quest->boss_group_offset[i].start + quest->boss_group_offset[i].count; j++)
			{
				editBuffer(tmp_value    + 40*(j-quest->boss_group_offset[i].start), (uint8_t*)&quest->boss_group[j].boss_type, 4);
				editBuffer(tmp_value+4  + 40*(j-quest->boss_group_offset[i].start), (uint8_t*)&quest->boss_group[j].spawn_quantity, 4);
				editBuffer(tmp_value+8  + 40*(j-quest->boss_group_offset[i].start), (uint8_t*)&quest->boss_group[j].spawn_condition, 1);
				editBuffer(tmp_value+9  + 40*(j-quest->boss_group_offset[i].start), (uint8_t*)&quest->boss_group[j].spawn_area, 1);
				editBuffer(tmp_value+10 + 40*(j-quest->boss_group_offset[i].start), (uint8_t*)&quest->boss_group[j].unknown0, 1);
				editBuffer(tmp_value+11 + 40*(j-quest->boss_group_offset[i].start), (uint8_t*)&quest->boss_group[j].form, 1);
				editBuffer(tmp_value+12 + 40*(j-quest->boss_group_offset[i].start), (uint8_t*)&quest->boss_group[j].size_related2, 1);
				editBuffer(tmp_value+13 + 40*(j-quest->boss_group_offset[i].start), (uint8_t*)&quest->boss_group[j].unknown1, 2);
				editBuffer(tmp_value+15 + 40*(j-quest->boss_group_offset[i].start), (uint8_t*)&quest->boss_group[j].frenzy, 1);
				editBuffer(tmp_value+16 + 40*(j-quest->boss_group_offset[i].start), (uint8_t*)quest->boss_group[j].coordinates, 12);
				editBuffer(tmp_value+28 + 40*(j-quest->boss_group_offset[i].start), (uint8_t*)quest->boss_group[j].angles, 12);
			}
		}
	}

	if(quest->intruder_offset != 0)
    {
        tmp_value = quest->intruder_offset;
        for(uint32_t i = 0; i < quest->intruder.size(); i++)
        {
            editBuffer(tmp_value    , (uint8_t*)&(quest->intruder[i].probability), 4);
            editBuffer(tmp_value+4  , (uint8_t*)&(quest->intruder[i].boss_type), 4);
            editBuffer(tmp_value+8  , (uint8_t*)&(quest->intruder[i].spawn_quantity), 4);
            editBuffer(tmp_value+12 , (uint8_t*)&(quest->intruder[i].spawn_condition), 1);
            editBuffer(tmp_value+13 , (uint8_t*)&(quest->intruder[i].spawn_area), 1);
            editBuffer(tmp_value+14 , (uint8_t*)&(quest->intruder[i].unknown0), 1);
            editBuffer(tmp_value+15 , (uint8_t*)&(quest->intruder[i].form), 1);
            editBuffer(tmp_value+16 , (uint8_t*)&(quest->intruder[i].size_related2), 1);
            editBuffer(tmp_value+17 , (uint8_t*)&(quest->intruder[i].unknown1), 2);
            editBuffer(tmp_value+19 , (uint8_t*)&(quest->intruder[i].frenzy), 1);
            editBuffer(tmp_value+20 , (uint8_t*)quest->intruder[i].coordinates, 12);
            editBuffer(tmp_value+32 , (uint8_t*)quest->intruder[i].angles, 12);
            tmp_value += 44;
        }
    }

	tmp_value = quest->monster_offset; // @ monster offset
	if(tmp_value != 0)
    {
        editBuffer(tmp_value  , (uint8_t*)&quest->possible_monster_offset[0], 4);
        editBuffer(tmp_value+4, (uint8_t*)&quest->possible_monster_offset[1], 4);
        editBuffer(tmp_value+8, (uint8_t*)&quest->possible_monster_offset[2], 4);

        for(uint32_t m = 0; m < 3; m++)
        {
            tmp_value = quest->possible_monster_offset[m]; // @ areas offset
            for(uint32_t i = 0; quest->possible_monster[m].isCopyOf == -1 && i < quest->possible_monster[m].group.size(); i++)
            {
                if(quest->possible_monster[m].group[i] != NULL)
                    editBuffer(tmp_value+4*i, (uint8_t*)&quest->possible_monster[m].group[i]->offset, 4);
            }
        }

        for(uint32_t m = 0; m < quest->monster_group_list.size(); m++)
        {
            if(quest->monster_group_list[m] != NULL)
            {
                tmp_value = quest->monster_group_list[m]->offset; // @ monster group offset
                for(uint32_t i = 0; i < quest->monster_group_list[m]->mon.size(); i++)
                {
                    editBuffer(tmp_value   +40*i, (uint8_t*)&quest->monster_group_list[m]->mon[i].monster_type, 4);
                    editBuffer(tmp_value+4 +40*i, (uint8_t*)&quest->monster_group_list[m]->mon[i].spawn_quantity, 4);
                    editBuffer(tmp_value+8 +40*i, (uint8_t*)&quest->monster_group_list[m]->mon[i].spawn_condition, 1);
                    editBuffer(tmp_value+9 +40*i, (uint8_t*)&quest->monster_group_list[m]->mon[i].spawn_area, 1);
                    editBuffer(tmp_value+10+40*i, (uint8_t*)&quest->monster_group_list[m]->mon[i].crash_flag, 1);
                    editBuffer(tmp_value+11+40*i, (uint8_t*)&quest->monster_group_list[m]->mon[i].form, 1);
                    editBuffer(tmp_value+12+40*i, (uint8_t*)&quest->monster_group_list[m]->mon[i].size_related1, 1);
                    editBuffer(tmp_value+13+40*i, (uint8_t*)&quest->monster_group_list[m]->mon[i].size_related2, 1);
                    editBuffer(tmp_value+14+40*i, (uint8_t*)&quest->monster_group_list[m]->mon[i].unknown1, 2);
                    editBuffer(tmp_value+16+40*i, (uint8_t*)quest->monster_group_list[m]->mon[i].coordinates, 12);
                    editBuffer(tmp_value+28+40*i, (uint8_t*)quest->monster_group_list[m]->mon[i].angles, 12);
                }
            }
        }
    }

    if(quest->text_offset != 0)
    {
        tmp_value = quest->text_offset; // @ text offset
        for(uint8_t i = 0; i < 5; i++)
            if(quest->quest_text_offset.language_offset[i] != 0)
                editBuffer(tmp_value+4*i, (uint8_t*)&(quest->quest_text_offset.language_offset[i]), 4);

        for(uint8_t i = 0; i < 5; i++)
        {
            if(i == 0 || (i > 0 && quest->quest_text_offset.language_offset[i] != quest->quest_text_offset.language_offset[i-1] && quest->quest_text_offset.language_offset[i] != 0))
            {
                tmp_value = quest->quest_text_offset.language_offset[i];

                editBuffer(tmp_value   , (uint8_t*)&(quest->language_offset[i].title_offset), 4);
                editBuffer(tmp_value+4 , (uint8_t*)&(quest->language_offset[i].success_offset), 4);
                editBuffer(tmp_value+8 , (uint8_t*)&(quest->language_offset[i].failure_offset), 4);
                editBuffer(tmp_value+12, (uint8_t*)&(quest->language_offset[i].summary_offset), 4);
                editBuffer(tmp_value+16, (uint8_t*)&(quest->language_offset[i].main_monsters_offset), 4);
                editBuffer(tmp_value+20, (uint8_t*)&(quest->language_offset[i].client_offset), 4);
                editBuffer(tmp_value+24, (uint8_t*)&(quest->language_offset[i].sub_quest_offset), 4);

                tmp_value = quest->language_offset[i].title_offset;
                for(uint32_t j = 0; j < quest->language_offset[i].title.size(); j++)
                {
                    editBuffer(tmp_value+j, (uint8_t*)&quest->language_offset[i].title[j], 1);
                }

                tmp_value = quest->language_offset[i].success_offset;
                for(uint32_t j = 0; j < quest->language_offset[i].success.size(); j++)
                {
                    editBuffer(tmp_value+j, (uint8_t*)&quest->language_offset[i].success[j], 1);
                }

                tmp_value = quest->language_offset[i].failure_offset;
                for(uint32_t j = 0; j < quest->language_offset[i].failure.size(); j++)
                {
                    editBuffer(tmp_value+j, (uint8_t*)&quest->language_offset[i].failure[j], 1);
                }

                tmp_value = quest->language_offset[i].summary_offset;
                for(uint32_t j = 0; j < quest->language_offset[i].summary.size(); j++)
                {
                    editBuffer(tmp_value+j, (uint8_t*)&quest->language_offset[i].summary[j], 1);
                }

                tmp_value = quest->language_offset[i].main_monsters_offset;
                for(uint32_t j = 0; j < quest->language_offset[i].main_monsters.size(); j++)
                {
                    editBuffer(tmp_value+j, (uint8_t*)&quest->language_offset[i].main_monsters[j], 1);
                }

                tmp_value = quest->language_offset[i].client_offset;
                for(uint32_t j = 0; j < quest->language_offset[i].client.size(); j++)
                {
                    editBuffer(tmp_value+j, (uint8_t*)&quest->language_offset[i].client[j], 1);
                }

                tmp_value = quest->language_offset[i].sub_quest_offset;
                for(uint32_t j = 0; j < quest->language_offset[i].sub_quest.size(); j++)
                {
                    editBuffer(tmp_value+j, (uint8_t*)&quest->language_offset[i].sub_quest[j], 1);
                }
            }
        }
    }

	for(tmp_value = buffer.size()-1; buffer[tmp_value] != 0x00; tmp_value--);

	tmp_value = buffer.size()-tmp_value;
	PlusLib::write_all("Extra Byte(s): " + PlusLib::int2str(tmp_value) + "\n");
	if(tmp_value / 16 >= 3 && buffer.size() % 16 != 0)
    {
        if(buffer.size() % 16 != 0)
        {
            while(buffer.size() % 16 != 0) buffer.pop_back();
        }
    }
    else
    {
        if(buffer.size() % 16 != 0)
        {
            while(buffer.size() % 16 != 0) buffer.push_back(0);
        }
    }

    return true;
}

void MH4U_QE::editBuffer(uint32_t pos, uint8_t* ptr, uint8_t size)
{
	if(!quest) return;
	for(uint32_t i = pos; i < buffer.size() && pos >= 0 && i < pos + size; i++)
        buffer[i] = ptr[i-pos];
}

void MH4U_QE::insertBuffer(uint32_t pos, uint8_t* ptr, uint8_t size)
{
	if(!quest) return;

	for(uint32_t i = pos; i < buffer.size() && pos >= 0 && i < pos + size; i++)
        buffer.insert(buffer.begin()+i, ptr[i-pos]);
}

void MH4U_QE::deleteBuffer(uint32_t pos, uint8_t size)
{
	if(!quest || pos + size >= buffer.size() || pos < 0) return;
	for(uint32_t i = 0; i < size; i++)
		buffer.erase(buffer.begin()+pos);
}

bool MH4U_QE::modifyBoss(int32_t n, bool add, int32_t bid)
{
	if(!quest || n < 0 || n >= (int32_t)quest->boss_group_offset.size()) return false;
	if(add && quest->boss_count < 5)
	{
		uint8_t empty_boss[40];
		for(uint32_t i = 0; i < 40; i++) empty_boss[i] = 0;
		insertBuffer(quest->boss_group_offset[n].offset + quest->boss_group_offset[n].count * 40, empty_boss, 40); // create empty space

		Quest_boss_group tmp;
		tmp.boss_type = 1;
		tmp.coordinates[0] = 0;
		tmp.coordinates[1] = 0;
		tmp.coordinates[2] = 0;
		tmp.frenzy = 0;
		tmp.form = 0;
		tmp.size_related2 = 0;
		tmp.spawn_area = 1;
		tmp.spawn_condition = 0xFF;
		tmp.spawn_quantity = 1;
		tmp.unknown0 = 0;
		tmp.unknown1 = 0;
		tmp.angles[0] = 0;
		tmp.angles[1] = 0;
		tmp.angles[2] = 0;

		quest->boss_stats[quest->boss_group_offset[n].start+quest->boss_group_offset[n].count].attack = 30;
		quest->boss_stats[quest->boss_group_offset[n].start+quest->boss_group_offset[n].count].defense = 20;
		quest->boss_stats[quest->boss_group_offset[n].start+quest->boss_group_offset[n].count].health = 10;
		quest->boss_stats[quest->boss_group_offset[n].start+quest->boss_group_offset[n].count].size_percentage = 100;
		quest->boss_stats[quest->boss_group_offset[n].start+quest->boss_group_offset[n].count].size_variation = 0;
		quest->boss_stats[quest->boss_group_offset[n].start+quest->boss_group_offset[n].count].stamina = 0;
		quest->boss_stats[quest->boss_group_offset[n].start+quest->boss_group_offset[n].count].unknown = 0;

		for(int32_t i = quest->boss_count-1; i >= (int32_t)(quest->boss_group_offset[n].start+quest->boss_group_offset[n].count); i--)
		{
			quest->boss_group[i+1] = quest->boss_group[i];
			quest->boss_stats[i+1] = quest->boss_stats[i];
		}

		quest->boss_group[quest->boss_group_offset[n].start+quest->boss_group_offset[n].count] = tmp;
		quest->boss_count++;
		quest->boss_group_offset[n].count++;
		updateOffset(quest->boss_group_offset[n].offset, 40);
		for(uint32_t i = 1; i < quest->boss_group_offset.size(); i++) // update the start values
			quest->boss_group_offset[i].start = quest->boss_group_offset[i-1].count + quest->boss_group_offset[i-1].start;
		PlusLib::write_all("Boss added\n");
	}
	else if(!add && quest->boss_count > 0 && bid >= 0 && bid < (int32_t)quest->boss_group_offset[n].count)
	{
		deleteBuffer(quest->boss_group_offset[n].offset + bid * 40, 40);

		for(uint32_t i = quest->boss_group_offset[n].start+bid+1; i < quest->boss_count; i++)
		{
			quest->boss_stats[i-1] = quest->boss_stats[i];
			quest->boss_group[i-1] = quest->boss_group[i];
		}
		quest->boss_stats[4] = Quest_monster_stats();
		quest->boss_count--;
		quest->boss_group_offset[n].count--;
		updateOffset(quest->boss_group_offset[n].offset, -40);

		for(uint32_t i = 1; i < quest->boss_group_offset.size(); i++)
			quest->boss_group_offset[i].start = quest->boss_group_offset[i-1].count + quest->boss_group_offset[i-1].start;
		PlusLib::write_all("Boss deleted\n");
	}
	else return false;
	return true;
}

bool MH4U_QE::modifyBossGroup(int32_t n, bool add)
{
	if(!quest) return false;

	if(add)
	{
		if(quest->boss_group_offset.size() >= MAX_BOSS_GROUP) return false;

		uint8_t empty_space[40];
		for(uint32_t i = 0; i < 40; i++)
			if((i >= 0 && i < 4) || i == 8) empty_space[i] = 0xFF;
			else empty_space[i] = 0;

		insertBuffer(quest->boss_group_offset.back().offset + (quest->boss_group_offset.back().count+1) * 40, empty_space, 40);
		updateOffset(quest->boss_group_offset.back().offset + (quest->boss_group_offset.back().count+1) * 40-1, 40);

		Boss_group_offset tmp_offset;
		tmp_offset.offset = quest->boss_group_offset.back().offset + (quest->boss_group_offset.back().count+1) * 40;
		tmp_offset.count = 0;
		tmp_offset.start = quest->boss_group_offset.back().count + quest->boss_group_offset.back().start;
		quest->boss_group_offset.push_back(tmp_offset);

		uint32_t empty_off = 0;
		insertBuffer(quest->boss_offset + (quest->boss_group_offset.size()-1) * 4, (uint8_t*)&empty_off, 4);
		updateOffset(quest->boss_offset, 4);
		PlusLib::write_all("Boss group offset: space added\n");

		PlusLib::write_all("Boss group added\n");
	}
	else if(!add)
	{
		if(quest->boss_group_offset.size() <= 1 || n < 0 || n >= (int32_t)quest->boss_group_offset.size()) return false;
		if(quest->boss_group_offset[n].count > 0) return false;

		deleteBuffer(quest->boss_group_offset[n].offset, 40);
		updateOffset(quest->boss_group_offset[n].offset, -40);

        deleteBuffer(quest->boss_offset + 4 * n, 4);
		updateOffset(quest->boss_offset, -4);

		quest->boss_group_offset.erase(quest->boss_group_offset.begin()+n);
		for(uint32_t i = 1; i < quest->boss_group_offset.size(); i++)
			quest->boss_group_offset[i].start = quest->boss_group_offset[i-1].count + quest->boss_group_offset[i-1].start;
		PlusLib::write_all("Boss group deleted\n");
	}
	return true;
}

bool MH4U_QE::moveBossToGroup(int32_t n, int32_t b, int32_t g)
{
	if(!quest || g < 0 || g >= (int32_t)quest->boss_group_offset.size()
	   || n < 0 || n >= (int32_t)quest->boss_group_offset.size()
	   || b < 0 || b >= (int32_t)quest->boss_group_offset[n].count) return false;

	if(n == g) return true; // if same, return

	Quest_boss_group tmp_boss = quest->boss_group[b+quest->boss_group_offset[n].start]; // get a copy of the boss

	Quest_monster_stats scopy[5], tmp1; // stat shifting
	uint32_t tmp2;

	tmp1 = quest->boss_stats[b+quest->boss_group_offset[n].start];
	if(g > n) tmp2 = quest->boss_group_offset[g].start+quest->boss_group_offset[g].count-1;
	else tmp2 = quest->boss_group_offset[g].start+quest->boss_group_offset[g].count;
	for(uint32_t i = 0; i < 5; i++)
	{
		if(i > tmp2)
		{
			if(i <= b+quest->boss_group_offset[n].start) scopy[i] = quest->boss_stats[i-1];
			else if(i > b+quest->boss_group_offset[n].start) scopy[i] = quest->boss_stats[i];
		}
		else
		{
			if(i < b+quest->boss_group_offset[n].start) scopy[i] = quest->boss_stats[i];
			else if(i >= b+quest->boss_group_offset[n].start) scopy[i] = quest->boss_stats[i+1];
		}
	}
	scopy[tmp2] = tmp1;

	//--------------------------------------------------------
	deleteBuffer(quest->boss_group_offset[n].offset + b * 40, 40); // delete the boss

	for(uint32_t i = quest->boss_group_offset[n].start+b+1; i < quest->boss_count; i++)
		quest->boss_group[i-1] = quest->boss_group[i]; // remove the boss from the struct

	quest->boss_count--; // one boss removed from the total
	quest->boss_group_offset[n].count--; // one boss removed from the group
	updateOffset(quest->boss_group_offset[n].offset, -40); // update the offsets

	for(uint32_t i = 1; i < quest->boss_group_offset.size(); i++) // update the starts value
		quest->boss_group_offset[i].start = quest->boss_group_offset[i-1].count + quest->boss_group_offset[i-1].start;

	//--------------------------------------------------------

	uint8_t empty_boss[40];
	for(uint32_t i = 0; i < 40; i++) empty_boss[i] = 0;
	insertBuffer(quest->boss_group_offset[g].offset + quest->boss_group_offset[g].count * 40, empty_boss, 40); // insert new space

	for(int32_t i = quest->boss_count - 1; i >= (int32_t)(quest->boss_group_offset[g].count + quest->boss_group_offset[g].start); i--) // make space in boss_group
		quest->boss_group[i+1] = quest->boss_group[i];

	quest->boss_group[quest->boss_group_offset[g].count + quest->boss_group_offset[g].start] = tmp_boss; // insert the copy
	quest->boss_count++; // one boss added to the total
	quest->boss_group_offset[g].count++; // one boss added to the group
	updateOffset(quest->boss_group_offset[g].offset, 40); // update the offsets
	for(uint32_t i = 1; i < quest->boss_group_offset.size(); i++) // update the start values
		quest->boss_group_offset[i].start = quest->boss_group_offset[i-1].count + quest->boss_group_offset[i-1].start;
	for(uint32_t i = 0; i < 5; i++) quest->boss_stats[i] = scopy[i]; // write new stats
	PlusLib::write_all("Boss moved\n");
	return true;
}

bool MH4U_QE::modifyIntruder(int32_t n, bool add)
{
	if(!quest) return false;
	if(quest->intruder_offset == 0) return false;

	if(add)
	{
		if(quest->intruder.size() >= MAX_INTRUDER) return false;

		uint8_t empty_boss[44];
		for(uint32_t i = 0; i < 44; i++) empty_boss[i] = 0;
		insertBuffer(quest->intruder_offset + quest->intruder.size() * 44, empty_boss, 44); // create empty space


		if(quest->intruder.empty())
        {
            quest->boss_stats[2].size_percentage = 100;
            quest->boss_stats[2].size_variation = 0;
            quest->boss_stats[2].health = 10;
            quest->boss_stats[2].attack = 30;
            quest->boss_stats[2].defense = 20;
            quest->boss_stats[2].stamina = 0;
            quest->boss_stats[2].unknown = 0;
        }

		Quest_intruder tmp;
		tmp.probability = 100;
		tmp.boss_type = 1;
		tmp.coordinates[0] = 0;
		tmp.coordinates[1] = 0;
		tmp.coordinates[2] = 0;
		tmp.frenzy = 0;
		tmp.spawn_area = 1;
		tmp.spawn_condition = 0xFF;
		tmp.spawn_quantity = 1;
		tmp.unknown0 = 0;
        tmp.form = 0;
        tmp.size_related2 = 0;
        tmp.unknown1 = 0;
		tmp.angles[0] = 0;
		tmp.angles[1] = 0;
		tmp.angles[2] = 0;

		quest->intruder.push_back(tmp);
		updateOffset(quest->intruder_offset, 44);
		PlusLib::write_all("Intruder added\n");
	}
	else if(!add)
	{
		if(quest->intruder.size() <= 0) return false;

		deleteBuffer(quest->intruder_offset + n * 44, 44);
		quest->intruder.erase(quest->intruder.begin()+n);
		updateOffset(quest->intruder_offset, -44);

		PlusLib::write_all("Intruder deleted\n");
	}
	return true;
}

bool MH4U_QE::modifyMonster(int32_t n, bool add, int32_t bid)
{
	if(!quest || n < 0 || n >= (int32_t)quest->monster_group_list.size()) return false;
	Small_monster_group* ptr = quest->monster_group_list[n];
	if(ptr == NULL) return false;
	if(add)
	{
		uint8_t empty_monster[40];
		for(uint32_t i = 0; i < 40; i++) empty_monster[i] = 0;
		insertBuffer(ptr->offset + ptr->mon.size() * 40, empty_monster, 40); // create empty space

		Quest_monster_group tmp;
		tmp.monster_type = 50;
		tmp.coordinates[0] = 0;
		tmp.coordinates[1] = 0;
		tmp.coordinates[2] = 0;
		tmp.spawn_area = 1;
		tmp.spawn_condition = 0xFF;
		tmp.spawn_quantity = 1;
		tmp.crash_flag = 5;
		tmp.form = 0;
        tmp.size_related1 = 0;
        tmp.size_related2 = 0;
        tmp.unknown1 = 0;
		tmp.angles[0] = 0;
		tmp.angles[1] = 0;
		tmp.angles[2] = 0;

        ptr->mon.push_back(tmp);
		updateOffset(ptr->offset, 40);
		PlusLib::write_all("Monster added\n");
	}
	else if(!add && ptr->mon.size() > 0 && bid >= 0 && bid < (int32_t)ptr->mon.size())
	{
		deleteBuffer(ptr->offset + bid * 40, 40);

		ptr->mon.erase(ptr->mon.begin()+bid);
		updateOffset(ptr->offset, -40);

		PlusLib::write_all("Monster deleted\n");
	}
	else return false;
	return true;
}

bool MH4U_QE::modifyMonsterGroup(int32_t g, int32_t n, bool add)
{
	if(!quest) return false;
	if(g < 0 || g > 2) return false;

	if(add)
	{
		uint8_t empty_space[40];
		for(uint32_t i = 0; i < 40; i++)
			if((i >= 0 && i < 4) || i == 8) empty_space[i] = 0xFF;
			else empty_space[i] = 0;

		insertBuffer(quest->monster_group_list.back()->offset + quest->monster_group_list.back()->mon.size() * 40 + 40, empty_space, 40);
		updateOffset(quest->monster_group_list.back()->offset + quest->monster_group_list.back()->mon.size() * 40 + 39, 40);

		Small_monster_group* ptr = new Small_monster_group();
		ptr->offset = quest->monster_group_list.back()->offset + quest->monster_group_list.back()->mon.size() * 40 + 40;
		ptr->nUse = 1;
		quest->monster_group_list.push_back(ptr);
		quest->possible_monster[g].group.push_back(ptr);

		uint32_t empty_off = 0;
		insertBuffer(quest->possible_monster_offset[g] + quest->possible_monster[g].group.size() * 4, (uint8_t*)&empty_off, 4);
		updateOffset(quest->possible_monster_offset[g], 4);
		PlusLib::write_all("Monster group offset: space added\n");
		PlusLib::write_all("Monster group added at " + PlusLib::int2hex(quest->possible_monster[g].group.back()->offset) + "\n");
	}
	else if(!add)
	{
	    if(n < 0 || n >= (int32_t)quest->possible_monster[g].group.size()) return false;
	    if(quest->monster_group_list.size() == 1 && quest->monster_group_list[0]->nUse <= 1) return false;
	    if(quest->possible_monster[g].group.size() < 2) return false;

        quest->possible_monster[g].group[n]->nUse--;
        if(quest->possible_monster[g].group[n]->nUse == 0)
        {
            while(!quest->possible_monster[g].group[n]->mon.empty())
                if(!modifyMonster(getMonsterGroupPos(quest->possible_monster[g].group[n]), false)) return false;
            deleteBuffer(quest->possible_monster[g].group[n]->offset, 40);
            updateOffset(quest->possible_monster[g].group[n]->offset, -40);
            quest->monster_group_list.erase(quest->monster_group_list.begin()+getMonsterGroupPos(quest->possible_monster[g].group[n]));
            cdelete(quest->possible_monster[g].group[n]);
        }

		deleteBuffer(quest->possible_monster_offset[g] + 4 * n, 4);
		updateOffset(quest->possible_monster_offset[g], -4);

		quest->possible_monster[g].group.erase(quest->possible_monster[g].group.begin()+n);
		PlusLib::write_all("Monster group deleted\n");
	}
	return true;
}

bool MH4U_QE::moveMonsterToGroup(int32_t n, int32_t b, int32_t g)
{
    if(n < 0 || n >= (int32_t)quest->monster_group_list.size() || g < 0 || g >= (int32_t)quest->monster_group_list.size()) return false;
    Small_monster_group* ptr1 = quest->monster_group_list[n];
    Small_monster_group* ptr2 = quest->monster_group_list[g];
	if(!quest || ptr1 != NULL || ptr2 != NULL || b < 0 || b >= (int32_t)ptr1->mon.size()) return false;

	if(n == g) return true; // if same, return

	Quest_monster_group tmp_monster = ptr1->mon[b]; // get a copy of the monster

	//--------------------------------------------------------
	deleteBuffer(ptr1->offset + b * 40, 40); // delete the monster
	ptr1->mon.erase(ptr1->mon.begin()+b); // remove the monster
	updateOffset(ptr1->offset, -40); // update the offsets

	//--------------------------------------------------------

	uint8_t empty_monster[40];
	for(uint32_t i = 0; i < 40; i++) empty_monster[i] = 0;
	insertBuffer(ptr2->offset + ptr2->mon.size() * 40, empty_monster, 40); // insert new space
	ptr2->mon.insert(ptr2->mon.begin()+ptr2->mon.size(), tmp_monster); // add the monster

	updateOffset(ptr2->offset, 40); // update the offsets
	PlusLib::write_all("Monster moved\n");
	return true;
}

Small_monster_group* MH4U_QE::searchMonsterGroup(uint32_t off)
{
    if(!quest) return NULL;

    if(off )

    for(uint32_t i = 0; i < quest->monster_group_list.size(); i++)
    {
        if(quest->monster_group_list[i] != NULL)
        {
            if(quest->monster_group_list[i]->offset == off) return quest->monster_group_list[i];
        }
    }
    return NULL;
}

int32_t MH4U_QE::getMonsterGroupPos(Small_monster_group* ptr)
{
    if(!quest || ptr == NULL) return -1;
    int32_t pos = 0;
    for(uint32_t i = 0; i < quest->monster_group_list.size(); i++)
    {
        if(ptr == quest->monster_group_list[i]) return pos;
        pos++;
    }

    return -1;
}

bool MH4U_QE::pasteMonsterGroup(Small_monster_group* ptr, int32_t g)
{
    if(!quest || getMonsterGroupPos(ptr) == -1 || g < 0 || g > 2) return false;
    quest->possible_monster[g].group.push_back(ptr);
    ptr->nUse++;
    uint32_t empty_off = 0;
    insertBuffer(quest->possible_monster_offset[g] + quest->possible_monster[g].group.size() * 4, (uint8_t*)&empty_off, 4);
    updateOffset(quest->possible_monster_offset[g], 4);
    PlusLib::write_all("Monster group offset: space added\n");
    return true;
}

bool MH4U_QE::unbindMonsterGroup(int32_t g)
{
    if(!quest) return false;
    if(g < 1 && g > 2) return false;
    if(quest->possible_monster[g].isCopyOf == -1) return false;

    PlusLib::write_all("Monster group unbind: subgroup " + PlusLib::int2str(g) + " is the same as " + PlusLib::int2str(quest->possible_monster[g].isCopyOf) + "\n");

    uint32_t offset = quest->possible_monster_offset[quest->possible_monster[g].isCopyOf] + (quest->possible_monster[quest->possible_monster[g].isCopyOf].group.size()+1)*4;

    PlusLib::write_all("Monster group unbind: offset " + PlusLib::int2hex(offset) + "\n");

    uint32_t empty_off = 0;
    insertBuffer(offset, (uint8_t*)&empty_off, 4);
    insertBuffer(offset, (uint8_t*)&empty_off, 4);
    updateOffset(offset-1, 8);
    quest->possible_monster[g].isCopyOf = - 1;
    quest->possible_monster_offset[g] = offset;
    modifyMonsterGroup(g, 0, true);
    return true;
}

bool MH4U_QE::modifyReward(int32_t n, bool add)
{
    if(!quest) return false;
    if(n < 0 || n > 2) return false;
    if(add)
    {
        uint8_t array[6];
        for(int32_t i = 0; i < 6; i++) array[i] = 0x00;

        uint32_t tmp_value = quest->reward_box[n].reward_items_offset + (quest->reward_box[n].reward_items.size() * 6);
        insertBuffer(tmp_value, array, 6);
        updateOffset(tmp_value, 6);

        PlusLib::write_all("MH4U_QE:: Reward Item add:: Offset = " + PlusLib::int2hex(tmp_value) + "\n");

        quest->reward_box[n].reward_items.push_back(Quest_reward_items());
        quest->reward_box[n].reward_items.back().chance = 0;
        quest->reward_box[n].reward_items.back().item_id = 8;
        quest->reward_box[n].reward_items.back().count = 1;

        PlusLib::write_all("MH4U_QE:: Item added to reward line " + PlusLib::int2str(n+1) + "\n");
    }
    else
    {
        if(quest->reward_box[n].reward_items.empty()) return false;
        uint32_t tmp_value = quest->reward_box[n].reward_items_offset + ((quest->reward_box[n].reward_items.size()-1) * 6);
        deleteBuffer(tmp_value, 6);
        updateOffset(tmp_value, -6);
        quest->reward_box[n].reward_items.pop_back();

        PlusLib::write_all("MH4U_QE:: Item removed from reward line " + PlusLib::int2str(n+1) + "\n");
    }

    return true;
}

bool MH4U_QE::unbindReward(int32_t n)
{
    if(!quest) return false;
    if(n < 1 || n > 2) return false;
    if(quest->reward_box_offset[n] != 0xFFFFFFFF && quest->reward_box_offset[n] != 0xFFFFFFFE) return false;

    int32_t cpy = 0;
    if(quest->reward_box_offset[n] == 0xFFFFFFFF) cpy = 1;
    else if(quest->reward_box_offset[n] == 0xFFFFFFFE) cpy = 0;
    PlusLib::write_all("MH4U_QE:: Unbind:: #" + PlusLib::int2str(n) + " is binded to #" + PlusLib::int2str(cpy) + "\n");

    int32_t bsize = (quest->reward_box[cpy].reward_items.size() * 6) + 16;
    if(bsize % 16 != 0) bsize += (16 - (bsize % 16));
    if(quest->reward_box[cpy].reward_items_offset % 16 != 0) bsize -= (quest->reward_box[cpy].reward_items_offset % 16);

    while(buffer[quest->reward_box[cpy].reward_items_offset+bsize] == 0x00) bsize++;
    bsize -= (bsize % 4);
    PlusLib::write_all("MH4U_QE:: Unbind:: Copy size = " + PlusLib::int2str(bsize) + "\n");

    unsigned char *array = new unsigned char[bsize];

    for(int32_t i = 0; i < bsize; i++) array[i] = 0x00;

    array[(quest->reward_box[cpy].reward_items.size() * 6)] = 0xFF;
    array[(quest->reward_box[cpy].reward_items.size() * 6)+1] = 0xFF;
    array[(quest->reward_box[cpy].reward_items.size() * 6)+12] = 0xFF;
    array[(quest->reward_box[cpy].reward_items.size() * 6)+13] = 0xFF;


    uint32_t tmp_value = quest->reward_box[cpy].reward_items_offset + bsize;
    PlusLib::write_all("MH4U_QE:: Unbind:: New offset = " + PlusLib::int2hex(tmp_value) + "\n");

    insertBuffer(tmp_value, array, bsize);
    updateOffset(tmp_value-1, bsize);


    quest->reward_box[n] = quest->reward_box[cpy];

    quest->reward_box[n].unknown0 = quest->reward_box[cpy].unknown0;
    quest->reward_box[n].unknown1 = quest->reward_box[cpy].unknown1;
    quest->reward_box[n].unknown2 = quest->reward_box[cpy].unknown2;

    quest->reward_box[n].reward_items.clear();
    for(uint32_t i = 0; i < quest->reward_box[cpy].reward_items.size(); i++)
        quest->reward_box[n].reward_items.push_back(quest->reward_box[cpy].reward_items[i]);

    quest->reward_box[n].reward_items_offset = tmp_value;
    quest->reward_box_offset[n] = quest->reward_box[n].reward_items_offset + (quest->reward_box[n].reward_items.size() * 6) + 4;
    PlusLib::write_all("MH4U_QE:: Unbind of reward line " + PlusLib::int2str(n+1) + ": Success\n");


    delete [] array;
    return true;
}

bool MH4U_QE::addReward(int32_t n)
{
    if(!quest) return false;
    if(n < 1 || n > 2) return false;
    if(quest->reward_box_offset[n] != 0) return false;

    uint8_t array[32];

    for(int32_t i = 0; i < 32; i++) array[i] = 0x00;

    array[0] = 0xFF;
    array[1] = 0xFF;
    array[12] = 0xFF;
    array[13] = 0xFF;

    int32_t bsize = (quest->reward_box[n-1].reward_items.size() * 6) + 16;
    if(bsize % 16 != 0) bsize += (16 - (bsize % 16));
    if(quest->reward_box[n-1].reward_items_offset % 16 != 0) bsize -= (quest->reward_box[n-1].reward_items_offset % 16);

    while(buffer[quest->reward_box[n-1].reward_items_offset+bsize] == 0x00) bsize++;
    bsize -= (bsize % 4);

    uint32_t tmp_value = quest->reward_box[n-1].reward_items_offset + bsize;
    PlusLib::write_all("MH4U_QE:: Add reward line:: New offset = " + PlusLib::int2hex(tmp_value) + "\n");

    insertBuffer(tmp_value, array, 32);
    updateOffset(tmp_value-1, 32);

    quest->reward_box[n].unknown0 = 0;
    quest->reward_box[n].unknown1 = 0x80;
    quest->reward_box[n].unknown2 = 0;
    quest->reward_box[n].reward_items.clear();
    quest->reward_box[n].reward_items_offset = tmp_value;
    quest->reward_box_offset[n] = quest->reward_box[n].reward_items_offset + 4;
    PlusLib::write_all("MH4U_QE:: Add reward line:: Global offset = " + PlusLib::int2hex(quest->reward_box_offset[n]) + "\n");
    PlusLib::write_all("MH4U_QE:: Add reward line " + PlusLib::int2str(n+1) + ": Success\n");

    return true;
}

bool MH4U_QE::modifySupplyGroup(bool add)
{
    if(!quest) return false;
    if(quest->supply_box_offset == 0) return false;

    if(add)
    {
        Supply_box_group tmp;
        tmp.id = quest->supply_box.size();
        tmp.count = 0;
        tmp.unknown = 0;
        tmp.offset = 0;
        tmp.start = 0;

        uint8_t array[8];
        for(size_t i = 0; i < 8; i++) array[i] = 0;
        insertBuffer(quest->supply_box_offset+8*quest->supply_box.size(), array, 8);
        updateOffset(quest->supply_box_offset, 8);
        quest->supply_box.push_back(tmp);
        PlusLib::write_all("MH4U_QE:: Added supply group\n");
    }
    else
    {
        if(quest->supply_box.size() < 2) return false;
        deleteBuffer(quest->supply_box_offset+8*(quest->supply_box.size()-1), 8);
        updateOffset(quest->supply_box_offset, -8);
        quest->supply_box.pop_back();
        PlusLib::write_all("MH4U_QE:: Deleted supply group\n");
    }
    return true;
}

bool MH4U_QE::modifySupplyItem(bool add)
{
    if(!quest) return false;
    if(quest->supply_box_offset == 0) return false;

    if(add)
    {
        if(quest->supply_list.items.size() >= MAX_SUPPLY) return false;

        Quest_supply_items tmp;
        tmp.item_id = 0;
        tmp.count = 0;

        uint8_t array[4];
        for(size_t i = 0; i < 4; i++) array[i] = 0;
        insertBuffer(quest->supply_list.start_offset+4*quest->supply_list.items.size(), array, 4);
        updateOffset(quest->supply_list.start_offset, 4);
        quest->supply_list.items.push_back(tmp);
        PlusLib::write_all("MH4U_QE:: Added supply item\n");
    }
    else
    {
        if(quest->supply_list.items.size() < 2) return false;
        deleteBuffer(quest->supply_list.start_offset+4*(quest->supply_list.items.size()-1), 4);
        updateOffset(quest->supply_list.start_offset, -4);
        quest->supply_list.items.pop_back();
        PlusLib::write_all("MH4U_QE:: Deleted supply item\n");
    }
    return true;
}

bool MH4U_QE::modifyChallenge(bool add)
{
    if(!quest) return false;

    if(add)
    {
        if(quest->challenge_quest_offset != 0) return false;
        uint32_t off = quest->text_offset + 16;
        off += (16 - (off % 16));
        quest->preset_count = 5;

        uint8_t *tmp = new uint8_t[352*quest->preset_count];
        for(uint16_t i = 0; i < 352*quest->preset_count; i++)
            tmp[i] = 0;

        insertBuffer(off, tmp, 352*quest->preset_count);
        updateOffset(off, 352*quest->preset_count);
        quest->challenge_quest_offset = off;

        adelete(tmp);
    }
    else
    {
        if(quest->challenge_quest_offset == 0) return false;
        deleteBuffer(quest->challenge_quest_offset, 352*quest->preset_count);
        updateOffset(quest->challenge_quest_offset, -352*quest->preset_count);
        quest->challenge_quest_offset = 0;
    }
    return true;
}

void MH4U_QE::updateOffset(uint32_t start, int32_t value)
{
	if(!quest) return;

    PlusLib::write_all("Update offsets from " + PlusLib::int2hex(start) + " - Change: " + PlusLib::int2str(value) + "\n");
	std::vector<uint32_t*> offset_table;
	offset_table.push_back(&quest->flags_offset);
	offset_table.push_back(&quest->supply_box_offset);
	offset_table.push_back(&quest->reward_box_offset[0]);
	offset_table.push_back(&quest->reward_box_offset[1]);
	offset_table.push_back(&quest->reward_box_offset[2]);
	offset_table.push_back(&quest->boss_offset);
	offset_table.push_back(&quest->monster_offset);
	offset_table.push_back(&quest->intruder_offset);
	offset_table.push_back(&quest->text_offset);
	offset_table.push_back(&quest->challenge_quest_offset);

	offset_table.push_back(&quest->supply_list.start_offset);

	offset_table.push_back(&quest->reward_box[0].reward_items_offset);
	offset_table.push_back(&quest->reward_box[1].reward_items_offset);
	offset_table.push_back(&quest->reward_box[2].reward_items_offset);

	for(uint32_t i = 0; i < quest->boss_group_offset.size(); i++)
		offset_table.push_back(&quest->boss_group_offset[i].offset);

    for(uint32_t i = 0; i < 3; i++)
        offset_table.push_back(&quest->possible_monster_offset[i]);

    for(uint32_t i = 0; i < quest->monster_group_list.size(); i++)
        if(quest->monster_group_list[i] != NULL) offset_table.push_back(&quest->monster_group_list[i]->offset);

	for(uint32_t i = 0; i < 5; i++)
	{
		offset_table.push_back(&quest->quest_text_offset.language_offset[i]);
		offset_table.push_back(&quest->language_offset[i].title_offset);
		offset_table.push_back(&quest->language_offset[i].success_offset);
		offset_table.push_back(&quest->language_offset[i].failure_offset);
		offset_table.push_back(&quest->language_offset[i].summary_offset);
		offset_table.push_back(&quest->language_offset[i].main_monsters_offset);
		offset_table.push_back(&quest->language_offset[i].client_offset);
		offset_table.push_back(&quest->language_offset[i].sub_quest_offset);
	}

	for(uint32_t i = 0; i < offset_table.size(); i++)
	{
		if((*offset_table[i]) < 0xFFFFFFFE && (*offset_table[i]) != 0 && (*offset_table[i]) > start)
			(*offset_table[i]) += value;
	}
}
