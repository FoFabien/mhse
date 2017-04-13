#ifndef MYDIALOG_HPP
#define MYDIALOG_HPP

#include <wx/wx.h>

#define L_TEXT_X 10
#define L_TEXT_Y 3

struct EWeap
{
    wxTextCtrl *lvl, *kinsect_stat[8];
    wxChoice *flag, *rarity, *attack, *sharpness, *attribute_id, *attribute_value, *honing, *kinsect, *kinsect_lvl, *state;
    wxStaticText *text[19];
};

struct EArmor
{
    wxTextCtrl *lvl, *color, *color_set;
    wxChoice *flag, *rarity, *defense, *resistance, *state;
    wxStaticText *text[8];
};

struct ETali
{
    wxChoice *slot, *skill[2];
    wxTextCtrl *skill_value[2];
    wxStaticText *text[5];
};

struct DEquip
{
    wxChoice *type, *id, *deco[3];
    wxCheckBox *fixed[3];
    wxStaticText *text[5];
    EWeap w;
    EArmor a;
    ETali t;
};

class EquipDialog : public wxDialog
{
    public:
        EquipDialog(wxWindow* parent, wxWindowID id, const wxString& title);
        virtual ~EquipDialog();
        void onCancel(wxCommandEvent& pEvent);
        void onOk(wxCommandEvent& pEvent);
        void onCopy(wxCommandEvent& pEvent);
        void onPaste(wxCommandEvent& pEvent);
        void onUpdateType(wxCommandEvent& pEvent);
        void read(uint8_t e[28]);
        void write();
    private:
        void update();
        void showWidget(uint32_t id, bool show = true);
        void clearWidget();
        uint8_t* equip;
        DEquip widget;
};

#endif // MYDIALOG_HPP
