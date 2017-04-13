#ifndef MAINFRAME_H
#define MAINFRAME_H

// Le header wxWidgets de base
#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/statline.h>
#include <wx/clrpicker.h>

#include "main.hpp"
#include "mh4u_qe.hpp"
#include "mh4u_pe.hpp"
#include "mh4u_se.hpp"
#include "mh4u_crypto.hpp"
#include "inifile.hpp"

#define Q_QUEST_EMPTY   0
#define Q_QUEST_INFO    1
#define Q_QUEST_DETAIL  2
#define Q_BOSS_GROUP    3
#define Q_INTRUDER      4
#define Q_SMALL_GROUP   5
#define Q_SUPPLY_BOX    6
#define Q_REWARD_A      7
#define Q_REWARD_B      8
#define Q_REWARD_C      9
#define Q_CHALLENGE     10
#define Q_TEXT_ENGLISH  11
#define Q_TEXT_FRENCH   12
#define Q_TEXT_SPANISH  13
#define Q_TEXT_GERMAN   14
#define Q_TEXT_ITALIAN  15
#define Q_CREATE        16
#define Q_TEST          17
#define Q_TOOLS         18
#define Q_PALICO        19
#define Q_SAVE          20
#define Q_COUNT         21

#define S_TEXT 35

#define TEXT_SUB 1

enum
{
    ID_BTN_LOAD,
    ID_BTN_SAVE,
    ID_BTN_SAVEAS,
    ID_BTN_ENCRYPTNOW,
    ID_BTN_ENCRYPTAS,
    ID_CHO_MENU,
    ID_CHO_QUEST_TYPE,
    ID_CHO_BGROUP,
    ID_CHO_BOSS,
    ID_BTN_BOSS_HELP1,
    ID_BTN_BOSS_HELP2,
    ID_CHO_MGROUP,
    ID_CHO_MONSTER,
    ID_CHO_PRESET,
    ID_CHO_EQUIPEMENT,
    ID_CHO_INTRUDER,
    ID_BTN_BOSS_ADD,
    ID_BTN_BOSS_DEL,
    ID_BTN_INTRUDER_ADD,
    ID_BTN_INTRUDER_DEL,
    ID_BTN_MONSTER_ADD,
    ID_BTN_MONSTER_DEL,
    ID_BTN_DECRYPT,
    ID_BTN_ENCRYPT,
    ID_BTN_TOOL_IN,
    ID_BTN_TOOL_OUT,
    ID_BTN_ADD_BOSS_GROUP,
    ID_BTN_DEL_BOSS_GROUP,
    ID_BTN_BOSS_MOVE,
    ID_BTN_REWARD_A_HELP,
    ID_BTN_ADD_MONSTER_GROUP,
    ID_BTN_DEL_MONSTER_GROUP,
    ID_BTN_UNBIND_MONSTER_GROUP,
    ID_BTN_MONSTER_MOVE,
    ID_BTN_CPY_TEXT_TITLE,
    ID_BTN_CPY_TEXT_SUCCESS,
    ID_BTN_CPY_TEXT_FAILURE,
    ID_BTN_CPY_TEXT_SUMMARY,
    ID_BTN_CPY_TEXT_MAIN_MONSTERS,
    ID_BTN_CPY_TEXT_CLIENT,
    ID_BTN_CPY_TEXT_SUB_QUEST,
    ID_BTN_PASTE_TEXT_TITLE,
    ID_BTN_PASTE_TEXT_SUCCESS,
    ID_BTN_PASTE_TEXT_FAILURE,
    ID_BTN_PASTE_TEXT_SUMMARY,
    ID_BTN_PASTE_TEXT_MAIN_MONSTERS,
    ID_BTN_PASTE_TEXT_CLIENT,
    ID_BTN_PASTE_TEXT_SUB_QUEST,
    ID_BTN_CREATE,
    ID_BTN_UNBIND_REWARD,
    ID_BTN_ADD_RITEM,
    ID_BTN_DEL_RITEM,
    ID_BTN_ADD_REWARD,
    ID_BTN_TOOL_PROXY_FILES,
    ID_BTN_TOOL_GEN_FILES,
    ID_BTN_ADD_SUPPLY_GROUP,
    ID_BTN_ADD_SUPPLY_ITEM,
    ID_BTN_DEL_SUPPLY_GROUP,
    ID_BTN_DEL_SUPPLY_ITEM,
    ID_BTN_SUPPLY_HELP,
    ID_TEXT_SUPPLY_COUNT,
    ID_CHO_MONSTER_CONDITION,
    ID_CHO_BOSS_ID,
    ID_CHO_MONSTER_ID,
    ID_CHO_INTRUDER_ID,
    ID_BTN_PASTE_SUBGROUP,
    ID_BTN_CPY_SUBGROUP,
    ID_BTN_MONSTER_HELP,
    ID_BTN_NEW_P,
    ID_BTN_LOAD_P,
    ID_BTN_SAVE_P,
    ID_BTN_SAVEAS_P,
    ID_BTN_ENCRYPTNOW_P,
    ID_BTN_ENCRYPTAS_P,
    ID_COLOR_PICK,
    ID_TEXT_QUEST_ID,
    ID_BTN_PLACE_BOSS,
    ID_BTN_PLACE_MONSTER,
    ID_BTN_PLACE_INTRUDER,
    ID_CHO_QUEST_TARGET,
    ID_CHO_CREATE_OBJTYPE,
    ID_CHO_CREATE_OBJID,
    ID_BTN_GO_CREATE,
    ID_BTN_GO_NEW,
    ID_BTN_LOAD_S,
    ID_BTN_SAVE_S,
    ID_BTN_SAVEAS_S,
    ID_BTN_EDIT_EQUIP,
    ID_CHO_UPDATE_SAVE,
    ID_CHO_UPDATE_GQ,
    ID_BTN_ENCRYPTNOW_S,
    ID_BTN_ENCRYPTAS_S,
    ID_BTN_CHALLENGE_EQUIP,
    ID_ADD_PRESET,
    ID_DEL_PRESET,
};

struct Challenge_struct
{
    std::vector<wxStaticText*> texts;
    std::vector<wxChoice*> choices;
    std::vector<wxTextCtrl*> quantity;
    int32_t count;
};

struct Supply_struct
{
    std::vector<wxStaticText*> texts;
    std::vector<wxChoice*> choices;
    std::vector<wxTextCtrl*> quantity;
    uint32_t count;
    std::vector<wxStaticText*> boxName;
    std::vector<wxStaticText*> countText;
    std::vector<wxTextCtrl*> start;
    std::vector<wxTextCtrl*> boxCount;
    uint32_t nBox;
    wxStaticLine* line[2];
};

struct Reward_struct
{
    wxStaticText* duplicate;
    std::vector<wxStaticText*> texts;
    std::vector<wxChoice*> choices;
    std::vector<wxTextCtrl*> quantity;
    std::vector<wxTextCtrl*> proba;
    std::vector<wxStaticText*> percents;
    uint32_t count;
    uint32_t loaded;
    wxButton *divide;
    wxButton *add, *del, *add_list;
};

struct Boss_struct
{
    wxTextCtrl *q_boss_size_pc, *q_boss_size_var, *q_boss_health, *q_boss_attack, *q_boss_defense, *q_boss_stamina, *q_boss_spawn_count,
    *q_boss_spawn_area, *q_boss_cor_x, *q_boss_cor_y, *q_boss_cor_z, *q_boss_spawn_condition, *q_boss_size2;
    wxChoice *q_boss_id, *q_boss_size1, *q_boss_frenzy;
    wxButton *q_boss_help1, *q_boss_help2;
    wxTextCtrl *q_boss_angles[3];
};

struct Intruder_struct
{
    wxTextCtrl *q_size_pc, *q_size_var, *q_health, *q_attack, *q_defense, *q_stamina;
    wxTextCtrl *q_spawn_count,
    *q_spawn_area, *q_cor_x, *q_cor_y, *q_cor_z, *q_spawn_condition, *q_spawn_chance, *q_size2;
    wxChoice *q_id, *q_size1, *q_frenzy;
    wxTextCtrl *q_angles[3];
};

struct Monster_struct
{
    wxTextCtrl *q_monster_size_pc, *q_monster_size_var, *q_monster_health, *q_monster_attack, *q_monster_defense, *q_monster_stamina,
    *q_monster_spawn_count, *q_monster_spawn_condition, *q_monster_spawn_area, *q_monster_cor_x, *q_monster_cor_y, *q_monster_cor_z,
    *q_monster_size2;
    wxChoice *q_monster_id, *q_monster_size1;
    wxTextCtrl *q_crash;
    wxTextCtrl *q_monster_angles[3];
};

struct Submonster_struct
{
    std::vector<wxStaticText*> text;
    std::vector<wxButton*> del;
    std::vector<wxButton*> cpy;
    wxButton* paste;
    wxButton* add;
    wxButton* unbind;
    uint32_t count;
};

struct Quest_creator
{
    MH4U_QE data;
    wxTextCtrl *name, *objective_countB[3], *objective_idB[3];
    wxChoice *id, *objective_type[3], *objective_idA[3], *objective_countA[3], *map_id, *rank, *boss[5], *frenzy[5], *group[5], *region;
    wxCheckBox *tri_objective;
    int32_t q_objective[3], q_objid[3], q_objcount[3];
    int32_t preq_objective[3];
};

struct Guildquest_struct
{
    int32_t current_choice;
    wxTextCtrl *creator, *id[8], *init_lvl, *lvl, *unknown[3];
    wxChoice *choice, *wbias, *abias, *aserie,*boss_id[5],*boss_spawn_area[5],*boss_other_area[5],*boss_sleep_area[5],
        *boss_spawn_point[5][5],*boss_frenzy[2],*poogie,*poogie_id,*felyne_area, *area[5][4];
    wxCheckBox *rare;
};

class MainFrame : public wxFrame
{
    public:
        MainFrame(const wxString& title);
        virtual ~MainFrame();
        void OnDropFiles(wxString filename);
    private:
        MH4U_QE qdata;
        MH4U_PE pdata;
        IniFile iniFile;
        wxBoxSizer *sizer_vertical;
        wxButton *m_load, *m_save, *m_saveas, *m_encrypt, *m_encryptas;
        wxButton *p_new, *p_load, *p_save, *p_saveas, *p_encrypt, *p_encryptas;

        wxChoice *m_choice, *m_encryptchoice, *p_encryptchoice;
        wxStaticText *m_questname_text, *m_paliconame_text;
        wxPanel* sub_sizers[Q_COUNT];
        wxPanel* current_sub_sizer;
        int32_t current_sub_sizer_id;
        wxString path;

        // quest info
        wxCheckBox *q_flags_check[15];
        wxTextCtrl *q_quest_id, *q_hr_points, *q_hr_points_penalty, *q_sub_hr_points, *q_fee, *q_reward, *q_penalty,
                   *q_sub_reward, *q_time, *q_dialog_id;
        wxChoice *q_map, *q_stars, *q_critere1, *q_critere2, *q_carve_type, *q_player_spawn, *q_collect_type, *q_monster_ai;
        wxCheckBox *q_arena_gate, *q_arena_state;
        wxTextCtrl *q_grid_offset[2];
        // quest detail
        int32_t q_objective[3], q_objid[3], q_objcount[3];
        int32_t preq_objective[3];
        wxTextCtrl *q_quest_count1[3], *q_quest_id2[3];
        wxChoice *q_quest_type[3], *q_quest_id1[3], *q_quest_count2[3], *q_boss_icons[5];

        // supply box
        bool enable_modif;
        wxChoice *q_delivery_type[2], *q_delivery_box[2], *q_delivery_target[2];
        wxTextCtrl *q_delivery_count[2];
        Supply_struct q_supply;
        wxStaticText *q_delivery_text[2], *q_supply_text[4];
        wxButton *q_supply_boxes[2], *q_supply_items[2], *q_supply_info;
        // reward
        Reward_struct q_reward_box[3];
        wxButton *q_reward_A_help;
        // boss group
        Boss_struct q_boss;
        wxChoice *q_boss_choice, *q_bgroup_choice, *q_boss_choice_move;
        int32_t q_boss_count, q_current_boss;
        int32_t q_bgroup_count, q_current_bgroup;
        // intruders
        wxTextCtrl *q_intruder_proba, *q_intruder_timer;
        Intruder_struct q_intruder;
        wxChoice* q_intruder_choice;
        int32_t q_intruder_count, q_current_intruder;
        wxButton *q_intruder_add, *q_intruder_del;
        // small group
        Monster_struct q_monster;
        Submonster_struct q_submon[3];
        wxChoice* q_monster_choice, *q_mgroup_choice, *q_monster_choice_move;
        int32_t q_monster_count, q_current_monster;
        int32_t q_mgroup_count, q_current_mgroup;

        wxChoice *q_mon_condition_type[2], *q_mon_condition_group[2], *q_mon_condition_id[2];
        wxTextCtrl *q_mon_condition_count[2];

        Small_monster_group* cpy_subgroup;
        // text
        wxTextCtrl *q_text[S_TEXT];
        wxButton *q_text_copy[S_TEXT];
        wxButton *q_text_paste[S_TEXT];
        wxString textBuffer;
        // challenge
        Challenge_struct q_challenge_items;
        wxChoice *q_preset_choice, *q_equipment_choice;
        int32_t q_current_preset, q_current_equipment;
        wxButton *challenge_equip[7], *add_ch, *del_ch;
        // tools
        wxTextCtrl *q_tool_file_in, *q_tool_file_out, *q_tool_proxy_folder;
        wxChoice *q_tool_region, *q_tool_pregion;
        MH4U_Crypto mh4u_crypto;
        // create
        Quest_creator qc;
        // tests
        wxTextCtrl *q_spawn_timer, *q_unknown;
        wxTextCtrl *q_main_quest_count[4], *q_intruder_condition_test[4];


        // events
        void commonLoad(wxString name);

        void OnBtnLoad(wxCommandEvent &event);
        void OnBtnSave(wxCommandEvent &event);
        void OnBtnSaveAs(wxCommandEvent &event);
        void OnBtnEncryptNow(wxCommandEvent &event);
        void OnBtnEncryptAs(wxCommandEvent &event);
        void OnChoMenu(wxCommandEvent &event);
        void OnChoQuestType(wxCommandEvent &event);
        void OnChoBossGroup(wxCommandEvent &event);
        void OnChoBoss(wxCommandEvent &event);
        void OnBtnBossHelp1(wxCommandEvent &event);
        void OnBtnBossHelp2(wxCommandEvent &event);
        void OnChoMonsterGroup(wxCommandEvent &event);
        void OnChoMonster(wxCommandEvent &event);
        void OnChoPreset(wxCommandEvent &event);
        void OnChoEquipement(wxCommandEvent &event);
        void OnChoIntruder(wxCommandEvent &event);
        void OnBtnAddBoss(wxCommandEvent &event);
        void OnBtnDelBoss(wxCommandEvent &event);
        void OnBtnAddIntruder(wxCommandEvent &event);
        void OnBtnDelIntruder(wxCommandEvent &event);
        void OnBtnDecrypt(wxCommandEvent &event);
        void OnBtnEncrypt(wxCommandEvent &event);
        void OnBtnToolIn(wxCommandEvent &event);
        void OnBtnToolOut(wxCommandEvent &event);
        void OnBtnAddBossGroup(wxCommandEvent &event);
        void OnBtnDelBossGroup(wxCommandEvent &event);
        void OnBtnMoveBoss(wxCommandEvent &event);
        void OnBtnRewardAHelp(wxCommandEvent &event);
        void OnBtnAddMonster(wxCommandEvent &event);
        void OnBtnDelMonster(wxCommandEvent &event);
        void OnBtnAddMonsterGroup(wxCommandEvent &event);
        void OnBtnDelMonsterGroup(wxCommandEvent &event);
        void OnBtnUnbindMonsterGroup(wxCommandEvent &event);
        void OnBtnMoveMonster(wxCommandEvent &event);
        void OnBtnCreateQuest(wxCommandEvent &event);
        void OnBtnUnbindReward(wxCommandEvent &event);
        void OnBtnAddRItem(wxCommandEvent &event);
        void OnBtnDelRItem(wxCommandEvent &event);
        void OnBtnAddReward(wxCommandEvent &event);
        void OnBtnCopyTitle(wxCommandEvent &event);
        void OnBtnCopySuccess(wxCommandEvent &event);
        void OnBtnCopyFailure(wxCommandEvent &event);
        void OnBtnCopySummary(wxCommandEvent &event);
        void OnBtnCopyMainMonsters(wxCommandEvent &event);
        void OnBtnCopyClient(wxCommandEvent &event);
        void OnBtnCopySubquest(wxCommandEvent &event);
        void OnBtnPasteTitle(wxCommandEvent &event);
        void OnBtnPasteSuccess(wxCommandEvent &event);
        void OnBtnPasteFailure(wxCommandEvent &event);
        void OnBtnPasteSummary(wxCommandEvent &event);
        void OnBtnPasteMainMonsters(wxCommandEvent &event);
        void OnBtnPasteClient(wxCommandEvent &event);
        void OnBtnPasteSubquest(wxCommandEvent &event);
        void OnBtnProxyFiles(wxCommandEvent &event);
        void OnBtnGenerateProxyFiles(wxCommandEvent &event);
        void OnBtnAddSupplyGroup(wxCommandEvent &event);
        void OnBtnAddSupplyItem(wxCommandEvent &event);
        void OnBtnDelSupplyGroup(wxCommandEvent &event);
        void OnBtnDelSupplyItem(wxCommandEvent &event);
        void OnBtnSupplyHelp(wxCommandEvent &event);
        void OnTextSupplyCount(wxCommandEvent &event);
        void OnChoGroupConditionType(wxCommandEvent &event);
        void OnChoUpdateBossID(wxCommandEvent &event);
        void OnChoUpdateMonsterID(wxCommandEvent &event);
        void OnChoUpdateIntruderID(wxCommandEvent &event);
        void OnBtnCpyMonsterSubgroup(wxCommandEvent &event);
        void OnBtnPasteMonsterSubgroup(wxCommandEvent &event);
        void OnBtnHelpMonster(wxCommandEvent &event);
        void OnBtnNewPalico(wxCommandEvent &event);
        void OnBtnLoadPalico(wxCommandEvent &event);
        void OnBtnSavePalico(wxCommandEvent &event);
        void OnBtnSaveAsPalico(wxCommandEvent &event);
        void OnBtnEncryptNowPalico(wxCommandEvent &event);
        void OnBtnEncryptAsPalico(wxCommandEvent &event);
        void checkID();
        void OnTextQuestID(wxCommandEvent &event);
        XYZ generateMonsterPosition(int32_t boss_id, int32_t map_id, bool &result, uint8_t &result_area, uint16_t area_count, uint16_t forbid_area);
        XYZ placeMonster(int32_t boss_id, int32_t map_id, int32_t area_id, bool &result, bool try2force = true);
        void OnBtnPlaceBoss(wxCommandEvent &event);
        void OnBtnPlaceMonster(wxCommandEvent &event);
        void OnBtnPlaceIntruder(wxCommandEvent &event);
        void OnChoUpdatePartBreak(wxCommandEvent &event);
        void OnChoCreateObjType(wxCommandEvent &event);
        void OnChoCreateObjID(wxCommandEvent &event);
        void OnBtnGoCreate(wxCommandEvent &event);
        void OnBtnGoNew(wxCommandEvent &event);
        void OnBtnLoadSave(wxCommandEvent &event);
        void OnBtnSaveSave(wxCommandEvent &event);
        void OnBtnSaveAsSave(wxCommandEvent &event);
        void OnBtnEditEquip(wxCommandEvent &event);
        void OnChoUpdateSave(wxCommandEvent &event);
        void OnChoUpdateGQ(wxCommandEvent &event);
        void OnBtnEncryptNowSave(wxCommandEvent &event);
        void OnBtnEncryptAsSave(wxCommandEvent &event);
        void OnBtnEditChallengeEquip(wxCommandEvent &event);
        void OnBtnAddChallenge(wxCommandEvent &event);
        void OnBtnDelChallenge(wxCommandEvent &event);

        // functions
        void setSizer(int id);

        void initAll();
        void initTools();
        void initCreate();
        void updateCreate(int state);
        void readAll();
        void writeAll();
        void readOne(int id);
        void writeOne(int id);

        void initQuestInfo();
        void readQuestInfo();
        void writeQuestInfo();

        void initQuestDetail();
        void readQuestDetail();
        void writeQuestDetail();
        void updateQuestDetail(int state);

        void initSupplyBox();
        void readSupplyBox();
        void writeSupplyBox(bool warning = true);

        void initReward();
        void readReward(uint32_t r = 0);
        void writeReward(uint32_t r = 0);

        void initBossGroup();
        void readBossGroup();
        void writeBossGroup();
        void readBoss(int b);
        void writeBoss(int b);

        void initMonsterGroup();
        void UpdateConditionType();
        void readMonsterGroup();
        void writeMonsterGroup();
        void readMonster(int m);
        void writeMonster(int m);

        void initText();
        void readText(uint32_t r = 0);
        void writeText(uint32_t r = 0);

        void initChallenge();
        void readChallenge();
        void writeChallenge();
        void readChallengeEquipment();

        void initIntruder();
        void readIntruderAll();
        void writeIntruderAll();
        void readIntruder(int b);
        void writeIntruder(int b);

        void initTest();
        void readTest();
        void writeTest();

        // palico
        wxTextCtrl *p_text[3], *p_id, *p_level;
        wxChoice *p_forte, *p_quest[3], *p_sign, *p_meow, *p_team, *p_cast;
        wxChoice *coat, *eye, *tail, *ear, *cloth, *voice;
        wxColourPickerCtrl *coat_col, *cloth_col;
        void OnColorPick(wxColourPickerEvent &event);
        void initPalico();
        void readPalico();
        void writePalico();

        // save
        MH4U_SE sdata;
        wxStaticText *s_savename_text;
        wxButton *s_load, *s_save, *s_saveas, *s_equip[100], *s_encrypt, *s_encryptas;
        wxChoice *s_inv_page, *s_inv_id[8], *s_box_page, *s_box_id[100], *s_equip_page;
        wxTextCtrl *s_inv_q[8], *s_box_q[100], *s_name, *s_password;
        wxCheckBox *s_sex;
        Guildquest_struct gq;
        int32_t s_current_choice[3];
        void initSave();
        void readSave();
        void writeSave();
        void updateItemPouch();
        void updateItemBox();
        void updateEquipment();
        bool checkPass(wxString pwd);
        void readGQ();
        void writeGQ();

        // Déclaration de la table des évènements
        DECLARE_EVENT_TABLE()
};

#endif // MAINFRAME_H
