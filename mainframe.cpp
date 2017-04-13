#include "mainframe.hpp"
#include "pluslib.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <wx/dnd.h>
#include <wx/dir.h>
#include <wx/tokenzr.h>
#include <math.h>
#include <time.h>
#include "mydialog.hpp"
#define PI 3.141592653589793

// list
wxArrayString map_list, term_list, monster_list, icon_list, item_list, arena_item_list, palico_skill_list, arena_list[26], frenzy_list, gq_boss_list;
std::vector<uint16_t> item_table, i_item_table, arena_item_table, i_arena_item_table, monster_table, i_monster_table, map_table, i_map_table, icon_table, i_icon_table, palico_skill_table, i_palico_skill_table, id_list, frenzy_table, i_frenzy_table, icon_translate, gq_boss_table, i_gq_boss_table;
std::map<uint16_t, std::vector<uint16_t> > alternative_list;
std::map<uint16_t, wxArrayString> alternative_text;
std::vector<XYZ> coordinate[125][22][20];
std::string parts[124][8];

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(ID_BTN_LOAD, MainFrame::OnBtnLoad)
    EVT_BUTTON(ID_BTN_SAVE, MainFrame::OnBtnSave)
    EVT_BUTTON(ID_BTN_SAVEAS, MainFrame::OnBtnSaveAs)
    EVT_BUTTON(ID_BTN_ENCRYPTNOW, MainFrame::OnBtnEncryptNow)
    EVT_BUTTON(ID_BTN_ENCRYPTAS, MainFrame::OnBtnEncryptAs)
    EVT_CHOICE(ID_CHO_MENU, MainFrame::OnChoMenu)
    EVT_CHOICE(ID_CHO_QUEST_TYPE, MainFrame::OnChoQuestType)
    EVT_CHOICE(ID_CHO_BGROUP, MainFrame::OnChoBossGroup)
    EVT_CHOICE(ID_CHO_BOSS, MainFrame::OnChoBoss)
    EVT_BUTTON(ID_BTN_BOSS_HELP1, MainFrame::OnBtnBossHelp1)
    EVT_BUTTON(ID_BTN_BOSS_HELP2, MainFrame::OnBtnBossHelp2)
    EVT_CHOICE(ID_CHO_MGROUP, MainFrame::OnChoMonsterGroup)
    EVT_CHOICE(ID_CHO_MONSTER, MainFrame::OnChoMonster)
    EVT_CHOICE(ID_CHO_PRESET, MainFrame::OnChoPreset)
    EVT_CHOICE(ID_CHO_EQUIPEMENT, MainFrame::OnChoEquipement)
    EVT_CHOICE(ID_CHO_INTRUDER, MainFrame::OnChoIntruder)
    EVT_BUTTON(ID_BTN_BOSS_ADD, MainFrame::OnBtnAddBoss)
    EVT_BUTTON(ID_BTN_BOSS_DEL, MainFrame::OnBtnDelBoss)
    EVT_BUTTON(ID_BTN_INTRUDER_ADD, MainFrame::OnBtnAddIntruder)
    EVT_BUTTON(ID_BTN_INTRUDER_DEL, MainFrame::OnBtnDelIntruder)
    EVT_BUTTON(ID_BTN_DECRYPT, MainFrame::OnBtnDecrypt)
    EVT_BUTTON(ID_BTN_ENCRYPT, MainFrame::OnBtnEncrypt)
    EVT_BUTTON(ID_BTN_TOOL_IN, MainFrame::OnBtnToolIn)
    EVT_BUTTON(ID_BTN_TOOL_OUT, MainFrame::OnBtnToolOut)
    EVT_BUTTON(ID_BTN_ADD_BOSS_GROUP, MainFrame::OnBtnAddBossGroup)
    EVT_BUTTON(ID_BTN_DEL_BOSS_GROUP, MainFrame::OnBtnDelBossGroup)
    EVT_BUTTON(ID_BTN_BOSS_MOVE, MainFrame::OnBtnMoveBoss)
    EVT_BUTTON(ID_BTN_REWARD_A_HELP, MainFrame::OnBtnRewardAHelp)
    EVT_BUTTON(ID_BTN_MONSTER_ADD, MainFrame::OnBtnAddMonster)
    EVT_BUTTON(ID_BTN_MONSTER_DEL, MainFrame::OnBtnDelMonster)
    EVT_BUTTON(ID_BTN_ADD_MONSTER_GROUP, MainFrame::OnBtnAddMonsterGroup)
    EVT_BUTTON(ID_BTN_DEL_MONSTER_GROUP, MainFrame::OnBtnDelMonsterGroup)
    EVT_BUTTON(ID_BTN_UNBIND_MONSTER_GROUP, MainFrame::OnBtnUnbindMonsterGroup)
    EVT_BUTTON(ID_BTN_MONSTER_MOVE, MainFrame::OnBtnMoveMonster)
    EVT_BUTTON(ID_BTN_CPY_TEXT_TITLE, MainFrame::OnBtnCopyTitle)
    EVT_BUTTON(ID_BTN_CPY_TEXT_SUCCESS, MainFrame::OnBtnCopySuccess)
    EVT_BUTTON(ID_BTN_CPY_TEXT_FAILURE, MainFrame::OnBtnCopyFailure)
    EVT_BUTTON(ID_BTN_CPY_TEXT_SUMMARY, MainFrame::OnBtnCopySummary)
    EVT_BUTTON(ID_BTN_CPY_TEXT_MAIN_MONSTERS, MainFrame::OnBtnCopyMainMonsters)
    EVT_BUTTON(ID_BTN_CPY_TEXT_CLIENT, MainFrame::OnBtnCopyClient)
    EVT_BUTTON(ID_BTN_CPY_TEXT_SUB_QUEST, MainFrame::OnBtnCopySubquest)
    EVT_BUTTON(ID_BTN_PASTE_TEXT_TITLE, MainFrame::OnBtnPasteTitle)
    EVT_BUTTON(ID_BTN_PASTE_TEXT_SUCCESS, MainFrame::OnBtnPasteSuccess)
    EVT_BUTTON(ID_BTN_PASTE_TEXT_FAILURE, MainFrame::OnBtnPasteFailure)
    EVT_BUTTON(ID_BTN_PASTE_TEXT_SUMMARY, MainFrame::OnBtnPasteSummary)
    EVT_BUTTON(ID_BTN_PASTE_TEXT_MAIN_MONSTERS, MainFrame::OnBtnPasteMainMonsters)
    EVT_BUTTON(ID_BTN_PASTE_TEXT_CLIENT, MainFrame::OnBtnPasteClient)
    EVT_BUTTON(ID_BTN_PASTE_TEXT_SUB_QUEST, MainFrame::OnBtnPasteSubquest)
    EVT_BUTTON(ID_BTN_CREATE, MainFrame::OnBtnCreateQuest)
    EVT_BUTTON(ID_BTN_UNBIND_REWARD, MainFrame::OnBtnUnbindReward)
    EVT_BUTTON(ID_BTN_ADD_RITEM, MainFrame::OnBtnAddRItem)
    EVT_BUTTON(ID_BTN_DEL_RITEM, MainFrame::OnBtnDelRItem)
    EVT_BUTTON(ID_BTN_ADD_REWARD, MainFrame::OnBtnAddReward)
    EVT_BUTTON(ID_BTN_TOOL_PROXY_FILES, MainFrame::OnBtnProxyFiles)
    EVT_BUTTON(ID_BTN_TOOL_GEN_FILES, MainFrame::OnBtnGenerateProxyFiles)
    EVT_BUTTON(ID_BTN_ADD_SUPPLY_GROUP, MainFrame::OnBtnAddSupplyGroup)
    EVT_BUTTON(ID_BTN_ADD_SUPPLY_ITEM, MainFrame::OnBtnAddSupplyItem)
    EVT_BUTTON(ID_BTN_DEL_SUPPLY_GROUP, MainFrame::OnBtnDelSupplyGroup)
    EVT_BUTTON(ID_BTN_DEL_SUPPLY_ITEM, MainFrame::OnBtnDelSupplyItem)
    EVT_BUTTON(ID_BTN_SUPPLY_HELP, MainFrame::OnBtnSupplyHelp)
    EVT_TEXT(ID_TEXT_SUPPLY_COUNT, MainFrame::OnTextSupplyCount)
    EVT_CHOICE(ID_CHO_MONSTER_CONDITION, MainFrame::OnChoGroupConditionType)
    EVT_CHOICE(ID_CHO_BOSS_ID, MainFrame::OnChoUpdateBossID)
    EVT_CHOICE(ID_CHO_MONSTER_ID, MainFrame::OnChoUpdateMonsterID)
    EVT_CHOICE(ID_CHO_INTRUDER_ID, MainFrame::OnChoUpdateIntruderID)
    EVT_BUTTON(ID_BTN_PASTE_SUBGROUP, MainFrame::OnBtnPasteMonsterSubgroup)
    EVT_BUTTON(ID_BTN_CPY_SUBGROUP, MainFrame::OnBtnCpyMonsterSubgroup)
    EVT_BUTTON(ID_BTN_MONSTER_HELP, MainFrame::OnBtnHelpMonster)
    EVT_BUTTON(ID_BTN_NEW_P, MainFrame::OnBtnNewPalico)
    EVT_BUTTON(ID_BTN_LOAD_P, MainFrame::OnBtnLoadPalico)
    EVT_BUTTON(ID_BTN_SAVE_P, MainFrame::OnBtnSavePalico)
    EVT_BUTTON(ID_BTN_SAVEAS_P, MainFrame::OnBtnSaveAsPalico)
    EVT_BUTTON(ID_BTN_ENCRYPTNOW_P, MainFrame::OnBtnEncryptNowPalico)
    EVT_BUTTON(ID_BTN_ENCRYPTAS_P, MainFrame::OnBtnEncryptAsPalico)
    EVT_COLOURPICKER_CHANGED(ID_COLOR_PICK, MainFrame::OnColorPick)
    EVT_TEXT(ID_TEXT_QUEST_ID, MainFrame::OnTextQuestID)
    EVT_BUTTON(ID_BTN_PLACE_BOSS, MainFrame::OnBtnPlaceBoss)
    EVT_BUTTON(ID_BTN_PLACE_MONSTER, MainFrame::OnBtnPlaceMonster)
    EVT_BUTTON(ID_BTN_PLACE_INTRUDER, MainFrame::OnBtnPlaceIntruder)
    EVT_CHOICE(ID_CHO_QUEST_TARGET, MainFrame::OnChoUpdatePartBreak)
    EVT_CHOICE(ID_CHO_CREATE_OBJTYPE, MainFrame::OnChoCreateObjType)
    EVT_CHOICE(ID_CHO_CREATE_OBJID, MainFrame::OnChoCreateObjID)
    EVT_BUTTON(ID_BTN_GO_CREATE, MainFrame::OnBtnGoCreate)
    EVT_BUTTON(ID_BTN_GO_NEW, MainFrame::OnBtnGoNew)
    EVT_BUTTON(ID_BTN_LOAD_S, MainFrame::OnBtnLoadSave)
    EVT_BUTTON(ID_BTN_SAVE_S, MainFrame::OnBtnSaveSave)
    EVT_BUTTON(ID_BTN_SAVEAS_S, MainFrame::OnBtnSaveAsSave)
    EVT_BUTTON(ID_BTN_EDIT_EQUIP, MainFrame::OnBtnEditEquip)
    EVT_CHOICE(ID_CHO_UPDATE_SAVE, MainFrame::OnChoUpdateSave)
    EVT_CHOICE(ID_CHO_UPDATE_GQ, MainFrame::OnChoUpdateGQ)
    EVT_BUTTON(ID_BTN_ENCRYPTNOW_S, MainFrame::OnBtnEncryptNowSave)
    EVT_BUTTON(ID_BTN_ENCRYPTAS_S, MainFrame::OnBtnEncryptAsSave)
    EVT_BUTTON(ID_BTN_CHALLENGE_EQUIP, MainFrame::OnBtnEditChallengeEquip)
    EVT_BUTTON(ID_ADD_PRESET, MainFrame::OnBtnAddChallenge)
    EVT_BUTTON(ID_DEL_PRESET, MainFrame::OnBtnDelChallenge)
END_EVENT_TABLE()

float rad2deg(float i)
{
    return i * 180.0 / PI;
}

float deg2rad(float i)
{
    return i * PI / 180.0;
}

/*#include <algorithm>
void sorting()
{
    std::vector<std::string> list1, list2;
    std::string tmp;

    std::ifstream f("data/palico_skill.txt", std::ios::in);
    if(!f) return;

    while(f.good())
    {
        PlusLib::getline(f, tmp);
        list1.push_back(tmp);
    }
    f.close();
    list2 = list1;
    std::sort(list1.begin(), list1.end());

    std::ofstream o("data/list.txt", std::ios::out | std::ios::trunc);
    std::ofstream n("data/table.txt", std::ios::out | std::ios::trunc);
    if(!o || !n) return;

    for(int i = 0; i < list1.size(); i++)
    {
        for(int j = 0; j < list2.size(); j++)
        {
            if(list1[i] == list2[j])
            {
                o << list1[i] << "\n";
                n << PlusLib::int2str(j) << "\n";
                j = list2.size();
            }
        }
    }
}*/

std::string wxStr2Dec(wxString i)
{
    std::stringstream o;
    for(size_t j = 0; j < i.size(); j++)
    {
        o << (int)i[j];
        if(j < i.size() - 1) o << ".";
    }
    return o.str();
}

void vTab(int32_t &a1, int32_t a2, std::vector<uint16_t> &t)
{
    if(a2 < 0 || a2 >= (int32_t)t.size()) return;
    a1 = t[a2];
}

wxString Dec2Str(std::string i)
{
    wxString o;
    std::vector<std::string> elems;
    PlusLib::split(i, '.', elems);
    for(size_t j = 0; j < elems.size(); j++)
        o.append((wxChar)PlusLib::str2short(elems[j]));
    return o;
}

int16_t getPosInVector(const std::vector<uint16_t> &v, const uint16_t &value)
{
    for(unsigned int i = 0; i < v.size(); i++)
        if(v[i] == value) return i;
    return -1;
}

std::map<uint16_t, std::vector<uint16_t> > loadSpecialList(std::string filename)
{
    std::ifstream f;
    try
    {
        f.open(filename.c_str(), std::ios::in);
        if(!f)
        {
            PlusLib::write_all("MH4U_QE:: Failed to open " + filename + "\n");
            return std::map<uint16_t, std::vector<uint16_t> >();
        }

        std::map<uint16_t, std::vector<uint16_t> > table;
        std::string tmp;
        while(f.good())
        {
            PlusLib::getline(f, tmp);
            std::vector<std::string> elems;
            PlusLib::split(tmp, '=', elems);
            if(elems.size() > 1)
            {
                if(table.find(PlusLib::str2int(elems[0])) ==  table.end())
                {
                    table[PlusLib::str2int(elems[0])] = std::vector<uint16_t>();
                    table[PlusLib::str2int(elems[0])].push_back(PlusLib::str2int(elems[1]));
                }
                else
                {
                    table.find(PlusLib::str2int(elems[0]))->second.push_back(PlusLib::str2int(elems[1]));
                }
            }
        }
        return table;
    }
    catch(std::istream::failure e)
    {
        PlusLib::write_all("MH4U_QE:: Exception opening/reading " + filename + "\n");
        PlusLib::write_all("MH4U_QE:: Explanatory string: " + std::string(e.what()) + "\n");
        return std::map<uint16_t, std::vector<uint16_t> >();
    }
}

std::map<uint16_t, wxArrayString> loadSpecialListText(std::string filename)
{
    std::ifstream f;
    try
    {
        f.open(filename.c_str(), std::ios::in);
        if(!f)
        {
            PlusLib::write_all("MH4U_QE:: Failed to open " + filename + "\n");
            return std::map<uint16_t, wxArrayString>();
        }

        std::map<uint16_t, wxArrayString> table;
        std::string tmp;
        while(f.good())
        {
            PlusLib::getline(f, tmp);
            std::vector<std::string> elems;
            PlusLib::split(tmp, '=', elems);
            if(elems.size() > 1)
            {
                if(table.find(PlusLib::str2int(elems[0])) ==  table.end())
                {
                    table[PlusLib::str2int(elems[0])] = wxArrayString();
                    table[PlusLib::str2int(elems[0])].Add(wxString(elems[1].c_str(), wxConvUTF8));
                }
                else
                {
                    table.find(PlusLib::str2int(elems[0]))->second.Add(wxString(elems[1].c_str(), wxConvUTF8));
                }
            }
        }
        return table;
    }
    catch(std::istream::failure e)
    {
        PlusLib::write_all("MH4U_QE:: Exception opening/reading " + filename + "\n");
        PlusLib::write_all("MH4U_QE:: Explanatory string: " + std::string(e.what()) + "\n");
        return std::map<uint16_t, wxArrayString>();
    }
}

std::vector<uint16_t> loadTable(std::string filename)
{
    std::ifstream f;
    try
    {
        f.open(filename.c_str(), std::ios::in);
        if(!f)
        {
            PlusLib::write_all("MH4U_QE:: Failed to open " + filename + "\n");
            return std::vector<uint16_t>();
        }

        std::vector<uint16_t> table;
        std::string tmp;
        while(f.good())
        {
            PlusLib::getline(f, tmp);
            table.push_back(PlusLib::str2int(tmp));
        }
        return table;
    }
    catch(std::istream::failure e)
    {
        PlusLib::write_all("MH4U_QE:: Exception opening/reading " + filename + "\n");
        PlusLib::write_all("MH4U_QE:: Explanatory string: " + std::string(e.what()) + "\n");
        return std::vector<uint16_t>();
    }
}

std::vector<uint16_t> getInvertedTable(std::vector<uint16_t> &input)
{
    std::vector<uint16_t> output;
    for(uint16_t i = 0; i < (uint16_t)input.size(); i++)
    {
        while((uint16_t)output.size() <= input[i]) output.push_back(0);
        output[input[i]] = i;
    }
    return output;
}

wxString convertStrFromQuest(const std::vector<uint8_t> &i)
{
    wxString o;
    wxChar c;
    for(unsigned int a = 0; a < i.size(); a=a+2)
    {
        if(i[a] == 0x0a && i[a+1] == 0x00) c = _T('\n'); // replace by \n
        else c = i[a] + (i[a+1] << 8);

        if(i[a] == 0x00 && i[a+1] == 0x00) a = i.size(); // in-game end of string
        else o.Append(c);
    }
    return o;
}

std::vector<uint8_t> convertStrToQuest(const wxString &i)
{
    std::vector<uint8_t> o;
    uint8_t c1, c2;
    for(unsigned int a = 0; a < i.size(); a++)
    {
        if(i[a] == L'\n') // replace \n by 0x000a
        {
            c1 = 0x0a;
            c2 = 0x00;
        }
        else
        {
            c1 = i[a] % 0x100; // byte 1
            c2 = i[a] / 0x100; // byte 2
        }
        o.push_back(c1);
        o.push_back(c2);
    }
    return o;
}

void loadCoordinateTable(std::string filename, std::vector<XYZ> (&tab)[125][22][20])
{
    std::ifstream f;
    try
    {
        f.open(filename.c_str(), std::ios::in);
        if(!f)
        {
            PlusLib::write_all("MH4U_QE:: Failed to open " + filename + "\n");
            return;
        }

        std::string tmp;
        int32_t mon = -1;
        while(f.good())
        {
            PlusLib::getline(f, tmp);
            if(!tmp.empty())
            {
                if(tmp[0] == '#')
                {
                    tmp.erase(tmp.begin());
                    mon = PlusLib::str2int(tmp);
                }
                else if(mon != -1)
                {
                    std::vector<std::string> elems;
                    PlusLib::split(tmp, '|', elems);
                    tab[mon][PlusLib::str2int(elems[0])][PlusLib::str2int(elems[1])].push_back(XYZ(PlusLib::str2float(elems[2]), PlusLib::str2float(elems[3]), PlusLib::str2float(elems[4])));
                }
            }
        }
    }
    catch(std::istream::failure e)
    {
        PlusLib::write_all("MH4U_QE:: Exception opening/reading " + filename + "\n");
        PlusLib::write_all("MH4U_QE:: Explanatory string: " + std::string(e.what()) + "\n");
        return;
    }
}

void loadPartList(std::string filename, std::string (&tab)[124][8])
{
    std::ifstream f;
    try
    {
        f.open(filename.c_str(), std::ios::in);
        if(!f)
        {
            PlusLib::write_all("MH4U_QE:: Failed to open " + filename + "\n");
            return;
        }

        std::string tmp;
        int32_t mon = -1;
        while(f.good())
        {
            PlusLib::getline(f, tmp);
            if(!tmp.empty())
            {
                if(tmp[0] == '#')
                {
                    tmp.erase(tmp.begin());
                    mon = PlusLib::str2int(tmp);
                }
                else if(mon != -1)
                {
                    std::vector<std::string> elems;
                    PlusLib::split(tmp, '|', elems);
                    tab[mon][PlusLib::str2int(elems[0])] = elems[1];
                }
            }
        }
    }
    catch(std::istream::failure e)
    {
        PlusLib::write_all("MH4U_QE:: Exception opening/reading " + filename + "\n");
        PlusLib::write_all("MH4U_QE:: Explanatory string: " + std::string(e.what()) + "\n");
        return;
    }
}

wxArrayString loadStringList(std::string filename)
{
    std::ifstream f;
    try
    {
        f.open(filename.c_str(), std::ios::in);
        if(!f)
        {
            PlusLib::write_all("MH4U_QE:: Failed to open " + filename + "\n");
            return false;
        }

        wxArrayString sl;
        std::string tmp;
        while(f.good())
        {
            PlusLib::getline(f, tmp);
            sl.Add(wxString(tmp.c_str(), wxConvUTF8));
        }
        return sl;
    }
    catch(std::istream::failure e)
    {
        PlusLib::write_all("MH4U_QE:: Exception opening/reading " + filename + "\n");
        PlusLib::write_all("MH4U_QE:: Explanatory string: " + std::string(e.what()) + "\n");
        return false;
    }
}

void writeUTF8String(wxString str, std::ostream &f) // convert wxString to UTF8 and write it
{
    char c;
    for(size_t i = 0; i < str.Length(); i++)
    {
        if(str[i] >= 0x00 && str[i] <= 0x7F)
        {
            c = str[i];
            f.write(&c, 1);
        }
        else if(str[i] >= 0x80 && str[i] <= 0x7FF)
        {
            c = ((str[i] / 0b1000000) % 0b100000) + 0b11000000;
            f.write(&c, 1);
            c = (str[i] % 0b1000000) + 0b10000000;
            f.write(&c, 1);
        }
        else if(str[i] >= 0x800 && str[i] <= 0x0FFF)
        {
            c = 0b11100000;
            f.write(&c, 1);
            c = ((str[i] / 0b1000000) % 0b1000000) + 0b10000000;
            c |= 1 << 5;
            f.write(&c, 1);
            c = ((str[i] % 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
        }
        else if(str[i] >= 0x1000 && str[i] <= 0x1FFF)
        {
            c = 0b11100001;
            f.write(&c, 1);
            c = ((str[i] / 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
            c = ((str[i] % 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
        }
        else if(str[i] >= 0x2000 && str[i] <= 0x3FFF)
        {
            c = (str[i] / 0b1000000000000) + 0b11100000;
            c |= 1 << 1;
            f.write(&c, 1);
            c = ((str[i] / 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
            c = ((str[i] % 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
        }
        else if(str[i] >= 0x4000 && str[i] <= 0x7FFF)
        {
            c = (str[i] / 0b1000000000000) + 0b11100000;
            c |= 1 << 2;
            f.write(&c, 1);
            c = ((str[i] / 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
            c = ((str[i] % 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
        }
        else if(str[i] >= 0x8000 && str[i] <= 0xBFFF)
        {
            c = (str[i] / 0b1000000000000) + 0b11100000;
            c |= 1 << 3;
            f.write(&c, 1);
            c = ((str[i] / 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
            c = ((str[i] % 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
        }
        else if(str[i] >= 0xC000 && str[i] <= 0xCFFF)
        {
            c = (str[i] / 0b1000000000000) + 0b11100000;
            c |= 1 << 2;
            c |= 1 << 3;
            f.write(&c, 1);
            c = ((str[i] / 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
            c = ((str[i] % 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
        }
        else if(str[i] >= 0xD000 && str[i] <= 0xD7FF)
        {
            c = (str[i] / 0b1000000000000) + 0b11100000;
            c |= 1;
            c |= 1 << 2;
            c |= 1 << 3;
            f.write(&c, 1);
            c = ((str[i] / 0b1000000) % 0b1000000) + 0b10000000;
            c &= 0 << 5;
            f.write(&c, 1);
            c = ((str[i] % 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
        }
        else if(str[i] >= 0xE000 && str[i] <= 0xFFFF)
        {
            c = (str[i] / 0b1000000000000) + 0b11100000;
            c |= 1 << 1;
            c |= 1 << 2;
            c |= 1 << 3;
            f.write(&c, 1);
            c = ((str[i] / 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
            c = ((str[i] % 0b1000000) % 0b1000000) + 0b10000000;
            f.write(&c, 1);
        }
    }
}

wxString getStringPart(const wxString& str, const wxString& delim, int token) // get a part of a wxString
{
    wxStringTokenizer wxt;
    wxt.SetString(str, delim);

    if(token < 0 || token >= (int)wxt.CountTokens()) return wxString(_T(""));
    for(; token > 0; token--) wxt.GetNextToken();
    return wxt.GetNextToken();
}

class DnDFile : public wxFileDropTarget // to drop file
{
    public:
        DnDFile(MainFrame *pOwner = NULL) { m_pOwner = pOwner; }

        virtual bool OnDropFiles(wxCoord x, wxCoord y,const wxArrayString& filenames)
        {
            if(filenames.size() > 0)
            {
                m_pOwner->OnDropFiles(filenames[0]);
                return true;
            }
            return false;
        };

    private:
        MainFrame *m_pOwner;
};

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 500), wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
    //sorting();
    srand(time(NULL));
    wxFrame::SetIcon(wxIcon(_T("MAINICON")));
    PlusLib::clear_log();
    PlusLib::init_log();
    iniFile.load("config.ini");
    //analyse();

    // Création du panel d'affichage
    wxPanel *panelAffichage = new wxPanel(this);

    // Création du wxBoxSizer vertical
    sizer_vertical = new wxBoxSizer(wxVERTICAL);
    // Affectation du wxBoxSizer vertical à la fenêtre
    SetSizer(sizer_vertical);

    // Création du wxPanel pour la zone en haut
    wxPanel *zone1 = new wxPanel(this);
    // On l'ajoute au wxSizer principal
    sizer_vertical->Add(zone1, 1, wxALL | wxEXPAND, 0);

    // Création de la ligne de séparation horizontale et ajout au sizer intermédiaire
    wxStaticLine *ligneHoriz = new wxStaticLine(panelAffichage, -1);
    sizer_vertical->Add(ligneHoriz, 0, wxALL | wxEXPAND, 0);

    for(int i = 0; i < Q_COUNT; i++)
    {
        if(i != Q_SUPPLY_BOX && (i < Q_REWARD_A || i > Q_REWARD_C) && (i < Q_TEXT_ENGLISH || i > Q_TEXT_ITALIAN) && i != Q_CHALLENGE && i != Q_SMALL_GROUP && i != Q_PALICO && i != Q_CREATE && i != Q_SAVE)
            sub_sizers[i] = new wxPanel(this);
        else sub_sizers[i] = new wxScrolledWindow(this);
        sizer_vertical->Add(sub_sizers[i], 10, wxEXPAND, 0);
        sub_sizers[i]->Hide();
    }

    wxArrayString choice_str;
    choice_str.Add(_T("Quest informations"));
    choice_str.Add(_T("Objectives and Icons"));
    choice_str.Add(_T("Bosses (Large monsters)"));
    choice_str.Add(_T("Intruders"));
    choice_str.Add(_T("Small monsters"));
    choice_str.Add(_T("Supply box"));
    choice_str.Add(_T("Reward line A"));
    choice_str.Add(_T("Reward line B"));
    choice_str.Add(_T("Subquest reward"));
    choice_str.Add(_T("Challenge quest Presets"));
    choice_str.Add(_T("Eng/Jpn/Korean text"));
    choice_str.Add(_T("French text"));
    choice_str.Add(_T("Spanish text"));
    choice_str.Add(_T("German text"));
    choice_str.Add(_T("Italian text"));
    choice_str.Add(_T("Create a quest"));
    choice_str.Add(_T("Debug"));
    choice_str.Add(_T("Tools"));
    choice_str.Add(_T("Palico Editor"));
    choice_str.Add(_T("Save Editor"));

    //--------------------------------------------------------
    std::string arena_file_list[] = {"chest.txt","arms.txt","waist.txt","legs.txt","head.txt","talisman.txt","great_sword.txt",
    "sword_shield.txt","hammer.txt","lance.txt","light_bowgun.txt","heavy_bowgun.txt","long_sword.txt","switch_axe.txt","gunlance.txt",
    "bow.txt","dual_blades.txt","hunting_horn.txt","insect_glaive.txt","charge_blade.txt","kinsect.txt","skill.txt","equipment_armor.txt","equipment_weapon.txt","jewel_list.txt","equipment.txt"};

    for(uint32_t i = 0; i < 26; i++) arena_list[i] = loadStringList("data/arena/" + arena_file_list[i]);
    loadPartList("data/part_list.txt", parts);
    loadCoordinateTable("data/coordinate_table.txt", coordinate);
    item_list = loadStringList("data/item.txt");
    item_table = loadTable("data/item_table.txt");
    i_item_table = getInvertedTable(item_table);

    arena_item_list = loadStringList("data/arena/item.txt");
    arena_item_table = loadTable("data/arena/item_table.txt");
    i_arena_item_table = getInvertedTable(arena_item_table);
    map_list = loadStringList("data/map.txt");
    map_table = loadTable("data/map_table.txt");
    i_map_table = getInvertedTable(map_table);
    term_list = loadStringList("data/term.txt");
    monster_list = loadStringList("data/monster.txt");
    monster_table = loadTable("data/monster_table.txt");
    i_monster_table = getInvertedTable(monster_table);
    icon_list = loadStringList("data/icon.txt");
    icon_table = loadTable("data/icon_table.txt");
    i_icon_table = getInvertedTable(icon_table);
    palico_skill_list = loadStringList("data/palico.txt");
    palico_skill_table = loadTable("data/palico_table.txt");
    i_palico_skill_table = getInvertedTable(palico_skill_table);
    alternative_list = loadSpecialList("data/form.txt");
    alternative_text = loadSpecialListText("data/form_string.txt");
    id_list = loadTable("data/free_id.txt");
    frenzy_list = loadStringList("data/frenzy.txt");
    frenzy_table = loadTable("data/frenzy_table.txt");
    i_frenzy_table = getInvertedTable(frenzy_table);
    icon_translate = loadTable("data/icon_translate.txt");
    gq_boss_list = loadStringList("data/others/monster.txt");
    gq_boss_table = loadTable("data/others/monster_table.txt");
    i_gq_boss_table = getInvertedTable(gq_boss_table);

    //--------------------------------------------------------
    m_load = new wxButton(zone1, ID_BTN_LOAD, _T("Load"), wxPoint(0,0), wxSize(50, 23));
    m_save = new wxButton(zone1, ID_BTN_SAVE, _T("Save"), wxPoint(50,0), wxSize(50, 23));
    m_save->Disable();
    m_saveas = new wxButton(zone1, ID_BTN_SAVEAS, _T("Save As"), wxPoint(100,0), wxSize(50, 23));
    m_saveas->Disable();
    m_encrypt = new wxButton(zone1, ID_BTN_ENCRYPTNOW, _T("Encrypt"), wxPoint(150,0), wxSize(60, 23));
    m_encrypt->Disable();
    m_encryptas = new wxButton(zone1, ID_BTN_ENCRYPTAS, _T("Encrypt As"), wxPoint(210,0), wxSize(70, 23));
    m_encryptas->Disable();

    m_encryptchoice = new wxChoice(zone1, -1, wxPoint(280,1), wxSize(60, 50));
    m_encryptchoice->Freeze();
    m_encryptchoice->Append(_T("JPN"));
    m_encryptchoice->Append(_T("US/EU"));
    m_encryptchoice->Append(_T("KOR"));
    m_encryptchoice->Append(_T("TWN"));
    m_encryptchoice->Thaw();
    m_encryptchoice->SetSelection(iniFile.getIntData("ENCRYPT_REGION", "GENERAL"));

    m_choice = new wxChoice(zone1, ID_CHO_MENU, wxPoint(340,1), wxSize(150, 50));
    m_choice->Freeze();
    m_choice->Append(choice_str);
    m_choice->Thaw();
    m_questname_text = new wxStaticText(zone1, -1, _T("No file loaded"), wxPoint(5,25));

    initAll();
    sub_sizers[Q_QUEST_EMPTY]->Show();
    current_sub_sizer = sub_sizers[Q_QUEST_EMPTY];
    sizer_vertical->Layout();

    new wxStaticText(sub_sizers[Q_QUEST_EMPTY], -1, _T("To start, drag and drop a Quest File, press \"Load\" or make a new quest."), wxPoint(20, 20));
    new wxButton(sub_sizers[Q_QUEST_EMPTY], ID_BTN_GO_NEW, _T("New"), wxPoint(20, 40), wxSize(40, 23));

    this->SetDropTarget((wxDropTarget*)new DnDFile(this));
}

MainFrame::~MainFrame()
{
    iniFile.editIntData("ENCRYPT_REGION", m_encryptchoice->GetSelection(), "GENERAL");
    iniFile.editIntData("TOOL_REGION", q_tool_region->GetSelection(), "GENERAL");
    iniFile.editIntData("TOOL_PROXY_REGION", q_tool_pregion->GetSelection(), "GENERAL");
    iniFile.editStrData("TOOL_QUEST_FOLDER", wxStr2Dec(q_tool_proxy_folder->GetValue()), "GENERAL");
    iniFile.editIntData("ENCRYPT_PALICO_REGION", p_encryptchoice->GetSelection(), "GENERAL");
    iniFile.editIntData("CREATE_REGION", qc.region->GetSelection(), "GENERAL");
    iniFile.editStrData("DEV_PASSWORD", wxStr2Dec(s_password->GetValue()), "GENERAL");
    iniFile.save();
}

void MainFrame::commonLoad(wxString name)
{
    m_questname_text->SetLabel(_T("No file loaded"));
    enable_modif = false;
    m_save->Disable();
    m_saveas->Disable();
    m_encrypt->Disable();
    m_encryptas->Disable();
    setSizer(Q_QUEST_EMPTY);
    q_current_bgroup = -1;
    q_current_boss = -1;
    q_current_mgroup = -1;
    q_current_monster = -1;
    q_current_preset = -1;
    q_current_equipment = -1;
    q_current_intruder = -1;
    cpy_subgroup = NULL;
    updateQuestDetail(3);
    if(!qdata.load(name.wc_str()))
    {
        wxMessageBox(_T("Error : Loading failed"));
    }
    else
    {
        m_questname_text->SetLabel(_T("Loading ..."));
        q_current_bgroup = 0;
        q_current_boss = 0;
        q_current_mgroup = 0;
        q_current_monster = 0;
        q_current_preset = 0;
        q_current_equipment = 0;
        q_current_intruder = 0;
        m_choice->SetSelection(0);
        q_boss_choice->SetSelection(0);
        readAll();
        path = name;
        m_save->Enable();
        m_saveas->Enable();
        m_encrypt->Enable();
        m_encryptas->Enable();
        m_questname_text->SetLabel(path);
        setSizer(Q_QUEST_INFO);
    }
    enable_modif = true;
}

void MainFrame::OnDropFiles(wxString filename)
{
    if(qdata.quest)
    {
        if(wxMessageBox(_T("A quest is already loaded. Continue ?"), _T("Confirm"), wxNO) == wxNO)
        {
            return;
        }
    }

    commonLoad(filename);
}

void MainFrame::OnBtnLoad(wxCommandEvent &event)
{
    wxFileDialog
        openFileDialog(this, _("Open Quest File"), _T(""), _T(""), _T(""), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    commonLoad(openFileDialog.GetPath());
}

void MainFrame::OnBtnSave(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    writeAll();
    readAll();
    if(!qdata.save(path.wc_str())) wxMessageBox(_T("Error : Saving failed"));
    else wxMessageBox(_T("File saved"));
}

void MainFrame::OnBtnSaveAs(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    wxFileDialog
        saveFileDialog(this, _T("Save Quest File"), _T(""), _T(""), _T(""), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    writeAll();
    readAll();
    if(!qdata.save(saveFileDialog.GetPath().wc_str())) wxMessageBox(_T("Error : Saving failed"));
    else
    {
        m_questname_text->SetLabel(saveFileDialog.GetPath());
        path = saveFileDialog.GetPath();
        wxMessageBox(_T("File saved"));
    }
}

void MainFrame::OnBtnEncryptNow(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    writeAll();
    readAll();
    if(!qdata.encrypt(path.wc_str(), m_encryptchoice->GetSelection())) wxMessageBox(_T("Error : Saving as an encrypted quest failed"));
    else
    {
        switch(m_encryptchoice->GetSelection())
        {
            case 0: wxMessageBox(_T("File saved and encrypted (JPN)")); break;
            case 1: wxMessageBox(_T("File saved and encrypted (US/EU)")); break;
            case 2: wxMessageBox(_T("File saved and encrypted (KOR)")); break;
            case 3: wxMessageBox(_T("File saved and encrypted (TWN)")); break;
            default: wxMessageBox(_T("File saved and encrypted (Unknown region)")); break;
        }
    }
}

void MainFrame::OnBtnEncryptAs(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    wxFileDialog
        saveFileDialog(this, _T("Encrypt Quest File"), _T(""), _T(""), _T(""), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    writeAll();
    readAll();
    if(!qdata.encrypt(saveFileDialog.GetPath().wc_str(), m_encryptchoice->GetSelection())) wxMessageBox(_T("Error : Saving as an encrypted quest failed"));
    else
    {
        switch(m_encryptchoice->GetSelection())
        {
            case 0: wxMessageBox(_T("File saved and encrypted (JPN)")); break;
            case 1: wxMessageBox(_T("File saved and encrypted (US/EU)")); break;
            case 2: wxMessageBox(_T("File saved and encrypted (KOR)")); break;
            case 3: wxMessageBox(_T("File saved and encrypted (TWN)")); break;
            default: wxMessageBox(_T("File saved and encrypted (Unknown region)")); break;
        }
    }
}

void MainFrame::OnChoMenu(wxCommandEvent &event)
{
    setSizer(event.GetSelection() + 1);
}

void MainFrame::OnChoQuestType(wxCommandEvent &event)
{
    if(!qdata.quest) return;
    updateQuestDetail(1);
}

void MainFrame::OnChoBossGroup(wxCommandEvent &event)
{
    if(!qdata.quest) return;
    if(q_bgroup_count == 0) return;
    writeBossGroup();
    q_current_bgroup = event.GetSelection();
    q_boss_count = 0;
    readBossGroup();
}

void MainFrame::OnChoBoss(wxCommandEvent &event)
{
    if(!qdata.quest) return;
    if(q_boss_count == 0) return;
    writeBossGroup();
    q_current_boss = event.GetSelection();
    readBossGroup();
}

void MainFrame::OnChoMonsterGroup(wxCommandEvent &event)
{
    if(!qdata.quest) return;
    if(q_mgroup_count == 0) return;
    writeMonsterGroup();
    q_current_mgroup = event.GetSelection();
    q_monster_count = 0;
    readMonsterGroup();
}

void MainFrame::OnChoMonster(wxCommandEvent &event)
{
    if(!qdata.quest) return;
    if(q_monster_count == 0) return;
    writeMonsterGroup();
    q_current_monster = event.GetSelection();
    readMonsterGroup();
}

void MainFrame::OnBtnBossHelp1(wxCommandEvent &event)
{
    wxMessageBox(_T("Boss groups spawn in order.\nGroup 2 can't spawn before Group 1."));
}

void MainFrame::OnBtnBossHelp2(wxCommandEvent &event)
{
    wxMessageBox(_T("Fatalis corpses can't despawn"));
}

void MainFrame::OnChoPreset(wxCommandEvent &event)
{
    writeChallenge();
    q_current_preset = event.GetSelection();
    readChallenge();
}

void MainFrame::OnChoEquipement(wxCommandEvent &event)
{
    writeChallenge();
    q_current_equipment = event.GetSelection();
    readChallenge();
}

void MainFrame::OnChoIntruder(wxCommandEvent &event)
{
    if(q_intruder_count == 0) return;
    writeIntruderAll();
    q_current_intruder = event.GetSelection();
    readIntruderAll();
}

void MainFrame::OnBtnAddBoss(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(q_bgroup_count == 0)
    {
        wxMessageBox(_T("Error : No boss group available\nAdd one first, please."));
        return;
    }

    if(qdata.quest->boss_count >= 5)
    {
        wxMessageBox(_T("Error : Maximum number of bosses reached (Max 5)"));
        return;
    }
    if(q_current_bgroup < 0 || q_current_bgroup >= q_bgroup_count)
    {
        wxMessageBox(_T("Error : Invalid group selection"));
        return;
    }
    if(qdata.quest->boss_group_offset[q_current_bgroup].count >= 2)
    {
        wxMessageBox(_T("Error : Maximum number of bosses in this group reached (Max 2)"));
        return;
    }
    if(wxMessageBox(wxString(std::string("Add a Boss to Group #" + PlusLib::int2str(q_current_bgroup+1) + " ?").c_str(), wxConvUTF8),
                    _T("Confirm"), wxNO) == wxYES)
    {
        writeBossGroup();
        if(qdata.modifyBoss(q_current_bgroup, true))
        {
            q_current_boss = q_boss_count;
            q_boss_choice->SetSelection(q_current_boss);
            wxMessageBox(_T("Boss Added"));
        }
        else wxMessageBox(_T("Error : Failed to add the boss"));
        readBossGroup();
    }
}

void MainFrame::OnBtnDelBoss(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(q_boss_count == 0)
    {
        wxMessageBox(_T("Error : There is no boss to delete in this group"));
        return;
    }
    if(q_current_boss < 0 || q_current_boss >= q_boss_count)
    {
        wxMessageBox(_T("Error : Invalid boss selection"));
        return;
    }
    if(q_current_bgroup < 0 || q_current_bgroup >= q_bgroup_count)
    {
        wxMessageBox(_T("Error : Invalid group selection"));
        return;
    }

    if(wxMessageBox(wxString(std::string("Delete the Boss #" + PlusLib::int2str(q_current_boss+1) + " of Group #" + PlusLib::int2str(q_current_bgroup+1) + " ?").c_str(), wxConvUTF8),
                    _T("Confirm"), wxNO) == wxYES)
    {
        writeBossGroup();
        if(qdata.modifyBoss(q_current_bgroup, false, q_current_boss))
        {
            q_current_boss = 0;
            q_boss_choice->SetSelection(0);
            wxMessageBox(_T("Boss Deleted"));
        }
        else wxMessageBox(_T("Error : Failed to delete the boss"));
        readBossGroup();
    }
}

void MainFrame::OnBtnAddIntruder(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(q_bgroup_count > 1)
    {
        wxMessageBox(_T("Error : Cannot add an intruder if there are more as one Boss Group"));
        return;
    }

    if(q_intruder_count >= MAX_INTRUDER)
    {
        wxMessageBox(wxString(std::string("Error : Maximum number of intruders reached (Max " + PlusLib::int2str(MAX_INTRUDER) + ")").c_str(), wxConvUTF8));
        return;
    }
    if(wxMessageBox(_T("Add an Intruder ?"), _T("Confirm"), wxNO) == wxYES)
    {
        writeIntruderAll();
        if(qdata.modifyIntruder(0, true))
        {
            q_current_intruder = q_intruder_count;
            q_intruder_choice->SetSelection(q_current_intruder);
            wxMessageBox(_T("Intruder Added"));
        }
        else wxMessageBox(_T("Error : Failed to add the intruder"));
        readIntruderAll();
    }
}

void MainFrame::OnBtnDelIntruder(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(qdata.quest->intruder.size() <= 0)
    {
        wxMessageBox(_T("Error : There is no intruder to delete"));
        return;
    }
    if(q_current_intruder < 0 || q_current_intruder >= q_intruder_count)
    {
        wxMessageBox(_T("Error : Invalid intruder selection"));
        return;
    }

    if(wxMessageBox(wxString(std::string("Delete the Intruder #" + PlusLib::int2str(q_current_intruder+1) + " ?").c_str(), wxConvUTF8),
                    _T("Confirm"), wxNO) == wxYES)
    {
        writeIntruderAll();
        if(qdata.modifyIntruder(q_current_intruder, false))
        {
            q_current_intruder = 0;
            q_intruder_choice->SetSelection(0);
            wxMessageBox(_T("Intruder Deleted"));
        }
        else wxMessageBox(_T("Error : Failed to delete the intruder"));
        readIntruderAll();
    }
}

void MainFrame::OnBtnDecrypt(wxCommandEvent &event)
{
    mh4u_crypto.setRegion(q_tool_region->GetSelection());
    PlusLib::write_all("MH4U_crypto:: region set to " + PlusLib::int2str(q_tool_region->GetSelection()) + "\n");

    switch(mh4u_crypto.decrypt(q_tool_file_in->GetValue().wc_str(), q_tool_file_out->GetValue().wc_str()))
    {
        case 1: wxMessageBox(_T("Error : Failed to open the input file")); return;
        case 2: wxMessageBox(_T("Error : Invalid file size in footer")); return;
        case 3: wxMessageBox(_T("Error : Invalid SHA1 hash in footer")); return;
        case 4: wxMessageBox(_T("Error : Failed to open the ouput file")); return;
        case -1: wxMessageBox(_T("Error : Invalid region selected")); return;
        default: wxMessageBox(_T("Success")); return;
    }
}

void MainFrame::OnBtnEncrypt(wxCommandEvent &event)
{
    mh4u_crypto.setRegion(q_tool_region->GetSelection());
    PlusLib::write_all("MH4U_crypto:: region set to " + PlusLib::int2str(q_tool_region->GetSelection()) + "\n");

    switch(mh4u_crypto.encrypt(q_tool_file_in->GetValue().wc_str(), q_tool_file_out->GetValue().wc_str()))
    {
        case 1: wxMessageBox(_T("Error : Failed to open the input file")); return;
        case 2: wxMessageBox(_T("Error : Failed to open the ouput file")); return;
        case -1: wxMessageBox(_T("Error : Invalid region selected")); return;
        default: wxMessageBox(_T("Success")); return;
    }
}

void MainFrame::OnBtnToolIn(wxCommandEvent &event)
{
    wxFileDialog
        openFileDialog(this, _("Input File"), _T(""), _T(""), _T(""), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    q_tool_file_in->ChangeValue(openFileDialog.GetPath());
}

void MainFrame::OnBtnToolOut(wxCommandEvent &event)
{
    wxFileDialog
        openFileDialog(this, _("Output File"), _T(""), _T(""), _T(""), wxFD_OPEN);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    q_tool_file_out->ChangeValue(openFileDialog.GetPath());
}

void MainFrame::OnBtnAddBossGroup(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(q_intruder_count > 0)
    {
        wxMessageBox(_T("Error : Cannot add a group if there is an intruder"));
        return;
    }

    if(q_bgroup_count >= MAX_BOSS_GROUP)
    {
        wxMessageBox(wxString(std::string("Error : Maximum number of groups reached (Max "+PlusLib::int2str(MAX_BOSS_GROUP)+")").c_str(), wxConvUTF8));
        return;
    }


    if(wxMessageBox(_T("Add a Boss Group ?"),
                    _T("Confirm"), wxNO) == wxYES)
    {
        writeBossGroup();
        if(qdata.modifyBossGroup(0, true))
        {
            q_current_bgroup = q_bgroup_count;
            q_bgroup_choice->SetSelection(q_current_bgroup);
            wxMessageBox(_T("Boss Group Added"));
        }
        else wxMessageBox(_T("Error : Failed to add the boss group"));
        readBossGroup();
    }
}

void MainFrame::OnBtnDelBossGroup(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(q_bgroup_count <= 1)
    {
        wxMessageBox(_T("Error : Can't delete last group"));
        return;
    }

    if(q_current_bgroup < 0 || q_current_bgroup >= q_bgroup_count)
    {
        wxMessageBox(_T("Error : Invalid group selection"));
        return;
    }

    if(qdata.quest->boss_group_offset[q_current_bgroup].count > 0)
    {
        wxMessageBox(_T("Error : Group isn't empty"));
        return;
    }

    if(wxMessageBox(wxString(std::string("Delete the Boss Group #" + PlusLib::int2str(q_current_bgroup+1) + " ?").c_str(), wxConvUTF8),
                    _T("Confirm"), wxNO) == wxYES)
    {
        writeBossGroup();
        if(qdata.modifyBossGroup(q_current_bgroup, false))
        {
            q_current_bgroup = 0;
            q_bgroup_choice->SetSelection(q_current_bgroup);
            wxMessageBox(_T("Boss Group Deleted"));
        }
        else wxMessageBox(_T("Error : Failed to delete the boss group"));
        readBossGroup();
    }
}

void MainFrame::OnBtnMoveBoss(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(q_boss_count == 0)
    {
        wxMessageBox(_T("Error : There is no boss to move in this group"));
        return;
    }
    if(q_current_boss < 0 || q_current_boss >= q_boss_count)
    {
        wxMessageBox(_T("Error : Invalid boss selection"));
        return;
    }
    if(q_current_bgroup < 0 || q_current_bgroup >= q_bgroup_count)
    {
        wxMessageBox(_T("Error : Invalid group selection"));
        return;
    }
    if(q_boss_choice_move->GetSelection() < 0 || q_boss_choice_move->GetSelection() >= q_bgroup_count)
    {
        wxMessageBox(_T("Error : Invalid destination"));
        return;
    }
    if(q_boss_choice_move->GetSelection() == q_current_bgroup)
    {
        wxMessageBox(_T("Error : You are trying to move\nthe boss to the same group."));
        return;
    }
    if(qdata.quest->boss_group_offset[q_boss_choice_move->GetSelection()].count >= 2)
    {
        wxMessageBox(_T("Error : Maximum number of bosses in the destination group reached (Max 2)"));
        return;
    }
    if(wxMessageBox(wxString(std::string("Move the Boss #" + PlusLib::int2str(q_current_boss+1) + " of Group #" + PlusLib::int2str(q_current_bgroup+1) + " to the Group #" + PlusLib::int2str(q_boss_choice_move->GetSelection()+1) + " ?").c_str(), wxConvUTF8),
                    _T("Confirm"), wxNO) == wxYES)
    {
        writeBossGroup();
        if(qdata.moveBossToGroup(q_current_bgroup, q_current_boss, q_boss_choice_move->GetSelection()))
        {
            q_current_boss = 0;
            q_boss_choice->SetSelection(0);
            wxMessageBox(_T("Boss Moved"));
        }
        else wxMessageBox(_T("Error : Failed to move the boss"));
        readBossGroup();
    }
}

void MainFrame::OnBtnRewardAHelp(wxCommandEvent &event)
{
    wxMessageBox(_T("Changing the first reward line\nrequires to use a valid unused ID"));
}

void MainFrame::OnBtnUnbindReward(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    writeReward(current_sub_sizer_id-Q_REWARD_A+1);
    if(!qdata.unbindReward(current_sub_sizer_id-Q_REWARD_A))
         wxMessageBox(_T("Error : Failed to unbind the reward line"));
    readReward(current_sub_sizer_id-Q_REWARD_A+1);
}

void MainFrame::OnBtnAddRItem(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    writeReward(current_sub_sizer_id-Q_REWARD_A+1);
    if(!qdata.modifyReward(current_sub_sizer_id-Q_REWARD_A, true))
         wxMessageBox(_T("Error : Failed to add an item"));
    readReward(current_sub_sizer_id-Q_REWARD_A+1);
}

void MainFrame::OnBtnDelRItem(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(q_reward_box[current_sub_sizer_id-Q_REWARD_A].count == 0)
    {
        wxMessageBox(_T("Error : There is no item to delete"));
        return;
    }
    writeReward(current_sub_sizer_id-Q_REWARD_A+1);
    if(!qdata.modifyReward(current_sub_sizer_id-Q_REWARD_A, false))
         wxMessageBox(_T("Error : Failed to delete an item"));
    readReward(current_sub_sizer_id-Q_REWARD_A+1);
}

void MainFrame::OnBtnAddReward(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(qdata.quest->reward_box_offset[0] == 0)
    {
        wxMessageBox(_T("Error : Your quest has no reward offset\n(I will fix it later)"));
        return;
    }

    if(qdata.quest->reward_box_offset[current_sub_sizer_id-Q_REWARD_A] != 0)
    {
        wxMessageBox(_T("Error : This reward line is valid"));
        return;
    }

    writeReward(current_sub_sizer_id-Q_REWARD_A+1);
    if(!qdata.addReward(current_sub_sizer_id-Q_REWARD_A))
        wxMessageBox(_T("Error : Failed to add a reward line"));
    readReward(current_sub_sizer_id-Q_REWARD_A+1);
}

void MainFrame::OnBtnAddMonster(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(q_mgroup_count == 0)
    {
        wxMessageBox(_T("Error : No monster subgroup available\nAdd one first, please."));
        return;
    }

    if(q_current_mgroup < 0 || q_current_mgroup >= q_mgroup_count)
    {
        wxMessageBox(_T("Error : Invalid subgroup selection"));
        return;
    }

    if(wxMessageBox(wxString(std::string("Add a Monster to Subgroup #" + PlusLib::int2str(q_current_mgroup+1) + " ?").c_str(), wxConvUTF8),
                    _T("Confirm"), wxNO) == wxYES)
    {
        writeMonsterGroup();
        if(qdata.modifyMonster(q_current_mgroup, true))
        {
            q_current_monster = q_monster_count;
            q_monster_choice->SetSelection(q_current_monster);
            wxMessageBox(_T("Monster Added"));
        }
        else wxMessageBox(_T("Error : Failed to add the monster"));
        readMonsterGroup();
    }
}

void MainFrame::OnBtnDelMonster(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(q_monster_count == 0)
    {
        wxMessageBox(_T("Error : There is no monster to delete in this subgroup"));
        return;
    }
    if(q_current_monster < 0 || q_current_monster >= q_monster_count)
    {
        wxMessageBox(_T("Error : Invalid monster selection"));
        return;
    }
    if(q_current_mgroup < 0 || q_current_mgroup >= q_mgroup_count)
    {
        wxMessageBox(_T("Error : Invalid group selection"));
        return;
    }

    if(wxMessageBox(wxString(std::string("Delete the Monster #" + PlusLib::int2str(q_current_monster+1) + " of Subgroup #" + PlusLib::int2str(q_current_mgroup+1) + " ?").c_str(), wxConvUTF8),
                    _T("Confirm"), wxNO) == wxYES)
    {
        writeMonsterGroup();
        if(qdata.modifyMonster(q_current_mgroup, false, q_current_monster))
        {
            q_current_monster = 0;
            q_monster_choice->SetSelection(0);
            wxMessageBox(_T("Monster Deleted"));
        }
        else wxMessageBox(_T("Error : Failed to delete the monster"));
        readMonsterGroup();
    }
}

void MainFrame::OnBtnAddMonsterGroup(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    wxButton* ptr = (wxButton*)event.GetEventObject();
    int32_t g = - 1;
    for(int32_t i = 0; i < 3; i++)
    {
        if(ptr == q_submon[i].add) g = i;
    }

    if(g == -1)
    {
        wxMessageBox(_T("Error: Wrong button ID"));
        return;
    }

    if(wxMessageBox(wxString(std::string("Add a new subgroup to group #" + PlusLib::int2str(g+1) + "?").c_str(), wxConvUTF8),
                    _T("Confirm"), wxNO) == wxYES)
    {
        writeMonsterGroup();
        if(qdata.modifyMonsterGroup(g, 0, true))
        {
            wxMessageBox(_T("Subgroup Added"));
        }
        else wxMessageBox(_T("Error : Failed to add the monster group"));
        readMonsterGroup();
    }
}

void MainFrame::OnBtnDelMonsterGroup(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(qdata.quest->monster_group_list.size() == 1 && qdata.quest->monster_group_list[0]->nUse == 1)
    {
        wxMessageBox(_T("Error : Can't delete last subgroup"));
        return;
    }

    wxButton* ptr = (wxButton*)event.GetEventObject();
    int32_t group_id = -1;
    int32_t subgroup_id = -1;

    for(uint32_t i = 0; i < 3; i++)
    {
        for(uint32_t j = 0; j < qdata.quest->possible_monster[i].group.size(); j++)
        {
            if(ptr == q_submon[i].del[j])
            {
                group_id = i;
                subgroup_id = j;
            }
        }
    }

    if(group_id == - 1 || subgroup_id == - 1)
    {
        wxMessageBox(_T("Error : If you see this error, you are lucky.\nOr maybe not."));
        return;
    }

    if(qdata.quest->possible_monster[group_id].group[subgroup_id]->nUse > 1 || wxMessageBox(wxString(std::string("The Subgroup isn't used by another group. It will be\ncompletely deleted. Continue ?").c_str(), wxConvUTF8),
                    _T("Confirm"), wxNO) == wxYES)
    {
        writeMonsterGroup();
        if(!qdata.modifyMonsterGroup(group_id, subgroup_id, false))
            wxMessageBox(_T("Error : Failed to delete the monster group"));
        readMonsterGroup();
    }
}

void MainFrame::OnBtnUnbindMonsterGroup(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    wxButton* ptr = (wxButton*)event.GetEventObject();
    int32_t g = - 1;
    for(int32_t i = 0; i < 3; i++)
    {
        if(ptr == q_submon[i].unbind) g = i;
    }

    if(g == -1)
    {
        wxMessageBox(_T("Error: Wrong button ID"));
        return;
    }

    writeMonsterGroup();
    if(!qdata.unbindMonsterGroup(g))
            wxMessageBox(_T("Error : Failed to unbind"));
    readMonsterGroup();
}

void MainFrame::OnBtnMoveMonster(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(q_monster_count == 0)
    {
        wxMessageBox(_T("Error : There is no monster to move in this group"));
        return;
    }
    if(q_current_monster < 0 || q_current_monster >= q_monster_count)
    {
        wxMessageBox(_T("Error : Invalid monster selection"));
        return;
    }
    if(q_current_mgroup < 0 || q_current_mgroup >= q_mgroup_count)
    {
        wxMessageBox(_T("Error : Invalid group selection"));
        return;
    }
    if(q_monster_choice_move->GetSelection() < 0 || q_monster_choice_move->GetSelection() >= q_mgroup_count)
    {
        wxMessageBox(_T("Error : Invalid destination"));
        return;
    }
    if(q_monster_choice_move->GetSelection() == q_current_mgroup)
    {
        wxMessageBox(_T("Error : You are trying to move\nthe monster to the same group."));
        return;
    }

    if(wxMessageBox(wxString(std::string("Move the Monster #" + PlusLib::int2str(q_current_monster+1) + " of Subgroup #" + PlusLib::int2str(q_current_mgroup+1) + " to the\nSubgroup #" + PlusLib::int2str(q_monster_choice_move->GetSelection()+1) + " ?").c_str(), wxConvUTF8),
                    _T("Confirm"), wxNO) == wxYES)
    {
        writeMonsterGroup();
        if(qdata.moveMonsterToGroup(q_current_mgroup, q_current_monster, q_monster_choice_move->GetSelection()))
        {
            q_current_monster = 0;
            q_monster_choice->SetSelection(0);
            wxMessageBox(_T("Monster Moved"));
        }
        else wxMessageBox(_T("Error : Failed to move the monster"));
        readMonsterGroup();
    }
}

void MainFrame::OnChoUpdateBossID(wxCommandEvent &event)
{
    if(!qdata.quest) return;
    writeBossGroup();
    readBossGroup();
}

void MainFrame::OnChoUpdateMonsterID(wxCommandEvent &event)
{
    if(!qdata.quest) return;
    writeMonsterGroup();
    readMonsterGroup();
}

void MainFrame::OnChoUpdateIntruderID(wxCommandEvent &event)
{
    if(!qdata.quest) return;
    writeIntruderAll();
    readIntruderAll();
}

void MainFrame::OnBtnCpyMonsterSubgroup(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    wxButton* ptr = (wxButton*)event.GetEventObject();
    for(uint32_t i = 0; i < 3; i++)
    {
        for(uint32_t j = 0; j < qdata.quest->possible_monster[i].group.size(); j++)
        {
            if(ptr == q_submon[i].cpy[j])
            {
                cpy_subgroup = qdata.quest->possible_monster[i].group[j];
                return;
            }
        }
    }
    wxMessageBox(_T("Error: Can't copy the subgroup.\nYou are not suppoed to see this error. :p"));
}

void MainFrame::OnBtnPasteMonsterSubgroup(wxCommandEvent &event)
{
    if(!qdata.quest) return;
    if(cpy_subgroup == NULL)
    {
        wxMessageBox(_T("Error: Nothing to paste"));
        return;
    }
    writeMonsterGroup();
    wxButton* ptr = (wxButton*)event.GetEventObject();
    for(int32_t i = 0; i < 3; i++)
    {
        if(ptr == q_submon[i].paste)
        {
            if(!qdata.pasteMonsterGroup(cpy_subgroup, i))
                wxMessageBox(_T("Error: Failed to paste the data"));
            readMonsterGroup();
            return;
        }
    }
    wxMessageBox(_T("Error: Can't paste the subgroup.\nYou are not suppoed to see this error. :p"));
    readMonsterGroup();
}

void MainFrame::OnBtnHelpMonster(wxCommandEvent &event)
{
    wxMessageBox(_T("The first group spawns by default. You have\nto use the group spawn conditions to make the others\nspawn."));
}

void MainFrame::OnBtnCopyTitle(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;
    textBuffer = q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7]->GetValue();
}

void MainFrame::OnBtnCopySuccess(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;
    textBuffer = q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+1]->GetValue();
}

void MainFrame::OnBtnCopyFailure(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;
    textBuffer = q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+2]->GetValue();
}

void MainFrame::OnBtnCopySummary(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;
    textBuffer = q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+3]->GetValue();
}

void MainFrame::OnBtnCopyMainMonsters(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;
    textBuffer = q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+4]->GetValue();
}

void MainFrame::OnBtnCopyClient(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;
    textBuffer = q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+5]->GetValue();
}

void MainFrame::OnBtnCopySubquest(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;
    textBuffer = q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+6]->GetValue();
}

void MainFrame::OnBtnPasteTitle(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;

    if(textBuffer.empty())
    {
        wxMessageBox(_T("Error : The buffer is empty"));
        return;
    }

    int tmp = (qdata.quest->language_offset[current_sub_sizer_id - Q_TEXT_ENGLISH].title.size()-TEXT_SUB)/2;

    if((int)textBuffer.size() > tmp)
    {
        wxMessageBox(wxString(std::string("Error : Max length is " + PlusLib::int2str(tmp) + ". " + PlusLib::int2str(textBuffer.size() - tmp) + " characters ignored").c_str(), wxConvUTF8));
        q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7]->SetValue(textBuffer.substr(0, tmp-1));
    }
    else q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7]->SetValue(textBuffer);
}

void MainFrame::OnBtnPasteSuccess(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;

    if(textBuffer.empty())
    {
        wxMessageBox(_T("Error : The buffer is empty"));
        return;
    }

    int tmp = (qdata.quest->language_offset[current_sub_sizer_id - Q_TEXT_ENGLISH].success.size()-TEXT_SUB)/2;

    if((int)textBuffer.size() > tmp)
    {
        wxMessageBox(wxString(std::string("Error : Max length is " + PlusLib::int2str(tmp) + ". " + PlusLib::int2str(textBuffer.size() - tmp) + " characters ignored").c_str(), wxConvUTF8));
        q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+1]->SetValue(textBuffer.substr(0, tmp-1));
    }
    else q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+1]->SetValue(textBuffer);
}

void MainFrame::OnBtnPasteFailure(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;

    if(textBuffer.empty())
    {
        wxMessageBox(_T("Error : The buffer is empty"));
        return;
    }

    int tmp = (qdata.quest->language_offset[current_sub_sizer_id - Q_TEXT_ENGLISH].failure.size()-TEXT_SUB)/2;

    if((int)textBuffer.size() > tmp)
    {
        wxMessageBox(wxString(std::string("Error : Max length is " + PlusLib::int2str(tmp) + ". " + PlusLib::int2str(textBuffer.size() - tmp) + " characters ignored").c_str(), wxConvUTF8));
        q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+2]->SetValue(textBuffer.substr(0, tmp-1));
    }
    else q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+2]->SetValue(textBuffer);
}

void MainFrame::OnBtnPasteSummary(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;

    if(textBuffer.empty())
    {
        wxMessageBox(_T("Error : The buffer is empty"));
        return;
    }

    int tmp = (qdata.quest->language_offset[current_sub_sizer_id - Q_TEXT_ENGLISH].summary.size()-TEXT_SUB)/2;

    if((int)textBuffer.size() > tmp)
    {
        wxMessageBox(wxString(std::string("Error : Max length is " + PlusLib::int2str(tmp) + ". " + PlusLib::int2str(textBuffer.size() - tmp) + " characters ignored").c_str(), wxConvUTF8));
        q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+3]->SetValue(textBuffer.substr(0, tmp-1));
    }
    else q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+3]->SetValue(textBuffer);
}

void MainFrame::OnBtnPasteMainMonsters(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;

    if(textBuffer.empty())
    {
        wxMessageBox(_T("Error : The buffer is empty"));
        return;
    }

    int tmp = (qdata.quest->language_offset[current_sub_sizer_id - Q_TEXT_ENGLISH].main_monsters.size()-TEXT_SUB)/2;

    if((int)textBuffer.size() > tmp)
    {
        wxMessageBox(wxString(std::string("Error : Max length is " + PlusLib::int2str(tmp) + ". " + PlusLib::int2str(textBuffer.size() - tmp) + " characters ignored").c_str(), wxConvUTF8));
        q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+4]->SetValue(textBuffer.substr(0, tmp-1));
    }
    else q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+4]->SetValue(textBuffer);
}

void MainFrame::OnBtnPasteClient(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;

    if(textBuffer.empty())
    {
        wxMessageBox(_T("Error : The buffer is empty"));
        return;
    }

    int tmp = (qdata.quest->language_offset[current_sub_sizer_id - Q_TEXT_ENGLISH].client.size()-TEXT_SUB)/2;

    if((int)textBuffer.size() > tmp)
    {
        wxMessageBox(wxString(std::string("Error : Max length is " + PlusLib::int2str(tmp) + ". " + PlusLib::int2str(textBuffer.size() - tmp) + " characters ignored").c_str(), wxConvUTF8));
        q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+5]->SetValue(textBuffer.substr(0, tmp-1));
    }
    else q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+5]->SetValue(textBuffer);
}

void MainFrame::OnBtnPasteSubquest(wxCommandEvent &event)
{
    if(!qdata.quest || current_sub_sizer_id < Q_TEXT_ENGLISH || current_sub_sizer_id > Q_TEXT_ITALIAN || qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == 0 || (current_sub_sizer_id-Q_TEXT_ENGLISH > 0 && qdata.quest->quest_text_offset.language_offset[current_sub_sizer_id-Q_TEXT_ENGLISH] == qdata.quest->quest_text_offset.language_offset[0])) return;

    if(textBuffer.empty())
    {
        wxMessageBox(_T("Error : The buffer is empty"));
        return;
    }

    int tmp = (qdata.quest->language_offset[current_sub_sizer_id - Q_TEXT_ENGLISH].sub_quest.size()-TEXT_SUB)/2;

    if((int)textBuffer.size() > tmp)
    {
        wxMessageBox(wxString(std::string("Error : Max length is " + PlusLib::int2str(tmp) + ". " + PlusLib::int2str(textBuffer.size() - tmp) + " characters ignored").c_str(), wxConvUTF8));
        q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+6]->SetValue(textBuffer.substr(0, tmp-1));
    }
    else q_text[(current_sub_sizer_id - Q_TEXT_ENGLISH)*7+6]->SetValue(textBuffer);
}

void MainFrame::OnBtnAddSupplyGroup(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    writeSupplyBox(false);
    if(!qdata.modifySupplyGroup(true))
    {
        wxMessageBox(_T("Error: failed to add a group"));
    }
    readSupplyBox();
}

void MainFrame::OnBtnAddSupplyItem(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(qdata.quest->supply_list.items.size() >= 40)
    {
        wxMessageBox(_T("Error: maximum number of items reached (40)"));
        return;
    }

    writeSupplyBox(false);
    if(!qdata.modifySupplyItem(true))
    {
        wxMessageBox(_T("Error: failed to add an item"));
    }
    readSupplyBox();
}

void MainFrame::OnBtnDelSupplyGroup(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(qdata.quest->supply_box.size() < 2)
    {
        wxMessageBox(_T("Error: can't delete last group"));
        return;
    }

    writeSupplyBox();
    if(!qdata.modifySupplyGroup(false))
    {
        wxMessageBox(_T("Error: failed to delete a group"));
    }
    readSupplyBox();
}

void MainFrame::OnBtnDelSupplyItem(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(qdata.quest->supply_list.items.size() < 2)
    {
        wxMessageBox(_T("Error: can't delete last item"));
        return;
    }

    writeSupplyBox();
    if(!qdata.modifySupplyItem(false))
    {
        wxMessageBox(_T("Error: failed to delete an item"));
    }
    readSupplyBox();
}

void MainFrame::OnTextSupplyCount(wxCommandEvent &event)
{
    if(!qdata.quest || !enable_modif) return;

    int32_t total = 0;
    for(size_t i = 0; i < qdata.quest->supply_box.size(); i++)
    {
        if(PlusLib::is_number(std::string(q_supply.boxCount[i]->GetValue().mb_str())))
           total += PlusLib::str2int(std::string(q_supply.boxCount[i]->GetValue().mb_str()));
    }

    q_supply_text[3]->SetLabel(wxString(std::string("Total    " + PlusLib::int2str(total) + " item(s)").c_str(), wxConvUTF8));
    if(total > MAX_SUPPLY || total < 0) q_supply_text[3]->SetForegroundColour(wxColor(255,0,0));
    else q_supply_text[3]->SetForegroundColour(wxColor(0,0,0));
}

void MainFrame::checkID()
{
    uint32_t tmp_id = PlusLib::str2int(std::string(q_quest_id->GetValue().mb_str()));
    for(size_t i = 0; i < id_list.size(); i++)
    {
        if(id_list[i] ==  tmp_id)
        {
            q_quest_id->SetForegroundColour(wxColor(0,255,0));
            return;
        }
    }
    q_quest_id->SetForegroundColour(wxColor(0,0,0));
}

void MainFrame::OnTextQuestID(wxCommandEvent &event)
{
    if(!qdata.quest || !enable_modif) return;

    checkID();
}

void MainFrame::OnBtnSupplyHelp(wxCommandEvent &event)
{
    wxMessageBox(_T("Each group are independant of each other"));
    wxMessageBox(_T("The maximum number of items is 40"));
    wxMessageBox(_T("The order of the groups determines the\nitem positions in the supply box"));
}

void MainFrame::OnBtnCreateQuest(wxCommandEvent &event)
{

}

void MainFrame::OnBtnProxyFiles(wxCommandEvent &event)
{
    wxDirDialog openDirDialog(this, _T("Quest folder"), _T(""), wxDD_DEFAULT_STYLE|wxDD_DIR_MUST_EXIST);
    if(openDirDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    q_tool_proxy_folder->ChangeValue(openDirDialog.GetPath());
}

void MainFrame::OnBtnGenerateProxyFiles(wxCommandEvent &event)
{
    wxDir dir(q_tool_proxy_folder->GetValue());

    if( !dir.IsOpened() )
    {
        wxMessageBox(_T("Error: Invalid folder selected"));
        return;
    }

    if(q_tool_pregion->GetSelection() < 0 || q_tool_pregion->GetSelection() > 3)
    {
        wxMessageBox(_T("Error: Invalid region selected"));
        return;
    }

    wxArrayString mib_list;
    if(dir.GetAllFiles(q_tool_proxy_folder->GetValue(), &mib_list, _T("*.mib"), wxDIR_FILES) <= 0)
    {
        wxMessageBox(_T("Error: No .mib found"));
        return;
    }

    char lc[2], bom[3];
    lc[0] = 0x0d;
    lc[1] = 0x0a;
    bom[0] = 0xef;
    bom[1] = 0xbb;
    bom[2] = 0xbf;

    MH4U_QE tmp_qe;
    if(q_tool_pregion->GetSelection() == 1)
    {
        boost::filesystem::ofstream p_out[5];
        boost::filesystem::path p_path[5];

        for(size_t j = 0; j < 5; j++)
        {
            p_path[j] = q_tool_proxy_folder->GetValue().wc_str();
            switch(j)
            {
                case 0: p_path[j] += std::string("\\DLC_EventQuestInfo_eng.txt"); break;
                case 1: p_path[j] += std::string("\\DLC_EventQuestInfo_fre.txt"); break;
                case 2: p_path[j] += std::string("\\DLC_EventQuestInfo_spa.txt"); break;
                case 3: p_path[j] += std::string("\\DLC_EventQuestInfo_ger.txt"); break;
                case 4: p_path[j] += std::string("\\DLC_EventQuestInfo_ita.txt"); break;
            }
            p_out[j].open(p_path[j], std::ios::out | std::ios::trunc | std::ios::binary);
            p_out[j].write(bom, 3); // BOM
        }

        for(size_t i = 0; i < mib_list.size(); i++)
        {
            if(tmp_qe.load(mib_list[i].wc_str(), 1))
            {
                for(size_t j = 0; j < 5; j++)
                {
                    p_out[j].write("2015010", 7);
                    if(i < 100) p_out[j].write("1", 1);
                    if(i < 10) p_out[j].write("0", 1);
                    p_out[j].write(PlusLib::int2str(i).c_str(), PlusLib::int2str(i).size());
                    p_out[j].write("|", 1);
                    p_out[j].write(PlusLib::int2str(tmp_qe.quest->id).c_str(), 5);

                    p_out[j].write("|", 1);
                    writeUTF8String(convertStrFromQuest(tmp_qe.quest->language_offset[j].title), p_out[j]);
                    p_out[j].write("|", 1);

                    p_out[j].write(PlusLib::int2str(tmp_qe.quest->flags).c_str(), PlusLib::int2str(tmp_qe.quest->flags).size());
                    p_out[j].write("|", 1);
                    p_out[j].write(PlusLib::int2str(tmp_qe.quest->stars).c_str(), PlusLib::int2str(tmp_qe.quest->stars).size());
                    p_out[j].write("|0|", 3);
                    p_out[j].write(PlusLib::int2str(tmp_qe.quest->locale).c_str(), PlusLib::int2str(tmp_qe.quest->locale).size());
                    p_out[j].write("|", 1);
                    p_out[j].write(PlusLib::int2str(tmp_qe.quest->time).c_str(), PlusLib::int2str(tmp_qe.quest->time).size());
                    p_out[j].write("|", 1);
                    p_out[j].write(PlusLib::int2str(tmp_qe.quest->reward).c_str(), PlusLib::int2str(tmp_qe.quest->reward).size());
                    p_out[j].write("|", 1);
                    p_out[j].write(PlusLib::int2str(tmp_qe.quest->fee).c_str(), PlusLib::int2str(tmp_qe.quest->fee).size());
                    p_out[j].write("|", 1);
                    for(size_t k = 0; k < 5; k++)
                    {
                        p_out[j].write(PlusLib::int2str(tmp_qe.quest->boss_icons[k]).c_str(), PlusLib::int2str(tmp_qe.quest->boss_icons[k]).size());
                        p_out[j].write("|", 1);
                    }
                    p_out[j].write(PlusLib::int2str(tmp_qe.quest->terms1).c_str(), PlusLib::int2str(tmp_qe.quest->terms1).size());
                    p_out[j].write("|", 1);
                    p_out[j].write(PlusLib::int2str(tmp_qe.quest->terms2).c_str(), PlusLib::int2str(tmp_qe.quest->terms2).size());
                    p_out[j].write("|", 1);

                    writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[j].success), _T("\n"), 0), p_out[j]);
                    p_out[j].write("|", 1);
                    writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[j].success), _T("\n"), 1), p_out[j]);
                    p_out[j].write("|", 1);
                    writeUTF8String(convertStrFromQuest(tmp_qe.quest->language_offset[j].sub_quest), p_out[j]);
                    p_out[j].write("|", 1);
                    writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[j].failure), _T("\n"), 0), p_out[j]);
                    p_out[j].write("|", 1);
                    writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[j].failure), _T("\n"), 1), p_out[j]);
                    p_out[j].write("|", 1);
                    writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[j].main_monsters), _T("\n"), 0), p_out[j]);
                    p_out[j].write("|", 1);
                    writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[j].main_monsters), _T("\n"), 1), p_out[j]);
                    p_out[j].write("|", 1);
                    writeUTF8String(convertStrFromQuest(tmp_qe.quest->language_offset[j].client), p_out[j]);
                    p_out[j].write("|", 1);
                    for(size_t popo = 0; popo < 7; popo++)
                    {
                        writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[j].summary), _T("\n"), popo), p_out[j]);
                        p_out[j].write("|", 1);
                    }
                    p_out[j].write(lc, 2);
                }
            }
        }
        MH4U_Crypto tmp_crypto;
        tmp_crypto.setRegion(1);
        for(size_t j = 0; j < 5; j++)
        {
            p_out[j].close();
            if(tmp_crypto.encrypt(p_path[j].wstring(), p_path[j].wstring()) != 0)
                wxMessageBox(_T("Error: One of the file can't be encrypted"));
        }
        wxMessageBox(_T("Operation terminated"));
    }
    else
    {
        boost::filesystem::ofstream p_out;
        boost::filesystem::path p_path = q_tool_proxy_folder->GetValue().wc_str();

        switch(q_tool_pregion->GetSelection())
        {
            case 0: p_path += std::string("\\DLC_EventQuestInfo_jpn.txt"); break;
            case 2: p_path += std::string("\\DLC_EventQuestInfo_kor.txt"); break;
            case 3: p_path += std::string("\\DLC_EventQuestInfo_jpn.txt"); break;
            default: return;
        }
        p_out.open(p_path, std::ios::out | std::ios::trunc | std::ios::binary);
        p_out.write(bom, 3);

        for(size_t i = 0; i < mib_list.size(); i++)
        {
            if(tmp_qe.load(mib_list[i].wc_str(), 1))
            {
                p_out.write("2015010", 7);
                if(i < 100) p_out.write("1", 1);
                if(i < 10) p_out.write("0", 1);
                p_out.write(PlusLib::int2str(i).c_str(), PlusLib::int2str(i).size());
                p_out.write("|", 1);
                p_out.write(PlusLib::int2str(tmp_qe.quest->id).c_str(), 5);

                p_out.write("|", 1);
                writeUTF8String(convertStrFromQuest(tmp_qe.quest->language_offset[0].title), p_out);
                p_out.write("|", 1);

                p_out.write(PlusLib::int2str(tmp_qe.quest->flags).c_str(), PlusLib::int2str(tmp_qe.quest->flags).size());
                p_out.write("|", 1);
                p_out.write(PlusLib::int2str(tmp_qe.quest->stars).c_str(), PlusLib::int2str(tmp_qe.quest->stars).size());
                p_out.write("|0|", 3);
                p_out.write(PlusLib::int2str(tmp_qe.quest->locale).c_str(), PlusLib::int2str(tmp_qe.quest->locale).size());
                p_out.write("|", 1);
                p_out.write(PlusLib::int2str(tmp_qe.quest->time).c_str(), PlusLib::int2str(tmp_qe.quest->time).size());
                p_out.write("|", 1);
                p_out.write(PlusLib::int2str(tmp_qe.quest->reward).c_str(), PlusLib::int2str(tmp_qe.quest->reward).size());
                p_out.write("|", 1);
                p_out.write(PlusLib::int2str(tmp_qe.quest->fee).c_str(), PlusLib::int2str(tmp_qe.quest->fee).size());
                p_out.write("|", 1);
                for(size_t k = 0; k < 5; k++)
                {
                    p_out.write(PlusLib::int2str(tmp_qe.quest->boss_icons[k]).c_str(), PlusLib::int2str(tmp_qe.quest->boss_icons[k]).size());
                    p_out.write("|", 1);
                }
                p_out.write(PlusLib::int2str(tmp_qe.quest->terms1).c_str(), PlusLib::int2str(tmp_qe.quest->terms1).size());
                p_out.write("|", 1);
                p_out.write(PlusLib::int2str(tmp_qe.quest->terms2).c_str(), PlusLib::int2str(tmp_qe.quest->terms2).size());
                p_out.write("|", 1);

                writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[0].success), _T("\n"), 0), p_out);
                p_out.write("|", 1);
                writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[0].success), _T("\n"), 1), p_out);
                p_out.write("|", 1);
                writeUTF8String(convertStrFromQuest(tmp_qe.quest->language_offset[0].sub_quest), p_out);
                p_out.write("|", 1);
                writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[0].failure), _T("\n"), 0), p_out);
                p_out.write("|", 1);
                writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[0].failure), _T("\n"), 1), p_out);
                p_out.write("|", 1);
                writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[0].main_monsters), _T("\n"), 0), p_out);
                p_out.write("|", 1);
                writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[0].main_monsters), _T("\n"), 1), p_out);
                p_out.write("|", 1);
                writeUTF8String(convertStrFromQuest(tmp_qe.quest->language_offset[0].client), p_out);
                p_out.write("|", 1);
                for(size_t popo = 0; popo < 7; popo++)
                {
                    writeUTF8String(getStringPart(convertStrFromQuest(tmp_qe.quest->language_offset[0].summary), _T("\n"), popo), p_out);
                    p_out.write("|", 1);
                }
                p_out.write(lc, 2);
            }
        }
        MH4U_Crypto tmp_crypto;
        tmp_crypto.setRegion(q_tool_pregion->GetSelection());
        p_out.close();
        if(tmp_crypto.encrypt(p_path.wstring(), p_path.wstring()) != 0)
            wxMessageBox(_T("Error: The file can't be encrypted"));
        wxMessageBox(_T("Operation terminated"));
    }
}

XYZ MainFrame::generateMonsterPosition(int32_t boss_id, int32_t map_id, bool &result, uint8_t &result_area, uint16_t area_count, uint16_t forbid_area)
{
    result = false;
    if(map_id == 0x0d)
    {
        wxMessageBox(_T("Can't use this function for everwood"));
        return XYZ();
    }

    if(area_count == 0)
    {
        wxMessageBox(_T("Invalid map"));
        return XYZ();
    }

    bool tmp_bo;
    uint16_t area_id;
    for(uint32_t i = 50; i > 0; i--)
    {
        area_id = (rand() % area_count) + 1;
        if(area_id != forbid_area)
        {
            XYZ xyz;
            if(i > 25) xyz = placeMonster(boss_id, map_id, area_id, tmp_bo, false);
            else xyz = placeMonster(boss_id, map_id, area_id, tmp_bo, true);
            if(tmp_bo)
            {
                result_area = area_id;
                result = true;
                return xyz;
            }
        }
    }

    return XYZ();
}

XYZ MainFrame::placeMonster(int32_t boss_id, int32_t map_id, int32_t area_id, bool &result, bool try2force)
{
    result = false;
    if(map_id == 0x0d)
    {
        wxMessageBox(_T("Can't use this function for everwood"));
        return XYZ();
    }

    if(coordinate[boss_id][map_id][area_id].empty())
    {
        if(!try2force) return XYZ();
        boss_id = 124;
        if(coordinate[boss_id][map_id][area_id].empty())
        {
            wxMessageBox(_T("No known coordinate for this monster and area"));
            return XYZ();
        }
    }
    int32_t d = rand() % coordinate[boss_id][map_id][area_id].size();
    result = true;
    return coordinate[boss_id][map_id][area_id][d];
}


void MainFrame::OnBtnPlaceBoss(wxCommandEvent &event)
{
    if(!qdata.quest || (q_current_boss < 0 && q_current_boss >= q_boss_count)) return;

    int32_t a = monster_table[q_boss.q_boss_id->GetSelection()];
    int32_t b = map_table[q_map->GetSelection()]+1;
    int32_t c = PlusLib::str2int(std::string(q_boss.q_boss_spawn_area->GetValue().mb_str()));
    bool result;
    XYZ xyz = placeMonster(a, b, c, result);

    if(result)
    {
        q_boss.q_boss_cor_x->ChangeValue(wxString(PlusLib::float2str(xyz.x).c_str(), wxConvUTF8));
        q_boss.q_boss_cor_y->ChangeValue(wxString(PlusLib::float2str(xyz.y).c_str(), wxConvUTF8));
        q_boss.q_boss_cor_z->ChangeValue(wxString(PlusLib::float2str(xyz.z).c_str(), wxConvUTF8));
    }
}

void MainFrame::OnBtnPlaceMonster(wxCommandEvent &event)
{
    if(!qdata.quest || (q_current_monster < 0 && q_current_monster >= q_monster_count) || (q_current_mgroup < 0 && q_current_mgroup >= q_mgroup_count)) return;

    int32_t a = monster_table[q_monster.q_monster_id->GetSelection()];
    int32_t b = map_table[q_map->GetSelection()]+1;
    int32_t c = PlusLib::str2int(std::string(q_monster.q_monster_spawn_area->GetValue().mb_str()));
    bool result;
    XYZ xyz = placeMonster(a, b, c, result);

    if(result)
    {
        q_monster.q_monster_cor_x->ChangeValue(wxString(PlusLib::float2str(xyz.x).c_str(), wxConvUTF8));
        q_monster.q_monster_cor_y->ChangeValue(wxString(PlusLib::float2str(xyz.y).c_str(), wxConvUTF8));
        q_monster.q_monster_cor_z->ChangeValue(wxString(PlusLib::float2str(xyz.z).c_str(), wxConvUTF8));
    }
}

void MainFrame::OnBtnPlaceIntruder(wxCommandEvent &event)
{
    if(!qdata.quest || (q_current_intruder < 0 && q_current_intruder >= q_intruder_count)) return;

    int32_t a = monster_table[q_intruder.q_id->GetSelection()];
    int32_t b = map_table[q_map->GetSelection()]+1;
    int32_t c = PlusLib::str2int(std::string(q_intruder.q_spawn_area->GetValue().mb_str()));
    bool result;
    XYZ xyz = placeMonster(a, b, c, result);

    if(result)
    {
        q_intruder.q_cor_x->ChangeValue(wxString(PlusLib::float2str(xyz.x).c_str(), wxConvUTF8));
        q_intruder.q_cor_y->ChangeValue(wxString(PlusLib::float2str(xyz.y).c_str(), wxConvUTF8));
        q_intruder.q_cor_z->ChangeValue(wxString(PlusLib::float2str(xyz.z).c_str(), wxConvUTF8));
    }
}

void MainFrame::setSizer(int id)
{
    if(id >= 0 && id < Q_COUNT)
    {
        if(current_sub_sizer == sub_sizers[id]) return;
        if(current_sub_sizer != NULL) current_sub_sizer->Hide();//sizer_vertical->Detach(current_sub_sizer);
        current_sub_sizer = sub_sizers[id];
        current_sub_sizer->Show();//sizer_vertical->Add(current_sub_sizer, 18, wxALL | wxEXPAND, 0);
        sizer_vertical->Layout();
        current_sub_sizer_id = id;
    }
}

void MainFrame::initAll()
{
    initQuestInfo();
    initQuestDetail();
    initSupplyBox();
    initReward();
    initBossGroup();
    initMonsterGroup();
    initText();
    initChallenge();
    initIntruder();
    initTools();
    initTest();
    initCreate();
    initPalico();
    initSave();
}

void MainFrame::initTools()
{
    new wxStaticText(sub_sizers[Q_TOOLS], -1, _T("Additional Tools"), wxPoint(L_TEXT_X,L_TEXT_Y));

    new wxStaticLine(sub_sizers[Q_TOOLS], -1, wxPoint(0,23), wxSize(500, 2));

    new wxStaticText(sub_sizers[Q_TOOLS], -1, _T("Encryption functions (for Quests and Palicoes)"), wxPoint(L_TEXT_X,25+L_TEXT_Y));
    new wxButton(sub_sizers[Q_TOOLS], ID_BTN_TOOL_IN, _T("File to decrypt/encrypt"), wxPoint(10,50), wxSize(140, 23));
    q_tool_file_in = new wxTextCtrl(sub_sizers[Q_TOOLS], -1, _T(""), wxPoint(155,50), wxSize(335, 23));
    new wxButton(sub_sizers[Q_TOOLS], ID_BTN_TOOL_OUT, _T("Save the result as..."), wxPoint(10,75), wxSize(140, 23));
    q_tool_file_out = new wxTextCtrl(sub_sizers[Q_TOOLS], -1, _T(""), wxPoint(155,75), wxSize(335, 23));

    new wxButton(sub_sizers[Q_TOOLS], ID_BTN_DECRYPT, _T("Decrypt"), wxPoint(10,100));
    new wxButton(sub_sizers[Q_TOOLS], ID_BTN_ENCRYPT, _T("Encrypt"), wxPoint(85,100));
    q_tool_region = new wxChoice(sub_sizers[Q_TOOLS], -1, wxPoint(160,101));
    q_tool_region->Freeze();
    q_tool_region->Append(_T("Japan"));
    q_tool_region->Append(_T("US/Europe"));
    q_tool_region->Append(_T("Korea"));
    q_tool_region->Append(_T("Taiwan"));
    q_tool_region->Thaw();
    q_tool_region->SetSelection(iniFile.getIntData("TOOL_REGION", "GENERAL"));

    new wxStaticLine(sub_sizers[Q_TOOLS], -1, wxPoint(0,123), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_TOOLS], -1, _T("Proxy files"), wxPoint(L_TEXT_X,125+L_TEXT_Y));

    new wxButton(sub_sizers[Q_TOOLS], ID_BTN_TOOL_PROXY_FILES, _T("Quest files folder..."), wxPoint(10,150), wxSize(140, 23));
    q_tool_proxy_folder = new wxTextCtrl(sub_sizers[Q_TOOLS], -1, Dec2Str(iniFile.getStrData("TOOL_QUEST_FOLDER", "GENERAL")), wxPoint(155,150), wxSize(335, 23));

    q_tool_pregion = new wxChoice(sub_sizers[Q_TOOLS], -1, wxPoint(10,176), wxSize(140, 23));
    q_tool_pregion->Freeze();
    q_tool_pregion->Append(_T("Japan"));
    q_tool_pregion->Append(_T("US/Europe"));
    q_tool_pregion->Append(_T("Korea"));
    q_tool_pregion->Append(_T("Taiwan"));
    q_tool_pregion->Thaw();
    q_tool_pregion->SetSelection(iniFile.getIntData("TOOL_PROXY_REGION", "GENERAL"));
    new wxButton(sub_sizers[Q_TOOLS], ID_BTN_TOOL_GEN_FILES, _T("Generate DLC_EventQuestInfo.txt"), wxPoint(155,175), wxSize(200, 23));

    new wxStaticLine(sub_sizers[Q_TOOLS], -1, wxPoint(0,198), wxSize(500, 2));
}
void MainFrame::initCreate()
{
    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Create a simple quest"), wxPoint(L_TEXT_X,L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Quest name"), wxPoint(L_TEXT_X,25+L_TEXT_Y));
    qc.name = new wxTextCtrl(sub_sizers[Q_CREATE], -1, _T("CQ"), wxPoint(100,25), wxSize(300, 23));
    qc.name->SetMaxLength(31);
    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Quest ID"), wxPoint(L_TEXT_X,50+L_TEXT_Y));
    qc.id = new wxChoice(sub_sizers[Q_CREATE], -1, wxPoint(100,50), wxSize(125, 23));
    qc.id->Freeze();
    for(size_t i = 0; i < id_list.size(); i++)
    {
        if(id_list[i] >= 60000 && id_list[i] < 60100) qc.id->Append(wxString(std::string(PlusLib::int2str(id_list[i]) + " (Low rank)").c_str(), wxConvUTF8));
        else if(id_list[i] >= 60100 && id_list[i] < 60200) qc.id->Append(wxString(std::string(PlusLib::int2str(id_list[i]) + " (High rank)").c_str(), wxConvUTF8));
        else if(id_list[i] >= 60200 && id_list[i] < 60300) qc.id->Append(wxString(std::string(PlusLib::int2str(id_list[i]) + " (G rank)").c_str(), wxConvUTF8));
        else if(id_list[i] >= 61000 && id_list[i] < 61100) qc.id->Append(wxString(std::string(PlusLib::int2str(id_list[i]) + " (Low rank)").c_str(), wxConvUTF8));
        else if(id_list[i] >= 61100 && id_list[i] < 61200) qc.id->Append(wxString(std::string(PlusLib::int2str(id_list[i]) + " (High rank)").c_str(), wxConvUTF8));
        else if(id_list[i] >= 61200 && id_list[i] < 61300) qc.id->Append(wxString(std::string(PlusLib::int2str(id_list[i]) + " (G rank)").c_str(), wxConvUTF8));
    }

    qc.id->Thaw();
    qc.id->SetSelection(0);

    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Rank"), wxPoint(250+L_TEXT_X,50+L_TEXT_Y));
    qc.rank = new wxChoice(sub_sizers[Q_CREATE], -1, wxPoint(350,50), wxSize(125, 23));
    qc.rank->Freeze();
    qc.rank->Append(_T("Low Rank"));
    qc.rank->Append(_T("High Rank"));
    qc.rank->Append(_T("G Rank"));
    qc.rank->Thaw();
    qc.rank->SetSelection(0);

    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Map"), wxPoint(L_TEXT_X,75+L_TEXT_Y));
    qc.map_id = new wxChoice(sub_sizers[Q_CREATE], -1, wxPoint(100,75), wxSize(125, 23));
    qc.map_id->Freeze();
    qc.map_id->Append(map_list);
    qc.map_id->Thaw();
    qc.map_id->SetSelection(0);

    /*new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Difficulty"), wxPoint(250+L_TEXT_X,75+L_TEXT_Y));
    qc.difficulty = new wxChoice(sub_sizers[Q_CREATE], -1, wxPoint(350,75), wxSize(125, 23));
    qc.difficulty->Freeze();
    qc.difficulty->Append(_T("Easy"));
    qc.difficulty->Append(_T("Normal"));
    qc.difficulty->Append(_T("Hard"));
    qc.difficulty->Append(_T("Impossible"));
    qc.difficulty->Thaw();
    qc.difficulty->SetSelection(0);*/

    //qc.arena_grid = new wxCheckBox(sub_sizers[Q_CREATE], -1, _T("Use arena fence"), wxPoint(L_TEXT_X,100+L_TEXT_Y));

    new wxStaticLine(sub_sizers[Q_CREATE], -1, wxPoint(0,123-25), wxSize(500, 2));
    /*new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Hunt Randomizer"), wxPoint(L_TEXT_X,125+L_TEXT_Y));
    qc.randomizer[0] = new wxCheckBox(sub_sizers[Q_CREATE], -1, _T("Enable"), wxPoint(100+L_TEXT_X,125+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("If enabled, please ignore the objective and boss sections"), wxPoint(170+L_TEXT_X,125+L_TEXT_Y));

    qc.randomizer[1] = new wxCheckBox(sub_sizers[Q_CREATE], -1, _T("Add intruder if possible"), wxPoint(L_TEXT_X,150+L_TEXT_Y));
    qc.randomizer[2] = new wxCheckBox(sub_sizers[Q_CREATE], -1, _T("Random boss forms"), wxPoint(130+L_TEXT_X,150+L_TEXT_Y));
    qc.randomizer[3] = new wxCheckBox(sub_sizers[Q_CREATE], -1, _T("Random Frenzy"), wxPoint(250+L_TEXT_X,150+L_TEXT_Y));
    qc.randomizer[4] = new wxCheckBox(sub_sizers[Q_CREATE], -1, _T("Random Apex"), wxPoint(345+L_TEXT_X,150+L_TEXT_Y));
    qc.randomizer[5] = new wxCheckBox(sub_sizers[Q_CREATE], -1, _T("Random Map"), wxPoint(L_TEXT_X,175+L_TEXT_Y));


    new wxStaticLine(sub_sizers[Q_CREATE], -1, wxPoint(0,223), wxSize(500, 2));*/
    #define SPACE_POS 125
    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Objectives"), wxPoint(L_TEXT_X,225+L_TEXT_Y-SPACE_POS));

    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Main 1"), wxPoint(L_TEXT_X,250+L_TEXT_Y-SPACE_POS));
    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Main 2"), wxPoint(L_TEXT_X,275+L_TEXT_Y-SPACE_POS));
    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Subquest"), wxPoint(L_TEXT_X,300+L_TEXT_Y-SPACE_POS));

    for(uint32_t i = 0; i < 3; i++)
    {
        qc.objective_type[i] = new wxChoice(sub_sizers[Q_CREATE], ID_CHO_CREATE_OBJTYPE, wxPoint(70,250+25*i-SPACE_POS), wxSize(125, 23));
        qc.objective_type[i]->Freeze();
        qc.objective_type[i]->Append(_T("No objective"));
        qc.objective_type[i]->Append(_T("Hunt"));
        qc.objective_type[i]->Append(_T("Deliver"));
        qc.objective_type[i]->Append(_T("Capture"));
        qc.objective_type[i]->Append(_T("Slay"));
        qc.objective_type[i]->Append(_T("Break part"));
        qc.objective_type[i]->Append(_T("Mount a monster"));
        qc.objective_type[i]->Append(_T("Suppress the frenzy"));
        qc.objective_type[i]->Append(_T("Return on the wagon"));
        qc.objective_type[i]->Thaw();
        qc.objective_type[i]->SetSelection(0);
        qc.objective_idA[i] = new wxChoice(sub_sizers[Q_CREATE], ID_CHO_CREATE_OBJID, wxPoint(195,250+25*i-SPACE_POS), wxSize(125, 23));
        qc.objective_idB[i] = new wxTextCtrl(sub_sizers[Q_CREATE], -1, _T("0"), wxPoint(195,250+25*i-SPACE_POS), wxSize(125, 23));
        qc.objective_idB[i]->Hide();
        qc.objective_countA[i] = new wxChoice(sub_sizers[Q_CREATE], -1, wxPoint(320,250+25*i-SPACE_POS), wxSize(150, 23));
        qc.objective_countA[i]->Hide();
        qc.objective_countB[i] = new wxTextCtrl(sub_sizers[Q_CREATE], -1, _T("0"), wxPoint(320,250+25*i-SPACE_POS), wxSize(125, 23));
    }

    qc.tri_objective = new wxCheckBox(sub_sizers[Q_CREATE], -1, _T("Use subquest as the 3rd objective"), wxPoint(L_TEXT_X,325+L_TEXT_Y-SPACE_POS));

    new wxStaticLine(sub_sizers[Q_CREATE], -1, wxPoint(0,348-SPACE_POS), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Bosses"), wxPoint(L_TEXT_X,350+L_TEXT_Y-SPACE_POS));
    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("5 bosses max, 2 bosses max by wave"), wxPoint(280+L_TEXT_X,350+L_TEXT_Y-SPACE_POS));

    for(uint32_t i = 0; i < 5; i++)
    {
        new wxStaticText(sub_sizers[Q_CREATE], -1, wxString(std::string("Boss #" + PlusLib::int2str(i+1)).c_str(), wxConvUTF8), wxPoint(L_TEXT_X,375+(25*i)+L_TEXT_Y-SPACE_POS));
        qc.boss[i] = new wxChoice(sub_sizers[Q_CREATE], -1, wxPoint(55,375+25*i-SPACE_POS), wxSize(125, 23));
        qc.boss[i]->Freeze();
        qc.boss[i]->Append(monster_list);
        qc.boss[i]->Thaw();
        qc.boss[i]->SetSelection(i_monster_table[0]);

        qc.frenzy[i] = new wxChoice(sub_sizers[Q_CREATE], -1, wxPoint(185,375+25*i-SPACE_POS), wxSize(100, 23));
        qc.frenzy[i]->Freeze();
        qc.frenzy[i]->Append(frenzy_list);
        qc.frenzy[i]->Thaw();
        qc.frenzy[i]->SetSelection(0);

        qc.group[i] = new wxChoice(sub_sizers[Q_CREATE], -1, wxPoint(290,375+25*i-SPACE_POS), wxSize(100, 23));
        qc.group[i]->Freeze();
        qc.group[i]->Append(_T("Wave 1"));
        qc.group[i]->Append(_T("Wave 2"));
        qc.group[i]->Append(_T("Wave 3"));
        qc.group[i]->Append(_T("Wave 4"));
        qc.group[i]->Append(_T("Wave 5"));
        qc.group[i]->Thaw();
        qc.group[i]->SetSelection(0);
    }
    new wxStaticLine(sub_sizers[Q_CREATE], -1, wxPoint(0,498-SPACE_POS), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Others"), wxPoint(L_TEXT_X,500+L_TEXT_Y-SPACE_POS));
    //qc.small_mob = new wxCheckBox(sub_sizers[Q_CREATE], -1, _T("Add small monster if possible"), wxPoint(L_TEXT_X,525+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_CREATE], -1, _T("Game region"), wxPoint(L_TEXT_X,550+L_TEXT_Y-SPACE_POS-25));
    qc.region = new wxChoice(sub_sizers[Q_CREATE], -1, wxPoint(100,550-SPACE_POS-25), wxSize(125, 23));
    qc.region->Freeze();
    qc.region->Append(_T("Japan"));
    qc.region->Append(_T("US/Europe"));
    qc.region->Append(_T("Korea"));
    qc.region->Append(_T("Taiwan"));
    qc.region->Thaw();
    qc.region->SetSelection(iniFile.getIntData("CREATE_REGION", "GENERAL"));
    new wxStaticLine(sub_sizers[Q_CREATE], -1, wxPoint(0,573-SPACE_POS-25), wxSize(500, 2));

    new wxButton(sub_sizers[Q_CREATE], ID_BTN_GO_CREATE, _T("GO"), wxPoint(150,600-SPACE_POS-25), wxSize(200, 50));
    updateCreate(2);

    ((wxScrolledWindow*)(sub_sizers[Q_CREATE]))->SetVirtualSize(wxSize(450, 650-SPACE_POS-25));
    ((wxScrolledWindow*)(sub_sizers[Q_CREATE]))->SetScrollRate(5, 5);
    ((wxScrolledWindow*)(sub_sizers[Q_CREATE]))->EnableScrolling(false, true);
}

void MainFrame::OnChoCreateObjType(wxCommandEvent &event)
{
    updateCreate(0);
}

void MainFrame::OnChoCreateObjID(wxCommandEvent &event)
{
    updateCreate(1);
}

void MainFrame::updateCreate(int state)
{
    int32_t tmp;
    switch(state)
    {
        case 0: // update objective
        {
            for(uint32_t i = 0; i < 3; i++)
            {
                qc.preq_objective[i] = qc.q_objective[i];
                qc.q_objective[i] = qc.objective_type[i]->GetSelection();
                if(qc.q_objective[i] != qc.preq_objective[i])
                {
                    if(qc.preq_objective[i] != 5) qc.q_objcount[i] = PlusLib::str2int(std::string(qc.objective_countB[i]->GetValue().mb_str()));
                    else if(qc.objective_countA[i]->GetSelection() < 0 || qc.objective_countA[i]->GetSelection() > 7) qc.q_objcount[i] = 0;
                    else qc.q_objcount[i] = qc.objective_countA[i]->GetSelection();

                    if(qc.q_objective[i] == 2) // deliver
                    {
                        if(qc.preq_objective[i] >= 6 && qc.preq_objective[i] <= 8 && qc.preq_objective[i] != 7) qc.q_objid[i] = PlusLib::str2int(std::string(qc.objective_idB[i]->GetValue().mb_str())); // take from other
                        else vTab(qc.q_objid[i], qc.objective_idA[i]->GetSelection(), monster_table); // take from monster
                    }
                    else if(qc.q_objective[i] >= 6 && qc.q_objective[i] <= 8 && qc.q_objective[i] != 7) // others
                    {
                        if(qc.preq_objective[i] >= 6 && qc.preq_objective[i] <= 8 && qc.preq_objective[i] != 7) qc.q_objid[i] = PlusLib::str2int(std::string(qc.objective_idB[i]->GetValue().mb_str())); // take from other
                        else if(qc.preq_objective[i] == 2) vTab(qc.q_objid[i], qc.objective_idA[i]->GetSelection(), item_table); // take from item
                        else vTab(qc.q_objid[i], qc.objective_idA[i]->GetSelection(), monster_table); // take from monster
                    }
                    else // monster
                    {
                        if(qc.preq_objective[i] >= 6 && qc.preq_objective[i] <= 8 && qc.preq_objective[i] != 7) qc.q_objid[i] = PlusLib::str2int(std::string(qc.objective_idB[i]->GetValue().mb_str())); // take from other
                        else if(qc.preq_objective[i] == 2) vTab(qc.q_objid[i], qc.objective_idA[i]->GetSelection(), item_table); // take from item
                        else vTab(qc.q_objid[i], qc.objective_idA[i]->GetSelection(), monster_table); // take from monster
                    }
                }
                else
                {
                    for(uint32_t i = 0; i < 3; i++)
                    {
                        if(qc.q_objective[i] != 5) qc.q_objcount[i] = PlusLib::str2int(std::string(qc.objective_countB[i]->GetValue().mb_str())); // take from text
                        else qc.q_objcount[i] = qc.objective_countA[i]->GetSelection(); // take from part selection

                        if(qc.q_objective[i] == 2) vTab(qc.q_objid[i], qc.objective_idA[i]->GetSelection(), item_table); // take from item
                        else if(qc.q_objective[i] < 6 || qc.q_objective[i] > 8 || qc.q_objective[i] == 7) vTab(qc.q_objid[i], qc.objective_idA[i]->GetSelection(), monster_table); // take from monster
                        else qc.q_objid[i] = PlusLib::str2int(std::string(qc.objective_idB[i]->GetValue().mb_str())); // take from text
                    }
                }
            }
        }
        break;
        case 1: // update id
        {
            for(uint32_t i = 0; i < 3; i++)
            {
                if(qc.q_objective[i] != 5) qc.q_objcount[i] = PlusLib::str2int(std::string(qc.objective_countB[i]->GetValue().mb_str())); // take from text
                else qc.q_objcount[i] = qc.objective_countA[i]->GetSelection(); // take from part selection

                if(qc.q_objective[i] == 2) vTab(qc.q_objid[i], qc.objective_idA[i]->GetSelection(), item_table); // take from item
                else if(qc.q_objective[i] < 6 || qc.q_objective[i] > 8 || qc.q_objective[i] == 7) vTab(qc.q_objid[i], qc.objective_idA[i]->GetSelection(), monster_table); // take from monster
                else qc.q_objid[i] = PlusLib::str2int(std::string(qc.objective_idB[i]->GetValue().mb_str())); // take from text
            }
        }
        break;
        case 2: // nope
        {
            for(uint32_t i = 0; i < 3; i++)
            {
                qc.q_objective[i] = 0;
                qc.q_objid[i] = 0;
                qc.q_objcount[i] = 0;
            }
        }
        break;
    }

    for(uint32_t i = 0; i < 3; i++)
    {
        switch(qc.q_objective[i])
        {
            case 5: qc.objective_idA[i]->Freeze();
                    qc.objective_idA[i]->Clear();
                    qc.objective_idA[i]->Append(monster_list);
                    qc.objective_idA[i]->Thaw();
                    tmp = 0;
                    vTab(tmp, qc.q_objid[i], i_monster_table);
                    qc.objective_idA[i]->SetSelection(tmp);
                    qc.objective_idA[i]->Show();
                    qc.objective_idB[i]->Hide();
                    qc.objective_countA[i]->Freeze();
                    qc.objective_countA[i]->Clear();
                    for(uint32_t j = 0; j < 8; j++)
                    {
                        tmp = 0;
                        vTab(tmp, qc.objective_idA[i]->GetSelection(), monster_table);
                        if(parts[tmp][j].empty()) qc.objective_countA[i]->Append(wxString(std::string(PlusLib::int2str(j) + "- Unknown").c_str(), wxConvUTF8));
                        else qc.objective_countA[i]->Append(wxString(std::string(PlusLib::int2str(j) + "- " + parts[tmp][j]).c_str(), wxConvUTF8));
                    }
                    qc.objective_countA[i]->Thaw();
                    qc.objective_countB[i]->Hide();
                    qc.objective_countA[i]->Show();
                    if(qc.q_objcount[i] < 0 || qc.q_objcount[i] > 7) qc.q_objcount[i] = 0;
                    qc.objective_countA[i]->SetSelection(qc.q_objcount[i]);
                    break;
            case 1: case 3: case 4: case 7: qc.objective_idA[i]->Freeze();
                    qc.objective_idA[i]->Clear();
                    qc.objective_idA[i]->Append(monster_list);
                    qc.objective_idA[i]->Thaw();
                    tmp = 0;
                    vTab(tmp, qc.q_objid[i], i_monster_table);
                    qc.objective_idA[i]->SetSelection(tmp);
                    qc.objective_idA[i]->Show();
                    qc.objective_idB[i]->Hide();
                    qc.objective_countB[i]->ChangeValue(wxString(PlusLib::int2str(qc.q_objcount[i]).c_str(), wxConvUTF8));
                    qc.objective_countB[i]->Show();
                    qc.objective_countA[i]->Hide();
                    break;
            case 2: qc.objective_idA[i]->Freeze();
                    qc.objective_idA[i]->Clear();
                    qc.objective_idA[i]->Append(item_list);
                    qc.objective_idA[i]->Thaw();
                    tmp = 0;
                    vTab(tmp, qc.q_objid[i], i_item_table);
                    qc.objective_idA[i]->SetSelection(tmp);
                    qc.objective_idA[i]->Show();
                    qc.objective_idB[i]->Hide();
                    qc.objective_countB[i]->ChangeValue(wxString(PlusLib::int2str(qc.q_objcount[i]).c_str(), wxConvUTF8));
                    qc.objective_countB[i]->Show();
                    qc.objective_countA[i]->Hide();
                    break;
            case 0: case 6: case 8: qc.objective_idB[i]->ChangeValue(wxString(PlusLib::int2str(qc.q_objid[i]).c_str(), wxConvUTF8));
                    qc.objective_idB[i]->Show();
                    qc.objective_idA[i]->Hide();
                    qc.objective_countB[i]->ChangeValue(wxString(PlusLib::int2str(qc.q_objcount[i]).c_str(), wxConvUTF8));
                    qc.objective_countB[i]->Show();
                    qc.objective_countA[i]->Hide();
                    break;
            default: qc.objective_idA[i]->Freeze();
                    qc.objective_idA[i]->Clear();
                    qc.objective_idA[i]->Thaw();
                    qc.objective_idA[i]->Show();
                    qc.objective_idB[i]->Hide();
                    qc.objective_idA[i]->Show();
                    qc.objective_idB[i]->Hide();
                    break;
        }
    }
}

void MainFrame::OnBtnGoCreate(wxCommandEvent &event)
{
    //wxMessageBox(_T("Work in progress.\nCurrently disabled."));
    //return;

    /*if(qc.randomizer[0]->GetValue())
    {
        wxMessageBox(_T("The randomizer is currently disabled"));
        return;
    }*/

    if(map_table[qc.map_id->GetSelection()]+1 == 0x0d)
    {
        wxMessageBox(_T("Can't create a quest with the Everwood map"));
        return;
    }

    if(qdata.quest && (wxMessageBox(_T("A quest is already loaded. Continue ?"), _T("Confirm"), wxNO) == wxNO))
        return;

    uint16_t area_count[22] = {0, 10, 10, 10, 9, 8, 2, 1, 1, 1, 1, 1, 1, 0, 1, 10, 1, 11, 11, 3, 1, 1};
    uint16_t forbid_area[22] = {0, 10, 10, 10, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 11, 11, 0, 0, 0};

    int32_t wave_count = 0;
    int32_t boss_count = 0;
    std::vector<int32_t> boss_list[5];
    std::vector<int32_t> boss_frenzy_list[5];
    for(uint32_t i = 0; i < 5; i++)
    {
        if(monster_table[qc.boss[i]->GetSelection()] != 0)
        {
            if(qc.group[i]->GetSelection()+1 > wave_count) wave_count = qc.group[i]->GetSelection()+1;
            boss_list[qc.group[i]->GetSelection()].push_back(monster_table[qc.boss[i]->GetSelection()]);
            boss_frenzy_list[qc.group[i]->GetSelection()].push_back(qc.frenzy[i]->GetSelection());
            boss_count++;
            if(boss_list[qc.group[i]->GetSelection()].size() > 2)
            {
                wxMessageBox(_T("The maximum is 2 boss by wave."));
                return;
            }
        }
    }

    bool multi_lang = false;
    bool loaded = false;
    if(qc.region->GetSelection() == 1)
    {
        loaded = qc.data.load(L"data/dummy/dummy_multi.mib");
        multi_lang = true;
    }
    else loaded = qc.data.load(L"data/dummy/dummy_jpn.mib");

    if(loaded)
    {
        // quest text
        {
            std::vector<uint8_t> tmp;
            for(uint32_t i = 0; (i < 5 && multi_lang) || (i < 1 && !multi_lang); i++)
            {
                tmp = convertStrToQuest(qc.name->GetValue());
                while(tmp.size() < qc.data.quest->language_offset[i].title.size()) tmp.push_back(0x00);
                qc.data.quest->language_offset[i].title = tmp;

                tmp = convertStrToQuest(_T("Beta 1.0"));
                while(tmp.size() < qc.data.quest->language_offset[i].summary.size()) tmp.push_back(0x00);
                qc.data.quest->language_offset[i].summary = tmp;
            }
        }

        // id
        qc.data.quest->id = id_list[qc.id->GetSelection()];

        // map
        qc.data.quest->locale = map_table[qc.map_id->GetSelection()]+1;

        // rank
        qc.data.quest->carve_type = qc.rank->GetSelection()+1;
        qc.data.quest->collect_type = qc.rank->GetSelection()+1;
        qc.data.quest->monster_ai = (qc.rank->GetSelection()*2)+1;

        Quest_monster_stats empty_stats;
        empty_stats.size_percentage = 0;
        empty_stats.size_variation = 0;
        empty_stats.stamina = 0;
        empty_stats.unknown = 0;
        empty_stats.health = 0;
        empty_stats.attack = 0;
        empty_stats.defense = 0;

        Quest_monster_stats boss_stats;
        boss_stats.size_percentage = 100;
        boss_stats.size_variation = 5;
        boss_stats.stamina = 0;
        boss_stats.unknown = 0;
        switch(qc.rank->GetSelection())
        {
            case 0:
                boss_stats.health = 25;
                boss_stats.attack = 30;
                boss_stats.defense = 20;
                qc.data.quest->stars = 1;
                break;
            case 1:
                boss_stats.health = 40;
                boss_stats.attack = 60;
                boss_stats.defense = 50;
                qc.data.quest->stars = 4;
                break;
            case 2:
                boss_stats.health = 70;
                boss_stats.attack = 110;
                boss_stats.defense = 80;
                qc.data.quest->stars = 8;
                break;
        }

        // difficulty
        //uint32_t difficulty_rating = qc.difficulty->GetSelection() + (qc.rank->GetSelection() * 4);

        // arena grid
        /*if(qc.arena_grid->GetValue())
        {
            qc.data.quest->arena_gate = 1;
            qc.data.quest->gate_init = 0;
            qc.data.quest->grid_time[0] = 60;
            qc.data.quest->grid_time[1] = 60;
        }*/

        // objectives
        {
            uint32_t tmp, *ptr_a = NULL;
            uint16_t *ptr_b = NULL, *ptr_c = NULL;
            for(uint32_t i = 0; i < 3; i++)
            {
                switch(i)
                {
                    case 0: ptr_a = &qc.data.quest->main_quest1_type; ptr_b = &qc.data.quest->main_quest1_id; ptr_c = &qc.data.quest->main_quest1_count; break;
                    case 1: ptr_a = &qc.data.quest->main_quest2_type; ptr_b = &qc.data.quest->main_quest2_id; ptr_c = &qc.data.quest->main_quest2_count; break;
                    case 2: ptr_a = &qc.data.quest->sub_quest_type; ptr_b = &qc.data.quest->sub_quest_id; ptr_c = &qc.data.quest->sub_quest_count; break;
                }

                tmp = qc.objective_type[i]->GetSelection();
                switch(tmp)
                {
                    case 0: *ptr_a = 0x0; break;
                    case 1: *ptr_a = 0x1; break;
                    case 2: *ptr_a = 0x2; break;
                    case 3: *ptr_a = 0x81; break;
                    case 4: *ptr_a = 0x101; break;
                    case 5: *ptr_a = 0x204; break;
                    case 6: *ptr_a = 0x2004; break;
                    case 7: *ptr_a = 0x4004; break;
                    case 8: *ptr_a = 0x800; break;
                    default: break;
                }
                if(tmp == 1 || tmp == 3 || tmp == 4 || tmp == 5 || tmp == 7) PlusLib::validate16(PlusLib::short2str(monster_table[qc.objective_idA[i]->GetSelection()]), *ptr_b, 0, 0xFFFF, "Quest " + PlusLib::int2str(i+1) + " ID");
                else if(tmp == 2) PlusLib::validate16(PlusLib::short2str(item_table[qc.objective_idA[i]->GetSelection()]), *ptr_b, 0, 0xFFFF, "Quest " + PlusLib::int2str(i+1) + " ID");
                else if(tmp == 6 || tmp == 8 || tmp == 0) PlusLib::validate16(std::string(qc.objective_idB[i]->GetValue().mb_str()), *ptr_b, 0, 0xFFFF, "Quest " + PlusLib::int2str(i+1) + " ID");

                if(tmp == 5) PlusLib::validate16(PlusLib::int2str(qc.objective_countA[i]->GetSelection()), *ptr_c, 0, 0xFFFF, "Quest " + PlusLib::int2str(i+1) + " Part");
                else PlusLib::validate16(std::string(qc.objective_countB[i]->GetValue().mb_str()), *ptr_c, 0, 0xFFFF, "Quest " + PlusLib::int2str(i+1) + " Count");
            }

            // objective count
            if(qc.data.quest->main_quest1_type != 0 && qc.data.quest->main_quest2_type != 0) qc.data.quest->main_quest_count[3] = 2;
            else qc.data.quest->main_quest_count[3] = 1;

            // subquest and tri objective flag
            if(qc.tri_objective->GetValue() != ((qc.data.quest->flags >> 23) & 1)) qc.data.quest->flags ^= 1 << 23;
            if((qc.data.quest->flags >> 23) & 1) qc.data.quest->flags &= ~(1 << 22); // if triple, set to 0
            else if(qc.data.quest->sub_quest_type != 0) qc.data.quest->flags |= 1 << 22; // if sub, set to 1
            else qc.data.quest->flags &= ~(1 << 22); // if not, 0
        }

        // bosses
        uint32_t boss_id = 0;
        for(uint32_t i = 0; i < 5; i++)
        {
            if(!boss_list[i].empty())
            {
                while(qc.data.quest->boss_group_offset.size() <= i) qc.data.modifyBossGroup(0, true);
                for(uint32_t j = 0; j < boss_list[i].size(); j++)
                {
                    qc.data.modifyBoss(i, true);
                    qc.data.quest->boss_group[boss_id].boss_type = boss_list[i][j];
                    bool result;
                    XYZ xyz = generateMonsterPosition(qc.data.quest->boss_group[boss_id].boss_type, qc.data.quest->locale, result, qc.data.quest->boss_group[boss_id].spawn_area, area_count[qc.data.quest->locale], forbid_area[qc.data.quest->locale]);
                    if(!result)
                    {
                        wxMessageBox(_T("Failed to place the bosses"));
                        return;
                    }
                    qc.data.quest->boss_group[boss_id].coordinates[0] = xyz.x;
                    qc.data.quest->boss_group[boss_id].coordinates[1] = xyz.y;
                    qc.data.quest->boss_group[boss_id].coordinates[2] = xyz.z;
                    if((qc.data.quest->boss_group[boss_id].boss_type >= 112 && qc.data.quest->boss_group[boss_id].boss_type <= 115)
                    || (qc.data.quest->boss_group[boss_id].boss_type >= 119 && qc.data.quest->boss_group[boss_id].boss_type <= 122))
                    {
                        qc.data.quest->boss_group[boss_id].frenzy = 9;
                        boss_frenzy_list[i][j] = 4;
                    }
                    else
                    {
                        int32_t tmpf;
                        vTab(tmpf, boss_frenzy_list[i][j], frenzy_table);
                        qc.data.quest->boss_group[boss_id].frenzy = tmpf;
                    }
                    qc.data.quest->boss_stats[boss_id] = boss_stats;
                    boss_id++;
                }
            }
        }

        for(uint32_t i = boss_id; i < 5; i++)
            qc.data.quest->boss_stats[i] = empty_stats;
        qc.data.quest->monster_stats = empty_stats;

        // flags
        if((((qc.data.quest->flags >> 3) & 1) == 0 && qc.data.quest->boss_group_offset.size() > 1) // group flag
       || (((qc.data.quest->flags >> 3) & 1) == 1 && qc.data.quest->boss_group_offset.size() <= 1)) qc.data.quest->flags ^= 1 << 3;
        if(qc.data.quest->main_quest1_type == 0x81 || qc.data.quest->main_quest2_type == 0x81)  qc.data.quest->flags |= 1 << 1; // capture flag
        if(qc.data.quest->main_quest1_type == 0x101 || qc.data.quest->main_quest2_type == 0x101)  qc.data.quest->flags |= 1; // slay flag
        if(qc.data.quest->main_quest1_type == 0x1 || qc.data.quest->main_quest2_type == 0x1)
        {
            qc.data.quest->flags |= 1; // slay flag
            qc.data.quest->flags |= 1 << 1; // capture flag
        }
        if(qc.data.quest->main_quest1_type == 0x2 && qc.data.quest->main_quest2_type == 0x0) qc.data.quest->flags |= 1 << 2; // collect flag
        if(qc.data.quest->locale == 6) qc.data.quest->flags |= 1 << 27; // ship

        // icons
        {
            uint32_t icon_count = 0;
            bool hasFrenzy = false;
            bool hasApex = false;
            for(uint32_t i = 0; i < 5; i++)
            {
                if(!boss_list[i].empty())
                {
                    for(uint32_t j = 0; j < boss_list[i].size(); j++)
                    {
                        if(icon_translate[boss_list[i][j]] != 98)
                        {
                            qc.data.quest->boss_icons[icon_count] = icon_translate[boss_list[i][j]];
                            icon_count++;
                            if(boss_frenzy_list[i][j] >= 1 && boss_frenzy_list[i][j] <= 3) hasFrenzy = true;
                            else if(boss_frenzy_list[i][j] >= 4 && boss_frenzy_list[i][j] <= 6) hasApex = true;
                        }
                    }
                }
            }
            if(icon_count < 5 && ((qc.data.quest->flags >> 2) & 1) == 1)
            {
                qc.data.quest->boss_icons[icon_count] = 86;
                icon_count++;
            }
            else if(icon_count == 0)
            {
                qc.data.quest->boss_icons[0] = 87;
                icon_count++;
            }

            if(icon_count == 3)
            {
                if(hasFrenzy)
                {
                    qc.data.quest->boss_icons[icon_count] = 88;
                    icon_count++;
                    hasFrenzy = false;
                }
            }

            if(icon_count == 4)
            {
                if(hasApex)
                {
                    qc.data.quest->boss_icons[icon_count] = 123;
                    icon_count++;
                    hasApex = false;
                }
                else if(hasFrenzy)
                {
                    qc.data.quest->boss_icons[icon_count] = 88;
                    icon_count++;
                    hasFrenzy = false;
                }
            }

            for(uint32_t i = icon_count; i < 5; i++) qc.data.quest->boss_icons[i] = 98;
        }

        wxMessageBox(_T("Please choose where you want to save your quest"));
        wxFileDialog saveFileDialog(this, _T("Save Quest File"), _T(""), _T(""), _T(""), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
        if (saveFileDialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        if(!qc.data.encrypt(saveFileDialog.GetPath().wc_str(), qc.region->GetSelection()))
        {
            wxMessageBox(_T("Error : Saving failed"));
            return;
        }
        wxMessageBox(_T("Success"));
        commonLoad(saveFileDialog.GetPath());
    }
    else
    {
        wxMessageBox(_T("Something weird happened.\nPlease report to me."));
    }
}

void MainFrame::OnBtnGoNew(wxCommandEvent &event)
{
    setSizer(Q_CREATE);
}

void MainFrame::readAll()
{
    for(int i = Q_QUEST_EMPTY+1; i < Q_COUNT; i++) readOne(i);
}

void MainFrame::writeAll()
{
    for(int i = Q_QUEST_EMPTY+1; i < Q_COUNT; i++) writeOne(i);
}

void MainFrame::readOne(int id)
{
    switch(id)
    {
        case Q_QUEST_INFO: readQuestInfo(); break;
        case Q_QUEST_DETAIL: readQuestDetail(); break;
        case Q_SUPPLY_BOX: readSupplyBox(); break;
        case Q_REWARD_A: readReward(1); break;
        case Q_REWARD_B: readReward(2); break;
        case Q_REWARD_C: readReward(3); break;
        case Q_BOSS_GROUP: readBossGroup(); break;
        case Q_INTRUDER: readIntruderAll(); break;
        case Q_SMALL_GROUP: readMonsterGroup(); break;
        case Q_CHALLENGE: readChallenge(); break;
        case Q_TEXT_ENGLISH: readText(1); break;
        case Q_TEXT_FRENCH: readText(2); break;
        case Q_TEXT_SPANISH: readText(3); break;
        case Q_TEXT_GERMAN: readText(4); break;
        case Q_TEXT_ITALIAN: readText(5); break;
        case Q_TEST: readTest(); break;
        default: break;
    }
}

void MainFrame::writeOne(int id)
{
    switch(id)
    {
        case Q_QUEST_INFO: writeQuestInfo(); break;
        case Q_QUEST_DETAIL: writeQuestDetail(); break;
        case Q_SUPPLY_BOX: writeSupplyBox(); break;
        case Q_REWARD_A: writeReward(1); break;
        case Q_REWARD_B: writeReward(2); break;
        case Q_REWARD_C: writeReward(3); break;
        case Q_BOSS_GROUP: writeBossGroup(); break;
        case Q_INTRUDER: writeIntruderAll(); break;
        case Q_SMALL_GROUP: writeMonsterGroup(); break;
        case Q_CHALLENGE: writeChallenge(); break;
        case Q_TEXT_ENGLISH: writeText(1); break;
        case Q_TEXT_FRENCH: writeText(2); break;
        case Q_TEXT_SPANISH: writeText(3); break;
        case Q_TEXT_GERMAN: writeText(4); break;
        case Q_TEXT_ITALIAN: writeText(5); break;
        case Q_TEST: writeTest(); break;
        default: break;
    }
}

#define EQ_DIFF 50
void MainFrame::initQuestInfo()
{
    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Quest flags"), wxPoint(L_TEXT_X,0+L_TEXT_Y));
    q_flags_check[0] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Kill"), wxPoint(85,0+L_TEXT_Y));
    q_flags_check[1] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Capture"), wxPoint(130,0+L_TEXT_Y));
    q_flags_check[2] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Collect"), wxPoint(190,0+L_TEXT_Y));
    q_flags_check[3] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Repel"), wxPoint(245,0+L_TEXT_Y));
    //q_flags_check[4] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Subquest"), wxPoint(295,0+L_TEXT_Y));
    q_flags_check[5] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Challenge/Training"), wxPoint(295,0+L_TEXT_Y));
    //q_flags_check[6] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Hunt a group"), wxPoint(125,25+L_TEXT_Y));
    q_flags_check[6] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Ship integrity"), wxPoint(85,25+L_TEXT_Y));
    q_flags_check[8] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Hunt-a-thon"), wxPoint(170,25+L_TEXT_Y));
    //q_flags_check[9] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Special"), wxPoint(316,25+L_TEXT_Y));
    q_flags_check[10] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("No time over"), wxPoint(250,25+L_TEXT_Y));

    // moved dialogue (flag 14)
    q_flags_check[14] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Arena reward"), wxPoint(330,25+L_TEXT_Y));

    new wxStaticLine(sub_sizers[Q_QUEST_INFO], -1, wxPoint(0,EQ_DIFF-2), wxSize(500, 2));

    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Quest ID"), wxPoint(L_TEXT_X,EQ_DIFF+L_TEXT_Y));
    q_quest_id = new wxTextCtrl(sub_sizers[Q_QUEST_INFO], ID_TEXT_QUEST_ID, _T("0"), wxPoint(100,EQ_DIFF));
    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Quest fee"), wxPoint(250+L_TEXT_X,EQ_DIFF+L_TEXT_Y));
    q_fee = new wxTextCtrl(sub_sizers[Q_QUEST_INFO], -1, _T("0"), wxPoint(350,EQ_DIFF));


    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Hr Points"), wxPoint(L_TEXT_X,25+EQ_DIFF+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Hr Penalty"), wxPoint(L_TEXT_X,50+EQ_DIFF+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Hr Subquest"), wxPoint(L_TEXT_X,75+EQ_DIFF+L_TEXT_Y));
    q_hr_points = new wxTextCtrl(sub_sizers[Q_QUEST_INFO], -1, _T("0"), wxPoint(100,25+EQ_DIFF));
    q_hr_points_penalty = new wxTextCtrl(sub_sizers[Q_QUEST_INFO], -1, _T("0"), wxPoint(100,50+EQ_DIFF));
    q_sub_hr_points = new wxTextCtrl(sub_sizers[Q_QUEST_INFO], -1, _T("0"), wxPoint(100,75+EQ_DIFF));

    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Reward"), wxPoint(250+L_TEXT_X,25+EQ_DIFF+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Penalty"), wxPoint(250+L_TEXT_X,50+EQ_DIFF+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Subquest reward"), wxPoint(250+L_TEXT_X,75+EQ_DIFF+L_TEXT_Y));
    q_reward = new wxTextCtrl(sub_sizers[Q_QUEST_INFO], -1, _T("0"), wxPoint(350,25+EQ_DIFF));
    q_penalty = new wxTextCtrl(sub_sizers[Q_QUEST_INFO], -1, _T("0"), wxPoint(350,50+EQ_DIFF));
    q_sub_reward = new wxTextCtrl(sub_sizers[Q_QUEST_INFO], -1, _T("0"), wxPoint(350,75+EQ_DIFF));


    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Time"), wxPoint(L_TEXT_X,100+EQ_DIFF+L_TEXT_Y));
    q_time = new wxTextCtrl(sub_sizers[Q_QUEST_INFO], -1, _T("0"), wxPoint(100,100+EQ_DIFF));
    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Stars"), wxPoint(250+L_TEXT_X,100+EQ_DIFF+L_TEXT_Y));
    q_stars = new wxChoice(sub_sizers[Q_QUEST_INFO], -1, wxPoint(350,100+EQ_DIFF), wxSize(100, 50));
    q_stars->Freeze();
    q_stars->Append(_T(" 1★"));
    q_stars->Append(_T(" 2★"));
    q_stars->Append(_T(" 3★"));
    q_stars->Append(_T(" 4★"));
    q_stars->Append(_T(" 5★"));
    q_stars->Append(_T(" 6★"));
    q_stars->Append(_T(" 7★"));
    q_stars->Append(_T(" 8★/G1★"));
    q_stars->Append(_T(" 9★/G2★"));
    q_stars->Append(_T("10★/G3★"));
    q_stars->Append(_T("11★"));
    q_stars->Append(_T("12★"));
    q_stars->Append(_T("13★"));
    q_stars->Append(_T("14★"));
    q_stars->Thaw();

    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Map"), wxPoint(L_TEXT_X,125+EQ_DIFF+L_TEXT_Y));
    q_map = new wxChoice(sub_sizers[Q_QUEST_INFO], -1, wxPoint(100,125+EQ_DIFF), wxSize(300, 50));
    q_map->Freeze();
    q_map->Append(map_list);
    q_map->Thaw();

    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Quest terms 1"), wxPoint(L_TEXT_X,150+EQ_DIFF+L_TEXT_Y));
    q_critere1 = new wxChoice(sub_sizers[Q_QUEST_INFO], -1, wxPoint(100,150+EQ_DIFF), wxSize(300, 50));
    q_critere1->Freeze();
    q_critere1->Append(term_list);
    q_critere1->Thaw();

    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Quest terms 2"), wxPoint(L_TEXT_X,175+EQ_DIFF+L_TEXT_Y));
    q_critere2 = new wxChoice(sub_sizers[Q_QUEST_INFO], -1, wxPoint(100,175+EQ_DIFF), wxSize(300, 50));
    q_critere2->Freeze();
    q_critere2->Append(term_list);
    q_critere2->Thaw();

    wxArrayString tmp_str;
    tmp_str.Add(_T("Camp"));
    tmp_str.Add(_T("Random"));
    tmp_str.Add(_T("Elder Dragon fight"));

    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Player spawn"), wxPoint(L_TEXT_X,200+EQ_DIFF+L_TEXT_Y));
    q_player_spawn = new wxChoice(sub_sizers[Q_QUEST_INFO], -1, wxPoint(100,200+EQ_DIFF), wxSize(300, 50));
    q_player_spawn->Freeze();
    q_player_spawn->Append(tmp_str);
    q_player_spawn->Thaw();

    tmp_str.clear();
    tmp_str.Add(_T("Arena"));
    tmp_str.Add(_T("Low rank"));
    tmp_str.Add(_T("High rank"));
    tmp_str.Add(_T("G rank"));

    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Carve rank"), wxPoint(L_TEXT_X,225+EQ_DIFF+L_TEXT_Y));
    q_carve_type = new wxChoice(sub_sizers[Q_QUEST_INFO], -1, wxPoint(100,225+EQ_DIFF), wxSize(100, 50));
    q_carve_type->Freeze();
    tmp_str.Add(_T("G Arena"));
    q_carve_type->Append(tmp_str);
    q_carve_type->Thaw();

    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Collect rank"), wxPoint(250+L_TEXT_X,225+EQ_DIFF+L_TEXT_Y));
    q_collect_type = new wxChoice(sub_sizers[Q_QUEST_INFO], -1, wxPoint(350,225+EQ_DIFF), wxSize(100, 50));
    q_collect_type->Freeze();
    q_collect_type->Append(tmp_str);
    q_collect_type->Thaw();

    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Monster AI"), wxPoint(L_TEXT_X,250+EQ_DIFF+L_TEXT_Y));
    q_monster_ai = new wxChoice(sub_sizers[Q_QUEST_INFO], -1, wxPoint(100,250+EQ_DIFF), wxSize(100, 50));
    q_monster_ai->Freeze();
    q_monster_ai->Append(_T("Low rank-"));
    q_monster_ai->Append(_T("Low rank"));
    q_monster_ai->Append(_T("Low rank+"));
    q_monster_ai->Append(_T("High rank"));
    q_monster_ai->Append(_T("High rank+"));
    q_monster_ai->Append(_T("G rank"));
    q_monster_ai->Append(_T("G rank+"));
    q_monster_ai->Thaw();

    q_arena_gate = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Enable Arena fence"), wxPoint(L_TEXT_X,275+EQ_DIFF+L_TEXT_Y));
    q_arena_state = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Initial state"), wxPoint(140+L_TEXT_X,275+EQ_DIFF+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Uptime (s)"), wxPoint(230+L_TEXT_X,275+EQ_DIFF+L_TEXT_Y));
    q_grid_offset[0] = new wxTextCtrl(sub_sizers[Q_QUEST_INFO], -1, _T("0"), wxPoint(300,275+EQ_DIFF), wxSize(50, 23));
    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Downtime (s)"), wxPoint(360+L_TEXT_X,275+EQ_DIFF+L_TEXT_Y));
    q_grid_offset[1] = new wxTextCtrl(sub_sizers[Q_QUEST_INFO], -1, _T("0"), wxPoint(440,275+EQ_DIFF), wxSize(50, 23));

    new wxStaticText(sub_sizers[Q_QUEST_INFO], -1, _T("Dialogue ID"), wxPoint(L_TEXT_X,300+EQ_DIFF+L_TEXT_Y));
    q_dialog_id = new wxTextCtrl(sub_sizers[Q_QUEST_INFO], -1, _T("0"), wxPoint(100,300+EQ_DIFF));
    q_flags_check[13] = new wxCheckBox(sub_sizers[Q_QUEST_INFO], -1, _T("Enable"), wxPoint(210,300+EQ_DIFF+L_TEXT_Y));
}

void MainFrame::readQuestInfo()
{
    if(!qdata.quest) return;

    q_quest_id->ChangeValue(wxString(PlusLib::int2str(qdata.quest->id).c_str(), wxConvUTF8));
    q_hr_points->ChangeValue(wxString(PlusLib::int2str(qdata.quest->hr_points).c_str(), wxConvUTF8));
    q_hr_points_penalty->ChangeValue(wxString(PlusLib::int2str(qdata.quest->hr_points_penalty).c_str(), wxConvUTF8));
    q_sub_hr_points->ChangeValue(wxString(PlusLib::int2str(qdata.quest->sub_hr_points).c_str(), wxConvUTF8));

    q_flags_check[0]->SetValue(qdata.quest->flags & 1); // slay
    q_flags_check[1]->SetValue((qdata.quest->flags >> 1) & 1); // capture
    q_flags_check[2]->SetValue((qdata.quest->flags >> 2) & 1); // collect
    q_flags_check[3]->SetValue((qdata.quest->flags >> 10) & 1); // repel
    //q_flags_check[4]->SetValue((qdata.quest->flags >> 22) & 1); // subquest
    q_flags_check[5]->SetValue((qdata.quest->flags >> 18) & 1); // challenge
    q_flags_check[6]->SetValue((qdata.quest->flags >> 27) & 1); // ship
    //q_flags_check[6]->SetValue((qdata.quest->flags >> 17) & 1);

    q_flags_check[8]->SetValue((qdata.quest->flags >> 8) & 1); // hunt-a-thon
    //q_flags_check[9]->SetValue((qdata.quest->flags >> 16) & 1); // special
    q_flags_check[10]->SetValue((qdata.quest->flags >> 17) & 1); // harvest tour

    q_flags_check[13]->SetValue((qdata.quest->flags >> 26) & 1); // dialogue
    q_flags_check[14]->SetValue((qdata.quest->flags >> 21) & 1); // arena reward

    q_fee->ChangeValue(wxString(PlusLib::int2str(qdata.quest->fee).c_str(), wxConvUTF8));
    q_reward->ChangeValue(wxString(PlusLib::int2str(qdata.quest->reward).c_str(), wxConvUTF8));
    q_penalty->ChangeValue(wxString(PlusLib::int2str(qdata.quest->penalty).c_str(), wxConvUTF8));
    q_sub_reward->ChangeValue(wxString(PlusLib::int2str(qdata.quest->sub_reward).c_str(), wxConvUTF8));
    q_time->ChangeValue(wxString(PlusLib::int2str(qdata.quest->time).c_str(), wxConvUTF8));
    q_stars->SetSelection(qdata.quest->stars-1);
    q_map->SetSelection(i_map_table[qdata.quest->locale-1]);
    q_critere1->SetSelection(qdata.quest->terms1);
    q_critere2->SetSelection(qdata.quest->terms2);
    q_carve_type->SetSelection(qdata.quest->carve_type);
    q_collect_type->SetSelection(qdata.quest->collect_type);
    q_player_spawn->SetSelection(qdata.quest->player_spawn);
    q_monster_ai->SetSelection(qdata.quest->monster_ai);

    q_arena_gate->SetValue(qdata.quest->arena_gate);
    q_arena_state->SetValue(qdata.quest->gate_init);
    q_grid_offset[0]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->grid_time[0]).c_str(), wxConvUTF8));
    q_grid_offset[1]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->grid_time[1]).c_str(), wxConvUTF8));

    q_dialog_id->ChangeValue(wxString(PlusLib::int2str(qdata.quest->main_quest_count[0]).c_str(), wxConvUTF8));
    checkID();
}

void MainFrame::writeQuestInfo()
{
    if(!qdata.quest) return;

    PlusLib::validate16(std::string(q_quest_id->GetValue().mb_str()), qdata.quest->id, 0, 0xFFFF, "Quest ID");
    PlusLib::validate32(std::string(q_hr_points->GetValue().mb_str()), qdata.quest->hr_points, 0, 0xFFFFFFFF, "HR points");
    PlusLib::validate32(std::string(q_hr_points_penalty->GetValue().mb_str()), qdata.quest->hr_points_penalty, 0, 0xFFFFFFFF, "HR points penalty");
    PlusLib::validate32(std::string(q_sub_hr_points->GetValue().mb_str()), qdata.quest->sub_hr_points, 0, 0xFFFFFFFF, "Sub Quest HR points");

    if(q_flags_check[0]->GetValue() != (qdata.quest->flags & 1)) qdata.quest->flags ^= 1;
    if(q_flags_check[1]->GetValue() != ((qdata.quest->flags >> 1) & 1)) qdata.quest->flags ^= 1 << 1;
    if(q_flags_check[2]->GetValue() != ((qdata.quest->flags >> 2) & 1)) qdata.quest->flags ^= 1 << 2;
    if(q_flags_check[3]->GetValue() != ((qdata.quest->flags >> 10) & 1)) qdata.quest->flags ^= 1 << 10;

    if(q_flags_check[5]->GetValue() != ((qdata.quest->flags >> 18) & 1)) qdata.quest->flags ^= 1 << 18;
    if((((qdata.quest->flags >> 3) & 1) == 0 && q_bgroup_count > 1) // group flag
       || (((qdata.quest->flags >> 3) & 1) == 1 && q_bgroup_count <= 1)) qdata.quest->flags ^= 1 << 3;
    if(q_flags_check[6]->GetValue() != ((qdata.quest->flags >> 27) & 1)) qdata.quest->flags ^= 1 << 27;
    if((((qdata.quest->flags >> 9) & 1) == 0 && q_intruder_count > 0) // intruder flag
       || (((qdata.quest->flags >> 9) & 1) == 1 && q_intruder_count <= 0)) qdata.quest->flags ^= 1 << 9;

    if(q_flags_check[8]->GetValue() != ((qdata.quest->flags >> 8) & 1)) qdata.quest->flags ^= 1 << 8;
    //if(q_flags_check[9]->GetValue() != ((qdata.quest->flags >> 16) & 1)) qdata.quest->flags ^= 1 << 16;
    if(q_flags_check[10]->GetValue() != ((qdata.quest->flags >> 17) & 1)) qdata.quest->flags ^= 1 << 17;
    //if(q_flags_check[4]->GetValue() != ((qdata.quest->flags >> 22) & 1)) qdata.quest->flags ^= 1 << 22;
    if(q_flags_check[13]->GetValue() != ((qdata.quest->flags >> 26) & 1)) qdata.quest->flags ^= 1 << 26;
    if(q_flags_check[14]->GetValue() != ((qdata.quest->flags >> 21) & 1)) qdata.quest->flags ^= 1 << 21;

    PlusLib::validate32(std::string(q_fee->GetValue().mb_str()), qdata.quest->fee, 0, 0xFFFFFFFF, "Quest Fee");
    PlusLib::validate32(std::string(q_reward->GetValue().mb_str()), qdata.quest->reward, 1, 0xFFFFFFFF, "Reward");
    PlusLib::validate32(std::string(q_penalty->GetValue().mb_str()), qdata.quest->penalty, 1, 0xFFFFFFFF, "Penalty");
    PlusLib::validate32(std::string(q_sub_reward->GetValue().mb_str()), qdata.quest->sub_reward, 0, 0xFFFFFFFF, "Sub Quest Reward");
    PlusLib::validate32(std::string(q_time->GetValue().mb_str()), qdata.quest->time, 0, 0xFFFFFFFF, "Time");
    qdata.quest->stars = q_stars->GetSelection()+1;

    PlusLib::validate8(PlusLib::int2str(map_table[q_map->GetSelection()]+1), qdata.quest->locale, 1, 20, "Map ID");
    PlusLib::validate8(PlusLib::int2str(q_critere1->GetSelection()), qdata.quest->terms1, 0, 36, "Quest term 1");
    PlusLib::validate8(PlusLib::int2str(q_critere2->GetSelection()), qdata.quest->terms2, 0, 36, "Quest term 2");
    PlusLib::validate8(PlusLib::int2str(q_carve_type->GetSelection()), qdata.quest->carve_type, 0, 4, "Carve rank");
    PlusLib::validate8(PlusLib::int2str(q_collect_type->GetSelection()), qdata.quest->collect_type, 0, 4, "Collect rank");
    PlusLib::validate8(PlusLib::int2str(q_player_spawn->GetSelection()), qdata.quest->player_spawn, 0, 2, "Player spawn");
    PlusLib::validate8(PlusLib::int2str(q_monster_ai->GetSelection()), qdata.quest->monster_ai, 0, 6, "Monster AI");

    qdata.quest->arena_gate = q_arena_gate->GetValue();
    qdata.quest->gate_init = q_arena_state->GetValue();
    PlusLib::validate8(std::string(q_grid_offset[0]->GetValue().mb_str()), qdata.quest->grid_time[0], 0, 0xFF, "Arena fence uptime");
    PlusLib::validate8(std::string(q_grid_offset[1]->GetValue().mb_str()), qdata.quest->grid_time[1], 0, 0xFF, "Arena fence downtime");

    PlusLib::validate8(std::string(q_dialog_id->GetValue().mb_str()), qdata.quest->main_quest_count[0], 0, 0xFF, "Dialogue ID");
}

void MainFrame::initQuestDetail()
{
    for(int32_t i = 0; i < 3; i++)
    {
        q_quest_type[i] = new wxChoice(sub_sizers[Q_QUEST_DETAIL], ID_CHO_QUEST_TYPE, wxPoint(150,75*i), wxSize(200, 50));
        q_quest_type[i]->Freeze();
        q_quest_type[i]->Append(_T("Hunt"));
        q_quest_type[i]->Append(_T("Deliver"));
        q_quest_type[i]->Append(_T("Capture"));
        q_quest_type[i]->Append(_T("Slay"));
        q_quest_type[i]->Append(_T("Break part"));
        q_quest_type[i]->Append(_T("Mount a monster"));
        q_quest_type[i]->Append(_T("Suppress the frenzy"));
        q_quest_type[i]->Append(_T("Return on the wagon"));
        q_quest_type[i]->Append(_T("No objective"));
        q_quest_type[i]->Append(_T("Unknown (nothing will change)"));
        q_quest_type[i]->Thaw();
        q_quest_id1[i] = new wxChoice(sub_sizers[Q_QUEST_DETAIL], ID_CHO_QUEST_TARGET, wxPoint(150,25+75*i), wxSize(200, 50));
        q_quest_id2[i] = new wxTextCtrl(sub_sizers[Q_QUEST_DETAIL], -1, _T("0"), wxPoint(150,25+75*i));
        q_quest_id2[i]->Hide();
        q_quest_count1[i] = new wxTextCtrl(sub_sizers[Q_QUEST_DETAIL], -1, _T("0"), wxPoint(150,50+75*i));
        q_quest_count2[i] = new wxChoice(sub_sizers[Q_QUEST_DETAIL], -1, wxPoint(150,50+75*i), wxSize(330, 50));
        q_quest_count2[i]->Hide();

        new wxStaticLine(sub_sizers[Q_QUEST_DETAIL], -1, wxPoint(0,73+75*i), wxSize(500, 2));
    }

    for(int i = 0; i < 5; i++)
    {
        q_boss_icons[i] = new wxChoice(sub_sizers[Q_QUEST_DETAIL], -1, wxPoint(150,225+25*i), wxSize(200, 50));
        q_boss_icons[i]->Freeze();
        q_boss_icons[i]->Append(icon_list);
        q_boss_icons[i]->Thaw();
    }

    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Quest 1 Type"), wxPoint(L_TEXT_X,L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Quest 1 ID"), wxPoint(L_TEXT_X,25+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Quest 1 Count"), wxPoint(L_TEXT_X,50+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Quest 2 Type"), wxPoint(L_TEXT_X,75+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Quest 2 ID"), wxPoint(L_TEXT_X,100+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Quest 2 Count"), wxPoint(L_TEXT_X,125+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Subquest Type"), wxPoint(L_TEXT_X,150+L_TEXT_Y));
    q_flags_check[12] = new wxCheckBox(sub_sizers[Q_QUEST_DETAIL], -1, _T("Use as 3rd objective"), wxPoint(370,150+L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Subquest ID"), wxPoint(L_TEXT_X,175+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Subquest Count"), wxPoint(L_TEXT_X,200+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Boss Icon 1"), wxPoint(L_TEXT_X,225+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Boss Icon 2"), wxPoint(L_TEXT_X,250+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Boss Icon 3"), wxPoint(L_TEXT_X,275+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Boss Icon 4"), wxPoint(L_TEXT_X,300+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_QUEST_DETAIL], -1, _T("Boss Icon 5"), wxPoint(L_TEXT_X,325+L_TEXT_Y));

    new wxStaticLine(sub_sizers[Q_QUEST_DETAIL], -1, wxPoint(0,348), wxSize(500, 2));

    for(size_t i = 0; i < 3; i++)
    {
        q_objective[i] = 0;
        q_objid[i] = 0;
        q_objcount[i] = 0;
    }
}

void MainFrame::readQuestDetail()
{
    if(!qdata.quest) return;

    q_objective[0] = qdata.quest->main_quest1_type;
    q_objid[0] = qdata.quest->main_quest1_id;
    q_objcount[0] = qdata.quest->main_quest1_count;
    q_objective[1] = qdata.quest->main_quest2_type;
    q_objid[1] = qdata.quest->main_quest2_id;
    q_objcount[1] = qdata.quest->main_quest2_count;
    q_objective[2] = qdata.quest->sub_quest_type;
    q_objid[2] = qdata.quest->sub_quest_id;
    q_objcount[2] = qdata.quest->sub_quest_count;

    updateQuestDetail(0);

    for(int i = 0; i < 5; i++) q_boss_icons[i]->SetSelection(i_icon_table[qdata.quest->boss_icons[i]]);

    q_flags_check[12]->SetValue((qdata.quest->flags >> 23) & 1); // 3 objectives
}

void MainFrame::writeQuestDetail()
{
    if(!qdata.quest) return;

    uint32_t tmp, *ptr_a = NULL;
    uint16_t *ptr_b = NULL, *ptr_c = NULL;
    for(uint32_t i = 0; i < 3; i++)
    {
        switch(i)
        {
            case 0: ptr_a = &qdata.quest->main_quest1_type; ptr_b = &qdata.quest->main_quest1_id; ptr_c = &qdata.quest->main_quest1_count; break;
            case 1: ptr_a = &qdata.quest->main_quest2_type; ptr_b = &qdata.quest->main_quest2_id; ptr_c = &qdata.quest->main_quest2_count; break;
            case 2: ptr_a = &qdata.quest->sub_quest_type; ptr_b = &qdata.quest->sub_quest_id; ptr_c = &qdata.quest->sub_quest_count; break;
        }

        tmp = q_quest_type[i]->GetSelection();
        switch(tmp)
        {
            case 0: *ptr_a = 0x1; break;
            case 1: *ptr_a = 0x2; break;
            case 2: *ptr_a = 0x81; break;
            case 3: *ptr_a = 0x101; break;
            case 4: *ptr_a = 0x204; break;
            case 5: *ptr_a = 0x2004; break;
            case 6: *ptr_a = 0x4004; break;
            case 7: *ptr_a = 0x800; break;
            case 8: *ptr_a = 0x0; break;
            default: break;
        }
        if(tmp == 0 || tmp == 2 || tmp == 3 || tmp == 4 || tmp == 6) PlusLib::validate16(PlusLib::short2str(monster_table[q_quest_id1[i]->GetSelection()]), *ptr_b, 0, 0xFFFF, "Quest " + PlusLib::int2str(i+1) + " ID");
        else if(tmp == 1) PlusLib::validate16(PlusLib::short2str(item_table[q_quest_id1[i]->GetSelection()]), *ptr_b, 0, 0xFFFF, "Quest " + PlusLib::int2str(i+1) + " ID");
        else if(tmp == 5 || tmp == 7 || tmp == 8) PlusLib::validate16(std::string(q_quest_id2[i]->GetValue().mb_str()), *ptr_b, 0, 0xFFFF, "Quest " + PlusLib::int2str(i+1) + " ID");

        if(tmp == 4) PlusLib::validate16(PlusLib::int2str(q_quest_count2[i]->GetSelection()), *ptr_c, 0, 0xFFFF, "Quest " + PlusLib::int2str(i+1) + " Part");
        else PlusLib::validate16(std::string(q_quest_count1[i]->GetValue().mb_str()), *ptr_c, 0, 0xFFFF, "Quest " + PlusLib::int2str(i+1) + " Count");

    }

    if(qdata.quest->main_quest1_type != 0 && qdata.quest->main_quest2_type != 0) qdata.quest->main_quest_count[3] = 2;
    else qdata.quest->main_quest_count[3] = 1;

    // subquest flag
    if(q_flags_check[12]->GetValue() != ((qdata.quest->flags >> 23) & 1)) qdata.quest->flags ^= 1 << 23;
    if((qdata.quest->flags >> 23) & 1) qdata.quest->flags &= ~(1 << 22); // if triple, set to 0
    else if(qdata.quest->sub_quest_type != 0) qdata.quest->flags |= 1 << 22; // if sub, set to 1
    else qdata.quest->flags &= ~(1 << 22); // if not, 0

    for(int i = 0; i < 5; i++)
        PlusLib::validate16(PlusLib::short2str(icon_table[q_boss_icons[i]->GetSelection()]), qdata.quest->boss_icons[i], 0, 0xFFFF, "Boss Icon " + PlusLib::int2str(i));
}

void MainFrame::updateQuestDetail(int state)
{
    int32_t tmp;
    switch(state)
    {
        case 0: // reading
        {
            for(uint32_t i = 0; i < 3; i++)
            {
                switch(q_objective[i])
                {
                    case 0x1: q_objective[i] = 0; break; // hunt
                    case 0x2: q_objective[i] = 1; break; // deliver
                    case 0x81: q_objective[i] = 2; break; // capture
                    case 0x101: q_objective[i] = 3; break; // slay
                    case 0x204: q_objective[i] = 4; break; // break parts
                    case 0x2004: q_objective[i] = 5; break; // mount
                    case 0x4004: q_objective[i] = 6; break; // frenzy
                    case 0x800: q_objective[i] = 7; break; // wagon
                    case 0: q_objective[i] = 8; break; // nothing
                    default: q_objective[i] = 9;
                }
                preq_objective[i] = q_objective[i];
                q_quest_type[i]->SetSelection(q_objective[i]);
            }
        }
        break;
        case 1: // update objective
        {
            for(uint32_t i = 0; i < 3; i++)
            {
                preq_objective[i] = q_objective[i];
                q_objective[i] = q_quest_type[i]->GetSelection();
                if(q_objective[i] != preq_objective[i])
                {
                    if(preq_objective[i] != 4) q_objcount[i] = PlusLib::str2int(std::string(q_quest_count1[i]->GetValue().mb_str()));
                    else if(q_quest_count2[i]->GetSelection() < 0 || q_quest_count2[i]->GetSelection() >= 8) q_objcount[i] = 0;
                    else q_objcount[i] = q_quest_count2[i]->GetSelection();

                    if(q_objective[i] == 1) // deliver
                    {
                        if(preq_objective[i] >= 5 && preq_objective[i] <= 9 && preq_objective[i] != 6) q_objid[i] = PlusLib::str2int(std::string(q_quest_id2[i]->GetValue().mb_str())); // take from other
                        else vTab(q_objid[i], q_quest_id1[i]->GetSelection(), monster_table); // take from monster
                    }
                    else if(q_objective[i] >= 5 && q_objective[i] <= 9 && q_objective[i] != 6) // others
                    {
                        if(preq_objective[i] >= 5 && preq_objective[i] <= 9 && preq_objective[i] != 6) q_objid[i] = PlusLib::str2int(std::string(q_quest_id2[i]->GetValue().mb_str())); // take from other
                        else if(preq_objective[i] == 1) vTab(q_objid[i], q_quest_id1[i]->GetSelection(), item_table); // take from item
                        else vTab(q_objid[i], q_quest_id1[i]->GetSelection(), monster_table); // take from monster
                    }
                    else // monster
                    {
                        if(preq_objective[i] >= 5 && preq_objective[i] <= 9 && preq_objective[i] != 6) q_objid[i] = PlusLib::str2int(std::string(q_quest_id2[i]->GetValue().mb_str())); // take from other
                        else if(preq_objective[i] == 1) vTab(q_objid[i], q_quest_id1[i]->GetSelection(), item_table); // take from item
                        else vTab(q_objid[i], q_quest_id1[i]->GetSelection(), monster_table); // take from monster
                    }
                }
            }
        }
        break;
        case 2: // update id
        {
            for(uint32_t i = 0; i < 3; i++)
            {
                if(q_objective[i] != 4) q_objcount[i] = PlusLib::str2int(std::string(q_quest_count1[i]->GetValue().mb_str())); // take from text
                else q_objcount[i] = q_quest_count2[i]->GetSelection(); // take from part selection

                if(q_objective[i] == 1) vTab(q_objid[i], q_quest_id1[i]->GetSelection(), item_table); // take from item
                else if(q_objective[i] < 5 || q_objective[i] > 9 || q_objective[i] == 6) vTab(q_objid[i], q_quest_id1[i]->GetSelection(), monster_table); // take from monster
                else q_objid[i] = PlusLib::str2int(std::string(q_quest_id2[i]->GetValue().mb_str())); // take from text
            }
        }
        break;
        case 3: // nope
        {
            for(uint32_t i = 0; i < 3; i++)
            {
                q_objective[i] = 0;
                q_objid[i] = 0;
                q_objcount[i] = 0;
            }
        }
        break;
    }

    for(uint32_t i = 0; i < 3; i++)
    {
        switch(q_objective[i])
        {
            case 4: q_quest_id1[i]->Freeze();
                    q_quest_id1[i]->Clear();
                    q_quest_id1[i]->Append(monster_list);
                    q_quest_id1[i]->Thaw();
                    tmp = 0;
                    vTab(tmp, q_objid[i], i_monster_table);
                    q_quest_id1[i]->SetSelection(tmp);
                    q_quest_id1[i]->Show();
                    q_quest_id2[i]->Hide();
                    q_quest_count2[i]->Freeze();
                    q_quest_count2[i]->Clear();
                    for(uint32_t j = 0; j < 8; j++)
                    {
                        tmp = 0;
                        vTab(tmp, q_quest_id1[i]->GetSelection(), monster_table);
                        if(parts[tmp][j].empty()) q_quest_count2[i]->Append(wxString(std::string(PlusLib::int2str(j) + " - Unknown").c_str(), wxConvUTF8));
                        else q_quest_count2[i]->Append(wxString(std::string(PlusLib::int2str(j) + " - " + parts[tmp][j]).c_str(), wxConvUTF8));
                    }
                    q_quest_count2[i]->Thaw();
                    q_quest_count1[i]->Hide();
                    q_quest_count2[i]->Show();
                    if(q_objcount[i] < 0 || q_objcount[i] > 7) q_objcount[i] = 0;
                    q_quest_count2[i]->SetSelection(q_objcount[i]);
                    break;
            case 0: case 2: case 3: case 6: q_quest_id1[i]->Freeze();
                    q_quest_id1[i]->Clear();
                    q_quest_id1[i]->Append(monster_list);
                    q_quest_id1[i]->Thaw();
                    tmp = 0;
                    vTab(tmp, q_objid[i], i_monster_table);
                    q_quest_id1[i]->SetSelection(tmp);
                    q_quest_id1[i]->Show();
                    q_quest_id2[i]->Hide();
                    q_quest_count1[i]->ChangeValue(wxString(PlusLib::int2str(q_objcount[i]).c_str(), wxConvUTF8));
                    q_quest_count1[i]->Show();
                    q_quest_count2[i]->Hide();
                    break;
            case 1: q_quest_id1[i]->Freeze();
                    q_quest_id1[i]->Clear();
                    q_quest_id1[i]->Append(item_list);
                    q_quest_id1[i]->Thaw();
                    tmp = 0;
                    vTab(tmp, q_objid[i], i_item_table);
                    q_quest_id1[i]->SetSelection(tmp);
                    q_quest_id1[i]->Show();
                    q_quest_id2[i]->Hide();
                    q_quest_count1[i]->ChangeValue(wxString(PlusLib::int2str(q_objcount[i]).c_str(), wxConvUTF8));
                    q_quest_count1[i]->Show();
                    q_quest_count2[i]->Hide();
                    break;
            case 5: case 7: case 8: case 9: q_quest_id2[i]->ChangeValue(wxString(PlusLib::int2str(q_objid[i]).c_str(), wxConvUTF8));
                    q_quest_id2[i]->Show();
                    q_quest_id1[i]->Hide();
                    q_quest_count1[i]->ChangeValue(wxString(PlusLib::int2str(q_objcount[i]).c_str(), wxConvUTF8));
                    q_quest_count1[i]->Show();
                    q_quest_count2[i]->Hide();
                    break;
            default: q_quest_id1[i]->Freeze();
                    q_quest_id1[i]->Clear();
                    q_quest_id1[i]->Thaw();
                    q_quest_id1[i]->Show();
                    q_quest_id2[i]->Hide();
                    q_quest_id1[i]->Show();
                    q_quest_id2[i]->Hide();
                    break;
        }
    }
}

void MainFrame::initSupplyBox()
{
    enable_modif = false;
    for(unsigned int i = 0; i < MAX_SUPPLY; i++)
    {
        q_supply.texts.push_back(new wxStaticText(sub_sizers[Q_SUPPLY_BOX], -1, wxString(std::string("Item slot " + PlusLib::int2str(i)).c_str(), wxConvUTF8), wxPoint(L_TEXT_X,75+i*25+L_TEXT_Y)));
        q_supply.texts.back()->Hide();
        q_supply.choices.push_back(new wxChoice(sub_sizers[Q_SUPPLY_BOX], -1, wxPoint(85,i*25+75), wxSize(200, 50)));
        q_supply.choices.back()->Freeze();
        q_supply.choices.back()->Append(item_list);
        q_supply.choices.back()->Thaw();
        q_supply.choices.back()->Hide();
        q_supply.quantity.push_back(new wxTextCtrl(sub_sizers[Q_SUPPLY_BOX], -1, _T("0"), wxPoint(290,i*25+75), wxSize(50, 23)));
        q_supply.quantity.back()->Hide();
    }
    ((wxScrolledWindow*)(sub_sizers[Q_SUPPLY_BOX]))->SetVirtualSize(wxSize(450, 300));
    ((wxScrolledWindow*)(sub_sizers[Q_SUPPLY_BOX]))->SetScrollRate(5, 5);
    ((wxScrolledWindow*)(sub_sizers[Q_SUPPLY_BOX]))->EnableScrolling(false, true);

    wxArrayString tmp_list;
    tmp_list.Add(_T("None"));
    tmp_list.Add(_T("Random"));
    tmp_list.Add(_T("Hunt"));
    tmp_list.Add(_T("Subquest"));
    tmp_list.Add(_T("2 minutes"));
    tmp_list.Add(_T("Unknown"));

    for(size_t i = 0; i < 2; i++)
    {
        q_delivery_type[i] = new wxChoice(sub_sizers[Q_SUPPLY_BOX], -1, wxPoint(75,75+i*25), wxSize(75, 50));
        q_delivery_type[i]->Freeze();
        q_delivery_type[i]->Append(tmp_list);
        q_delivery_type[i]->Thaw();

        q_delivery_box[i] = new wxChoice(sub_sizers[Q_SUPPLY_BOX], -1, wxPoint(155,75+i*25), wxSize(75, 50));

        q_delivery_target[i] = new wxChoice(sub_sizers[Q_SUPPLY_BOX], -1, wxPoint(235,75+i*25), wxSize(160, 50));
        q_delivery_target[i]->Freeze();
        q_delivery_target[i]->Append(monster_list);
        q_delivery_target[i]->Thaw();

        q_delivery_count[i] = new wxTextCtrl(sub_sizers[Q_SUPPLY_BOX], -1, _T("0"), wxPoint(400,75+i*25), wxSize(40, 23));
    }
    q_delivery_text[0] = new wxStaticText(sub_sizers[Q_SUPPLY_BOX], -1, _T("Delivery 1"), wxPoint(L_TEXT_X,75+L_TEXT_Y));
    q_delivery_text[1] = new wxStaticText(sub_sizers[Q_SUPPLY_BOX], -1, _T("Delivery 2"), wxPoint(L_TEXT_X,100+L_TEXT_Y));

    q_supply_boxes[0] = new wxButton(sub_sizers[Q_SUPPLY_BOX], ID_BTN_ADD_SUPPLY_GROUP, _T("Add group"), wxPoint(0,25), wxSize(60, 23));
    q_supply_boxes[1] = new wxButton(sub_sizers[Q_SUPPLY_BOX], ID_BTN_DEL_SUPPLY_GROUP, _T("Del group"), wxPoint(65,25), wxSize(60, 23));

    q_supply_items[0] = new wxButton(sub_sizers[Q_SUPPLY_BOX], ID_BTN_ADD_SUPPLY_ITEM, _T("Add item"), wxPoint(100,125), wxSize(60, 23));
    q_supply_items[1] = new wxButton(sub_sizers[Q_SUPPLY_BOX], ID_BTN_DEL_SUPPLY_ITEM, _T("Del item"), wxPoint(165,125), wxSize(60, 23));

    q_supply.line[0] = new wxStaticLine(sub_sizers[Q_SUPPLY_BOX], -1, wxPoint(0,48), wxSize(500, 2));
    q_supply.line[1] = new wxStaticLine(sub_sizers[Q_SUPPLY_BOX], -1, wxPoint(0,123), wxSize(500, 2));
    q_supply.nBox = 0;

    q_supply_text[0] = new wxStaticText(sub_sizers[Q_SUPPLY_BOX], -1, _T("Item group list"), wxPoint(L_TEXT_X,L_TEXT_Y));
    q_supply_text[1] = new wxStaticText(sub_sizers[Q_SUPPLY_BOX], -1, _T("Delivery condition"), wxPoint(L_TEXT_X,50+L_TEXT_Y));
    q_supply_text[2] = new wxStaticText(sub_sizers[Q_SUPPLY_BOX], -1, _T("Complete item list"), wxPoint(L_TEXT_X,125+L_TEXT_Y));
    q_supply_text[3] = new wxStaticText(sub_sizers[Q_SUPPLY_BOX], -1, _T("Total    0 item(s)"), wxPoint(255+L_TEXT_X,25+L_TEXT_Y));

    q_supply_info = new wxButton(sub_sizers[Q_SUPPLY_BOX], ID_BTN_SUPPLY_HELP, _T("?"), wxPoint(100,0), wxSize(20, 23));
}

void MainFrame::readSupplyBox()
{
    if(!qdata.quest) return;
    int vx, vy;
    ((wxScrolledWindow*)(sub_sizers[Q_SUPPLY_BOX]))->GetViewStart(&vx, &vy);
    ((wxScrolledWindow*)(sub_sizers[Q_SUPPLY_BOX]))->Scroll(0, 0);
    q_supply.count = qdata.quest->supply_list.items.size();
    if(q_supply.count > MAX_SUPPLY) q_supply.count = MAX_SUPPLY;

    uint32_t i = 0;
    for(i = q_supply.nBox; i < qdata.quest->supply_box.size(); i++)
    {
        q_supply.boxName.push_back(new wxStaticText(sub_sizers[Q_SUPPLY_BOX], -1, wxString(std::string("Group " + PlusLib::int2str(i+1) + "  starts at slot").c_str(), wxConvUTF8), wxPoint(L_TEXT_X,25+i*25+L_TEXT_Y)));
        q_supply.countText.push_back(new wxStaticText(sub_sizers[Q_SUPPLY_BOX], -1, _T("Number of items"), wxPoint(200+L_TEXT_X,25+i*25+L_TEXT_Y)));
        q_supply.start.push_back(new wxTextCtrl(sub_sizers[Q_SUPPLY_BOX], -1, _T("0"), wxPoint(120,25+i*25), wxSize(50, 23)));
        q_supply.boxCount.push_back(new wxTextCtrl(sub_sizers[Q_SUPPLY_BOX], ID_TEXT_SUPPLY_COUNT, _T("0"), wxPoint(300,25+i*25), wxSize(50, 23)));
    }
    q_supply.nBox = q_supply.boxName.size();
    size_t to = 0;
    for(i = 0; i < qdata.quest->supply_box.size(); i++)
    {
        q_supply.boxName[i]->Show();
        q_supply.countText[i]->Show();
        q_supply.start[i]->Show();
        q_supply.boxCount[i]->Show();

        q_supply.start[i]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->supply_box[i].start).c_str(), wxConvUTF8));
        q_supply.boxCount[i]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->supply_box[i].count).c_str(), wxConvUTF8));
        to += qdata.quest->supply_box[i].count;
    }
    for(; i < q_supply.nBox; i++)
    {
        q_supply.boxName[i]->Hide();
        q_supply.countText[i]->Hide();
        q_supply.start[i]->Hide();
        q_supply.boxCount[i]->Hide();
    }
    q_supply_boxes[0]->SetPosition(wxPoint(0, 25 + qdata.quest->supply_box.size() * 25));
    q_supply_boxes[1]->SetPosition(wxPoint(65, 25 + qdata.quest->supply_box.size() * 25));

    q_supply_text[3]->SetLabel(wxString(std::string("Total    " + PlusLib::int2str(to) + " item(s)").c_str(), wxConvUTF8));
    q_supply_text[3]->SetPosition(wxPoint(255+L_TEXT_X,(qdata.quest->supply_box.size()*25)+25+L_TEXT_Y));

    q_supply.line[0]->SetPosition(wxPoint(0, ((qdata.quest->supply_box.size()+2) * 25) - 2));
    // --------------------------------------------------------------------------------------------------------
    q_supply_text[1]->SetPosition(wxPoint(L_TEXT_X, L_TEXT_Y+(qdata.quest->supply_box.size()+2) * 25));
    for(size_t x = 0; x < 2; x++)
    {
        q_delivery_text[x]->SetPosition(wxPoint(L_TEXT_X, L_TEXT_Y+ (qdata.quest->supply_box.size() + x + 3) * 25));
        q_delivery_type[x]->SetPosition(wxPoint(75, (qdata.quest->supply_box.size() + x + 3) * 25));
        q_delivery_box[x]->SetPosition(wxPoint(155, (qdata.quest->supply_box.size() + x + 3) * 25));
        q_delivery_target[x]->SetPosition(wxPoint(235, (qdata.quest->supply_box.size() + x + 3) * 25));
        q_delivery_count[x]->SetPosition(wxPoint(400, (qdata.quest->supply_box.size() + x + 3) * 25));
    }

    switch(qdata.quest->supply_condition1[1])
    {
        case 0: q_delivery_type[0]->SetSelection(0); break;
        case 1: q_delivery_type[0]->SetSelection(1); break;
        case 2: q_delivery_type[0]->SetSelection(2); break;
        case 3: q_delivery_type[0]->SetSelection(3); break;
        case 4: q_delivery_type[0]->SetSelection(4); break;
        default: q_delivery_type[0]->SetSelection(5); break;
    }

    switch(qdata.quest->supply_condition2[1])
    {
        case 0: q_delivery_type[1]->SetSelection(0); break;
        case 1: q_delivery_type[1]->SetSelection(1); break;
        case 2: q_delivery_type[1]->SetSelection(2); break;
        case 3: q_delivery_type[1]->SetSelection(3); break;
        case 4: q_delivery_type[1]->SetSelection(4); break;
        default: q_delivery_type[1]->SetSelection(5); break;
    }

    q_delivery_box[0]->Freeze();
    q_delivery_box[1]->Freeze();
    q_delivery_box[0]->Clear();
    q_delivery_box[1]->Clear();
    for(uint32_t i = 0; i < qdata.quest->supply_box.size(); i++)
    {
        q_delivery_box[0]->Append(wxString(std::string("Group " + PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
        q_delivery_box[1]->Append(wxString(std::string("Group " + PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    }

    q_delivery_box[0]->Thaw();
    q_delivery_box[1]->Thaw();

    q_delivery_box[0]->SetSelection(qdata.quest->supply_condition1[0]);
    q_delivery_box[1]->SetSelection(qdata.quest->supply_condition2[0]);

    q_delivery_target[0]->SetSelection(i_monster_table[qdata.quest->supply_condition1[2] + (qdata.quest->supply_condition1[3] << 8)]);
    q_delivery_target[1]->SetSelection(i_monster_table[qdata.quest->supply_condition2[2] + (qdata.quest->supply_condition2[3] << 8)]);

    q_delivery_count[0]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->supply_condition1[4] + (qdata.quest->supply_condition1[5] << 8)).c_str(), wxConvUTF8));
    q_delivery_count[1]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->supply_condition2[4] + (qdata.quest->supply_condition2[5] << 8)).c_str(), wxConvUTF8));

    q_supply.line[1]->Show();
    q_supply.line[1]->SetPosition(wxPoint(0, ((qdata.quest->supply_box.size()+5) * 25) - 2));
    // --------------------------------------------------------------------------------------------------------
    q_supply_text[2]->SetPosition(wxPoint(L_TEXT_X, L_TEXT_Y+(qdata.quest->supply_box.size()+5) * 25));
    q_supply_items[0]->SetPosition(wxPoint(100, (qdata.quest->supply_box.size()+5) * 25));
    q_supply_items[1]->SetPosition(wxPoint(165, (qdata.quest->supply_box.size()+5) * 25));
    for(i = 0; i < q_supply.count; i++)
    {
        q_supply.texts[i]->Show();
        q_supply.choices[i]->Show();
        q_supply.choices[i]->SetSelection(i_item_table[qdata.quest->supply_list.items[i].item_id]);
        q_supply.quantity[i]->Show();
        q_supply.quantity[i]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->supply_list.items[i].count).c_str(), wxConvUTF8));

        q_supply.texts[i]->SetPosition(wxPoint(L_TEXT_X, 25*(i+6+qdata.quest->supply_box.size())));
        q_supply.choices[i]->SetPosition(wxPoint(85, 25*(i+6+qdata.quest->supply_box.size())));
        q_supply.quantity[i]->SetPosition(wxPoint(290, 25*(i+6+qdata.quest->supply_box.size())));
    }
    ((wxScrolledWindow*)(sub_sizers[Q_SUPPLY_BOX]))->SetVirtualSize(wxSize(450, 25*(i+6+qdata.quest->supply_box.size())));
    ((wxScrolledWindow*)(sub_sizers[Q_SUPPLY_BOX]))->Scroll(vx, vy);
    for(; i < q_supply.choices.size(); i++)
    {
        q_supply.texts[i]->Hide();
        q_supply.choices[i]->Hide();
        q_supply.quantity[i]->Hide();
    }
}

void MainFrame::writeSupplyBox(bool warning)
{
    if(!qdata.quest) return;

    // GROUPS ---------------------------------------------------------------------------
    bool invalid_start = false;
    bool invalid_count = false;
    int64_t start, count;
    for(unsigned int i = 0; i < qdata.quest->supply_box.size(); i++)
    {
        start = PlusLib::str2int(std::string(q_supply.start[i]->GetValue().mb_str()));
        count = PlusLib::str2int(std::string(q_supply.boxCount[i]->GetValue().mb_str()));

        if(start < 0 || (start >= qdata.quest->supply_list.items.size() && count != 0)) invalid_start = true;
        else
        {
            qdata.quest->supply_box[i].start = start % 0x100000000;
            if(count < 0 || count > qdata.quest->supply_list.items.size()-start) invalid_count = true;
            else
            {
                qdata.quest->supply_box[i].count = count % 0x100;
            }
        }
    }

    if(warning && invalid_count) wxMessageBox(_T("Supply box error : One of the item counts value is invalid.\nPlease check the supply group values."));
    else if(warning && invalid_start) wxMessageBox(_T("Supply box error : One of the starting point is invalid.\nPlease check the supply group values."));

    // ITEMS ---------------------------------------------------------------------------
    for(unsigned int i = 0; i < qdata.quest->supply_list.items.size(); i++)
    {
        PlusLib::validate16(PlusLib::int2str(item_table[q_supply.choices[i]->GetSelection()]), qdata.quest->supply_list.items[i].item_id, 0, 1937, "Item ID " + PlusLib::int2str(i));
        PlusLib::validate16(std::string(q_supply.quantity[i]->GetValue().mb_str()), qdata.quest->supply_list.items[i].count, 0, 99, "Item Quantity " + PlusLib::int2str(i));
    }

    // DELIVERY ---------------------------------------------------------------------------
    switch(q_delivery_type[0]->GetSelection())
    {
        case 0: qdata.quest->supply_condition1[1] = 0; break;
        case 1: qdata.quest->supply_condition1[1] = 1; break;
        case 2: qdata.quest->supply_condition1[1] = 2; break;
        case 3: qdata.quest->supply_condition1[1] = 3; break;
        case 4: qdata.quest->supply_condition1[1] = 4; break;
        default: break;
    }

    switch(q_delivery_type[1]->GetSelection())
    {
        case 0: qdata.quest->supply_condition2[1] = 0; break;
        case 1: qdata.quest->supply_condition2[1] = 1; break;
        case 2: qdata.quest->supply_condition2[1] = 2; break;
        case 3: qdata.quest->supply_condition2[1] = 3; break;
        case 4: qdata.quest->supply_condition2[1] = 4; break;
        default: break;
    }

    qdata.quest->supply_condition1[0] = q_delivery_box[0]->GetSelection();
    qdata.quest->supply_condition2[0] = q_delivery_box[1]->GetSelection();

    uint16_t tmp16 = monster_table[q_delivery_target[0]->GetSelection()];
    qdata.quest->supply_condition1[2] = tmp16 % 0x100;
    qdata.quest->supply_condition1[3] = tmp16 / 0x100;

    tmp16 = monster_table[q_delivery_target[1]->GetSelection()];
    qdata.quest->supply_condition2[2] = tmp16 % 0x100;
    qdata.quest->supply_condition2[3] = tmp16 / 0x100;

    tmp16 = PlusLib::str2int(std::string(q_delivery_count[0]->GetValue().mb_str()));
    qdata.quest->supply_condition1[4] = tmp16 % 0x100;
    qdata.quest->supply_condition1[5] = tmp16 / 0x100;

    tmp16 = PlusLib::str2int(std::string(q_delivery_count[1]->GetValue().mb_str()));
    qdata.quest->supply_condition2[4] = tmp16 % 0x100;
    qdata.quest->supply_condition2[5] = tmp16 / 0x100;
}

#define LOAD_CHOICE_REWARD 8

void MainFrame::initReward()
{
    for(unsigned r = 0; r < 3; r++)
    {
        q_reward_box[r].duplicate = new wxStaticText(sub_sizers[Q_REWARD_A+r], -1, _T(""), wxPoint(20+L_TEXT_X,20+L_TEXT_Y));
        q_reward_box[r].duplicate->Hide();

        q_reward_box[r].divide = new wxButton(sub_sizers[Q_REWARD_A+r], ID_BTN_UNBIND_REWARD, _T("Unbind"), wxPoint(20, 45), wxSize(50, 23));
        q_reward_box[r].divide->Hide();
        if(r == 0) q_reward_box[r].divide->Disable();

        q_reward_box[r].add = new wxButton(sub_sizers[Q_REWARD_A+r], ID_BTN_ADD_RITEM, _T("Add one item"), wxPoint(5, 0), wxSize(100, 23));
        q_reward_box[r].del = new wxButton(sub_sizers[Q_REWARD_A+r], ID_BTN_DEL_RITEM, _T("Delete last item"), wxPoint(105, 0), wxSize(100, 23));
        q_reward_box[r].add->Hide();
        q_reward_box[r].del->Hide();

        q_reward_box[r].add_list = new wxButton(sub_sizers[Q_REWARD_A+r], ID_BTN_ADD_REWARD, _T("Add a reward line"), wxPoint(5, 0), wxSize(100, 23));
        q_reward_box[r].add_list->Hide();

        q_reward_box[r].count = 0;
        q_reward_box[r].loaded = 0;
        for(unsigned int i = 0; i < LOAD_CHOICE_REWARD; i++)
        {
                q_reward_box[r].texts.push_back(new wxStaticText(sub_sizers[Q_REWARD_A+r], -1, wxString(std::string("Item " + PlusLib::int2str(i+1)).c_str(), wxConvUTF8), wxPoint(L_TEXT_X,i*25+L_TEXT_Y)));
                q_reward_box[r].texts.back()->Hide();
                q_reward_box[r].choices.push_back(new wxChoice(sub_sizers[Q_REWARD_A+r], -1, wxPoint(55,i*25), wxSize(200, 50)));
                q_reward_box[r].choices.back()->Freeze();
                q_reward_box[r].choices.back()->Append(item_list);
                q_reward_box[r].choices.back()->Thaw();
                q_reward_box[r].choices.back()->Hide();
                q_reward_box[r].quantity.push_back(new wxTextCtrl(sub_sizers[Q_REWARD_A+r], -1, _T("0"), wxPoint(260,i*25), wxSize(50, 23)));
                q_reward_box[r].quantity.back()->Hide();
                q_reward_box[r].proba.push_back(new wxTextCtrl(sub_sizers[Q_REWARD_A+r], -1, _T("0"), wxPoint(315,i*25), wxSize(50, 23)));
                q_reward_box[r].proba.back()->Hide();
                q_reward_box[r].percents.push_back(new wxStaticText(sub_sizers[Q_REWARD_A+r], -1, _T("%"), wxPoint(365+L_TEXT_X,i*25+L_TEXT_Y)));
                q_reward_box[r].percents.back()->Hide();
                q_reward_box[r].loaded++;
        }
        ((wxScrolledWindow*)(sub_sizers[Q_REWARD_A+r]))->SetVirtualSize(wxSize(450, 300));
        ((wxScrolledWindow*)(sub_sizers[Q_REWARD_A+r]))->SetScrollRate(5, 5);
        ((wxScrolledWindow*)(sub_sizers[Q_REWARD_A+r]))->EnableScrolling(false, true);
    }
    q_reward_A_help = new wxButton(sub_sizers[Q_REWARD_A], ID_BTN_REWARD_A_HELP, _T("?"), wxPoint(410,0), wxSize(20, 23));
}

void MainFrame::readReward(uint32_t r)
{
    if(r > 3 && !qdata.quest) return;

    if(r == 0)
    {
        readReward(1);
        readReward(2);
        readReward(3);
        return;
    }

    if(qdata.quest->reward_box_offset[r-1] == 0xFFFFFFFF || qdata.quest->reward_box_offset[r-1] == 0xFFFFFFFE)
    {
        q_reward_box[r-1].divide->Show();
        if(qdata.quest->reward_box_offset[r-1] == 0xFFFFFFFF) q_reward_box[r-1].duplicate->SetLabel(_T("Same items as line B"));
        else if(qdata.quest->reward_box_offset[r-1] == 0xFFFFFFFE) q_reward_box[r-1].duplicate->SetLabel(_T("Same items as line A"));
        q_reward_box[r-1].duplicate->Show();
        ((wxScrolledWindow*)(sub_sizers[Q_REWARD_A+r-1]))->SetVirtualSize(wxSize(450, 300));
        for(unsigned int i = 0; i < q_reward_box[r-1].choices.size(); i++)
        {
            q_reward_box[r-1].texts[i]->Hide();
            q_reward_box[r-1].choices[i]->Hide();
            q_reward_box[r-1].quantity[i]->Hide();
            q_reward_box[r-1].proba[i]->Hide();
            q_reward_box[r-1].percents[i]->Hide();
            q_reward_box[r-1].add->Hide();
            q_reward_box[r-1].del->Hide();
            q_reward_box[r-1].add_list->Hide();
        }
        return;
    }
    else
    {
        q_reward_box[r-1].duplicate->Hide();
        q_reward_box[r-1].divide->Hide();
    }

    q_reward_box[r-1].count = qdata.quest->reward_box[r-1].reward_items.size();
    for(uint32_t i = q_reward_box[r-1].loaded; q_reward_box[r-1].loaded < q_reward_box[r-1].count; i++)
    {
        q_reward_box[r-1].texts.push_back(new wxStaticText(sub_sizers[Q_REWARD_A+r-1], -1, wxString(std::string("Item " + PlusLib::int2str(i+1)).c_str(), wxConvUTF8), wxPoint(L_TEXT_X,i*25+L_TEXT_Y)));
        q_reward_box[r-1].texts.back()->Hide();
        q_reward_box[r-1].choices.push_back(new wxChoice(sub_sizers[Q_REWARD_A+r-1], -1, wxPoint(55,i*25), wxSize(200, 50)));
        q_reward_box[r-1].choices.back()->Freeze();
        q_reward_box[r-1].choices.back()->Append(item_list);
        q_reward_box[r-1].choices.back()->Thaw();
        q_reward_box[r-1].choices.back()->Hide();
        q_reward_box[r-1].quantity.push_back(new wxTextCtrl(sub_sizers[Q_REWARD_A+r-1], -1, _T("0"), wxPoint(260,i*25), wxSize(50, 23)));
        q_reward_box[r-1].quantity.back()->Hide();
        q_reward_box[r-1].proba.push_back(new wxTextCtrl(sub_sizers[Q_REWARD_A+r-1], -1, _T("0"), wxPoint(315,i*25), wxSize(50, 23)));
        q_reward_box[r-1].proba.back()->Hide();
        q_reward_box[r-1].percents.push_back(new wxStaticText(sub_sizers[Q_REWARD_A+r-1], -1, _T("%"), wxPoint(365+L_TEXT_X,i*25+L_TEXT_Y)));
        q_reward_box[r-1].percents.back()->Hide();
        q_reward_box[r-1].loaded++;
    }

    unsigned int i;
    for(i = 0; i < q_reward_box[r-1].count && i < q_reward_box[r-1].choices.size(); i++)
    {
        q_reward_box[r-1].texts[i]->Show();
        q_reward_box[r-1].choices[i]->Show();
        q_reward_box[r-1].choices[i]->SetSelection(i_item_table[qdata.quest->reward_box[r-1].reward_items[i].item_id]);
        q_reward_box[r-1].quantity[i]->Show();
        q_reward_box[r-1].quantity[i]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->reward_box[r-1].reward_items[i].count).c_str(), wxConvUTF8));
        q_reward_box[r-1].proba[i]->Show();
        q_reward_box[r-1].proba[i]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->reward_box[r-1].reward_items[i].chance).c_str(), wxConvUTF8));
        q_reward_box[r-1].percents[i]->Show();
    }

    if(qdata.quest->reward_box_offset[r-1] == 0)
    {
        q_reward_box[r-1].del->Hide();
        q_reward_box[r-1].add->Hide();
        q_reward_box[r-1].add_list->Show();
    }
    else
    {
        q_reward_box[r-1].add->Show();
        q_reward_box[r-1].add->SetPosition(wxPoint(5, 25*i));
        q_reward_box[r-1].add_list->Hide();
        if(q_reward_box[r-1].count > 0)
        {
            q_reward_box[r-1].del->Show();
            q_reward_box[r-1].del->SetPosition(wxPoint(105, 25*i));
        }
        else q_reward_box[r-1].del->Hide();
    }

    ((wxScrolledWindow*)(sub_sizers[Q_REWARD_A+r-1]))->SetVirtualSize(wxSize(450, 25*(i+2)));
    for(; i < q_reward_box[r-1].choices.size(); i++)
    {
        q_reward_box[r-1].texts[i]->Hide();
        q_reward_box[r-1].choices[i]->Hide();
        q_reward_box[r-1].quantity[i]->Hide();
        q_reward_box[r-1].proba[i]->Hide();
        q_reward_box[r-1].percents[i]->Hide();
    }
}

void MainFrame::writeReward(uint32_t r)
{
    if(r > 3 && !qdata.quest) return;

    if(r == 0)
    {
        readReward(1);
        readReward(2);
        readReward(3);
        return;
    }
    if(qdata.quest->reward_box_offset[r-1] == 0xFFFFFFFF || qdata.quest->reward_box_offset[r-1] == 0xFFFFFFFE) return;

    std::string tmp;
    switch(r)
    {
        case 1: tmp = "Reward A "; break;
        case 2: tmp = "Reward B "; break;
        case 3: tmp = "Reward C "; break;
    }

    for(unsigned int i = 0; i < q_reward_box[r-1].count && i < 32; i++)
    {
        PlusLib::validate16(std::string(q_reward_box[r-1].proba[i]->GetValue().mb_str()), qdata.quest->reward_box[r-1].reward_items[i].chance, 0, 0x64, tmp + "Chance " + PlusLib::int2str(i));
        PlusLib::validate16(PlusLib::short2str(item_table[q_reward_box[r-1].choices[i]->GetSelection()]), qdata.quest->reward_box[r-1].reward_items[i].item_id, 0, 1937, tmp + "ID " + PlusLib::int2str(i));
        PlusLib::validate16(std::string(q_reward_box[r-1].quantity[i]->GetValue().mb_str()), qdata.quest->reward_box[r-1].reward_items[i].count, 0, 0x63, tmp + "Quantity " + PlusLib::int2str(i));
    }
}

void MainFrame::initText()
{
    wxString text_text[7] = {_T("Title"), _T("Success"), _T("Failure"), _T("Summary"), _T("Main small monsters"), _T("Client"), _T("Subquest objective")};

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 7; j++)
        {
            new wxStaticText(sub_sizers[Q_TEXT_ENGLISH+i], -1, text_text[j], wxPoint(L_TEXT_X,j*75+L_TEXT_Y));
            q_text[j+i*7] = new wxTextCtrl(sub_sizers[Q_TEXT_ENGLISH+i], -1, _T("(Empty)"), wxPoint(5,25+j*75), wxSize(380, 50), wxTE_MULTILINE);

            q_text_copy[j+i*7] = new wxButton(sub_sizers[Q_TEXT_ENGLISH+i], ID_BTN_CPY_TEXT_TITLE+j, _T("Copy"), wxPoint(390,25+j*75));
            q_text_paste[j+i*7] = new wxButton(sub_sizers[Q_TEXT_ENGLISH+i], ID_BTN_PASTE_TEXT_TITLE+j, _T("Paste"), wxPoint(390,50+j*75));
        }
        ((wxScrolledWindow*)(sub_sizers[Q_TEXT_ENGLISH+i]))->SetVirtualSize(wxSize(450, 530));
        ((wxScrolledWindow*)(sub_sizers[Q_TEXT_ENGLISH+i]))->SetScrollRate(5, 5);
        ((wxScrolledWindow*)(sub_sizers[Q_TEXT_ENGLISH+i]))->EnableScrolling(false, true);
    }
}

void MainFrame::readText(uint32_t r)
{
    if(!qdata.quest) return;

    if(r == 0)
    {
        for(uint32_t i = 1; i <= 5; i++) readText(i);
        return;
    }

    if(qdata.quest->quest_text_offset.language_offset[r-1] != 0 && (r == 1 || (r > 1 && qdata.quest->quest_text_offset.language_offset[r-1] != qdata.quest->quest_text_offset.language_offset[0])))
    {
        q_text[0+(r-1)*7]->ChangeValue(convertStrFromQuest(qdata.quest->language_offset[r-1].title));
        q_text[1+(r-1)*7]->ChangeValue(convertStrFromQuest(qdata.quest->language_offset[r-1].success));
        q_text[2+(r-1)*7]->ChangeValue(convertStrFromQuest(qdata.quest->language_offset[r-1].failure));
        q_text[3+(r-1)*7]->ChangeValue(convertStrFromQuest(qdata.quest->language_offset[r-1].summary));
        q_text[4+(r-1)*7]->ChangeValue(convertStrFromQuest(qdata.quest->language_offset[r-1].main_monsters));
        q_text[5+(r-1)*7]->ChangeValue(convertStrFromQuest(qdata.quest->language_offset[r-1].client));
        q_text[6+(r-1)*7]->ChangeValue(convertStrFromQuest(qdata.quest->language_offset[r-1].sub_quest));

        q_text[0+(r-1)*7]->SetMaxLength((qdata.quest->language_offset[r-1].title.size()-TEXT_SUB)/2);
        q_text[1+(r-1)*7]->SetMaxLength((qdata.quest->language_offset[r-1].success.size()-TEXT_SUB)/2);
        q_text[2+(r-1)*7]->SetMaxLength((qdata.quest->language_offset[r-1].failure.size()-TEXT_SUB)/2);
        q_text[3+(r-1)*7]->SetMaxLength((qdata.quest->language_offset[r-1].summary.size()-TEXT_SUB)/2);
        q_text[4+(r-1)*7]->SetMaxLength((qdata.quest->language_offset[r-1].main_monsters.size()-TEXT_SUB)/2);
        q_text[5+(r-1)*7]->SetMaxLength((qdata.quest->language_offset[r-1].client.size()-TEXT_SUB)/2);
        q_text[6+(r-1)*7]->SetMaxLength((qdata.quest->language_offset[r-1].sub_quest.size()-TEXT_SUB)/2);
    }
    else
    {
        q_text[0+(r-1)*7]->ChangeValue(_T("(Empty)"));
        q_text[1+(r-1)*7]->ChangeValue(_T("(Empty)"));
        q_text[2+(r-1)*7]->ChangeValue(_T("(Empty)"));
        q_text[3+(r-1)*7]->ChangeValue(_T("(Empty)"));
        q_text[4+(r-1)*7]->ChangeValue(_T("(Empty)"));
        q_text[5+(r-1)*7]->ChangeValue(_T("(Empty)"));
        q_text[6+(r-1)*7]->ChangeValue(_T("(Empty)"));
    }


}

void MainFrame::writeText(uint32_t r)
{
    if(!qdata.quest) return;

    if(r == 0)
    {
        for(uint32_t i = 1; i <= 5; i++) writeText(i);
        return;
    }

    if(qdata.quest->quest_text_offset.language_offset[r-1] != 0 && (r == 1 || (r > 1 && qdata.quest->quest_text_offset.language_offset[r-1] != qdata.quest->quest_text_offset.language_offset[0])))
    {
        std::vector<uint8_t> tmp[7];

        for(unsigned int i = 0; i < 7; i++)
        {
            tmp[i] = convertStrToQuest(q_text[i+(r-1)*7]->GetValue());
        }

        while(tmp[0].size() < qdata.quest->language_offset[r-1].title.size()) tmp[0].push_back(0x00);
        while(tmp[1].size() < qdata.quest->language_offset[r-1].success.size()) tmp[1].push_back(0x00);
        while(tmp[2].size() < qdata.quest->language_offset[r-1].failure.size()) tmp[2].push_back(0x00);
        while(tmp[3].size() < qdata.quest->language_offset[r-1].summary.size()) tmp[3].push_back(0x00);
        while(tmp[4].size() < qdata.quest->language_offset[r-1].main_monsters.size()) tmp[4].push_back(0x00);
        while(tmp[5].size() < qdata.quest->language_offset[r-1].client.size()) tmp[5].push_back(0x00);
        while(tmp[6].size() < qdata.quest->language_offset[r-1].sub_quest.size()) tmp[6].push_back(0x00);

        qdata.quest->language_offset[r-1].title         = tmp[0];
        qdata.quest->language_offset[r-1].success       = tmp[1];
        qdata.quest->language_offset[r-1].failure       = tmp[2];
        qdata.quest->language_offset[r-1].summary       = tmp[3];
        qdata.quest->language_offset[r-1].main_monsters = tmp[4];
        qdata.quest->language_offset[r-1].client        = tmp[5];
        qdata.quest->language_offset[r-1].sub_quest     = tmp[6];
    }
}

void MainFrame::initBossGroup()
{
    q_current_boss = -1;
    q_current_bgroup = -1;

    q_bgroup_choice = new wxChoice(sub_sizers[Q_BOSS_GROUP], ID_CHO_BGROUP, wxPoint(100,0), wxSize(200, 50));
    q_boss.q_boss_help1 = new wxButton(sub_sizers[Q_BOSS_GROUP], ID_BTN_BOSS_HELP1, _T("?"), wxPoint(305,0), wxSize(20, 22));
    q_boss.q_boss_help2 = new wxButton(sub_sizers[Q_BOSS_GROUP], ID_BTN_BOSS_HELP2, _T("?"), wxPoint(305,50), wxSize(20, 22));
    q_boss_choice = new wxChoice(sub_sizers[Q_BOSS_GROUP], ID_CHO_BOSS, wxPoint(100,25), wxSize(200, 50));

    q_boss.q_boss_id = new wxChoice(sub_sizers[Q_BOSS_GROUP], ID_CHO_BOSS_ID, wxPoint(100,50), wxSize(200, 50));
    q_boss.q_boss_id->Freeze();
    q_boss.q_boss_id->Append(monster_list);
    q_boss.q_boss_id->Thaw();

    q_boss.q_boss_size_pc = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(100,75));
    q_boss.q_boss_size_var = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(300,75));
    q_boss.q_boss_health = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(100,100));
    q_boss.q_boss_attack = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(300,100));
    q_boss.q_boss_defense = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(100,125));
    q_boss.q_boss_stamina = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(300,125));
    q_boss.q_boss_spawn_count = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(100,150));
    q_boss.q_boss_spawn_area = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(100,175));
    //q_boss.q_boss_frenzy = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(300,200));
    q_boss.q_boss_frenzy = new wxChoice(sub_sizers[Q_BOSS_GROUP], -1, wxPoint(300,200), wxSize(100, 50));
    q_boss.q_boss_frenzy->Freeze();
    q_boss.q_boss_frenzy->Append(frenzy_list);
    q_boss.q_boss_frenzy->Thaw();
    q_boss.q_boss_spawn_condition = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(100,200));
    q_boss.q_boss_size2 = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(100,225));
    q_boss.q_boss_size1 = new wxChoice(sub_sizers[Q_BOSS_GROUP], -1, wxPoint(300,225), wxSize(100, 50));

    q_boss.q_boss_cor_x = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(300,150), wxSize(60, 23));
    q_boss.q_boss_cor_y = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(360,150), wxSize(60, 23));
    q_boss.q_boss_cor_z = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(420,150), wxSize(60, 23));
    new wxButton(sub_sizers[Q_BOSS_GROUP], ID_BTN_PLACE_BOSS, _T("Auto position"), wxPoint(300, 175), wxSize(180, 23));

    //q_boss.q_boss_angles[0] = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(300,175), wxSize(60, 23));
    //q_boss.q_boss_angles[1] = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(360,175), wxSize(60, 23));
   // q_boss.q_boss_angles[2] = new wxTextCtrl(sub_sizers[Q_BOSS_GROUP], -1, _T("0"), wxPoint(420,175), wxSize(60, 23));

    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Size (%)"), wxPoint(L_TEXT_X,75+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Size Variation (%)"), wxPoint(200+L_TEXT_X,75+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Health"), wxPoint(L_TEXT_X,100+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Attack"), wxPoint(200+L_TEXT_X,100+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Defense"), wxPoint(L_TEXT_X,125+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Stamina"), wxPoint(200+L_TEXT_X,125+L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Spawn count"), wxPoint(L_TEXT_X,150+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Position (X,Y,Z)"), wxPoint(200+L_TEXT_X,150+L_TEXT_Y));
    //new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Angles   (X,Y,Z)"), wxPoint(200+L_TEXT_X,175+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Spawn area"), wxPoint(L_TEXT_X,175+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Frenzy"), wxPoint(200+L_TEXT_X,200+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Spawn condition"), wxPoint(L_TEXT_X,200+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Size related"), wxPoint(L_TEXT_X,225+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Alternative Form"), wxPoint(200+L_TEXT_X,225+L_TEXT_Y));

    new wxButton(sub_sizers[Q_BOSS_GROUP], ID_BTN_ADD_BOSS_GROUP, _T("Add group"), wxPoint(0, 300), wxSize(100, 23));
    new wxButton(sub_sizers[Q_BOSS_GROUP], ID_BTN_DEL_BOSS_GROUP, _T("Delete group"), wxPoint(105, 300), wxSize(100, 23));

    new wxButton(sub_sizers[Q_BOSS_GROUP], ID_BTN_BOSS_ADD, _T("Add boss"), wxPoint(0, 325), wxSize(100, 23));
    new wxButton(sub_sizers[Q_BOSS_GROUP], ID_BTN_BOSS_DEL, _T("Delete boss"), wxPoint(105, 325), wxSize(100, 23));

    q_boss_choice_move = new wxChoice(sub_sizers[Q_BOSS_GROUP], -1, wxPoint(105,350), wxSize(100, 50));
    new wxButton(sub_sizers[Q_BOSS_GROUP], ID_BTN_BOSS_MOVE, _T("Move boss to..."), wxPoint(0, 350), wxSize(100, 23));

    new wxStaticText(sub_sizers[Q_BOSS_GROUP], -1, _T("Changing the monster type or the\nnumber of groups require to\nuse a valid unused ID"), wxPoint(220+L_TEXT_X,300+L_TEXT_Y));
    new wxStaticLine(sub_sizers[Q_BOSS_GROUP], -1, wxPoint(0,148), wxSize(500, 2));
    new wxStaticLine(sub_sizers[Q_BOSS_GROUP], -1, wxPoint(0,73), wxSize(500, 2));
}

void MainFrame::readBossGroup()
{
    if(!qdata.quest) return;

    q_bgroup_count = qdata.quest->boss_group_offset.size();
    wxArrayString boss_str;

    for(int i = 0; i < (int)qdata.quest->boss_group_offset.size(); i++)
        boss_str.Add(wxString(std::string("Group " + PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    if(qdata.quest->boss_group_offset.size() == 0) boss_str.Add(_T("[NO GROUP IN THE QUEST]"));

    q_bgroup_choice->Freeze();
    q_bgroup_choice->Clear();
    q_bgroup_choice->Append(boss_str);
    q_bgroup_choice->Thaw();

    if(qdata.quest->boss_group_offset.size() == 0)
    {
        boss_str.clear();
        boss_str.Add(_T("[NO GROUP]"));
    }

    q_boss_choice_move->Freeze();
    q_boss_choice_move->Clear();
    q_boss_choice_move->Append(boss_str);
    q_boss_choice_move->Thaw();
    if(q_boss_choice_move->GetSelection() < 0 || q_boss_choice_move->GetSelection() >= q_bgroup_count) q_boss_choice_move->SetSelection(0);

    boss_str.clear();

    if(q_bgroup_count > 0)
    {
        if(q_current_bgroup < 0 || q_current_bgroup >= q_bgroup_count)
        {
            q_bgroup_choice->SetSelection(0);
            q_current_bgroup = 0;
        }
        else q_bgroup_choice->SetSelection(q_current_bgroup);
        q_boss_count = qdata.quest->boss_group_offset[q_current_bgroup].count;
        if(q_boss_count > 0)
        {
            if(q_current_boss < 0 || q_current_boss >= q_boss_count) q_current_boss = 0;
            for(int i = 0; i < q_boss_count; i++)
                boss_str.Add(wxString(std::string("#" + PlusLib::int2str(i+1) + " - " + std::string(monster_list[i_monster_table[qdata.quest->boss_group[i+qdata.quest->boss_group_offset[q_current_bgroup].start].boss_type]].mb_str())).c_str(), wxConvUTF8));
            q_boss_choice->Freeze();
            q_boss_choice->Clear();
            q_boss_choice->Append(boss_str);
            q_boss_choice->Thaw();
            q_boss_choice->SetSelection(q_current_boss);
        }
        else
        {
            boss_str.Add(_T("[NO BOSS IN THE GROUP]"));
            q_boss_choice->Freeze();
            q_boss_choice->Clear();
            q_boss_choice->Append(boss_str);
            q_boss_choice->Thaw();
            q_boss_choice->SetSelection(0);
            q_current_boss = -1;
        }
    }
    else
    {
        q_bgroup_choice->SetSelection(0);
        q_current_bgroup = -1;
        q_current_boss = -1;
        q_boss_choice->Freeze();
        q_boss_choice->Clear();
        q_boss_choice->Thaw();
        q_boss_choice->SetSelection(q_current_boss);
    }
    readBoss(q_current_boss);
}

void MainFrame::writeBossGroup()
{
    if(!qdata.quest) return;

    writeBoss(q_current_boss);
}

void MainFrame::readBoss(int b)
{
    if(!qdata.quest) return;

    q_boss.q_boss_size1->Freeze();
    q_boss.q_boss_size1->Clear();
    q_boss.q_boss_size1->Append(_T("Default"));
    if(b == -1)
    {
        q_boss.q_boss_size_pc->ChangeValue(_T("0"));
        q_boss.q_boss_size_var->ChangeValue(_T("0"));
        q_boss.q_boss_health->ChangeValue(_T("0"));
        q_boss.q_boss_attack->ChangeValue(_T("0"));
        q_boss.q_boss_defense->ChangeValue(_T("0"));
        q_boss.q_boss_stamina->ChangeValue(_T("0"));
        q_boss.q_boss_id->SetSelection(-1);
        q_boss.q_boss_spawn_count->ChangeValue(_T("0"));
        q_boss.q_boss_cor_x->ChangeValue(_T("0"));
        q_boss.q_boss_spawn_area->ChangeValue(_T("0"));
        q_boss.q_boss_cor_y->ChangeValue(_T("0"));
        q_boss.q_boss_frenzy->SetSelection(0);
        q_boss.q_boss_cor_z->ChangeValue(_T("0"));
        q_boss.q_boss_spawn_condition->ChangeValue(_T("0"));
        q_boss.q_boss_size2->ChangeValue(_T("0"));
        q_boss.q_boss_size1->Thaw();
        q_boss.q_boss_size1->SetSelection(0);
        /*q_boss.q_boss_angles[0]->ChangeValue(_T("0"));
        q_boss.q_boss_angles[1]->ChangeValue(_T("0"));
        q_boss.q_boss_angles[2]->ChangeValue(_T("0"));*/
        return;
    }

    if(b >= (int)q_boss_count) return;
    if(q_current_bgroup < 0 || q_current_bgroup >= q_bgroup_count) return;

    b += qdata.quest->boss_group_offset[q_current_bgroup].start;

    q_boss.q_boss_size_pc->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_stats[b].size_percentage).c_str(), wxConvUTF8));
    q_boss.q_boss_size_var->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_stats[b].size_variation).c_str(), wxConvUTF8));
    q_boss.q_boss_health->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_stats[b].health).c_str(), wxConvUTF8));
    q_boss.q_boss_attack->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_stats[b].attack).c_str(), wxConvUTF8));
    q_boss.q_boss_defense->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_stats[b].defense).c_str(), wxConvUTF8));
    q_boss.q_boss_stamina->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_stats[b].stamina).c_str(), wxConvUTF8));

    q_boss.q_boss_id->SetSelection(i_monster_table[qdata.quest->boss_group[b].boss_type]);
    q_boss.q_boss_spawn_count->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_group[b].spawn_quantity).c_str(), wxConvUTF8));
    q_boss.q_boss_cor_x->ChangeValue(wxString(PlusLib::float2str(qdata.quest->boss_group[b].coordinates[0]).c_str(), wxConvUTF8));
    q_boss.q_boss_spawn_area->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_group[b].spawn_area).c_str(), wxConvUTF8));
    q_boss.q_boss_cor_y->ChangeValue(wxString(PlusLib::float2str(qdata.quest->boss_group[b].coordinates[1]).c_str(), wxConvUTF8));

    int32_t tmp;
    vTab(tmp, qdata.quest->boss_group[b].frenzy, i_frenzy_table);
    q_boss.q_boss_frenzy->SetSelection(tmp);

    q_boss.q_boss_cor_z->ChangeValue(wxString(PlusLib::float2str(qdata.quest->boss_group[b].coordinates[2]).c_str(), wxConvUTF8));
    q_boss.q_boss_spawn_condition->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_group[b].spawn_condition).c_str(), wxConvUTF8));
    q_boss.q_boss_size2->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_group[b].size_related2).c_str(), wxConvUTF8));

    if(alternative_list.find(qdata.quest->boss_group[b].boss_type) != alternative_list.end() && alternative_text.find(qdata.quest->boss_group[b].boss_type) != alternative_text.end())
    {
        q_boss.q_boss_size1->Append(alternative_text[qdata.quest->boss_group[b].boss_type]);
        q_boss.q_boss_size1->SetSelection(getPosInVector(alternative_list[qdata.quest->boss_group[b].boss_type], qdata.quest->boss_group[b].form)+1);
    }
    else q_boss.q_boss_size1->SetSelection(0);
    q_boss.q_boss_size1->Thaw();

    /*q_boss.q_boss_angles[0]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_group[b].angles[0]).c_str(), wxConvUTF8));
    q_boss.q_boss_angles[1]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_group[b].angles[1]).c_str(), wxConvUTF8));
    q_boss.q_boss_angles[2]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_group[b].angles[2]).c_str(), wxConvUTF8));*/
}

void MainFrame::writeBoss(int b)
{
    if(!qdata.quest) return;

    if(b >= (int)q_boss_count || b < 0) return;
    if(q_current_bgroup < 0 || q_current_bgroup >= q_bgroup_count) return;

    b += qdata.quest->boss_group_offset[q_current_bgroup].start;

    PlusLib::validate16(std::string(q_boss.q_boss_size_pc->GetValue().mb_str()), qdata.quest->boss_stats[b].size_percentage, 0, 0xFFFF, "Boss " + PlusLib::int2str(b+1) + " size (%)");
    PlusLib::validate8(std::string(q_boss.q_boss_size_var->GetValue().mb_str()), qdata.quest->boss_stats[b].size_variation, 0, 0xFF, "Boss " + PlusLib::int2str(b+1) + " size variation (%)");
    PlusLib::validate8(std::string(q_boss.q_boss_health->GetValue().mb_str()), qdata.quest->boss_stats[b].health, 0, 0xFF, "Boss " + PlusLib::int2str(b+1) + " health");
    PlusLib::validate8(std::string(q_boss.q_boss_attack->GetValue().mb_str()), qdata.quest->boss_stats[b].attack, 0, 0xFF, "Boss " + PlusLib::int2str(b+1) + " attack");
    PlusLib::validate8(std::string(q_boss.q_boss_defense->GetValue().mb_str()), qdata.quest->boss_stats[b].defense, 0, 0xFF, "Boss " + PlusLib::int2str(b+1) + " defense");
    PlusLib::validate8(std::string(q_boss.q_boss_stamina->GetValue().mb_str()), qdata.quest->boss_stats[b].stamina, 0, 4, "Boss " + PlusLib::int2str(b+1) + " stamina");

    PlusLib::validate32(PlusLib::int2str(monster_table[q_boss.q_boss_id->GetSelection()]), qdata.quest->boss_group[b].boss_type, 0, 0xFFFFFFFF, "Boss " + PlusLib::int2str(b+1) + " ID");
    PlusLib::validate32(std::string(q_boss.q_boss_spawn_count->GetValue().mb_str()), qdata.quest->boss_group[b].spawn_quantity, 0, 0xFFFFFFFF, "Boss " + PlusLib::int2str(b+1) + " spawn quantity");
    PlusLib::validate8(std::string(q_boss.q_boss_spawn_area->GetValue().mb_str()), qdata.quest->boss_group[b].spawn_area, 0, 0xFF, "Boss " + PlusLib::int2str(b+1) + " spawn area");

    int32_t tmp;
    vTab(tmp, q_boss.q_boss_frenzy->GetSelection(), frenzy_table);
    qdata.quest->boss_group[b].frenzy = tmp;

    qdata.quest->boss_group[b].coordinates[0] = PlusLib::str2float(std::string(q_boss.q_boss_cor_x->GetValue().mb_str()));
    qdata.quest->boss_group[b].coordinates[1] = PlusLib::str2float(std::string(q_boss.q_boss_cor_y->GetValue().mb_str()));
    qdata.quest->boss_group[b].coordinates[2] = PlusLib::str2float(std::string(q_boss.q_boss_cor_z->GetValue().mb_str()));
    PlusLib::validate8(std::string(q_boss.q_boss_spawn_condition->GetValue().mb_str()), qdata.quest->boss_group[b].spawn_condition, 0, 0xFF, "Boss " + PlusLib::int2str(b+1) + " spawn condition");
    PlusLib::validate8(std::string(q_boss.q_boss_size2->GetValue().mb_str()), qdata.quest->boss_group[b].size_related2, 0, 0xFF, "Boss " + PlusLib::int2str(b+1) + " size related");

    if(q_boss.q_boss_size1->GetSelection() <= 0) qdata.quest->boss_group[b].form = 0;
    else
    {
        if(alternative_list.find(qdata.quest->boss_group[b].boss_type) != alternative_list.end())
        {
            int32_t form_count = alternative_list.find(qdata.quest->boss_group[b].boss_type)->second.size();
            if(q_boss.q_boss_size1->GetSelection() <= form_count)
            {
                qdata.quest->boss_group[b].form = alternative_list.find(qdata.quest->boss_group[b].boss_type)->second[q_boss.q_boss_size1->GetSelection()-1];
            }
        }
    }

    /*qdata.quest->boss_group[b].angles[0] = PlusLib::str2int(std::string(q_boss.q_boss_angles[0]->GetValue().mb_str()));
    qdata.quest->boss_group[b].angles[1] = PlusLib::str2int(std::string(q_boss.q_boss_angles[1]->GetValue().mb_str()));
    qdata.quest->boss_group[b].angles[2] = PlusLib::str2int(std::string(q_boss.q_boss_angles[2]->GetValue().mb_str()));*/
}

void MainFrame::initMonsterGroup()
{
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Shared Stats"), wxPoint(L_TEXT_X, L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Size (%)"), wxPoint(L_TEXT_X,25+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Size Variation (%)"), wxPoint(200+L_TEXT_X,25+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Health"), wxPoint(L_TEXT_X,50+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Attack"), wxPoint(200+L_TEXT_X,50+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Defense"), wxPoint(L_TEXT_X,75+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Stamina"), wxPoint(200+L_TEXT_X,75+L_TEXT_Y));

    q_monster.q_monster_size_pc = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(100,25));
    q_monster.q_monster_size_var = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(300,25));
    q_monster.q_monster_health = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(100,50));
    q_monster.q_monster_attack = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(300,50));
    q_monster.q_monster_defense = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(100,75));
    q_monster.q_monster_stamina = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(300,75));

    new wxStaticLine(sub_sizers[Q_SMALL_GROUP], -1, wxPoint(0,98), wxSize(500, 2));
    // ------------------------------------------------------------------------------
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Edit a subgroup"), wxPoint(L_TEXT_X,100+L_TEXT_Y));

    q_current_monster = -1;
    q_current_mgroup = -1;

    q_mgroup_choice = new wxChoice(sub_sizers[Q_SMALL_GROUP], ID_CHO_MGROUP, wxPoint(100,100), wxSize(100, 50));
    q_monster_choice = new wxChoice(sub_sizers[Q_SMALL_GROUP], ID_CHO_MONSTER, wxPoint(205,100), wxSize(100, 50));
    q_monster.q_monster_id = new wxChoice(sub_sizers[Q_SMALL_GROUP], ID_CHO_MONSTER_ID, wxPoint(310,100), wxSize(160, 50));
    q_monster.q_monster_id->Freeze();
    q_monster.q_monster_id->Append(monster_list);
    q_monster.q_monster_id->Thaw();

    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Number to spawn"), wxPoint(L_TEXT_X,125+L_TEXT_Y));
    q_monster.q_monster_spawn_count = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(100,125));
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Spawn area"), wxPoint(L_TEXT_X,150+L_TEXT_Y));
    q_monster.q_monster_spawn_area = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(100,150));
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Spawn condition"), wxPoint(L_TEXT_X,175+L_TEXT_Y));
    q_monster.q_monster_spawn_condition = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(100,175));

    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Position (X,Y,Z)"), wxPoint(200+L_TEXT_X,125+L_TEXT_Y));
    q_monster.q_monster_cor_x = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(300,125), wxSize(58, 23));
    q_monster.q_monster_cor_y = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(358,125), wxSize(58, 23));
    q_monster.q_monster_cor_z = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(416,125), wxSize(58, 23));

    new wxButton(sub_sizers[Q_SMALL_GROUP], ID_BTN_PLACE_MONSTER, _T("Auto position"), wxPoint(300, 150), wxSize(174, 23));

    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("0x0A value"), wxPoint(200+L_TEXT_X,175+L_TEXT_Y));
    q_monster.q_crash = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(300,175));

    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Size related"), wxPoint(L_TEXT_X,200+L_TEXT_Y));
    q_monster.q_monster_size2 = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(100,200));
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Alternative Form"), wxPoint(200+L_TEXT_X,200+L_TEXT_Y));
    q_monster.q_monster_size1 = new wxChoice(sub_sizers[Q_SMALL_GROUP], -1, wxPoint(300,200), wxSize(100, 50));

    new wxButton(sub_sizers[Q_SMALL_GROUP], ID_BTN_MONSTER_ADD, _T("Add monster"), wxPoint(0, 225), wxSize(75, 23));
    new wxButton(sub_sizers[Q_SMALL_GROUP], ID_BTN_MONSTER_DEL, _T("Del monster"), wxPoint(75, 225), wxSize(75, 23));
    new wxButton(sub_sizers[Q_SMALL_GROUP], ID_BTN_MONSTER_MOVE, _T("Move monster to..."), wxPoint(150, 225), wxSize(110, 23));
    q_monster_choice_move = new wxChoice(sub_sizers[Q_SMALL_GROUP], -1, wxPoint(263,226), wxSize(100, 50));

    new wxStaticLine(sub_sizers[Q_SMALL_GROUP], -1, wxPoint(0,248), wxSize(500, 2));
    // ------------------------------------------------------------------------------
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Edit the groups"), wxPoint(200+L_TEXT_X,250+L_TEXT_Y));
    new wxStaticLine(sub_sizers[Q_SMALL_GROUP], -1, wxPoint(0,273), wxSize(500, 2));
    // ------------------------------------------------------------------------------
    wxArrayString tmp_str1, tmp_str2;
    tmp_str1.Add(_T("None"));
    tmp_str1.Add(_T("Hunt"));
    tmp_str1.Add(_T("Collect"));
    tmp_str1.Add(_T("Deliver"));
    tmp_str1.Add(_T("Unknown"));

    tmp_str2.Add(_T("Group 1"));
    tmp_str2.Add(_T("Group 2"));
    tmp_str2.Add(_T("Group 3"));

    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Group spawn condition 1"), wxPoint(L_TEXT_X, 275+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Group spawn condition 2"), wxPoint(L_TEXT_X, 300+L_TEXT_Y));
    for(uint32_t i = 0; i < 2; i++)
    {
        q_mon_condition_type[i] = new wxChoice(sub_sizers[Q_SMALL_GROUP], ID_CHO_MONSTER_CONDITION, wxPoint(140,275+25*i), wxSize(75, 50));
        q_mon_condition_type[i]->Freeze();
        q_mon_condition_type[i]->Append(tmp_str1);
        q_mon_condition_type[i]->Thaw();
        q_mon_condition_group[i] = new wxChoice(sub_sizers[Q_SMALL_GROUP], -1, wxPoint(220,275+25*i), wxSize(65, 50));
        q_mon_condition_group[i]->Freeze();
        q_mon_condition_group[i]->Append(tmp_str2);
        q_mon_condition_group[i]->Thaw();
        q_mon_condition_id[i] = new wxChoice(sub_sizers[Q_SMALL_GROUP], -1, wxPoint(290,275+25*i), wxSize(100, 50));
        q_mon_condition_count[i] = new wxTextCtrl(sub_sizers[Q_SMALL_GROUP], -1, _T("0"), wxPoint(395, 275+25*i), wxSize(40, 23));
    }

    new wxStaticLine(sub_sizers[Q_SMALL_GROUP], -1, wxPoint(0,323), wxSize(500, 2));
    // ------------------------------------------------------------------------------
    new wxStaticLine(sub_sizers[Q_SMALL_GROUP], -1, wxPoint(160,323), wxSize(2, 600), wxVERTICAL);
    new wxStaticLine(sub_sizers[Q_SMALL_GROUP], -1, wxPoint(320,323), wxSize(2, 600), wxVERTICAL);
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Group 1"), wxPoint(50+L_TEXT_X,325+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Group 2"), wxPoint(50+160+L_TEXT_X,325+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T("Group 3"), wxPoint(50+320+L_TEXT_X,325+L_TEXT_Y));
    new wxButton(sub_sizers[Q_SMALL_GROUP], ID_BTN_MONSTER_HELP, _T("?"), wxPoint(5,325), wxSize(20, 23));

    for(uint32_t i = 0; i < 3; i++)
    {
        q_submon[i].text.push_back(new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T(""), wxPoint((160*i)+L_TEXT_X,375+L_TEXT_Y)));
        q_submon[i].paste = new wxButton(sub_sizers[Q_SMALL_GROUP], ID_BTN_PASTE_SUBGROUP, _T("paste"), wxPoint((160*i)+105, 325), wxSize(50, 23));
        q_submon[i].paste->Hide();
        q_submon[i].add = new wxButton(sub_sizers[Q_SMALL_GROUP], ID_BTN_ADD_MONSTER_GROUP, _T("Add new subgroup"), wxPoint((160*i)+20, 350), wxSize(100, 23));
        q_submon[i].add->Hide();
        q_submon[i].unbind = new wxButton(sub_sizers[Q_SMALL_GROUP], ID_BTN_UNBIND_MONSTER_GROUP, _T("Unbind"), wxPoint((160*i)+45, 400), wxSize(50, 23));
        q_submon[i].unbind->Hide();
        q_submon[i].count = 0;
    }

    ((wxScrolledWindow*)(sub_sizers[Q_SMALL_GROUP]))->SetVirtualSize(wxSize(450, 500));
    ((wxScrolledWindow*)(sub_sizers[Q_SMALL_GROUP]))->SetScrollRate(5, 5);
    ((wxScrolledWindow*)(sub_sizers[Q_SMALL_GROUP]))->EnableScrolling(false, true);
}

void MainFrame::OnChoGroupConditionType(wxCommandEvent &event)
{
    if(!qdata.quest) return;
    UpdateConditionType();
}

void MainFrame::UpdateConditionType()
{
    for(uint32_t i = 0; i < 2; i++)
    {
        switch(q_mon_condition_type[i]->GetSelection())
        {
            case 1: q_mon_condition_id[i]->Freeze(); q_mon_condition_id[i]->Clear();
            q_mon_condition_id[i]->Append(monster_list); q_mon_condition_id[i]->Thaw();
            q_mon_condition_id[i]->SetSelection(i_monster_table[qdata.quest->monster_condition[4+8*i] + (qdata.quest->monster_condition[5+8*i] << 8)]); break;
            case 2: case 3: q_mon_condition_id[i]->Freeze(); q_mon_condition_id[i]->Clear();
            q_mon_condition_id[i]->Append(item_list); q_mon_condition_id[i]->Thaw();
            q_mon_condition_id[i]->SetSelection(i_item_table[qdata.quest->monster_condition[4+8*i] + (qdata.quest->monster_condition[5+8*i] << 8)]); break;
            default: q_mon_condition_id[i]->Clear(); q_mon_condition_id[i]->SetSelection(0); break;
        }
    }
}

void MainFrame::readMonsterGroup()
{
    if(!qdata.quest) return;

    int vx, vy;
    ((wxScrolledWindow*)(sub_sizers[Q_SMALL_GROUP]))->GetViewStart(&vx, &vy);
    ((wxScrolledWindow*)(sub_sizers[Q_SMALL_GROUP]))->Scroll(0, 0);

    q_monster.q_monster_size_pc->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_stats.size_percentage).c_str(), wxConvUTF8));
    q_monster.q_monster_size_var->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_stats.size_variation).c_str(), wxConvUTF8));
    q_monster.q_monster_health->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_stats.health).c_str(), wxConvUTF8));
    q_monster.q_monster_attack->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_stats.attack).c_str(), wxConvUTF8));
    q_monster.q_monster_defense->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_stats.defense).c_str(), wxConvUTF8));
    q_monster.q_monster_stamina->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_stats.stamina).c_str(), wxConvUTF8));
    // ------------------------------------------------------------------------------
    for(uint32_t i = 0; i < 2; i++)
    {
        switch(qdata.quest->monster_condition[8*i])
        {
            case 0: q_mon_condition_type[i]->SetSelection(0); break;
            case 1: q_mon_condition_type[i]->SetSelection(1); break;
            case 2: q_mon_condition_type[i]->SetSelection(2); break;
            case 4: q_mon_condition_type[i]->SetSelection(3); break;
            default: q_mon_condition_type[i]->SetSelection(4); break;
        }
        q_mon_condition_group[i]->SetSelection(qdata.quest->monster_condition[7+8*i]);
        q_mon_condition_count[i]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_condition[6+8*i]).c_str(), wxConvUTF8));
    }
    UpdateConditionType();
    // ------------------------------------------------------------------------------
    q_mgroup_count = qdata.quest->monster_group_list.size();
    wxArrayString monster_str;
    for(int i = 0; i < q_mgroup_count; i++)
        monster_str.Add(wxString(std::string("Subgroup " + PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    if(q_mgroup_count == 0)
        monster_str.Add(_T("[NO GROUP]"));

    q_mgroup_choice->Freeze();
    q_mgroup_choice->Clear();
    q_mgroup_choice->Append(monster_str);
    q_mgroup_choice->Thaw();

    q_monster_choice_move->Freeze();
    q_monster_choice_move->Clear();
    q_monster_choice_move->Append(monster_str);
    q_monster_choice_move->Thaw();

    monster_str.clear();

    if(q_mgroup_count > 0)
    {
        if(q_current_mgroup < 0 || q_current_mgroup >= q_mgroup_count)
        {
            q_mgroup_choice->SetSelection(0);
            q_current_mgroup = 0;
        }
        else q_mgroup_choice->SetSelection(q_current_mgroup);

        q_monster_count = qdata.quest->monster_group_list[q_current_mgroup]->mon.size();
        if(q_monster_count > 0)
        {
            if(q_current_monster < 0 || q_current_monster >= q_monster_count) q_current_monster = 0;
            for(int i = 0; i < q_monster_count; i++)
                monster_str.Add(wxString(std::string("#" + PlusLib::int2str(i+1) + " - " + std::string(monster_list[i_monster_table[qdata.quest->monster_group_list[q_current_mgroup]->mon[i].monster_type]].mb_str())).c_str(), wxConvUTF8));
            q_monster_choice->Freeze();
            q_monster_choice->Clear();
            q_monster_choice->Append(monster_str);
            q_monster_choice->Thaw();
            q_monster_choice->SetSelection(q_current_monster);
        }
        else
        {
            monster_str.Add(_T("[NO MONSTER]"));
            q_monster_choice->Freeze();
            q_monster_choice->Clear();
            q_monster_choice->Append(monster_str);
            q_monster_choice->Thaw();
            q_monster_choice->SetSelection(0);
            q_current_monster = -1;
        }
    }
    else
    {
        q_mgroup_choice->SetSelection(0);
        q_current_mgroup = -1;
        q_current_monster = -1;
        q_monster_choice->Freeze();
        q_monster_choice->Clear();
        q_monster_choice->Thaw();
        q_monster_choice->SetSelection(q_current_monster);
    }
    readMonster(q_current_monster);
    // ------------------------------------------------------------------------------
    uint32_t wsize = 0, cwsize = 0;
    for(uint32_t m = 0; m < 3; m++)
    {
        while(qdata.quest->possible_monster[m].group.size() > q_submon[m].count)
        {
            if(q_submon[m].count > 0) q_submon[m].text.push_back(new wxStaticText(sub_sizers[Q_SMALL_GROUP], -1, _T(""), wxPoint((160*m)+L_TEXT_X, (25*q_submon[m].count)+375+L_TEXT_Y)));
            q_submon[m].del.push_back(new wxButton(sub_sizers[Q_SMALL_GROUP], ID_BTN_DEL_MONSTER_GROUP, _T("X"), wxPoint((160*m)+75, (25*q_submon[m].count)+375), wxSize(20, 23)));
            q_submon[m].cpy.push_back(new wxButton(sub_sizers[Q_SMALL_GROUP], ID_BTN_CPY_SUBGROUP, _T("copy"), wxPoint((160*m)+95, (25*q_submon[m].count)+375), wxSize(40, 23)));
            q_submon[m].count++;
        }
        uint32_t c;
        for(c = 0; c < qdata.quest->possible_monster[m].group.size() && qdata.quest->possible_monster[m].isCopyOf == -1; c++)
        {
            q_submon[m].text[c]->SetLabel(wxString(std::string("Subgroup " + PlusLib::int2str(qdata.getMonsterGroupPos(qdata.quest->possible_monster[m].group[c])+1)).c_str(), wxConvUTF8));
            q_submon[m].text[c]->Show();
            q_submon[m].del[c]->Show();
            q_submon[m].cpy[c]->Show();
        }
        cwsize = c;
        for(; c < q_submon[m].count; c++)
        {
            q_submon[m].text[c]->Hide();
            q_submon[m].del[c]->Hide();
            q_submon[m].cpy[c]->Hide();
        }

        if(qdata.quest->possible_monster[m].isCopyOf != -1)
        {
            q_submon[m].text[0]->SetLabel(wxString(std::string("Shared list with group " + PlusLib::int2str(qdata.quest->possible_monster[m].isCopyOf+1)).c_str(), wxConvUTF8));
            q_submon[m].text[0]->Show();
            q_submon[m].paste->Hide();
            q_submon[m].add->Hide();
            q_submon[m].unbind->Show();
            cwsize = 1;
        }
        else
        {
            q_submon[m].paste->Show();
            q_submon[m].add->Show();
            q_submon[m].unbind->Hide();
        }
        if(cwsize > wsize) wsize = cwsize;
    }
    ((wxScrolledWindow*)(sub_sizers[Q_SMALL_GROUP]))->SetVirtualSize(wxSize(450, 400+25*wsize));
    ((wxScrolledWindow*)(sub_sizers[Q_SMALL_GROUP]))->Scroll(vx, vy);
}

void MainFrame::writeMonsterGroup()
{
    if(!qdata.quest) return;

    PlusLib::validate16(std::string(q_monster.q_monster_size_pc->GetValue().mb_str()), qdata.quest->monster_stats.size_percentage, 0, 0xFFFF, "Small monster size (%)");
    PlusLib::validate8(std::string(q_monster.q_monster_size_var->GetValue().mb_str()), qdata.quest->monster_stats.size_variation, 0, 0xFF, "Small monster size variation (%)");
    PlusLib::validate8(std::string(q_monster.q_monster_health->GetValue().mb_str()), qdata.quest->monster_stats.health, 0, 0xFF, "Small monster health");
    PlusLib::validate8(std::string(q_monster.q_monster_attack->GetValue().mb_str()), qdata.quest->monster_stats.attack, 0, 0xFF, "Small monster attack");
    PlusLib::validate8(std::string(q_monster.q_monster_defense->GetValue().mb_str()), qdata.quest->monster_stats.defense, 0, 0xFF, "Small monster defense");
    PlusLib::validate8(std::string(q_monster.q_monster_stamina->GetValue().mb_str()), qdata.quest->monster_stats.stamina, 0, 0xFF, "Small monster stamina");

    uint32_t tmp = 0;
    for(uint32_t i = 0; i < 2; i++)
    {
        switch(q_mon_condition_type[i]->GetSelection())
        {
            case 0: case 1: case 2: tmp = q_mon_condition_type[i]->GetSelection(); break;
            case 3: tmp = 4; break;
            case 4: tmp = 0; break;
        }
        if(q_mon_condition_type[i]->GetSelection() != 4)
            PlusLib::validate8(PlusLib::int2str(tmp), qdata.quest->monster_condition[8*i], 0, 5, "Small monster condition type " + PlusLib::int2str(i));

        switch(q_mon_condition_type[i]->GetSelection())
        {
            case 1: tmp = monster_table[q_mon_condition_id[i]->GetSelection()]; break;
            case 2: case 3: tmp = item_table[q_mon_condition_id[i]->GetSelection()]; break;
            case 0: case 4: tmp = 0; break;
        }
        if(q_mon_condition_type[i]->GetSelection() != 4)
        {
            qdata.quest->monster_condition[4+8*i] = tmp % 0x100;
            qdata.quest->monster_condition[5+8*i] = (tmp / 0x100) % 0x100;
        }
        qdata.quest->monster_condition[7+8*i] = q_mon_condition_group[i]->GetSelection();

        PlusLib::validate8(std::string(q_mon_condition_count[i]->GetValue().mb_str()), qdata.quest->monster_condition[6+8*i], 0, 0xFF, "Small monster condition count " + PlusLib::int2str(i));
    }

    writeMonster(q_current_monster);
}

void MainFrame::readMonster(int m)
{
    if(!qdata.quest) return;
    q_monster.q_monster_size1->Freeze();
    q_monster.q_monster_size1->Clear();
    q_monster.q_monster_size1->Append(_T("Default"));
    if(m == -1)
    {
        q_monster.q_monster_id->SetSelection(-1);
        q_monster.q_monster_spawn_count->ChangeValue(_T("0"));
        q_monster.q_monster_cor_x->ChangeValue(_T("0"));
        q_monster.q_monster_spawn_area->ChangeValue(_T("0"));
        q_monster.q_monster_cor_y->ChangeValue(_T("0"));
        q_monster.q_monster_cor_z->ChangeValue(_T("0"));
        q_monster.q_monster_spawn_condition->ChangeValue(_T("0"));
        q_monster.q_crash->ChangeValue(_T("0"));
        /*q_monster.q_monster_angles[0]->ChangeValue(_T("0"));
        q_monster.q_monster_angles[1]->ChangeValue(_T("0"));
        q_monster.q_monster_angles[2]->ChangeValue(_T("0"));*/
        q_monster.q_monster_size2->ChangeValue(_T("0"));
        q_monster.q_monster_size1->Thaw();
        q_monster.q_monster_size1->SetSelection(0);
        return;
    }

    if(m >= (int)q_monster_count) return;
    if(q_current_mgroup < 0 || q_current_mgroup >= q_mgroup_count) return;

    q_monster.q_monster_id->SetSelection(i_monster_table[qdata.quest->monster_group_list[q_current_mgroup]->mon[m].monster_type]);
    q_monster.q_monster_spawn_count->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].spawn_quantity).c_str(), wxConvUTF8));
    q_monster.q_monster_cor_x->ChangeValue(wxString(PlusLib::float2str(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].coordinates[0]).c_str(), wxConvUTF8));
    q_monster.q_monster_spawn_area->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].spawn_area).c_str(), wxConvUTF8));
    q_monster.q_monster_cor_y->ChangeValue(wxString(PlusLib::float2str(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].coordinates[1]).c_str(), wxConvUTF8));
    q_monster.q_monster_cor_z->ChangeValue(wxString(PlusLib::float2str(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].coordinates[2]).c_str(), wxConvUTF8));
    q_monster.q_monster_spawn_condition->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].spawn_condition).c_str(), wxConvUTF8));
    q_monster.q_crash->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].crash_flag).c_str(), wxConvUTF8));
    q_monster.q_monster_size2->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].size_related1).c_str(), wxConvUTF8));

    if(alternative_list.find(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].monster_type) != alternative_list.end() && alternative_text.find(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].monster_type) != alternative_text.end())
    {
        q_monster.q_monster_size1->Append(alternative_text[qdata.quest->monster_group_list[q_current_mgroup]->mon[m].monster_type]);
        q_monster.q_monster_size1->SetSelection(getPosInVector(alternative_list[qdata.quest->monster_group_list[q_current_mgroup]->mon[m].monster_type], qdata.quest->monster_group_list[q_current_mgroup]->mon[m].form)+1);
    }
    else q_monster.q_monster_size1->SetSelection(0);
    q_monster.q_monster_size1->Thaw();

    /*q_monster.q_monster_angles[0]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].angles[0]).c_str(), wxConvUTF8));
    q_monster.q_monster_angles[1]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].angles[1]).c_str(), wxConvUTF8));
    q_monster.q_monster_angles[2]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].angles[2]).c_str(), wxConvUTF8));*/
}

void MainFrame::writeMonster(int m)
{
    if(m >= (int)q_monster_count || m < 0) return;
    if(q_current_mgroup < 0 || q_current_mgroup >= q_mgroup_count) return;

    PlusLib::validate32(PlusLib::int2str(monster_table[q_monster.q_monster_id->GetSelection()]), qdata.quest->monster_group_list[q_current_mgroup]->mon[m].monster_type, 0, 0xFFFFFFFF, "Monster " + PlusLib::int2str(m+1) + " ID");
    PlusLib::validate32(std::string(q_monster.q_monster_spawn_count->GetValue().mb_str()), qdata.quest->monster_group_list[q_current_mgroup]->mon[m].spawn_quantity, 0, 0xFFFFFFFF, "Monster " + PlusLib::int2str(m+1) + " spawn quantity");
    PlusLib::validate8(std::string(q_monster.q_monster_spawn_area->GetValue().mb_str()), qdata.quest->monster_group_list[q_current_mgroup]->mon[m].spawn_area, 0, 0xFF, "Monster " + PlusLib::int2str(m+1) + " spawn area");
    qdata.quest->monster_group_list[q_current_mgroup]->mon[m].coordinates[0] = PlusLib::str2float(std::string(q_monster.q_monster_cor_x->GetValue().mb_str()));
    qdata.quest->monster_group_list[q_current_mgroup]->mon[m].coordinates[1] = PlusLib::str2float(std::string(q_monster.q_monster_cor_y->GetValue().mb_str()));
    qdata.quest->monster_group_list[q_current_mgroup]->mon[m].coordinates[2] = PlusLib::str2float(std::string(q_monster.q_monster_cor_z->GetValue().mb_str()));
    PlusLib::validate8(std::string(q_monster.q_monster_spawn_condition->GetValue().mb_str()), qdata.quest->monster_group_list[q_current_mgroup]->mon[m].spawn_condition, 0, 0xFF, "Monster " + PlusLib::int2str(m+1) + " spawn condition");
    PlusLib::validate8(std::string(q_monster.q_crash->GetValue().mb_str()), qdata.quest->monster_group_list[q_current_mgroup]->mon[m].crash_flag, 0, 0xFF, "Monster " + PlusLib::int2str(m+1) + "  0x0A Offset");

    PlusLib::validate8(std::string(q_monster.q_monster_size2->GetValue().mb_str()), qdata.quest->monster_group_list[q_current_mgroup]->mon[m].size_related1, 0, 0xFF, "Monster " + PlusLib::int2str(m+1) + "  size related");

    if(q_monster.q_monster_size1->GetSelection() <= 0) qdata.quest->monster_group_list[q_current_mgroup]->mon[m].size_related1 = 0;
    else
    {
        if(alternative_list.find(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].monster_type) != alternative_list.end())
        {
            int32_t form_count = alternative_list.find(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].monster_type)->second.size();
            if(q_monster.q_monster_size1->GetSelection() <= form_count)
            {
                qdata.quest->monster_group_list[q_current_mgroup]->mon[m].form = alternative_list.find(qdata.quest->monster_group_list[q_current_mgroup]->mon[m].monster_type)->second[q_monster.q_monster_size1->GetSelection()-1];
            }
        }
    }
    /*qdata.quest->monster_group_list[q_current_mgroup]->mon[m].angles[0] = PlusLib::str2int(std::string(q_monster.q_monster_angles[0]->GetValue().mb_str()));
    qdata.quest->monster_group_list[q_current_mgroup]->mon[m].angles[1] = PlusLib::str2int(std::string(q_monster.q_monster_angles[1]->GetValue().mb_str()));
    qdata.quest->monster_group_list[q_current_mgroup]->mon[m].angles[2] = PlusLib::str2int(std::string(q_monster.q_monster_angles[2]->GetValue().mb_str()));*/
}

void MainFrame::OnBtnEditChallengeEquip(wxCommandEvent &event)
{
    if(!qdata.quest) return;
    if(qdata.quest->challenge_quest_offset == 0 || q_current_preset < 0 || q_current_preset >= qdata.quest->preset_count)
    {
        wxMessageBox(_T("Invalid preset"));
        return;
    }

    uint32_t slot;
    for(slot = 0; slot < 7; slot++)
        if(challenge_equip[slot] == event.GetEventObject()) break;
    if(slot == 7) return;

    EquipDialog eqDiag(NULL, wxID_ANY, wxString(std::string("Preset " + PlusLib::int2str(1+q_current_preset) + " - Slot " + PlusLib::int2str(1+slot)).c_str(), wxConvUTF8));

    if(slot == 0) eqDiag.read(((uint8_t*)&(qdata.quest->challenge[q_current_preset].weapon)));
    else if(slot == 6) eqDiag.read(((uint8_t*)&(qdata.quest->challenge[q_current_preset].talisman)));
    else eqDiag.read(((uint8_t*)&(qdata.quest->challenge[q_current_preset].armor[slot-1])));
    if(eqDiag.ShowModal() == wxID_OK)
    {
        eqDiag.write();
        readChallengeEquipment();
    }
}

void MainFrame::initChallenge()
{
    new wxStaticText(sub_sizers[Q_CHALLENGE], -1, _T("Preset"), wxPoint(L_TEXT_X,L_TEXT_Y));
    q_preset_choice = new wxChoice(sub_sizers[Q_CHALLENGE], ID_CHO_PRESET, wxPoint(100,0), wxSize(100, 50));

    add_ch = new wxButton(sub_sizers[Q_CHALLENGE], ID_ADD_PRESET, _T("Add presets"), wxPoint(250,0), wxSize(100, 23));
    del_ch = new wxButton(sub_sizers[Q_CHALLENGE], ID_DEL_PRESET, _T("Remove presets"), wxPoint(250,0), wxSize(100, 23));
    add_ch->Hide();
    del_ch->Hide();

    new wxStaticLine(sub_sizers[Q_CHALLENGE], -1, wxPoint(0,23), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_CHALLENGE], -1, _T("Preset equipment"), wxPoint(L_TEXT_X,25+L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_CHALLENGE], -1, _T("Weapon"), wxPoint(L_TEXT_X,50+L_TEXT_Y));
    challenge_equip[0] = new wxButton(sub_sizers[Q_CHALLENGE], ID_BTN_CHALLENGE_EQUIP, _T("Not loaded"), wxPoint(60,50), wxSize(160, 23));
    new wxStaticText(sub_sizers[Q_CHALLENGE], -1, _T("Chest"), wxPoint(250+L_TEXT_X,50+L_TEXT_Y));
    challenge_equip[1] = new wxButton(sub_sizers[Q_CHALLENGE], ID_BTN_CHALLENGE_EQUIP, _T("Not loaded"), wxPoint(310,50), wxSize(160, 23));
    new wxStaticText(sub_sizers[Q_CHALLENGE], -1, _T("Waist"), wxPoint(L_TEXT_X,75+L_TEXT_Y));
    challenge_equip[2] = new wxButton(sub_sizers[Q_CHALLENGE], ID_BTN_CHALLENGE_EQUIP, _T("Not loaded"), wxPoint(60,75), wxSize(160, 23));
    new wxStaticText(sub_sizers[Q_CHALLENGE], -1, _T("Arms"), wxPoint(250+L_TEXT_X,75+L_TEXT_Y));
    challenge_equip[3] = new wxButton(sub_sizers[Q_CHALLENGE], ID_BTN_CHALLENGE_EQUIP, _T("Not loaded"), wxPoint(310,75), wxSize(160, 23));
    new wxStaticText(sub_sizers[Q_CHALLENGE], -1, _T("Legs"), wxPoint(L_TEXT_X,100+L_TEXT_Y));
    challenge_equip[4] = new wxButton(sub_sizers[Q_CHALLENGE], ID_BTN_CHALLENGE_EQUIP, _T("Not loaded"), wxPoint(60,100), wxSize(160, 23));
    new wxStaticText(sub_sizers[Q_CHALLENGE], -1, _T("Head"), wxPoint(250+L_TEXT_X,100+L_TEXT_Y));
    challenge_equip[5] = new wxButton(sub_sizers[Q_CHALLENGE], ID_BTN_CHALLENGE_EQUIP, _T("Not loaded"), wxPoint(310,100), wxSize(160, 23));
    new wxStaticText(sub_sizers[Q_CHALLENGE], -1, _T("Talisman"), wxPoint(L_TEXT_X,125+L_TEXT_Y));
    challenge_equip[6] = new wxButton(sub_sizers[Q_CHALLENGE], ID_BTN_CHALLENGE_EQUIP, _T("Not loaded"), wxPoint(60,125), wxSize(160, 23));

    new wxStaticLine(sub_sizers[Q_CHALLENGE], -1, wxPoint(0,148), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_CHALLENGE], -1, _T("Preset inventory"), wxPoint(L_TEXT_X,150+L_TEXT_Y));

    q_challenge_items.count = C_ITEMS;
    for(int i = 0; i < C_ITEMS; i++)
    {
        q_challenge_items.texts.push_back(new wxStaticText(sub_sizers[Q_CHALLENGE], -1, wxString(std::string("Item " + PlusLib::int2str(i+1)).c_str(), wxConvUTF8), wxPoint(L_TEXT_X,175+(i*25)+L_TEXT_Y)));
        q_challenge_items.choices.push_back(new wxChoice(sub_sizers[Q_CHALLENGE], -1, wxPoint(55,175+i*25), wxSize(200, 50)));
        q_challenge_items.choices.back()->Freeze();
        q_challenge_items.choices.back()->Append(arena_item_list);
        q_challenge_items.choices.back()->Thaw();
        q_challenge_items.quantity.push_back(new wxTextCtrl(sub_sizers[Q_CHALLENGE], -1, _T("0"), wxPoint(260,175+i*25)));
    }
    ((wxScrolledWindow*)(sub_sizers[Q_CHALLENGE]))->SetVirtualSize(wxSize(450, 775));
    ((wxScrolledWindow*)(sub_sizers[Q_CHALLENGE]))->SetScrollRate(5, 5);
    ((wxScrolledWindow*)(sub_sizers[Q_CHALLENGE]))->EnableScrolling(false, true);
}

void MainFrame::OnBtnAddChallenge(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(qdata.quest->challenge_quest_offset != 0)
    {
        wxMessageBox(_T("Can't add the presets. They already exist"));
        return;
    }

    if(qdata.modifyChallenge(true))
    {
        wxMessageBox(_T("Success !\nDon't forget to set the equipments and the challenge flag"));
        readChallenge();
    }
    else
    {
        wxMessageBox(_T("Error: something gone wrong"));
    }
}

void MainFrame::OnBtnDelChallenge(wxCommandEvent &event)
{
    if(!qdata.quest) return;

    if(qdata.quest->challenge_quest_offset == 0)
    {
        wxMessageBox(_T("Can't delete the presets. They don't exist"));
        return;
    }

    if(wxMessageBox(_T("Are you sure you want to delete the presets ?"), _T("Confirm"), wxNO) == wxNO) return;

    if(qdata.modifyChallenge(false))
    {
        wxMessageBox(_T("Success !"));
        readChallenge();
    }
    else
    {
        wxMessageBox(_T("Error: something gone wrong"));
    }
}

void MainFrame::readChallengeEquipment()
{
    if(q_current_preset < 0 || q_current_preset >= qdata.quest->preset_count) return;

    if(qdata.quest->challenge[q_current_preset].weapon.category == 0) challenge_equip[0]->SetLabel(_T("(None)"));
    else challenge_equip[0]->SetLabel(arena_list[qdata.quest->challenge[q_current_preset].weapon.category-1][qdata.quest->challenge[q_current_preset].weapon.id]);
    for(uint32_t i = 0; i < 5; i++)
    {
        if(qdata.quest->challenge[q_current_preset].armor[i].category == 0) challenge_equip[i+1]->SetLabel(_T("(None)"));
        else challenge_equip[i+1]->SetLabel(arena_list[qdata.quest->challenge[q_current_preset].armor[i].category-1][qdata.quest->challenge[q_current_preset].armor[i].id]);
    }
    if(qdata.quest->challenge[q_current_preset].talisman.category == 0) challenge_equip[6]->SetLabel(_T("(None)"));
    else challenge_equip[6]->SetLabel(arena_list[qdata.quest->challenge[q_current_preset].talisman.category-1][qdata.quest->challenge[q_current_preset].talisman.id]);
}

void MainFrame::readChallenge()
{
    if(!qdata.quest) return;

    if(qdata.quest->challenge_quest_offset == 0)
    {
        add_ch->Show();
        del_ch->Hide();
    }
    else
    {
        del_ch->Show();
        add_ch->Hide();
    }

    q_preset_choice->Freeze();
    q_preset_choice->Clear();
    for(int32_t i = 0; i < qdata.quest->preset_count; i++)
        q_preset_choice->Append(wxString(std::string("Preset " + PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    if(qdata.quest->preset_count == 0) q_preset_choice->Append(_T("[NO PRESET]"));
    q_preset_choice->Thaw();

    q_preset_choice->SetSelection(q_current_preset);
    if(qdata.quest->challenge_quest_offset == 0 || q_current_preset < 0 || q_current_preset >= qdata.quest->preset_count) return;
    readChallengeEquipment();

    for(int i = 0; i < C_ITEMS; i++)
    {
        q_challenge_items.choices[i]->SetSelection(i_arena_item_table[qdata.quest->challenge[q_current_preset].items[i].item_id]);
        q_challenge_items.quantity[i]->SetValue(wxString(std::string(PlusLib::int2str(qdata.quest->challenge[q_current_preset].items[i].count)).c_str(), wxConvUTF8));
    }
}

void MainFrame::writeChallenge()
{
    if(!qdata.quest) return;
    if(qdata.quest->challenge_quest_offset == 0) return; // remove later

    if(q_current_preset < 0 || q_current_preset >= 5) return;
    for(int i = 0; i < C_ITEMS; i++)
    {
        PlusLib::validate16(PlusLib::int2str(arena_item_table[q_challenge_items.choices[i]->GetSelection()]), qdata.quest->challenge[q_current_preset].items[i].item_id, 0, 1937, "Challenge Item ID " + PlusLib::int2str(i));
        PlusLib::validate16(std::string(q_challenge_items.quantity[i]->GetValue().mb_str()), qdata.quest->challenge[q_current_preset].items[i].count, 0, 0x63, "Challenge Item Quantity " + PlusLib::int2str(i));
    }
}

void MainFrame::initIntruder()
{
    q_intruder_proba = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(175, 0));
    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Intruder probability (%)"), wxPoint(40+L_TEXT_X, L_TEXT_Y));

    q_intruder_timer = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(175, 25));
    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Intruder timer (minute)"), wxPoint(40+L_TEXT_X, 25+L_TEXT_Y));

    q_current_intruder = - 1;

    q_intruder_choice = new wxChoice(sub_sizers[Q_INTRUDER], ID_CHO_INTRUDER, wxPoint(100,50), wxSize(200, 50));

    q_intruder.q_id = new wxChoice(sub_sizers[Q_INTRUDER], ID_CHO_INTRUDER_ID, wxPoint(100,75), wxSize(200, 50));
    q_intruder.q_id->Freeze();
    q_intruder.q_id->Append(monster_list);
    q_intruder.q_id->Thaw();

    q_intruder.q_spawn_count = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(100,175));
    q_intruder.q_spawn_area = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(100,200));
    //q_intruder.q_frenzy = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(100,225));
    q_intruder.q_frenzy = new wxChoice(sub_sizers[Q_INTRUDER], -1, wxPoint(100,225), wxSize(100, 50));
    q_intruder.q_frenzy->Freeze();
    q_intruder.q_frenzy->Append(frenzy_list);
    q_intruder.q_frenzy->Thaw();
    q_intruder.q_spawn_condition = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(100,250));
    q_intruder.q_spawn_chance = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(300,250));

    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Position (X,Y,Z)"), wxPoint(200+L_TEXT_X,175+L_TEXT_Y));
    q_intruder.q_cor_x = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(300,175), wxSize(60, 23));
    q_intruder.q_cor_y = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(360,175), wxSize(60, 23));
    q_intruder.q_cor_z = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(420,175), wxSize(60, 23));

    new wxButton(sub_sizers[Q_INTRUDER], ID_BTN_PLACE_INTRUDER, _T("Auto position"), wxPoint(300, 200), wxSize(180, 23));

    /*new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Angles   (X,Y,Z)"), wxPoint(200+L_TEXT_X,200+L_TEXT_Y));
    q_intruder.q_angles[0] = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(300,200), wxSize(60, 23));
    q_intruder.q_angles[1] = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(360,200), wxSize(60, 23));
    q_intruder.q_angles[2] = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(420,200), wxSize(60, 23));*/

    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Spawn count"), wxPoint(L_TEXT_X,175+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Spawn area"), wxPoint(L_TEXT_X,200+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Frenzy"), wxPoint(L_TEXT_X,225+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Spawn condition"), wxPoint(L_TEXT_X,250+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Spawn chance (%)"), wxPoint(200+L_TEXT_X,250+L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Size related"), wxPoint(L_TEXT_X,275+L_TEXT_Y));
    q_intruder.q_size2 = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(100,275));
    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Alternative Form"), wxPoint(200+L_TEXT_X,275+L_TEXT_Y));
    q_intruder.q_size1 = new wxChoice(sub_sizers[Q_INTRUDER], -1, wxPoint(300,275), wxSize(100, 50));


    q_intruder_add = new wxButton(sub_sizers[Q_INTRUDER], ID_BTN_INTRUDER_ADD, _T("Add Intruder"), wxPoint(0,325), wxSize(100, 23));
    q_intruder_del = new wxButton(sub_sizers[Q_INTRUDER], ID_BTN_INTRUDER_DEL, _T("Delete Intruder"), wxPoint(105,325), wxSize(100, 23));

    q_intruder.q_size_pc = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(100,100));
    q_intruder.q_size_var = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(300,100));
    q_intruder.q_health = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(100,125));
    q_intruder.q_attack = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(300,125));
    q_intruder.q_defense = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(100,150));
    q_intruder.q_stamina = new wxTextCtrl(sub_sizers[Q_INTRUDER], -1, _T("0"), wxPoint(300,150));

    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Size (%)"), wxPoint(L_TEXT_X,100+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Size Variation (%)"), wxPoint(200+L_TEXT_X,100+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Health"), wxPoint(L_TEXT_X,125+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Attack"), wxPoint(200+L_TEXT_X,125+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Defense"), wxPoint(L_TEXT_X,150+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Stamina"), wxPoint(200+L_TEXT_X,150+L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_INTRUDER], -1, _T("Stats are shared"), wxPoint(395+L_TEXT_X,125+L_TEXT_Y));

    new wxStaticLine(sub_sizers[Q_INTRUDER], -1, wxPoint(0,173), wxSize(500, 2));
    new wxStaticLine(sub_sizers[Q_INTRUDER], -1, wxPoint(0,98), wxSize(500, 2));
}

void MainFrame::readIntruderAll()
{
    if(!qdata.quest) return;

    q_intruder_proba->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder_probability).c_str(), wxConvUTF8));
    q_intruder_timer->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder_condition[0]).c_str(), wxConvUTF8));

    q_intruder_count = qdata.quest->intruder.size();
    wxArrayString int_str;
    for(int i = 0; i < q_intruder_count; i++)
        int_str.Add(wxString(std::string("#" + PlusLib::int2str(i+1) + " - " + std::string(monster_list[i_monster_table[qdata.quest->intruder[i].boss_type]].mb_str())).c_str(), wxConvUTF8));
    if(q_intruder_count == 0) int_str.Add(_T("[NO INTRUDER IN THE QUEST]"));

    q_intruder_choice->Freeze();
    q_intruder_choice->Clear();
    q_intruder_choice->Append(int_str);
    q_intruder_choice->Thaw();

    if(q_intruder_count > 0)
    {
        q_intruder.q_size_pc->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_stats[2].size_percentage).c_str(), wxConvUTF8));
        q_intruder.q_size_var->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_stats[2].size_variation).c_str(), wxConvUTF8));
        q_intruder.q_health->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_stats[2].health).c_str(), wxConvUTF8));
        q_intruder.q_attack->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_stats[2].attack).c_str(), wxConvUTF8));
        q_intruder.q_defense->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_stats[2].defense).c_str(), wxConvUTF8));
        q_intruder.q_stamina->ChangeValue(wxString(PlusLib::int2str(qdata.quest->boss_stats[2].stamina).c_str(), wxConvUTF8));
        if(q_current_intruder < 0 && q_current_intruder >= q_intruder_count)
        {
            q_intruder_choice->SetSelection(0);
            q_current_intruder = 0;
        }
        else q_intruder_choice->SetSelection(q_current_intruder);
        readIntruder(q_current_intruder);
    }
    else
    {
        q_intruder_choice->SetSelection(0);
        q_current_intruder = -1;
        readIntruder(-1);
    }
}

void MainFrame::writeIntruderAll()
{
    if(!qdata.quest) return;
    writeIntruder(q_current_intruder);
}

void MainFrame::readIntruder(int b)
{
    if(!qdata.quest) return;
    q_intruder.q_size1->Freeze();
    q_intruder.q_size1->Clear();
    q_intruder.q_size1->Append(_T("Default"));
    if(b == -1)
    {
        q_intruder.q_id->SetSelection(-1);
        q_intruder.q_spawn_count->ChangeValue(_T("0"));
        q_intruder.q_cor_x->ChangeValue(_T("0"));
        q_intruder.q_spawn_area->ChangeValue(_T("0"));
        q_intruder.q_cor_y->ChangeValue(_T("0"));
        q_intruder.q_frenzy->SetSelection(0);
        q_intruder.q_cor_z->ChangeValue(_T("0"));
        q_intruder.q_spawn_condition->ChangeValue(_T("0"));
        q_intruder.q_spawn_chance->ChangeValue(_T("0"));
        /*q_intruder.q_angles[0]->ChangeValue(_T("0"));
        q_intruder.q_angles[1]->ChangeValue(_T("0"));
        q_intruder.q_angles[2]->ChangeValue(_T("0"));*/
        q_intruder.q_size2->ChangeValue(_T("0"));
        q_intruder.q_size1->Thaw();
        q_intruder.q_size1->SetSelection(0);
        return;
    }

    if(b >= (int)qdata.quest->intruder.size()) return;

    q_intruder.q_id->SetSelection(i_monster_table[qdata.quest->intruder[b].boss_type]);
    q_intruder.q_spawn_count->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder[b].spawn_quantity).c_str(), wxConvUTF8));
    q_intruder.q_cor_x->ChangeValue(wxString(PlusLib::float2str(qdata.quest->intruder[b].coordinates[0]).c_str(), wxConvUTF8));
    q_intruder.q_spawn_area->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder[b].spawn_area).c_str(), wxConvUTF8));
    q_intruder.q_cor_y->ChangeValue(wxString(PlusLib::float2str(qdata.quest->intruder[b].coordinates[1]).c_str(), wxConvUTF8));
    int32_t tmp;
    vTab(tmp, qdata.quest->intruder[b].frenzy, i_frenzy_table);
    q_intruder.q_frenzy->SetSelection(tmp);

    q_intruder.q_cor_z->ChangeValue(wxString(PlusLib::float2str(qdata.quest->intruder[b].coordinates[2]).c_str(), wxConvUTF8));
    q_intruder.q_spawn_condition->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder[b].spawn_condition).c_str(), wxConvUTF8));
    q_intruder.q_spawn_chance->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder[b].probability).c_str(), wxConvUTF8));

    /*q_intruder.q_angles[0]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder[b].angles[0]).c_str(), wxConvUTF8));
    q_intruder.q_angles[1]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder[b].angles[1]).c_str(), wxConvUTF8));
    q_intruder.q_angles[2]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder[b].angles[2]).c_str(), wxConvUTF8));*/

    q_intruder.q_size2->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder[b].size_related2).c_str(), wxConvUTF8));

    if(alternative_list.find(qdata.quest->intruder[b].boss_type) != alternative_list.end() && alternative_text.find(qdata.quest->intruder[b].boss_type) != alternative_text.end())
    {
        q_intruder.q_size1->Append(alternative_text[qdata.quest->intruder[b].boss_type]);
        q_intruder.q_size1->SetSelection(getPosInVector(alternative_list[qdata.quest->intruder[b].boss_type], qdata.quest->intruder[b].form)+1);
    }
    else q_intruder.q_size1->SetSelection(0);
    q_intruder.q_size1->Thaw();
}

void MainFrame::writeIntruder(int b)
{
    if(!qdata.quest) return;

    if(qdata.quest->intruder.size() == 0)
    {
        qdata.quest->intruder_probability = 0;
        qdata.quest->intruder_condition[0] = 0;
    }
    else
    {
        PlusLib::validate32(std::string(q_intruder_proba->GetValue().mb_str()), qdata.quest->intruder_probability, 0, 0x64, "Intruder probability");
        PlusLib::validate8(std::string(q_intruder_timer->GetValue().mb_str()), qdata.quest->intruder_condition[0], 0, 0xFF, "Intruder Timer");
        PlusLib::validate16(std::string(q_intruder.q_size_pc->GetValue().mb_str()), qdata.quest->boss_stats[2].size_percentage, 0, 0xFFFF, "Intruder size (%)");
        PlusLib::validate8(std::string(q_intruder.q_size_var->GetValue().mb_str()), qdata.quest->boss_stats[2].size_variation, 0, 0xFF, "Intruder size variation (%)");
        PlusLib::validate8(std::string(q_intruder.q_health->GetValue().mb_str()), qdata.quest->boss_stats[2].health, 0, 0xFF, "Intruder health");
        PlusLib::validate8(std::string(q_intruder.q_attack->GetValue().mb_str()), qdata.quest->boss_stats[2].attack, 0, 0xFF, "Intruder attack");
        PlusLib::validate8(std::string(q_intruder.q_defense->GetValue().mb_str()), qdata.quest->boss_stats[2].defense, 0, 0xFF, "Intruder defense");
        PlusLib::validate8(std::string(q_intruder.q_stamina->GetValue().mb_str()), qdata.quest->boss_stats[2].stamina, 0, 4, "Intruder stamina");
    }

    qdata.quest->intruder_condition[2] = qdata.quest->intruder_probability % 0x100;

    if(b >= (int)qdata.quest->intruder.size() || b < 0) return;

    PlusLib::validate32(PlusLib::int2str(monster_table[q_intruder.q_id->GetSelection()]), qdata.quest->intruder[b].boss_type, 0, 0xFFFFFFFF, "Intruder " + PlusLib::int2str(b+1) + " ID");
    PlusLib::validate32(std::string(q_intruder.q_spawn_count->GetValue().mb_str()), qdata.quest->intruder[b].spawn_quantity, 0, 0xFFFFFFFF, "Intruder " + PlusLib::int2str(b+1) + " spawn quantity");
    PlusLib::validate8(std::string(q_intruder.q_spawn_area->GetValue().mb_str()), qdata.quest->intruder[b].spawn_area, 0, 0xFF, "Intruder " + PlusLib::int2str(b+1) + " spawn area");
    //PlusLib::validate8(std::string(q_intruder.q_frenzy->GetValue().mb_str()), qdata.quest->intruder[b].frenzy, 0, 0xFF, "Intruder " + PlusLib::int2str(b+1) + " frenzy");
    int32_t tmp;
    vTab(tmp, q_intruder.q_frenzy->GetSelection(), frenzy_table);
    qdata.quest->intruder[b].frenzy = tmp;

    qdata.quest->intruder[b].coordinates[0] = PlusLib::str2float(std::string(q_intruder.q_cor_x->GetValue().mb_str()));
    qdata.quest->intruder[b].coordinates[1] = PlusLib::str2float(std::string(q_intruder.q_cor_y->GetValue().mb_str()));
    qdata.quest->intruder[b].coordinates[2] = PlusLib::str2float(std::string(q_intruder.q_cor_z->GetValue().mb_str()));
    PlusLib::validate8(std::string(q_intruder.q_spawn_condition->GetValue().mb_str()), qdata.quest->intruder[b].spawn_condition, 0, 0xFF, "Intruder " + PlusLib::int2str(b+1) + " spawn condition");
    PlusLib::validate32(std::string(q_intruder.q_spawn_chance->GetValue().mb_str()), qdata.quest->intruder[b].probability, 0, 0x64, "Intruder " + PlusLib::int2str(b+1) + " spawn chance");

    /*qdata.quest->intruder[b].angles[0] = PlusLib::str2int(std::string(q_intruder.q_angles[0]->GetValue().mb_str()));
    qdata.quest->intruder[b].angles[1] = PlusLib::str2int(std::string(q_intruder.q_angles[1]->GetValue().mb_str()));
    qdata.quest->intruder[b].angles[2] = PlusLib::str2int(std::string(q_intruder.q_angles[2]->GetValue().mb_str()));*/

    PlusLib::validate8(std::string(q_intruder.q_size2->GetValue().mb_str()), qdata.quest->intruder[b].size_related2, 0, 0xFF, "Intruder " + PlusLib::int2str(b+1) + " size related");

    if(q_intruder.q_size1->GetSelection() <= 0) qdata.quest->intruder[b].form = 0;
    else
    {
        if(alternative_list.find(qdata.quest->intruder[b].boss_type) != alternative_list.end())
        {
            int32_t form_count = alternative_list.find(qdata.quest->intruder[b].boss_type)->second.size();
            if(q_intruder.q_size1->GetSelection() <= form_count)
            {
                qdata.quest->intruder[b].form = alternative_list.find(qdata.quest->intruder[b].boss_type)->second[q_intruder.q_size1->GetSelection()-1];
            }
        }
    }
}

void MainFrame::initTest()
{
    new wxStaticText(sub_sizers[Q_TEST], -1, _T("Spawn timer"), wxPoint(L_TEXT_X,25+L_TEXT_Y));
    q_spawn_timer = new wxTextCtrl(sub_sizers[Q_TEST], -1, _T("0"), wxPoint(100,25));

    new wxStaticText(sub_sizers[Q_TEST], -1, _T("Offset 0x88"), wxPoint(L_TEXT_X,50+L_TEXT_Y));
    q_unknown = new wxTextCtrl(sub_sizers[Q_TEST], -1, _T("0"), wxPoint(100,50));

    new wxStaticText(sub_sizers[Q_TEST], -1, _T("0xC8 (read-only)"), wxPoint(L_TEXT_X,75+L_TEXT_Y));
    q_main_quest_count[0] = new wxTextCtrl(sub_sizers[Q_TEST], -1, _T("0"), wxPoint(100,75), wxSize(50, 23), wxTE_READONLY);
    q_main_quest_count[1] = new wxTextCtrl(sub_sizers[Q_TEST], -1, _T("0"), wxPoint(150,75), wxSize(50, 23), wxTE_READONLY);
    q_main_quest_count[2] = new wxTextCtrl(sub_sizers[Q_TEST], -1, _T("0"), wxPoint(200,75), wxSize(50, 23), wxTE_READONLY);
    q_main_quest_count[3] = new wxTextCtrl(sub_sizers[Q_TEST], -1, _T("0"), wxPoint(250,75), wxSize(50, 23), wxTE_READONLY);

    new wxStaticText(sub_sizers[Q_TEST], -1, _T("Int.Condition (read-only)"), wxPoint(L_TEXT_X,100+L_TEXT_Y));
    q_intruder_condition_test[0] = new wxTextCtrl(sub_sizers[Q_TEST], -1, _T("0"), wxPoint(150,100), wxSize(50, 23), wxTE_READONLY);
    q_intruder_condition_test[1] = new wxTextCtrl(sub_sizers[Q_TEST], -1, _T("0"), wxPoint(200,100), wxSize(50, 23), wxTE_READONLY);
    q_intruder_condition_test[2] = new wxTextCtrl(sub_sizers[Q_TEST], -1, _T("0"), wxPoint(250,100), wxSize(50, 23), wxTE_READONLY);
    q_intruder_condition_test[3] = new wxTextCtrl(sub_sizers[Q_TEST], -1, _T("0"), wxPoint(300,100), wxSize(50, 23), wxTE_READONLY);

    q_flags_check[7] = new wxCheckBox(sub_sizers[Q_TEST], -1, _T("??? Flag 5"), wxPoint(L_TEXT_X,L_TEXT_Y));
    q_flags_check[11] = new wxCheckBox(sub_sizers[Q_TEST], -1, _T("Rusted Kushala"), wxPoint(85,L_TEXT_Y));
}

void MainFrame::readTest()
{
    if(!qdata.quest) return;

    q_spawn_timer->ChangeValue(wxString(PlusLib::int2str(qdata.quest->spawn_timer).c_str(), wxConvUTF8));
    q_unknown->ChangeValue(wxString(PlusLib::int2str(qdata.quest->unknown2c).c_str(), wxConvUTF8));

    q_main_quest_count[0]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->main_quest_count[0]).c_str(), wxConvUTF8));
    q_main_quest_count[1]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->main_quest_count[1]).c_str(), wxConvUTF8));
    q_main_quest_count[2]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->main_quest_count[2]).c_str(), wxConvUTF8));
    q_main_quest_count[3]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->main_quest_count[3]).c_str(), wxConvUTF8));

    q_intruder_condition_test[0]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder_condition[3]).c_str(), wxConvUTF8));
    q_intruder_condition_test[1]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder_condition[2]).c_str(), wxConvUTF8));
    q_intruder_condition_test[2]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder_condition[1]).c_str(), wxConvUTF8));
    q_intruder_condition_test[3]->ChangeValue(wxString(PlusLib::int2str(qdata.quest->intruder_condition[0]).c_str(), wxConvUTF8));

    q_flags_check[7]->SetValue((qdata.quest->flags >> 4) & 1); // everwood
    q_flags_check[11]->SetValue((qdata.quest->flags >> 20) & 1); // kushala
}

void MainFrame::writeTest()
{
    if(!qdata.quest) return;

    PlusLib::validate32(std::string(q_spawn_timer->GetValue().mb_str()), qdata.quest->spawn_timer, 0, 0xFFFFFFFF, "Spawn timer");
    PlusLib::validate8(std::string(q_unknown->GetValue().mb_str()), qdata.quest->unknown2c, 0, 0xFF, "offset 0x8B");

    if(q_flags_check[7]->GetValue() != ((qdata.quest->flags >> 4) & 1)) qdata.quest->flags ^= 1 << 4;
    if(q_flags_check[11]->GetValue() != ((qdata.quest->flags >> 20) & 1)) qdata.quest->flags ^= 1 << 20;
}

void MainFrame::OnColorPick(wxColourPickerEvent &event)
{
    if(event.GetEventObject() == coat_col) coat_col->SetColour(event.GetColour());
    else if(event.GetEventObject() == cloth_col) cloth_col->SetColour(event.GetColour());
}

void MainFrame::initPalico()
{
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Palico editor"), wxPoint(L_TEXT_X,L_TEXT_Y));

    p_new = new wxButton(sub_sizers[Q_PALICO], ID_BTN_NEW_P, _T("New"), wxPoint(0,25), wxSize(50, 23));
    p_load = new wxButton(sub_sizers[Q_PALICO], ID_BTN_LOAD_P, _T("Load"), wxPoint(50,25), wxSize(50, 23));
    p_save = new wxButton(sub_sizers[Q_PALICO], ID_BTN_SAVE_P, _T("Save"), wxPoint(100,25), wxSize(50, 23));
    p_save->Disable();
    p_saveas = new wxButton(sub_sizers[Q_PALICO], ID_BTN_SAVEAS_P, _T("Save As"), wxPoint(150,25), wxSize(50, 23));
    p_saveas->Disable();
    p_encrypt = new wxButton(sub_sizers[Q_PALICO], ID_BTN_ENCRYPTNOW_P, _T("Encrypt"), wxPoint(200,25), wxSize(60, 23));
    p_encrypt->Disable();
    p_encryptas = new wxButton(sub_sizers[Q_PALICO], ID_BTN_ENCRYPTAS_P, _T("Encrypt As"), wxPoint(260,25), wxSize(70, 23));
    p_encryptas->Disable();

    p_encryptchoice = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(330,26), wxSize(60, 50));
    p_encryptchoice->Freeze();
    p_encryptchoice->Append(_T("JPN"));
    p_encryptchoice->Append(_T("US/EU"));
    p_encryptchoice->Append(_T("KOR"));
    p_encryptchoice->Append(_T("TWN"));
    p_encryptchoice->Thaw();
    p_encryptchoice->SetSelection(iniFile.getIntData("ENCRYPT_PALICO_REGION", "GENERAL"));

    m_paliconame_text = new wxStaticText(sub_sizers[Q_PALICO], -1, _T("No file loaded"), wxPoint(L_TEXT_X,50+L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Name"), wxPoint(L_TEXT_X,75+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Owner"), wxPoint(L_TEXT_X,100+L_TEXT_Y));
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Greetings"), wxPoint(L_TEXT_X,125+L_TEXT_Y));
    for(uint32_t i = 0; i < 3; i++)
        p_text[i] = new wxTextCtrl(sub_sizers[Q_PALICO], -1, _T("(Empty)"), wxPoint(75,75+i*25), wxSize(400, 23));

    new wxStaticLine(sub_sizers[Q_PALICO], -1, wxPoint(0,148), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Palico stats"), wxPoint(L_TEXT_X,150+L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("ID"), wxPoint(L_TEXT_X,175+L_TEXT_Y));
    p_id = new wxTextCtrl(sub_sizers[Q_PALICO], -1, _T("0"), wxPoint(90,175), wxSize(135, 23));
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Level"), wxPoint(250+L_TEXT_X,175+L_TEXT_Y));
    p_level = new wxTextCtrl(sub_sizers[Q_PALICO], -1, _T("0"), wxPoint(340,175), wxSize(135, 23));
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Forte"), wxPoint(L_TEXT_X,200+L_TEXT_Y));
    p_forte = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(90,200), wxSize(135, 50));
    p_forte->Freeze();
    p_forte->Append(_T("None/Leadership"));
    p_forte->Append(_T("Fighting"));
    p_forte->Append(_T("Protection"));
    p_forte->Append(_T("Support"));
    p_forte->Append(_T("Healing"));
    p_forte->Append(_T("Bombing"));
    p_forte->Append(_T("Stealing"));
    p_forte->Append(_T("Treasure"));
    p_forte->Append(_T("Launching"));
    p_forte->Append(_T("Stamina"));
    p_forte->Thaw();


    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Quest skill 1"), wxPoint(L_TEXT_X,225+L_TEXT_Y));
    p_quest[0] = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(90,225), wxSize(135, 50));
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Quest skill 2"), wxPoint(L_TEXT_X,250+L_TEXT_Y));
    p_quest[1] = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(90,250), wxSize(135, 50));
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Quest skill 3"), wxPoint(L_TEXT_X,275+L_TEXT_Y));
    p_quest[2] = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(90,275), wxSize(135, 50));

    for(uint8_t i = 0; i < 3; i++)
    {
        p_quest[i]->Freeze();
        p_quest[i]->Append(palico_skill_list);
        p_quest[i]->Thaw();
    }

    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Signature skill"), wxPoint(250+L_TEXT_X,200+L_TEXT_Y));
    p_sign = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(340,200), wxSize(135, 50));
    p_sign->Freeze();
    p_sign->Append(palico_skill_list);
    p_sign->Thaw();
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Meownster skill"), wxPoint(250+L_TEXT_X,225+L_TEXT_Y));
    p_meow = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(340,225), wxSize(135, 50));
    p_meow->Freeze();
    p_meow->Append(palico_skill_list);
    p_meow->Thaw();
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Casting skill"), wxPoint(250+L_TEXT_X,250+L_TEXT_Y));
    p_cast = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(340,250), wxSize(135, 50));
    p_cast->Freeze();
    p_cast->Append(palico_skill_list);
    p_cast->Thaw();
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Team skill"), wxPoint(250+L_TEXT_X,275+L_TEXT_Y));
    p_team = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(340,275), wxSize(135, 50));
    p_team->Freeze();
    p_team->Append(_T("Flying F-Bomb"));
    p_team->Append(_T("Purrtuoso"));
    p_team->Append(_T("Rath-of-Meow"));
    p_team->Thaw();

    new wxStaticLine(sub_sizers[Q_PALICO], -1, wxPoint(0,298), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Palico body"), wxPoint(L_TEXT_X,300+L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Coat color"), wxPoint(L_TEXT_X,325+L_TEXT_Y));
    coat_col = new wxColourPickerCtrl(sub_sizers[Q_PALICO], ID_COLOR_PICK, wxColor(0, 0, 0), wxPoint(100,325));

    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Coat type"), wxPoint(250+L_TEXT_X,325+L_TEXT_Y));
    coat = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(340,325), wxSize(135, 50));
    coat->Freeze();
    coat->Append(_T("Felyne"));
    coat->Append(_T("Melynx"));
    coat->Append(_T("Striped"));
    coat->Append(_T("Plain"));
    coat->Append(_T("Patchy"));
    coat->Append(_T("Calico"));
    coat->Thaw();

    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Eye type"), wxPoint(L_TEXT_X,350+L_TEXT_Y));
    eye = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(90,350), wxSize(135, 50));
    eye->Freeze();
    eye->Append(_T("Cute"));
    eye->Append(_T("Slits"));
    eye->Append(_T("Angry"));
    eye->Append(_T("Smile"));
    eye->Append(_T("Shut"));
    eye->Append(_T("Scarred eye"));
    eye->Thaw();

    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Ear type"), wxPoint(250+L_TEXT_X,350+L_TEXT_Y));
    ear = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(340,350), wxSize(135, 50));
    ear->Freeze();
    ear->Append(_T("Straight"));
    ear->Append(_T("Fold"));
    ear->Append(_T("Back"));
    ear->Thaw();

    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Tail type"), wxPoint(L_TEXT_X,375+L_TEXT_Y));
    tail = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(90,375), wxSize(135, 50));
    tail->Freeze();
    tail->Append(_T("Straight"));
    tail->Append(_T("Manx"));
    tail->Append(_T("Kinked up"));
    tail->Thaw();

    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Voice type"), wxPoint(250+L_TEXT_X,375+L_TEXT_Y));
    voice = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(340,375), wxSize(135, 50));
    voice->Freeze();
    voice->Append(_T("Regular"));
    voice->Append(_T("Gratey"));
    voice->Append(_T("High pitched"));
    voice->Append(_T("Kitten"));
    voice->Append(_T("Old cat"));
    voice->Thaw();

    new wxStaticLine(sub_sizers[Q_PALICO], -1, wxPoint(0,398), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Palico clothing"), wxPoint(L_TEXT_X,400+L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Clothing color"), wxPoint(L_TEXT_X,425+L_TEXT_Y));
    cloth_col = new wxColourPickerCtrl(sub_sizers[Q_PALICO], ID_COLOR_PICK, wxColor(0, 0, 0), wxPoint(100,425));

    new wxStaticText(sub_sizers[Q_PALICO], -1, _T("Clothing type"), wxPoint(250+L_TEXT_X,425+L_TEXT_Y));
    cloth = new wxChoice(sub_sizers[Q_PALICO], -1, wxPoint(340,425), wxSize(135, 50));
    cloth->Freeze();
    cloth->Append(_T("Waistcoat"));
    cloth->Append(_T("Armor"));
    cloth->Thaw();

    ((wxScrolledWindow*)(sub_sizers[Q_PALICO]))->SetVirtualSize(wxSize(450, 450));
    ((wxScrolledWindow*)(sub_sizers[Q_PALICO]))->SetScrollRate(5, 5);
    ((wxScrolledWindow*)(sub_sizers[Q_PALICO]))->EnableScrolling(false, true);
}

void MainFrame::readPalico()
{
    if(!pdata.palico) return;

    for(uint8_t i = 0; i < 3; i++)
    {
        p_text[i]->SetMaxLength((pdata.palico->text.texts[i].size()/2)-TEXT_SUB);
        p_text[i]->ChangeValue(convertStrFromQuest(pdata.palico->text.texts[i]));
        p_quest[i]->SetSelection(i_palico_skill_table[pdata.palico->quest_skill[i]]);
    }

    p_id->ChangeValue(wxString(PlusLib::int2str(pdata.palico->id).c_str(), wxConvUTF8));
    p_level->ChangeValue(wxString(PlusLib::int2str(pdata.palico->level).c_str(), wxConvUTF8));
    p_forte->SetSelection(pdata.palico->forte);
    p_sign->SetSelection(i_palico_skill_table[pdata.palico->signature_skill]);
    p_meow->SetSelection(i_palico_skill_table[pdata.palico->meownster_skill]);
    p_team->SetSelection(pdata.palico->team_skill);
    p_cast->SetSelection(i_palico_skill_table[pdata.palico->casting_skill]);
    coat->SetSelection(pdata.palico->coat);
    eye->SetSelection(pdata.palico->eyes);
    tail->SetSelection(pdata.palico->tail);
    ear->SetSelection(pdata.palico->ears);
    voice->SetSelection(pdata.palico->voice);
    cloth->SetSelection(pdata.palico->clothing);
    coat_col->SetColour(wxColor(pdata.palico->coat_rgb[0], pdata.palico->coat_rgb[1], pdata.palico->coat_rgb[2]));
    cloth_col->SetColour(wxColor(pdata.palico->cloth_rgb[0], pdata.palico->cloth_rgb[1], pdata.palico->cloth_rgb[2]));
}

void MainFrame::writePalico()
{
    if(!pdata.palico) return;

    std::vector<uint8_t> tmp;
    for(uint8_t i = 0; i < 3; i++)
    {
        tmp = convertStrToQuest(p_text[i]->GetValue());
        while(tmp.size() < pdata.palico->text.texts[i].size()) tmp.push_back(0x00);
        pdata.palico->text.texts[i] = tmp;

        pdata.palico->quest_skill[i] = palico_skill_table[p_quest[i]->GetSelection()];
    }

    PlusLib::validate8(std::string(p_id->GetValue().mb_str()), pdata.palico->id, 0, 255, "Palico ID");
    PlusLib::validate8(std::string(p_level->GetValue().mb_str()), pdata.palico->level, 0, 30, "Palico ID");

    pdata.palico->forte = p_forte->GetSelection();
    pdata.palico->signature_skill = palico_skill_table[p_sign->GetSelection()];
    pdata.palico->meownster_skill = palico_skill_table[p_meow->GetSelection()];
    pdata.palico->team_skill = p_team->GetSelection();
    pdata.palico->casting_skill = palico_skill_table[p_cast->GetSelection()];

    pdata.palico->coat = coat->GetSelection();
    pdata.palico->eyes = eye->GetSelection();
    pdata.palico->tail = tail->GetSelection();
    pdata.palico->ears = ear->GetSelection();
    pdata.palico->voice = voice->GetSelection();
    pdata.palico->clothing = cloth->GetSelection();

    pdata.palico->coat_rgb[0] = coat_col->GetColour().Red();
    pdata.palico->coat_rgb[1] = coat_col->GetColour().Green();
    pdata.palico->coat_rgb[2] = coat_col->GetColour().Blue();

    pdata.palico->cloth_rgb[0] = cloth_col->GetColour().Red();
    pdata.palico->cloth_rgb[1] = cloth_col->GetColour().Green();
    pdata.palico->cloth_rgb[2] = cloth_col->GetColour().Blue();
}

void MainFrame::OnBtnNewPalico(wxCommandEvent &event)
{
    if(pdata.palico && wxMessageBox(_T("A palico is already loaded. Continue ?"), _T("Confirm"), wxNO) == wxNO)
        return;

    m_paliconame_text->SetLabel(_T("No file loaded"));

    p_save->Disable();
    p_saveas->Disable();
    p_encrypt->Disable();
    p_encryptas->Disable();

    if(!pdata.load(wxString("data/dummy/dummy.otb", wxConvUTF8).wc_str()))
    {
        wxMessageBox(_T("Error"));
    }
    else
    {
        m_paliconame_text->SetLabel(_T("New Palico"));
        readPalico();
        p_saveas->Enable();
        p_encryptas->Enable();
    }
}

void MainFrame::OnBtnLoadPalico(wxCommandEvent &event)
{
    wxFileDialog openFileDialog(this, _("Open Palico File"), _T(""), _T(""), _T(""), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...


    m_paliconame_text->SetLabel(_T("No file loaded"));

    p_save->Disable();
    p_saveas->Disable();
    p_encrypt->Disable();
    p_encryptas->Disable();

    if(!pdata.load(openFileDialog.GetPath().wc_str()))
    {
        wxMessageBox(_T("Error : Loading failed"));
    }
    else
    {
        m_paliconame_text->SetLabel(_T("Loading ..."));
        readPalico();
        p_save->Enable();
        p_saveas->Enable();
        p_encrypt->Enable();
        p_encryptas->Enable();
        m_paliconame_text->SetLabel(openFileDialog.GetPath());
    }
}

void MainFrame::OnBtnSavePalico(wxCommandEvent &event)
{
    if(!pdata.palico) return;

    writePalico();
    readPalico();
    if(!pdata.save(m_paliconame_text->GetLabel().wc_str())) wxMessageBox(_T("Error : Saving failed"));
    else wxMessageBox(_T("File saved"));
}

void MainFrame::OnBtnSaveAsPalico(wxCommandEvent &event)
{
    if(!pdata.palico) return;

    wxFileDialog
        saveFileDialog(this, _T("Save Palico File"), _T(""), _T(""), _T(""), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    writePalico();
    readPalico();
    if(!pdata.save(saveFileDialog.GetPath().wc_str())) wxMessageBox(_T("Error : Saving failed"));
    else
    {
        m_paliconame_text->SetLabel(saveFileDialog.GetPath());
        wxMessageBox(_T("File saved"));
        p_save->Enable();
        p_saveas->Enable();
        p_encrypt->Enable();
        p_encryptas->Enable();
        m_paliconame_text->SetLabel(saveFileDialog.GetPath());
    }
}

void MainFrame::OnBtnEncryptNowPalico(wxCommandEvent &event)
{
    if(!pdata.palico) return;

    writePalico();
    readPalico();
    if(!pdata.encrypt(m_paliconame_text->GetLabel().wc_str(), p_encryptchoice->GetSelection())) wxMessageBox(_T("Error : Saving as an encrypted palico failed"));
    else
    {
        switch(p_encryptchoice->GetSelection())
        {
            case 0: wxMessageBox(_T("File saved and encrypted (JPN)")); break;
            case 1: wxMessageBox(_T("File saved and encrypted (US/EU)")); break;
            case 2: wxMessageBox(_T("File saved and encrypted (KOR)")); break;
            case 3: wxMessageBox(_T("File saved and encrypted (TWN)")); break;
            default: wxMessageBox(_T("File saved and encrypted (Unknown region)")); break;
        }
    }
}

void MainFrame::OnBtnEncryptAsPalico(wxCommandEvent &event)
{
    if(!pdata.palico) return;

    wxFileDialog
        saveFileDialog(this, _T("Encrypt Palico File"), _T(""), _T(""), _T(""), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    writePalico();
    readPalico();
    if(!pdata.encrypt(saveFileDialog.GetPath().wc_str(), p_encryptchoice->GetSelection())) wxMessageBox(_T("Error : Saving as an encrypted palico failed"));
    else
    {
        switch(p_encryptchoice->GetSelection())
        {
            case 0: wxMessageBox(_T("File saved and encrypted (JPN)")); break;
            case 1: wxMessageBox(_T("File saved and encrypted (US/EU)")); break;
            case 2: wxMessageBox(_T("File saved and encrypted (KOR)")); break;
            case 3: wxMessageBox(_T("File saved and encrypted (TWN)")); break;
            default: wxMessageBox(_T("File saved and encrypted (Unknown region)")); break;
        }
        p_save->Enable();
        p_saveas->Enable();
        p_encrypt->Enable();
        p_encryptas->Enable();
    }
}

void MainFrame::OnChoUpdatePartBreak(wxCommandEvent &event)
{
    if(!qdata.quest) return;
    updateQuestDetail(2);
}

void MainFrame::initSave()
{
    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Save editor (WIP)"), wxPoint(L_TEXT_X,L_TEXT_Y));

    s_load = new wxButton(sub_sizers[Q_SAVE], ID_BTN_LOAD_S, _T("Load"), wxPoint(0,25), wxSize(50, 23));
    s_encrypt = new wxButton(sub_sizers[Q_SAVE], ID_BTN_ENCRYPTNOW_S, _T("Save Encrypted"), wxPoint(50,25), wxSize(100, 23));
    s_encrypt->Disable();
    s_encryptas = new wxButton(sub_sizers[Q_SAVE], ID_BTN_ENCRYPTAS_S, _T("Save Encrypted As"), wxPoint(150,25), wxSize(100, 23));
    s_encryptas->Disable();
    s_save = new wxButton(sub_sizers[Q_SAVE], ID_BTN_SAVE_S, _T("Save"), wxPoint(250,25), wxSize(50, 23));
    s_save->Disable();
    s_saveas = new wxButton(sub_sizers[Q_SAVE], ID_BTN_SAVEAS_S, _T("Save As"), wxPoint(300,25), wxSize(50, 23));
    s_saveas->Disable();
    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Dev password"), wxPoint(260+L_TEXT_X,L_TEXT_Y));
    s_password = new wxTextCtrl(sub_sizers[Q_SAVE], -1, Dec2Str(iniFile.getStrData("DEV_PASSWORD", "GENERAL")), wxPoint(340, 0), wxSize(150, 23));

    s_savename_text = new wxStaticText(sub_sizers[Q_SAVE], -1, _T("No file loaded"), wxPoint(L_TEXT_X,50+L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Name"), wxPoint(L_TEXT_X,75+L_TEXT_Y));
    s_name = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T(""), wxPoint(75, 75), wxSize(150, 23));
    s_name->SetMaxLength(10);
    s_sex = new wxCheckBox(sub_sizers[Q_SAVE], -1, _T("is female"), wxPoint(250+L_TEXT_X, 75+L_TEXT_Y));
    new wxStaticLine(sub_sizers[Q_SAVE], -1, wxPoint(0,98), wxSize(500, 2));

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Item pouch"), wxPoint(L_TEXT_X,100+L_TEXT_Y));
    s_inv_page = new wxChoice(sub_sizers[Q_SAVE], ID_CHO_UPDATE_SAVE, wxPoint(75,100), wxSize(100, 23));
    s_inv_page->Freeze();
    s_inv_page->Append(_T("Page 1"));
    s_inv_page->Append(_T("Page 2"));
    s_inv_page->Append(_T("Page 3"));
    //s_inv_page->Append(_T("Gunner Pouch"));
    s_inv_page->Thaw();
    s_inv_page->SetSelection(0);

    for(uint32_t i = 0; i < 8; i++)
    {
        new wxStaticText(sub_sizers[Q_SAVE], -1, wxString(std::string("Item #"+PlusLib::int2str(i+1)).c_str(), wxConvUTF8), wxPoint(245*(i%2)+L_TEXT_X,25*(i/2)+125+L_TEXT_Y));
        s_inv_id[i] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(245*(i%2)+70,25*(i/2)+125), wxSize(130, 23));
        s_inv_id[i]->Freeze();
        s_inv_id[i]->Append(item_list);
        s_inv_id[i]->Thaw();
        s_inv_q[i] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("0"), wxPoint(245*(i%2)+200,25*(i/2)+125), wxSize(30, 23));
    }

    new wxStaticLine(sub_sizers[Q_SAVE], -1, wxPoint(0,223), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Item box"), wxPoint(L_TEXT_X,225+L_TEXT_Y));

    s_box_page = new wxChoice(sub_sizers[Q_SAVE], ID_CHO_UPDATE_SAVE, wxPoint(75,225), wxSize(100, 23));
    s_box_page->Freeze();
    for(uint32_t i = 0; i < 14; i++)
        s_box_page->Append(wxString(std::string("Page " + PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    s_box_page->Thaw();
    s_box_page->SetSelection(0);

    for(uint32_t i = 0; i < 100; i++)
    {
        new wxStaticText(sub_sizers[Q_SAVE], -1, wxString(std::string("Item #"+PlusLib::int2str(i+1)).c_str(), wxConvUTF8), wxPoint(245*(i%2)+L_TEXT_X,25*(i/2)+250+L_TEXT_Y));
        s_box_id[i] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(245*(i%2)+70,25*(i/2)+250), wxSize(130, 23));
        s_box_id[i]->Freeze();
        s_box_id[i]->Append(item_list);
        s_box_id[i]->Thaw();
        s_box_q[i] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("0"), wxPoint(245*(i%2)+200,25*(i/2)+250), wxSize(30, 23));
    }

    new wxStaticLine(sub_sizers[Q_SAVE], -1, wxPoint(0,1498), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Equipments"), wxPoint(L_TEXT_X,1500+L_TEXT_Y));

    s_equip_page = new wxChoice(sub_sizers[Q_SAVE], ID_CHO_UPDATE_SAVE, wxPoint(75,1500), wxSize(100, 23));
    s_equip_page->Freeze();
    for(uint32_t i = 0; i < 15; i++)
        s_equip_page->Append(wxString(std::string("Page " + PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    s_equip_page->Thaw();
    s_equip_page->SetSelection(0);

    for(uint32_t i = 0; i < 100; i++)
    {
        new wxStaticText(sub_sizers[Q_SAVE], -1, wxString(std::string("Slot #"+PlusLib::int2str(i+1)).c_str(), wxConvUTF8), wxPoint(245*(i%2)+L_TEXT_X,25*(i/2)+1525+L_TEXT_Y));
        s_equip[i] = new wxButton(sub_sizers[Q_SAVE], ID_BTN_EDIT_EQUIP, _T("Not loaded"), wxPoint(245*(i%2)+70,25*(i/2)+1525), wxSize(135, 23));
    }

    new wxStaticLine(sub_sizers[Q_SAVE], -1, wxPoint(0,2773), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Guild quest / Expedition editing"), wxPoint(L_TEXT_X,2775+L_TEXT_Y));
    new wxStaticLine(sub_sizers[Q_SAVE], -1, wxPoint(0,2798), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Current slot"), wxPoint(L_TEXT_X,2800+L_TEXT_Y));
    gq.choice = new wxChoice(sub_sizers[Q_SAVE], ID_CHO_UPDATE_GQ, wxPoint(75, 2801), wxSize(150, 23));
    gq.choice->Freeze();
    gq.choice->Append(_T("Guild quest 1"));
    gq.choice->Append(_T("Guild quest 2"));
    gq.choice->Append(_T("Guild quest 3"));
    gq.choice->Append(_T("Guild quest 4"));
    gq.choice->Append(_T("Guild quest 5"));
    gq.choice->Append(_T("Guild quest 6"));
    gq.choice->Append(_T("Guild quest 7"));
    gq.choice->Append(_T("Guild quest 8"));
    gq.choice->Append(_T("Guild quest 9"));
    gq.choice->Append(_T("Guild quest 10"));
    gq.choice->Append(_T("Expedition low rank"));
    gq.choice->Append(_T("Expedition high rank"));
    gq.choice->Append(_T("Expedition G rank"));
    gq.choice->Thaw();
    gq.current_choice = 0;
    //new wxButton(sub_sizers[Q_SAVE], -1, _T("Copy"), wxPoint(225,2800), wxSize(50, 23));
    //new wxButton(sub_sizers[Q_SAVE], -1, _T("Paste"), wxPoint(275,2800), wxSize(50, 23));

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Weapon bias"), wxPoint(L_TEXT_X,2825+L_TEXT_Y));
    gq.wbias = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(75, 2825), wxSize(150, 23));
    gq.wbias->Freeze();
    gq.wbias->Append(_T("SA/CB/IG"));
    gq.wbias->Append(_T("GS/LS"));
    gq.wbias->Append(_T("SnS/DS"));
    gq.wbias->Append(_T("Lance/GL"));
    gq.wbias->Append(_T("Hammer/HH"));
    gq.wbias->Append(_T("Bow/Bowgun"));
    gq.wbias->Thaw();

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Armor piece"), wxPoint(250+L_TEXT_X,2825+L_TEXT_Y));
    gq.abias = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(325, 2825), wxSize(150, 23));
    gq.abias->Freeze();
    gq.abias->Append(_T("Chest"));
    gq.abias->Append(_T("Arms"));
    gq.abias->Append(_T("Waist"));
    gq.abias->Append(_T("Legs"));
    gq.abias->Append(_T("Head"));
    gq.abias->Thaw();

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Armor serie"), wxPoint(L_TEXT_X,2850+L_TEXT_Y));
    gq.aserie = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(75, 2850), wxSize(150, 23));
    gq.aserie->Freeze();
    gq.aserie->Append(_T("Original A"));
    gq.aserie->Append(_T("Original B"));
    gq.aserie->Append(_T("Original C"));
    gq.aserie->Append(_T("Original D"));
    gq.aserie->Append(_T("Original E"));
    gq.aserie->Append(_T("Original F"));
    gq.aserie->Append(_T("Tri A"));
    gq.aserie->Append(_T("Tri B"));
    gq.aserie->Append(_T("Tri C"));
    gq.aserie->Append(_T("Tri D"));
    gq.aserie->Append(_T("Tri E"));
    gq.aserie->Append(_T("Tri F"));
    gq.aserie->Append(_T("Freedom A"));
    gq.aserie->Append(_T("Freedom B"));
    gq.aserie->Append(_T("Freedom C"));
    gq.aserie->Append(_T("Freedom D"));
    gq.aserie->Append(_T("Freedom E"));
    gq.aserie->Append(_T("Freedom F"));
    gq.aserie->Thaw();

    new wxStaticLine(sub_sizers[Q_SAVE], -1, wxPoint(0,2873), wxSize(500, 2));

    for(uint32_t i = 0; i < 5; i++)
    {
        switch(i)
        {
            case 0: new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Boss 1"), wxPoint(L_TEXT_X,50*i+2875+L_TEXT_Y)); break;
            case 1: new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Boss 2"), wxPoint(L_TEXT_X,50*i+2875+L_TEXT_Y)); break;
            case 2: new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Boss 3"), wxPoint(L_TEXT_X,50*i+2875+L_TEXT_Y)); break;
            case 3: new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Boss 4"), wxPoint(L_TEXT_X,50*i+2875+L_TEXT_Y)); break;
            case 4: new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Boss 5"), wxPoint(L_TEXT_X,50*i+2875+L_TEXT_Y)); break;
        }
        gq.boss_id[i] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(75, 50*i+2875), wxSize(150, 23));
        new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Spawn area"), wxPoint(225+L_TEXT_X,50*i+2875+L_TEXT_Y));
        gq.boss_spawn_area[i] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(300,50*i+2875), wxSize(50, 23));
        gq.boss_spawn_area[i]->Freeze();
        gq.boss_spawn_area[i]->Append(_T("Camp"));
        gq.boss_spawn_area[i]->Append(_T("1"));
        gq.boss_spawn_area[i]->Append(_T("2"));
        gq.boss_spawn_area[i]->Append(_T("3"));
        gq.boss_spawn_area[i]->Append(_T("4"));
        gq.boss_spawn_area[i]->Append(_T("5"));
        gq.boss_spawn_area[i]->Thaw();

        new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Other area"), wxPoint(350+L_TEXT_X,50*i+2875+L_TEXT_Y));
        gq.boss_other_area[i] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(425,50*i+2875), wxSize(50, 23));
        gq.boss_other_area[i]->Freeze();
        gq.boss_other_area[i]->Append(_T("Camp"));
        gq.boss_other_area[i]->Append(_T("1"));
        gq.boss_other_area[i]->Append(_T("2"));
        gq.boss_other_area[i]->Append(_T("3"));
        gq.boss_other_area[i]->Append(_T("4"));
        gq.boss_other_area[i]->Append(_T("5"));
        gq.boss_other_area[i]->Thaw();

        new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Sleep area"), wxPoint(L_TEXT_X,50*i+2900+L_TEXT_Y));
        gq.boss_sleep_area[i] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(75,50*i+2900), wxSize(50, 23));
        gq.boss_sleep_area[i]->Freeze();
        gq.boss_sleep_area[i]->Append(_T("Camp"));
        gq.boss_sleep_area[i]->Append(_T("1"));
        gq.boss_sleep_area[i]->Append(_T("2"));
        gq.boss_sleep_area[i]->Append(_T("3"));
        gq.boss_sleep_area[i]->Append(_T("4"));
        gq.boss_sleep_area[i]->Append(_T("5"));
        gq.boss_sleep_area[i]->Thaw();

        new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Spawn points"), wxPoint(125+L_TEXT_X,50*i+2900+L_TEXT_Y));
        for(uint16_t j = 0; j < 5; j++)
        {
            gq.boss_spawn_point[i][j] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(200+j*50,50*i+2900), wxSize(50, 23));
            gq.boss_spawn_point[i][j]->Freeze();
            gq.boss_spawn_point[i][j]->Append(_T("0"));
            gq.boss_spawn_point[i][j]->Append(_T("1"));
            gq.boss_spawn_point[i][j]->Append(_T("2"));
            gq.boss_spawn_point[i][j]->Append(_T("3"));
            gq.boss_spawn_point[i][j]->Thaw();
        }
        new wxStaticLine(sub_sizers[Q_SAVE], -1, wxPoint(0,50*i+2923), wxSize(500, 2));
    }

    for(uint32_t i = 0; i < 5; i++)
    {
        switch(i)
        {
            case 0: new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Area 1"), wxPoint(L_TEXT_X,25*i+3125+L_TEXT_Y)); break;
            case 1: new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Area 2"), wxPoint(L_TEXT_X,25*i+3125+L_TEXT_Y)); break;
            case 2: new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Area 3"), wxPoint(L_TEXT_X,25*i+3125+L_TEXT_Y)); break;
            case 3: new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Area 4"), wxPoint(L_TEXT_X,25*i+3125+L_TEXT_Y)); break;
            case 4: new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Area 5"), wxPoint(L_TEXT_X,25*i+3125+L_TEXT_Y)); break;
        }
        gq.area[i][0] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(50, 25*i+3125), wxSize(100, 23));
        gq.area[i][0]->Freeze();
        gq.area[i][0]->Append(_T("Camp"));
        gq.area[i][0]->Append(_T("Labyrinth"));
        gq.area[i][0]->Append(_T("Hill"));
        gq.area[i][0]->Append(_T("Ruins"));
        gq.area[i][0]->Append(_T("Ground"));
        gq.area[i][0]->Append(_T("Felyne habitat"));
        gq.area[i][0]->Append(_T("Vines"));
        gq.area[i][0]->Append(_T("Caves"));
        gq.area[i][0]->Append(_T("Columns"));
        gq.area[i][0]->Append(_T("Wagon"));
        gq.area[i][0]->Append(_T("Treasure"));
        gq.area[i][0]->Append(_T("Quicksand cave"));
        gq.area[i][0]->Append(_T("Sand area"));
        gq.area[i][0]->Thaw();
        gq.area[i][1] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(150, 25*i+3125), wxSize(100, 23));
        gq.area[i][1]->Freeze();
        gq.area[i][1]->Append(_T("None"));
        for(uint16_t j = 0; j < 8; j++)
            gq.area[i][1]->Append(wxString(std::string("Entry " + PlusLib::int2str(j+1)).c_str(), wxConvUTF8));
        gq.area[i][1]->Thaw();

        gq.area[i][2] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(250, 25*i+3125), wxSize(100, 23));
        gq.area[i][2]->Freeze();
        gq.area[i][2]->Append(_T("None"));
        for(uint16_t j = 0; j < 8; j++)
            gq.area[i][2]->Append(wxString(std::string("Exit " + PlusLib::int2str(j+1)).c_str(), wxConvUTF8));
        gq.area[i][2]->Thaw();

        gq.area[i][3] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(350, 25*i+3125), wxSize(125, 23));
        gq.area[i][3]->Freeze();
        gq.area[i][3]->Append(_T("None"));
        gq.area[i][3]->Append(_T("Ex.last area"));
        gq.area[i][3]->Append(_T("GQ.last area"));
        gq.area[i][3]->Thaw();
    }

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("???"), wxPoint(L_TEXT_X,3250+L_TEXT_Y));
    gq.unknown[0] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("0"), wxPoint(50, 3250), wxSize(25, 23));
    gq.unknown[1] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("0"), wxPoint(75, 3250), wxSize(25, 23));
    gq.unknown[2] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("0"), wxPoint(100, 3250), wxSize(25, 23));
    new wxStaticLine(sub_sizers[Q_SAVE], -1, wxPoint(0,3273), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Guild quest Only"), wxPoint(L_TEXT_X,3275+L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Creator"), wxPoint(L_TEXT_X,3300+L_TEXT_Y));
    gq.creator = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T(""), wxPoint(75, 3300), wxSize(125, 23));
    gq.creator->SetMaxLength(10);

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Initial level"), wxPoint(200+L_TEXT_X,3300+L_TEXT_Y));
    gq.init_lvl = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("0"), wxPoint(275, 3300), wxSize(50, 23));
    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Current level"), wxPoint(325+L_TEXT_X,3300+L_TEXT_Y));
    gq.lvl = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("0"), wxPoint(400, 3300), wxSize(50, 23));

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Unique ID 0x"), wxPoint(L_TEXT_X,3325+L_TEXT_Y));
    gq.id[0] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("00"), wxPoint(75, 3325), wxSize(25, 23));
    gq.id[1] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("00"), wxPoint(100, 3325), wxSize(25, 23));
    gq.id[2] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("00"), wxPoint(125, 3325), wxSize(25, 23));
    gq.id[3] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("00"), wxPoint(150, 3325), wxSize(25, 23));
    gq.id[4] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("00"), wxPoint(175, 3325), wxSize(25, 23));
    gq.id[5] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("00"), wxPoint(200, 3325), wxSize(25, 23));
    gq.id[6] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("00"), wxPoint(225, 3325), wxSize(25, 23));
    gq.id[7] = new wxTextCtrl(sub_sizers[Q_SAVE], -1, _T("00"), wxPoint(250, 3325), wxSize(25, 23));
    gq.rare = new wxCheckBox(sub_sizers[Q_SAVE], -1, _T("Rare"), wxPoint(300+L_TEXT_X, 3325), wxSize(50, 23));

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Boss 1 frenzy"), wxPoint(L_TEXT_X,3350+L_TEXT_Y));
    gq.boss_frenzy[0] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(100,3350), wxSize(100, 23));
    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Boss 2 frenzy"), wxPoint(250+L_TEXT_X,3350+L_TEXT_Y));
    gq.boss_frenzy[1] = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(350,3350), wxSize(100, 23));
    for(uint16_t i = 0; i < 2; i++)
    {
        gq.boss_frenzy[i]->Freeze();
        for(uint16_t j = 0; j < 17; j++)
            gq.boss_frenzy[i]->Append(wxString(PlusLib::int2str(j).c_str(), wxConvUTF8));
        gq.boss_frenzy[i]->Thaw();
    }

    new wxStaticLine(sub_sizers[Q_SAVE], -1, wxPoint(0,3373), wxSize(500, 2));
    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Expedition Only"), wxPoint(L_TEXT_X,3375+L_TEXT_Y));

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Has..."), wxPoint(L_TEXT_X,3400+L_TEXT_Y));
    gq.poogie = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(75,3400), wxSize(100, 23));
    gq.poogie->Freeze();
    gq.poogie->Append(_T("None"));
    gq.poogie->Append(_T("Felyne"));
    gq.poogie->Append(_T("Poogie"));
    gq.poogie->Thaw();

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Poogie ID"), wxPoint(250+L_TEXT_X,3400+L_TEXT_Y));
    gq.poogie_id = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(325,3400), wxSize(100, 23));
    gq.poogie_id->Freeze();
    for(uint16_t i = 0; i < 80; i++)
        gq.poogie_id->Append(wxString(PlusLib::int2str(i).c_str(), wxConvUTF8));
    gq.poogie_id->Thaw();

    new wxStaticText(sub_sizers[Q_SAVE], -1, _T("Felyne area"), wxPoint(L_TEXT_X,3425+L_TEXT_Y));
    gq.felyne_area = new wxChoice(sub_sizers[Q_SAVE], -1, wxPoint(75,3425), wxSize(100, 23));
    gq.felyne_area->Freeze();
    gq.felyne_area->Append(_T("None"));
    gq.felyne_area->Append(_T("1"));
    gq.felyne_area->Append(_T("2"));
    gq.felyne_area->Append(_T("3"));
    gq.felyne_area->Append(_T("4"));
    gq.felyne_area->Append(_T("5"));
    gq.felyne_area->Thaw();
    new wxStaticLine(sub_sizers[Q_SAVE], -1, wxPoint(0,3448), wxSize(500, 2));

    s_current_choice[0]=0;
    s_current_choice[1]=0;
    s_current_choice[2]=0;

    ((wxScrolledWindow*)(sub_sizers[Q_SAVE]))->SetVirtualSize(wxSize(450, 3475));
    ((wxScrolledWindow*)(sub_sizers[Q_SAVE]))->SetScrollRate(5, 5);
    ((wxScrolledWindow*)(sub_sizers[Q_SAVE]))->EnableScrolling(false, true);
}

void MainFrame::readSave()
{
    if(!sdata.savedata) return;

    wxString wxname;
    for(uint16_t i = 0; i < 10; i++)
        wxname += sdata.savedata->name[i];
    s_name->ChangeValue(wxname);
    s_name->SetMaxLength(10);

    s_sex->SetValue(sdata.savedata->sex);

    updateItemPouch();
    updateItemBox();
    updateEquipment();
    readGQ();
}

void MainFrame::updateItemPouch()
{
    if(s_current_choice[0] < 0 || s_current_choice[0] > 2) return;

    for(uint32_t i = 0; i < 8; i++)
    {
        s_inv_id[i]->SetSelection(i_item_table[sdata.savedata->inventory[8*s_current_choice[0]+i].item_id]);
        s_inv_q[i]->ChangeValue(wxString(PlusLib::int2str(sdata.savedata->inventory[8*s_current_choice[0]+i].count).c_str(), wxConvUTF8));
    }
}

void MainFrame::updateItemBox()
{
    if(s_current_choice[1] < 0 || s_current_choice[1] > 13) return;

    for(uint32_t i = 0; i < 100; i++)
    {
        s_box_id[i]->SetSelection(i_item_table[sdata.savedata->box[s_current_choice[1]][i].item_id]);
        s_box_q[i]->ChangeValue(wxString(PlusLib::int2str(sdata.savedata->box[s_current_choice[1]][i].count).c_str(), wxConvUTF8));
    }
}

void MainFrame::updateEquipment()
{
    if(s_current_choice[2] < 0 || s_current_choice[2] > 14) return;

    for(uint32_t i = 0; i < 100; i++)
    {
        if(sdata.savedata->equipment[s_current_choice[2]][i][0] == 0) s_equip[i]->SetLabel(_T("(None)"));
        else s_equip[i]->SetLabel(arena_list[sdata.savedata->equipment[s_current_choice[2]][i][0]-1][(sdata.savedata->equipment[s_current_choice[2]][i][3]<<8)+sdata.savedata->equipment[s_current_choice[2]][i][2]]);
    }
}

void MainFrame::OnChoUpdateGQ(wxCommandEvent &event)
{
    if(!sdata.savedata) return;
    writeGQ();
    gq.current_choice = gq.choice->GetSelection();
    readGQ();
}

void MainFrame::readGQ()
{
    if(gq.current_choice < 0 || gq.current_choice >= 13) return;
    bool expedition = false;
    if(gq.current_choice >= 10 && gq.current_choice <= 12) expedition = true;
    gq.choice->SetSelection(gq.current_choice);

    if(!expedition)
    {
        wxString wxname;
        for(uint16_t i = 0; i < 10; i++)
            wxname += sdata.savedata->gq[gq.current_choice].creator[i];
        gq.creator->ChangeValue(wxname);
        gq.creator->SetMaxLength(10);

        for(uint16_t i = 0; i < 8; i++)
        {
            if(sdata.savedata->gq[gq.current_choice].unique_ID[i] < 0x10) gq.id[i]->ChangeValue(wxString(std::string("0"+PlusLib::int2hex(sdata.savedata->gq[gq.current_choice].unique_ID[i])).c_str(), wxConvUTF8));
            else gq.id[i]->ChangeValue(wxString(PlusLib::int2hex(sdata.savedata->gq[gq.current_choice].unique_ID[i]).c_str(), wxConvUTF8));
        }

        gq.init_lvl->ChangeValue(wxString(PlusLib::int2str(sdata.savedata->gq[gq.current_choice].initial_lvl).c_str(), wxConvUTF8));
        gq.lvl->ChangeValue(wxString(PlusLib::int2str(sdata.savedata->gq[gq.current_choice].lvl).c_str(), wxConvUTF8));

        for(uint16_t i = 0; i < 3; i++)
            gq.unknown[i]->ChangeValue(wxString(PlusLib::int2str(sdata.savedata->gq[gq.current_choice].unknownd[i]).c_str(), wxConvUTF8));

        for(uint32_t i = 0; i < 5; i++)
        {
            gq.boss_id[i]->Freeze();
            gq.boss_id[i]->Clear();
            gq.boss_id[i]->Append(gq_boss_list);
            gq.boss_id[i]->Thaw();
            gq.boss_id[i]->SetSelection(i_gq_boss_table[sdata.savedata->gq[gq.current_choice].boss[i].boss_type]);
        }

        gq.boss_frenzy[0]->SetSelection(sdata.savedata->gq[gq.current_choice].boss_frenzy[0]);
        gq.boss_frenzy[1]->SetSelection(sdata.savedata->gq[gq.current_choice].boss_frenzy[1]);
        gq.rare->SetValue(sdata.savedata->gq[gq.current_choice].treasure);
    }
    else
    {
        gq.poogie->SetSelection(sdata.savedata->gq[gq.current_choice].unknownb[5]);
        gq.poogie_id->SetSelection(sdata.savedata->gq[gq.current_choice].unknownb[4]);
        if(sdata.savedata->gq[gq.current_choice].unknownb[3] == 0xFF) gq.felyne_area->SetSelection(0);
        else gq.felyne_area->SetSelection(sdata.savedata->gq[gq.current_choice].unknownb[3]);
        for(uint32_t i = 0; i < 5; i++)
        {
            gq.boss_id[i]->Freeze();
            gq.boss_id[i]->Clear();
            gq.boss_id[i]->Append(monster_list);
            gq.boss_id[i]->Thaw();
            gq.boss_id[i]->SetSelection(i_monster_table[sdata.savedata->gq[gq.current_choice].boss[i].boss_type]);
        }
    }

    for(uint16_t i = 0; i < 5; i++)
    {
        gq.area[i][0]->SetSelection(sdata.savedata->gq[gq.current_choice].area[i].area_map);
        gq.area[i][1]->SetSelection(sdata.savedata->gq[gq.current_choice].area[i].area_entrance);
        gq.area[i][2]->SetSelection(sdata.savedata->gq[gq.current_choice].area[i].area_exit);
        gq.area[i][3]->SetSelection(sdata.savedata->gq[gq.current_choice].area[i].area_end);

        gq.boss_spawn_area[i]->SetSelection(sdata.savedata->gq[gq.current_choice].boss[i].spawn_area);
        gq.boss_other_area[i]->SetSelection(sdata.savedata->gq[gq.current_choice].info[i].other_area);
        gq.boss_sleep_area[i]->SetSelection(sdata.savedata->gq[gq.current_choice].info[i].sleep_area);
        for(uint16_t j = 0; j < 5; j++)
            gq.boss_spawn_point[i][j]->SetSelection(sdata.savedata->gq[gq.current_choice].info[i].spawn_area[j]);

    }

    gq.wbias->SetSelection(sdata.savedata->gq[gq.current_choice].weapon_bias);
    gq.abias->SetSelection(sdata.savedata->gq[gq.current_choice].armor_piece);
    gq.aserie->SetSelection(sdata.savedata->gq[gq.current_choice].armor_serie);
}

void MainFrame::writeGQ()
{
    if(gq.current_choice < 0 || gq.current_choice >= 13) return;
    bool expedition = false;
    if(gq.current_choice >= 10 && gq.current_choice <= 12) expedition = true;

    if(!expedition)
    {
        std::vector<uint8_t> tmp;
        tmp = convertStrToQuest(gq.creator->GetValue());
        while(tmp.size() < 21) tmp.push_back(0x00);

        for(uint16_t i = 0; i < 20; i++)
            ((uint8_t*)(sdata.savedata->gq[gq.current_choice].creator))[i] = tmp[i];

        for(uint16_t i = 0; i < 8; i++)
            sdata.savedata->gq[gq.current_choice].unique_ID[i] = PlusLib::hex2int(std::string(gq.id[i]->GetValue().mb_str()));

        sdata.savedata->gq[gq.current_choice].initial_lvl = PlusLib::str2int(std::string(gq.init_lvl->GetValue().mb_str()));
        sdata.savedata->gq[gq.current_choice].lvl = PlusLib::str2int(std::string(gq.lvl->GetValue().mb_str()));

        for(uint16_t i = 0; i < 3; i++)
            sdata.savedata->gq[gq.current_choice].unknownd[i] = PlusLib::str2int(std::string(gq.unknown[i]->GetValue().mb_str()));

        for(uint32_t i = 0; i < 5; i++)
            sdata.savedata->gq[gq.current_choice].boss[i].boss_type = gq_boss_table[gq.boss_id[i]->GetSelection()];

        sdata.savedata->gq[gq.current_choice].boss_frenzy[0] = gq.boss_frenzy[0]->GetSelection();
        sdata.savedata->gq[gq.current_choice].boss_frenzy[1] = gq.boss_frenzy[1]->GetSelection();
        sdata.savedata->gq[gq.current_choice].treasure = gq.rare->GetValue();
    }
    else
    {
        sdata.savedata->gq[gq.current_choice].unknownb[5] = gq.poogie->GetSelection();
        sdata.savedata->gq[gq.current_choice].unknownb[4] = gq.poogie_id->GetSelection();
        if(gq.felyne_area->GetSelection() == 0) sdata.savedata->gq[gq.current_choice].unknownb[3] = 0xFF;
        else sdata.savedata->gq[gq.current_choice].unknownb[3] = gq.felyne_area->GetSelection();

        for(uint32_t i = 0; i < 5; i++)
            sdata.savedata->gq[gq.current_choice].boss[i].boss_type = monster_table[gq.boss_id[i]->GetSelection()];
    }

    for(uint16_t i = 0; i < 5; i++)
    {
        sdata.savedata->gq[gq.current_choice].area[i].area_map = gq.area[i][0]->GetSelection();
        sdata.savedata->gq[gq.current_choice].area[i].area_entrance = gq.area[i][1]->GetSelection();
        sdata.savedata->gq[gq.current_choice].area[i].area_exit = gq.area[i][2]->GetSelection();
        sdata.savedata->gq[gq.current_choice].area[i].area_end = gq.area[i][3]->GetSelection();

        sdata.savedata->gq[gq.current_choice].boss[i].spawn_area = gq.boss_spawn_area[i]->GetSelection();
        sdata.savedata->gq[gq.current_choice].info[i].other_area = gq.boss_other_area[i]->GetSelection();
        sdata.savedata->gq[gq.current_choice].info[i].sleep_area = gq.boss_sleep_area[i]->GetSelection();
        for(uint16_t j = 0; j < 5; j++)
            sdata.savedata->gq[gq.current_choice].info[i].spawn_area[j] = gq.boss_spawn_point[i][j]->GetSelection();
    }
    sdata.savedata->gq[gq.current_choice].weapon_bias = gq.wbias->GetSelection();
    sdata.savedata->gq[gq.current_choice].armor_piece = gq.abias->GetSelection();
    sdata.savedata->gq[gq.current_choice].armor_serie = gq.aserie->GetSelection();
}

void MainFrame::OnBtnEditEquip(wxCommandEvent &event)
{
    if(!sdata.savedata) return;
    if(s_current_choice[2] < 0 || s_current_choice[2] > 14) return;
    uint32_t slot;
    for(slot = 0; slot < 100; slot++)
        if(s_equip[slot] == event.GetEventObject()) break;
    if(slot == 100) return;

    EquipDialog eqDiag(NULL, wxID_ANY, wxString(std::string("Page " + PlusLib::int2str(1+s_current_choice[2]) + " - Slot " + PlusLib::int2str(1+slot)).c_str(), wxConvUTF8));

    eqDiag.read(sdata.savedata->equipment[s_current_choice[2]][slot]);
    if(eqDiag.ShowModal() == wxID_OK)
    {
        eqDiag.write();
        updateEquipment();
    }
}

void MainFrame::OnChoUpdateSave(wxCommandEvent &event)
{
    writeSave();
    s_current_choice[0] = s_inv_page->GetSelection();
    s_current_choice[1] = s_box_page->GetSelection();
    s_current_choice[2] = s_equip_page->GetSelection();
    readSave();
}

void MainFrame::writeSave()
{
    std::vector<uint8_t> tmp;
    tmp = convertStrToQuest(s_name->GetValue());
    while(tmp.size() < 21) tmp.push_back(0x00);

    for(uint16_t i = 0; i < 20; i++)
        ((uint8_t*)(sdata.savedata->name))[i] = tmp[i];

    sdata.savedata->sex = s_sex->GetValue();

    if(s_current_choice[0] >= 0 || s_current_choice[0] <= 2)
    {
        for(uint32_t i = 0; i < 8; i++)
        {
            sdata.savedata->inventory[8*s_current_choice[0]+i].item_id = item_table[s_inv_id[i]->GetSelection()];
            int32_t tmp = PlusLib::str2int(std::string(s_inv_q[i]->GetValue().mb_str()));
            if(tmp < 0) tmp = 0;
            else if(tmp > 99) tmp = 99;
            sdata.savedata->inventory[8*s_current_choice[0]+i].count = tmp % 0x100;
        }
    }

    if(s_current_choice[1] >= 0 || s_current_choice[1] <= 13)
    {
        for(uint32_t i = 0; i < 100; i++)
        {
            sdata.savedata->box[s_current_choice[1]][i].item_id = item_table[s_box_id[i]->GetSelection()];
            int32_t tmp = PlusLib::str2int(std::string(s_box_q[i]->GetValue().mb_str()));
            if(tmp < 0) tmp = 0;
            else if(tmp > 99) tmp = 99;
            sdata.savedata->box[s_current_choice[1]][i].count = tmp % 0x100;
        }
    }

    writeGQ();
}

void MainFrame::OnBtnLoadSave(wxCommandEvent &event)
{
    wxFileDialog openFileDialog(this, _("Open Save File"), _T(""), _T(""), _T(""), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...


    s_savename_text->SetLabel(_T("No file loaded"));

    s_save->Disable();
    s_saveas->Disable();
    s_encrypt->Disable();
    s_encryptas->Disable();

    if(!sdata.load(openFileDialog.GetPath().wc_str()))
    {
        wxMessageBox(_T("Error : Loading failed"));
    }
    else
    {
        s_savename_text->SetLabel(_T("Loading ..."));
        readSave();
        s_save->Enable();
        s_saveas->Enable();
        s_encrypt->Enable();
        s_encryptas->Enable();
        s_savename_text->SetLabel(openFileDialog.GetPath());
    }
}

// ?280g(9f
// 63 50 56 48 103 40 57 102
bool MainFrame::checkPass(wxString pwd)
{
    if(pwd.size() != 8) return false;
    if(pwd[0] != 63) return false;
    if(pwd[1] != 50) return false;
    if(pwd[2] != 56) return false;
    if(pwd[3] != 48) return false;
    if(pwd[4] != 103) return false;
    if(pwd[5] != 40) return false;
    if(pwd[6] != 57) return false;
    if(pwd[7] != 102) return false;
    return true;
}

void MainFrame::OnBtnSaveSave(wxCommandEvent &event)
{
    if(!sdata.savedata) return;

    if(!checkPass(s_password->GetValue()))
    {
        sdata.setProtection(true);
        wxMessageBox(_T("Invalid password. Only the name, the sex\nand the GQ/Expedition will be modified"));
    }
    else sdata.setProtection(false);

    writeSave();
    readSave();
    if(!sdata.save(s_savename_text->GetLabel().wc_str())) wxMessageBox(_T("Error : Saving failed"));
    else wxMessageBox(_T("File saved\nRemember: Save encrypted before using the file with the game.\nOtherwise, it won't work"));
}

void MainFrame::OnBtnSaveAsSave(wxCommandEvent &event)
{
    if(!sdata.savedata) return;

    if(!checkPass(s_password->GetValue()))
    {
        sdata.setProtection(true);
        wxMessageBox(_T("Invalid password. Only the name, the sex\nand the GQ/Expedition will be modified"));
    }
    else sdata.setProtection(false);

    wxFileDialog
        saveFileDialog(this, _T("Save Save File"), _T(""), _T(""), _T(""), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    writeSave();
    readSave();
    if(!sdata.save(saveFileDialog.GetPath().wc_str())) wxMessageBox(_T("Error : Saving failed"));
    else
    {
        s_savename_text->SetLabel(saveFileDialog.GetPath());
        wxMessageBox(_T("File saved\nRemember: Save encrypted before using the file with the game.\nOtherwise, it won't work"));
    }
}

void MainFrame::OnBtnEncryptNowSave(wxCommandEvent &event)
{
    if(!sdata.savedata) return;

    if(!checkPass(s_password->GetValue()))
    {
        sdata.setProtection(true);
        wxMessageBox(_T("Invalid password. Only the name, the sex\nand the GQ/Expedition will be modified"));
    }
    else sdata.setProtection(false);

    writeSave();
    readSave();
    if(sdata.encrypt(s_savename_text->GetLabel().wc_str())) wxMessageBox(_T("File saved and encrypted"));
}

void MainFrame::OnBtnEncryptAsSave(wxCommandEvent &event)
{
    if(!sdata.savedata) return;

    if(!checkPass(s_password->GetValue()))
    {
        sdata.setProtection(true);
        wxMessageBox(_T("Invalid password. Only the name, the sex\nand the GQ/Expedition will be modified"));
    }
    else sdata.setProtection(false);

    wxFileDialog
        saveFileDialog(this, _T("Encrypt Save File"), _T(""), _T(""), _T(""), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    writeSave();
    readSave();
    if(sdata.encrypt(saveFileDialog.GetPath().wc_str()))
    {
        s_savename_text->SetLabel(saveFileDialog.GetPath());
        wxMessageBox(_T("File saved and encrypted"));
    }
}
