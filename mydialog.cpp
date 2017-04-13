#include "mydialog.hpp"
#include "main.hpp"
#include "pluslib.hpp"

// list
extern wxArrayString arena_list[26];

uint8_t eq_buffer[28];
bool cpyAvailable = false;

EquipDialog::EquipDialog(wxWindow* parent, wxWindowID id, const wxString& title)
    : wxDialog(parent, id, title, wxDefaultPosition, wxSize(500, 400))
{
    /*// Création du panel d'affichage
    wxPanel *panelAffichage = new wxPanel(this);*/

    // Création du wxBoxSizer vertical
    wxBoxSizer *sizer_vertical = new wxBoxSizer(wxVERTICAL);
    // Affectation du wxBoxSizer vertical à la fenêtre
    SetSizer(sizer_vertical);

    wxPanel *panel = new wxPanel(this);
    sizer_vertical->Add(panel, 10, wxEXPAND, 0);

    wxButton* cancelButton = new wxButton(panel, -1, _T("Cancel"), wxPoint(50,350), wxSize(50, 23));
    wxButton* okButton = new wxButton(panel, -1, _T("Ok"), wxPoint(0,350), wxSize(50, 23));
    wxButton* copyButton = new wxButton(panel, -1, _T("Copy"), wxPoint(100,350), wxSize(50, 23));
    wxButton* pasteButton = new wxButton(panel, -1, _T("Paste"), wxPoint(150,350), wxSize(50, 23));

    widget.text[0] = new wxStaticText(panel, -1, _T("Category"), wxPoint(L_TEXT_X, L_TEXT_Y));
    widget.type = new wxChoice(panel, -1, wxPoint(100, 0), wxSize(130, 23));
    widget.type->Freeze();
    widget.type->Append(arena_list[25]);
    widget.type->Thaw();
    widget.text[1] = new wxStaticText(panel, -1, _T("ID"), wxPoint(250+L_TEXT_X, L_TEXT_Y));
    widget.id = new wxChoice(panel, -1, wxPoint(350, 0), wxSize(130, 23));
    widget.id->Freeze();
    widget.id->Append(_T("None"));
    widget.id->Thaw();
    for(uint32_t i = 0; i < 3; i++)
    {
        widget.text[2+i] = new wxStaticText(panel, -1, wxString(std::string("Decoration "+PlusLib::int2str(i+1)).c_str(), wxConvUTF8), wxPoint(L_TEXT_X, i*25+25+L_TEXT_Y));
        widget.deco[i] = new wxChoice(panel, -1, wxPoint(100, 25*i+25), wxSize(130, 23));
        widget.deco[i]->Freeze();
        widget.deco[i]->Append(arena_list[24]);
        widget.deco[i]->Thaw();
        widget.fixed[i] = new wxCheckBox(panel, -1, _T("is fixed"), wxPoint(250+L_TEXT_X, 25*i+25+L_TEXT_Y));
    }
    // WEAPON ==========================================
    widget.w.text[0] = new wxStaticText(panel, -1, _T("Relic level"), wxPoint(L_TEXT_X, 100+L_TEXT_Y));
    widget.w.lvl = new wxTextCtrl(panel, -1, _T("0"), wxPoint(100, 100), wxSize(130, 23));
    widget.w.text[1] = new wxStaticText(panel, -1, _T("Relic state"), wxPoint(L_TEXT_X, 125+L_TEXT_Y));
    widget.w.state = new wxChoice(panel, -1, wxPoint(100, 125), wxSize(130, 23));
    widget.w.state->Freeze();
    widget.w.state->Append(_T("Clean"));
    widget.w.state->Append(_T("Rusted"));
    widget.w.state->Append(_T("Glow"));
    widget.w.state->Append(_T("Rusted/Glow"));
    widget.w.state->Append(_T("1 slot"));
    widget.w.state->Append(_T("1 slot/Rusted"));
    widget.w.state->Append(_T("1 slot/Glow"));
    widget.w.state->Append(_T("1 slot/Glow/Rusted"));
    widget.w.state->Append(_T("2 slot"));
    widget.w.state->Append(_T("2 slot/Rusted"));
    widget.w.state->Append(_T("2 slot/Glow"));
    widget.w.state->Append(_T("2 slot/Glow/Rusted"));
    widget.w.state->Append(_T("3 slot"));
    widget.w.state->Append(_T("3 slot/Rusted"));
    widget.w.state->Append(_T("3 slot/Glow"));
    widget.w.state->Append(_T("3 slot/Glow/Rusted"));
    widget.w.state->Thaw();
    widget.w.text[2] = new wxStaticText(panel, -1, _T("Relic flag"), wxPoint(250+L_TEXT_X, 100+L_TEXT_Y));
    widget.w.flag = new wxChoice(panel, -1, wxPoint(350, 100), wxSize(130, 23));
    widget.w.flag->Freeze();
    for(uint16_t i = 0; i < 5; i++)
        widget.w.flag->Append(wxString(PlusLib::int2str(i).c_str(), wxConvUTF8));
    widget.w.flag->Thaw();
    widget.w.text[3] = new wxStaticText(panel, -1, _T("Relic rarity"), wxPoint(250+L_TEXT_X, 125+L_TEXT_Y));
    widget.w.rarity = new wxChoice(panel, -1, wxPoint(350, 125), wxSize(130, 23));
    widget.w.rarity->Freeze();
    for(uint16_t i = 0; i < 10; i++)
        widget.w.rarity->Append(wxString(std::string("Rare "+PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    widget.w.rarity->Thaw();
    widget.w.text[4] = new wxStaticText(panel, -1, _T("Relic attack"), wxPoint(L_TEXT_X, 150+L_TEXT_Y));
    widget.w.attack = new wxChoice(panel, -1, wxPoint(100, 150), wxSize(130, 23));
    widget.w.attack->Freeze();
    for(uint16_t i = 0; i < 21; i++)
        widget.w.attack->Append(wxString(std::string("Value #"+PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    widget.w.attack->Thaw();
    widget.w.text[5] = new wxStaticText(panel, -1, _T("Relic sharpness"), wxPoint(250+L_TEXT_X, 150+L_TEXT_Y));
    widget.w.sharpness = new wxChoice(panel, -1, wxPoint(350, 150), wxSize(130, 23));
    widget.w.sharpness->Freeze();
    for(uint16_t i = 0; i < 0x16; i++)
        widget.w.sharpness->Append(wxString(std::string("Type #"+PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    widget.w.sharpness->Thaw();
    widget.w.text[6] = new wxStaticText(panel, -1, _T("Relic attribute"), wxPoint(L_TEXT_X, 175+L_TEXT_Y));
    widget.w.attribute_id = new wxChoice(panel, -1, wxPoint(100, 175), wxSize(130, 23));
    widget.w.attribute_id->Freeze();
    widget.w.attribute_id->Append(_T("(None)"));
    widget.w.attribute_id->Append(_T("Fire"));
    widget.w.attribute_id->Append(_T("Water"));
    widget.w.attribute_id->Append(_T("Thunder"));
    widget.w.attribute_id->Append(_T("Dragon"));
    widget.w.attribute_id->Append(_T("Ice"));
    widget.w.attribute_id->Append(_T("Poison"));
    widget.w.attribute_id->Append(_T("Paralysis"));
    widget.w.attribute_id->Append(_T("Sleep"));
    widget.w.attribute_id->Append(_T("Blast"));
    widget.w.attribute_id->Thaw();
    widget.w.text[7] = new wxStaticText(panel, -1, _T("Attribute value"), wxPoint(250+L_TEXT_X, 175+L_TEXT_Y));
    widget.w.attribute_value = new wxChoice(panel, -1, wxPoint(350, 175), wxSize(130, 23));
    widget.w.attribute_value->Freeze();
    for(uint16_t i = 0; i < 0x12; i++)
        widget.w.attribute_value->Append(wxString(std::string("Value #"+PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    widget.w.attribute_value->Thaw();
    widget.w.text[8] = new wxStaticText(panel, -1, _T("Honing"), wxPoint(L_TEXT_X, 200+L_TEXT_Y));
    widget.w.honing = new wxChoice(panel, -1, wxPoint(100, 200), wxSize(130, 23));
    widget.w.honing->Freeze();
    widget.w.honing->Append(_T("(None)"));
    widget.w.honing->Append(_T("Attack"));
    widget.w.honing->Append(_T("Defense"));
    widget.w.honing->Append(_T("Health"));
    widget.w.honing->Thaw();
    widget.w.text[9] = new wxStaticText(panel, -1, _T("Kinsect"), wxPoint(L_TEXT_X, 225+L_TEXT_Y));
    widget.w.kinsect = new wxChoice(panel, -1, wxPoint(100, 225), wxSize(130, 23));
    widget.w.kinsect->Freeze();
    widget.w.kinsect->Append(arena_list[20]);
    widget.w.kinsect->Thaw();
    widget.w.text[10] = new wxStaticText(panel, -1, _T("Kinsect level"), wxPoint(250+L_TEXT_X, 225+L_TEXT_Y));
    widget.w.kinsect_lvl = new wxChoice(panel, -1, wxPoint(350, 225), wxSize(130, 23));
    widget.w.kinsect_lvl->Freeze();
    for(uint16_t i = 0; i < 12; i++)
        widget.w.kinsect_lvl->Append(wxString(PlusLib::int2str(i+1).c_str(), wxConvUTF8));
    widget.w.kinsect_lvl->Thaw();
    widget.w.text[11] = new wxStaticText(panel, -1, _T("Kinsect Power"), wxPoint(L_TEXT_X, 250+L_TEXT_Y));
    widget.w.text[12] = new wxStaticText(panel, -1, _T("Kinsect Stamina"), wxPoint(250+L_TEXT_X, 250+L_TEXT_Y));
    widget.w.text[13] = new wxStaticText(panel, -1, _T("Kinsect Speed"), wxPoint(L_TEXT_X, 275+L_TEXT_Y));
    widget.w.text[14] = new wxStaticText(panel, -1, _T("Kinsect Fire"), wxPoint(250+L_TEXT_X, 275+L_TEXT_Y));
    widget.w.text[15] = new wxStaticText(panel, -1, _T("Kinsect Water"), wxPoint(L_TEXT_X, 300+L_TEXT_Y));
    widget.w.text[16] = new wxStaticText(panel, -1, _T("Kinsect Thunder"), wxPoint(250+L_TEXT_X, 300+L_TEXT_Y));
    widget.w.text[17] = new wxStaticText(panel, -1, _T("Kinsect Ice"), wxPoint(L_TEXT_X, 325+L_TEXT_Y));
    widget.w.text[18] = new wxStaticText(panel, -1, _T("Kinsect Dragon"), wxPoint(250+L_TEXT_X, 325+L_TEXT_Y));
    for(uint32_t i = 0; i < 8; i++)
    {
        widget.w.kinsect_stat[i] = new wxTextCtrl(panel, -1, _T("0"), wxPoint(100+250*(i%2), 250+25*(i/2)), wxSize(130, 23));
    }
    showWidget(0, false);

    // ARMOR ==========================================
    widget.a.text[0] = new wxStaticText(panel, -1, _T("Level"), wxPoint(L_TEXT_X, 100+L_TEXT_Y));
    widget.a.lvl = new wxTextCtrl(panel, -1, _T("0"), wxPoint(100, 100), wxSize(130, 23));
    widget.a.text[1] = new wxStaticText(panel, -1, _T("Relic state"), wxPoint(L_TEXT_X, 125+L_TEXT_Y));
    widget.a.state = new wxChoice(panel, -1, wxPoint(100, 125), wxSize(130, 23));
    widget.a.state->Freeze();
    widget.a.state->Append(_T("Clean"));
    widget.a.state->Append(_T("Rusted"));
    widget.a.state->Append(_T("Glow"));
    widget.a.state->Append(_T("Rusted/Glow"));
    widget.a.state->Append(_T("1 slot"));
    widget.a.state->Append(_T("1 slot/Rusted"));
    widget.a.state->Append(_T("1 slot/Glow"));
    widget.a.state->Append(_T("1 slot/Glow/Rusted"));
    widget.a.state->Append(_T("2 slot"));
    widget.a.state->Append(_T("2 slot/Rusted"));
    widget.a.state->Append(_T("2 slot/Glow"));
    widget.a.state->Append(_T("2 slot/Glow/Rusted"));
    widget.a.state->Append(_T("3 slot"));
    widget.a.state->Append(_T("3 slot/Rusted"));
    widget.a.state->Append(_T("3 slot/Glow"));
    widget.a.state->Append(_T("3 slot/Glow/Rusted"));
    widget.a.state->Thaw();
    widget.a.text[2] = new wxStaticText(panel, -1, _T("Relic flag"), wxPoint(250+L_TEXT_X, 100+L_TEXT_Y));
    widget.a.flag = new wxChoice(panel, -1, wxPoint(350, 100), wxSize(130, 23));
    widget.a.flag->Freeze();
    for(uint16_t i = 0; i < 5; i++)
        widget.a.flag->Append(wxString(PlusLib::int2str(i).c_str(), wxConvUTF8));
    widget.a.flag->Thaw();
    widget.a.text[3] = new wxStaticText(panel, -1, _T("Relic rarity"), wxPoint(250+L_TEXT_X, 125+L_TEXT_Y));
    widget.a.rarity = new wxChoice(panel, -1, wxPoint(350, 125), wxSize(130, 23));
    widget.a.rarity->Freeze();
    for(uint16_t i = 0; i < 10; i++)
        widget.a.rarity->Append(wxString(std::string("Rare "+PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    widget.a.rarity->Thaw();
    widget.a.text[4] = new wxStaticText(panel, -1, _T("Relic defense"), wxPoint(L_TEXT_X, 150+L_TEXT_Y));
    widget.a.defense = new wxChoice(panel, -1, wxPoint(100, 150), wxSize(130, 23));
    widget.a.defense->Freeze();
    widget.a.defense->Append(_T("10 (Gunner)"));
    widget.a.defense->Append(_T("12 (Gunner)"));
    widget.a.defense->Append(_T("16 (Gunner)"));
    widget.a.defense->Append(_T("18 (Gunner)"));
    widget.a.defense->Append(_T("20 (Gunner)"));
    widget.a.defense->Append(_T("22 (Gunner)"));
    widget.a.defense->Append(_T("33 (Gunner)"));
    widget.a.defense->Append(_T("36 (Gunner)"));
    widget.a.defense->Append(_T("39 (Gunner)"));
    widget.a.defense->Append(_T("20 (Blade.)"));
    widget.a.defense->Append(_T("24 (Blade.)"));
    widget.a.defense->Append(_T("30 (Blade.)"));
    widget.a.defense->Append(_T("34 (Blade.)"));
    widget.a.defense->Append(_T("40 (Blade.)"));
    widget.a.defense->Append(_T("45 (Blade.)"));
    widget.a.defense->Append(_T("65 (Blade.)"));
    widget.a.defense->Append(_T("72 (Blade.)"));
    widget.a.defense->Append(_T("78 (Blade.)"));
    widget.a.defense->Append(_T("46 (G Gunner)"));
    widget.a.defense->Append(_T("50 (G Gunner)"));
    widget.a.defense->Append(_T("54 (G Gunner)"));
    widget.a.defense->Append(_T("64 (G Gunner)"));
    widget.a.defense->Append(_T("72 (G Gunner)"));
    widget.a.defense->Append(_T("86 (G Blade.)"));
    widget.a.defense->Append(_T("92 (G Blade.)"));
    widget.a.defense->Append(_T("98 (G Blade.)"));
    widget.a.defense->Append(_T("112 (G Blade.)"));
    widget.a.defense->Append(_T("120 (G Blade.)  "));
    widget.a.defense->Thaw();
    widget.a.text[5] = new wxStaticText(panel, -1, _T("Relic resistance"), wxPoint(250+L_TEXT_X, 150+L_TEXT_Y));
    widget.a.resistance = new wxChoice(panel, -1, wxPoint(350, 150), wxSize(130, 23));
    widget.a.resistance->Freeze();
    for(uint16_t i = 0; i < 0x14; i++)
        widget.a.resistance->Append(wxString(std::string("Gunner #"+PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    for(uint16_t i = 0; i < 0x14; i++)
        widget.a.resistance->Append(wxString(std::string("Blade. #"+PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
    widget.a.resistance->Thaw();
    widget.a.text[6] = new wxStaticText(panel, -1, _T("Color"), wxPoint(L_TEXT_X, 175+L_TEXT_Y));
    widget.a.color = new wxTextCtrl(panel, -1, _T("0"), wxPoint(100, 175), wxSize(130, 23));
    widget.a.text[7] = new wxStaticText(panel, -1, _T("Color setting"), wxPoint(250+L_TEXT_X, 175+L_TEXT_Y));
    widget.a.color_set = new wxTextCtrl(panel, -1, _T("0"), wxPoint(350, 175), wxSize(130, 23));
    showWidget(1, false);

    // TALISMAN ==========================================
    widget.t.text[0] = new wxStaticText(panel, -1, _T("Slot"), wxPoint(L_TEXT_X, 100+L_TEXT_Y));
    widget.t.slot = new wxChoice(panel, -1, wxPoint(100, 100), wxSize(130, 23));
    widget.t.slot->Freeze();
    widget.t.slot->Append(_T("0"));
    widget.t.slot->Append(_T("1"));
    widget.t.slot->Append(_T("2"));
    widget.t.slot->Append(_T("3"));
    widget.t.slot->Thaw();

    widget.t.text[1] = new wxStaticText(panel, -1, _T("Skill 1"), wxPoint(L_TEXT_X, 125+L_TEXT_Y));
    widget.t.text[2] = new wxStaticText(panel, -1, _T("Value"), wxPoint(250+L_TEXT_X, 125+L_TEXT_Y));
    widget.t.text[3] = new wxStaticText(panel, -1, _T("Skill 2"), wxPoint(L_TEXT_X, 150+L_TEXT_Y));
    widget.t.text[4] = new wxStaticText(panel, -1, _T("Value"), wxPoint(250+L_TEXT_X, 150+L_TEXT_Y));
    for(uint32_t i = 0; i < 2; i++)
    {
        widget.t.skill[i] = new wxChoice(panel, -1, wxPoint(100, 125+25*i), wxSize(130, 23));
        widget.t.skill[i]->Freeze();
        widget.t.skill[i]->Append(arena_list[21]);
        widget.t.skill[i]->Thaw();
        widget.t.skill_value[i] = new wxTextCtrl(panel, -1, _T("0"), wxPoint(350, 125+25*i), wxSize(130, 23));
    }
    showWidget(2, false);
    equip = NULL;

    // Bind the widgets and event handlers.
    Connect(cancelButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EquipDialog::onCancel));
    Connect(okButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EquipDialog::onOk));
    Connect(copyButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EquipDialog::onCopy));
    Connect(pasteButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EquipDialog::onPaste));
    Connect(widget.type->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(EquipDialog::onUpdateType));

    Center();
}

EquipDialog::~EquipDialog() {}

void EquipDialog::onCancel(wxCommandEvent& WXUNUSED(pEvent))
{
    EndModal(wxID_CANCEL);
    Destroy();
}

void EquipDialog::onOk(wxCommandEvent& WXUNUSED(pEvent))
{
    EndModal(wxID_OK);
    Destroy();
}

void EquipDialog::onCopy(wxCommandEvent& WXUNUSED(pEvent))
{
    if(!equip) return;
    uint8_t tmp[28];
    for(uint32_t i = 0; i < 28; i++) tmp[i] = equip[i];
    write();
    for(uint32_t i = 0; i < 28; i++)
    {
        eq_buffer[i] = equip[i];
        equip[i] = tmp[i];
    }
    cpyAvailable = true;
}

void EquipDialog::onPaste(wxCommandEvent& pEvent)
{
    if(!equip) return;
    if(!cpyAvailable)
    {
        wxMessageBox(_T("Error: Nothing in the buffer"));
        return;
    }
    for(uint32_t i = 0; i < 28; i++) equip[i] = eq_buffer[i];
    update();
}

void EquipDialog::onUpdateType(wxCommandEvent& pEvent)
{
    if(!equip) return;
    write();
    update();
}

void EquipDialog::read(uint8_t e[28])
{
    clearWidget();
    equip = e;
    update();
}

void EquipDialog::write()
{
    if(!equip) return;
    for(uint32_t i = 0; i < 28; i++) equip[i] = 0;
    if(widget.type->GetSelection() < 1 || widget.type->GetSelection() > 0x14) return;
    else equip[0] = widget.type->GetSelection();

    for(uint32_t i = 0; i < 3; i++)
    {
        equip[6+i*2] = widget.deco[i]->GetSelection() % 0x100;
        equip[7+i*2] = ((widget.fixed[i]->GetValue()*8) << 4) + (widget.deco[i]->GetSelection() / 0x100);
    }

    equip[2] = widget.id->GetSelection() % 0x100;
    equip[3] = widget.id->GetSelection() / 0x100;

    if(widget.type->GetSelection() == 6)
    {
        for(uint32_t i = 0; i < 2; i++)
        {
            equip[12+i*4] = widget.t.skill[i]->GetSelection();
            equip[14+i*4] = PlusLib::str2int(std::string(widget.t.skill_value[i]->GetValue().mb_str()));
        }
        equip[1] = widget.t.slot->GetSelection();
    }
    else if(widget.type->GetSelection() < 6)
    {
        equip[1] = PlusLib::str2int(std::string(widget.a.lvl->GetValue().mb_str()));
        equip[16] = widget.a.state->GetSelection();
        equip[12] = widget.a.resistance->GetSelection();

        uint16_t tmp_value = PlusLib::str2int(std::string(widget.a.color->GetValue().mb_str()));
        equip[4] = tmp_value % 0x100;
        equip[5] = tmp_value / 0x100;
        equip[19] = PlusLib::str2int(std::string(widget.a.color_set->GetValue().mb_str()));

        equip[18] = widget.a.flag->GetSelection();
        equip[17] = widget.a.rarity->GetSelection();
        equip[13] = widget.a.defense->GetSelection();
    }
    else
    {
        equip[14] = PlusLib::str2int(std::string(widget.w.lvl->GetValue().mb_str()));
        equip[16] = widget.w.state->GetSelection();

        for(uint16_t i = 0; i < 8; i++)
            equip[20+i] = PlusLib::str2int(std::string(widget.w.kinsect_stat[i]->GetValue().mb_str()));

        equip[18] = widget.w.flag->GetSelection();
        equip[17] = widget.w.rarity->GetSelection();
        equip[13] = widget.w.attack->GetSelection();
        equip[12] = widget.w.sharpness->GetSelection();
        equip[5] = widget.w.attribute_id->GetSelection();
        equip[4] = widget.w.attribute_value->GetSelection();
        equip[15] = widget.w.kinsect->GetSelection();
        if(equip[0] == 0x0b)
        {
            switch(widget.w.kinsect_lvl->GetSelection())
            {
                case 0: equip[1] = 0; break;
                case 1: equip[1] = 0x08; break;
                case 2: equip[1] = 0x10; break;
                case 3: equip[1] = 0x18; break;
                case 4: equip[1] = 0x20; break;
                case 5: equip[1] = 0x28; break;
                case 6: equip[1] = 0x40; break;
                case 7: equip[1] = 0x48; break;
                case 8: equip[1] = 0x50; break;
                case 9: equip[1] = 0x58; break;
                case 10: equip[1] = 0x60; break;
                case 11: equip[1] = 0x68; break;
            }
        }
        else if(equip[0] == 0x0c)
        {
            switch(widget.w.kinsect_lvl->GetSelection())
            {
                case 0: equip[1] = 0; break;
                case 1: equip[1] = 0x08; break;
                case 2: equip[1] = 0x20; break;
                case 3: equip[1] = 0x28; break;
                case 4: equip[1] = 0x40; break;
                case 5: equip[1] = 0x48; break;
                case 6: equip[1] = 0x80; break;
                case 7: equip[1] = 0x88; break;
                case 8: equip[1] = 0x60; break;
                case 9: equip[1] = 0x68; break;
                case 10: equip[1] = 0xC0; break;
                case 11: equip[1] = 0xC8; break;
            }
        }
        else equip[1] = widget.w.kinsect_lvl->GetSelection();

        switch(widget.w.honing->GetSelection())
        {
            case 0: equip[19] = 0x00; break;
            case 1: equip[19] = 0x40; break;
            case 2: equip[19] = 0x80; break;
            case 3: equip[19] = 0xC0; break;
        }
    }
}

/*0x0C: Reload and Recoil (slightly affects Loading)
0x0D: Attack, Affinity, Defense, Deviation (doesn't affect Loading)
0x0F: Rapid/Crouching Fire (greatly affects Loading)
*/

void EquipDialog::update()
{
    widget.type->SetSelection(equip[0]);
    for(uint32_t i = 0; i < 3; i++)
    {
        widget.deco[i]->SetSelection(equip[6+i*2]+((equip[7+i*2]%0x10)<<8));
        widget.fixed[i]->SetValue((equip[7+i*2]/0x10) == 8);
    }
    if(equip[0] < 1 || equip[0] > 0x14)
    {
        widget.id->Freeze();
        widget.id->Clear();
        widget.id->Append(_T("None"));
        widget.id->Thaw();
        widget.id->SetSelection(0);
        showWidget(0, false);
        showWidget(1, false);
        showWidget(2, false);
    }
    else
    {
        widget.id->Freeze();
        widget.id->Clear();
        widget.id->Append(arena_list[equip[0]-1]);
        widget.id->Thaw();
        widget.id->SetSelection(equip[2]+(equip[3]<<8));

        if(equip[0] == 6)
        {
            showWidget(0, false);
            showWidget(1, false);
            showWidget(2);
            for(uint32_t i = 0; i < 2; i++)
            {
                widget.t.skill[i]->SetSelection(equip[12+i*4]);
                widget.t.skill_value[i]->ChangeValue(wxString(PlusLib::int2str((int8_t)equip[14+i*4]).c_str(), wxConvUTF8));
            }
            widget.t.slot->SetSelection(equip[1]);
        }
        else if(equip[0] < 6)
        {
            showWidget(0, false);
            showWidget(1);
            showWidget(2, false);

            widget.a.lvl->ChangeValue(wxString(PlusLib::int2str(equip[1]).c_str(), wxConvUTF8));
            widget.a.state->SetSelection(equip[16]);
            widget.a.resistance->SetSelection(equip[12]);
            widget.a.color->ChangeValue(wxString(PlusLib::int2str(equip[4]+(equip[5]<<8)).c_str(), wxConvUTF8));
            widget.a.color_set->ChangeValue(wxString(PlusLib::int2str(equip[19]).c_str(), wxConvUTF8));

            widget.a.flag->SetSelection(equip[18]);
            widget.a.rarity->SetSelection(equip[17]);
            widget.a.defense->SetSelection(equip[13]);
        }
        else
        {
            showWidget(0);
            showWidget(1, false);
            showWidget(2, false);

            widget.w.lvl->ChangeValue(wxString(PlusLib::int2str(equip[14]).c_str(), wxConvUTF8));
            widget.w.state->SetSelection(equip[16]);
            for(uint16_t i = 0; i < 8; i++)
                widget.w.kinsect_stat[i]->ChangeValue(wxString(PlusLib::int2str(equip[20+i]).c_str(), wxConvUTF8));
            widget.w.flag->SetSelection(equip[18]);
            widget.w.rarity->SetSelection(equip[17]);
            widget.w.attack->SetSelection(equip[13]);
            widget.w.attribute_id->SetSelection(equip[5]);
            widget.w.attribute_value->SetSelection(equip[4]);

            switch(equip[0])
            {
                case 0x0b: case 0x0c:
                    widget.w.text[5]->SetLabel(_T("Reload/Recoil"));
                    widget.w.sharpness->Freeze();
                    widget.w.sharpness->Clear();
                    widget.w.sharpness->Append(_T("Slow/High")); break;
                    widget.w.sharpness->Append(_T("Bel.Avg./Average")); break;
                    widget.w.sharpness->Append(_T("Bel.Avg./Average")); break;
                    widget.w.sharpness->Append(_T("Bel.Avg./Average")); break;
                    widget.w.sharpness->Append(_T("Average/High")); break;
                    widget.w.sharpness->Append(_T("Average/Average")); break;
                    widget.w.sharpness->Append(_T("Bel.Avg./Some")); break;
                    widget.w.sharpness->Append(_T("Abv.Avg./Average")); break;
                    widget.w.sharpness->Append(_T("Bel.Avg./Some")); break;
                    widget.w.sharpness->Append(_T("Abv.Avg./Average")); break;
                    widget.w.sharpness->Append(_T("Fast/High")); break;
                    widget.w.sharpness->Append(_T("Average/Some")); break;
                    widget.w.sharpness->Append(_T("Fast/Average")); break;
                    widget.w.sharpness->Append(_T("Bel.Avg./Low")); break;
                    widget.w.sharpness->Append(_T("Abv.Avg./Average")); break;
                    widget.w.sharpness->Append(_T("Average/Some")); break;
                    widget.w.sharpness->Append(_T("Fast/Average")); break;
                    widget.w.sharpness->Append(_T("Abv.Avg./Some")); break;
                    widget.w.sharpness->Append(_T("Abv.Avg./Low")); break;
                    widget.w.sharpness->Append(_T("Slow/Min")); break;
                    widget.w.sharpness->Append(_T("Fastest/Min")); break;
                    widget.w.sharpness->Thaw();
                    break;
                case 0x10:
                    widget.w.text[5]->SetLabel(_T("Configuration"));
                    widget.w.sharpness->Freeze();
                    widget.w.sharpness->Clear();
                    widget.w.sharpness->Append(_T("Wide/Rapid 3/None"));
                    widget.w.sharpness->Append(_T("Focus/Spread 3/None"));
                    widget.w.sharpness->Append(_T("Wide/Pierce 3/None"));
                    widget.w.sharpness->Append(_T("Focus/Spread 3/None"));
                    widget.w.sharpness->Append(_T("Power/Spread 3/None"));
                    widget.w.sharpness->Append(_T("Focus/Rapid 3/None"));
                    widget.w.sharpness->Append(_T("Focus/Spread 2/None"));
                    widget.w.sharpness->Append(_T("Wide/Pierce 3/None"));
                    widget.w.sharpness->Append(_T("Power/Rapid 3/None"));
                    widget.w.sharpness->Append(_T("Blast/Rapid 3/None"));
                    widget.w.sharpness->Append(_T("Wide/Pierce 3/Blast"));
                    widget.w.sharpness->Append(_T("Blast/Spread 3/None"));
                    widget.w.sharpness->Append(_T("Power/Spread 3/None"));
                    widget.w.sharpness->Append(_T("Blast/Pierce 4/Para"));
                    widget.w.sharpness->Append(_T("Focus/Spread 4/Exhaust"));
                    widget.w.sharpness->Append(_T("Power/Rapid 4/Poison"));
                    widget.w.sharpness->Append(_T("Wide/Pierce 4/Blast"));
                    widget.w.sharpness->Append(_T("Blast/Rapid 4/None"));
                    widget.w.sharpness->Append(_T("Power/Pierce 4/None"));
                    widget.w.sharpness->Append(_T("Focus/Spread 5/Para"));
                    widget.w.sharpness->Append(_T("Power/Rapid 5/Sleep"));
                    widget.w.sharpness->Append(_T("Power/Spread 4/Exhaust"));
                    widget.w.sharpness->Thaw();
                    break;
                default:
                    widget.w.text[5]->SetLabel(_T("Relic sharpness"));
                    widget.w.sharpness->Freeze();
                    widget.w.sharpness->Clear();
                    for(uint16_t i = 0; i < 0x16; i++)
                        widget.w.sharpness->Append(wxString(std::string("Type #"+PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
                    widget.w.sharpness->Thaw();
                    break;
            }
            widget.w.sharpness->SetSelection(equip[12]);

            switch(equip[0])
            {
                case 0x0b:
                    widget.w.text[10]->SetLabel(_T("Attachment"));
                    widget.w.kinsect_lvl->Freeze();
                    widget.w.kinsect_lvl->Clear();
                    widget.w.kinsect_lvl->Append(_T("None")); // 00
                    widget.w.kinsect_lvl->Append(_T("No limiter")); // 08
                    widget.w.kinsect_lvl->Append(_T("Silencer")); // 10
                    widget.w.kinsect_lvl->Append(_T("Silent/No limit"));
                    widget.w.kinsect_lvl->Append(_T("Long Barrel")); // 20
                    widget.w.kinsect_lvl->Append(_T("Barrel/No limit"));
                    widget.w.kinsect_lvl->Append(_T("Zoom")); // 40
                    widget.w.kinsect_lvl->Append(_T("Zoom/No limit")); // 48
                    widget.w.kinsect_lvl->Append(_T("Zoom/Silencer")); // 50
                    widget.w.kinsect_lvl->Append(_T("Zoom/Silent/No limit")); //58
                    widget.w.kinsect_lvl->Append(_T("Zoom/Long Barrel")); // 60
                    widget.w.kinsect_lvl->Append(_T("Zoom/Barrel/No limit")); // 68
                    widget.w.kinsect_lvl->Thaw();
                    break;
                case 0x0c:
                    widget.w.text[10]->SetLabel(_T("Attachment"));
                    widget.w.kinsect_lvl->Freeze();
                    widget.w.kinsect_lvl->Clear();
                    widget.w.kinsect_lvl->Append(_T("None")); // 00
                    widget.w.kinsect_lvl->Append(_T("No limiter")); // 08
                    widget.w.kinsect_lvl->Append(_T("Long Barrel"));
                    widget.w.kinsect_lvl->Append(_T("Barrel/No limit"));
                    widget.w.kinsect_lvl->Append(_T("Zoom"));
                    widget.w.kinsect_lvl->Append(_T("Zoom/No limit"));
                    widget.w.kinsect_lvl->Append(_T("Shield")); // 80
                    widget.w.kinsect_lvl->Append(_T("Shield/No limit"));
                    widget.w.kinsect_lvl->Append(_T("Zoom/Long Barrel")); // 60
                    widget.w.kinsect_lvl->Append(_T("Zoom/Barrel/No limit")); // 68
                    widget.w.kinsect_lvl->Append(_T("Zoom/Shield")); // C0
                    widget.w.kinsect_lvl->Append(_T("Zoom/Shield/No limit")); // C8
                    widget.w.kinsect_lvl->Thaw();
                    break;
                case 0x13:
                    widget.w.text[10]->SetLabel(_T("Kinsect level"));
                    widget.w.kinsect_lvl->Freeze();
                    widget.w.kinsect_lvl->Clear();
                    for(uint16_t i = 0; i < 12; i++)
                        widget.w.kinsect_lvl->Append(wxString(PlusLib::int2str(i+1).c_str(), wxConvUTF8));
                    widget.w.kinsect_lvl->Thaw();
                    break;
                default:
                    widget.w.text[10]->SetLabel(_T("Unknown"));
                    widget.w.kinsect_lvl->Freeze();
                    widget.w.kinsect_lvl->Clear();
                    for(uint32_t i = 0; i < 256; i++)
                        widget.w.kinsect_lvl->Append(wxString(PlusLib::int2str(i).c_str(), wxConvUTF8));
                    widget.w.kinsect_lvl->Thaw();
                    break;
            }
            if(equip[0] == 0x0b)
            {
                switch(equip[1])
                {
                    case 0x08: widget.w.kinsect_lvl->SetSelection(1);
                        break;
                    case 0x10: widget.w.kinsect_lvl->SetSelection(2);
                        break;
                    case 0x18: widget.w.kinsect_lvl->SetSelection(3);
                        break;
                    case 0x20: widget.w.kinsect_lvl->SetSelection(4);
                        break;
                    case 0x28: widget.w.kinsect_lvl->SetSelection(5);
                        break;
                    case 0x40: widget.w.kinsect_lvl->SetSelection(6);
                        break;
                    case 0x48: widget.w.kinsect_lvl->SetSelection(7);
                        break;
                    case 0x50: widget.w.kinsect_lvl->SetSelection(8);
                        break;
                    case 0x58: widget.w.kinsect_lvl->SetSelection(9);
                        break;
                    case 0x60: widget.w.kinsect_lvl->SetSelection(10);
                        break;
                    case 0x68: widget.w.kinsect_lvl->SetSelection(11);
                        break;
                    default: widget.w.kinsect_lvl->SetSelection(0);
                        break;
                }
            }
            else if(equip[0] == 0x0c)
            {
                switch(equip[1])
                {
                    case 0x08: widget.w.kinsect_lvl->SetSelection(1);
                        break;
                    case 0x20: widget.w.kinsect_lvl->SetSelection(2);
                        break;
                    case 0x28: widget.w.kinsect_lvl->SetSelection(3);
                        break;
                    case 0x40: widget.w.kinsect_lvl->SetSelection(4);
                        break;
                    case 0x48: widget.w.kinsect_lvl->SetSelection(5);
                        break;
                    case 0x80: widget.w.kinsect_lvl->SetSelection(6);
                        break;
                    case 0x88: widget.w.kinsect_lvl->SetSelection(7);
                        break;
                    case 0x60: widget.w.kinsect_lvl->SetSelection(8);
                        break;
                    case 0x68: widget.w.kinsect_lvl->SetSelection(9);
                        break;
                    case 0xC0: widget.w.kinsect_lvl->SetSelection(10);
                        break;
                    case 0xC8: widget.w.kinsect_lvl->SetSelection(11);
                        break;
                    default: widget.w.kinsect_lvl->SetSelection(0);
                        break;
                }
            }
            else widget.w.kinsect_lvl->SetSelection(equip[1]);

            switch(equip[0])
            {
                case 0x0b:
                    widget.w.kinsect->Freeze();
                    widget.w.kinsect->Clear();
                    for(uint16_t i = 0; i < 0x15; i++)
                        widget.w.kinsect->Append(wxString(std::string("Type #" + PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
                    widget.w.kinsect->Thaw();
                    widget.w.text[9]->SetLabel(_T("Rapid and Shots"));
                    break;
                case 0x0c:
                    widget.w.kinsect->Freeze();
                    widget.w.kinsect->Clear();
                    for(uint16_t i = 0; i < 0x15; i++)
                        widget.w.kinsect->Append(wxString(std::string("Type #" + PlusLib::int2str(i+1)).c_str(), wxConvUTF8));
                    widget.w.kinsect->Thaw();
                    widget.w.text[9]->SetLabel(_T("Siege and Shots"));
                    break;
                case 0x0e:
                    widget.w.kinsect->Freeze();
                    widget.w.kinsect->Clear();
                    widget.w.kinsect->Append(_T("Power"));
                    widget.w.kinsect->Append(_T("Element"));
                    widget.w.kinsect->Append(_T("Para"));
                    widget.w.kinsect->Append(_T("Dragon"));
                    widget.w.kinsect->Append(_T("Exhaust"));
                    widget.w.kinsect->Append(_T("Poison"));
                    widget.w.kinsect->Thaw();
                    widget.w.text[9]->SetLabel(_T("Phial"));
                    break;
                case 0x0f:
                    widget.w.kinsect->Freeze();
                    widget.w.kinsect->Clear();
                    widget.w.kinsect->Append(_T("Normal L1"));
                    widget.w.kinsect->Append(_T("Wide L1"));
                    widget.w.kinsect->Append(_T("Long L1"));
                    widget.w.kinsect->Append(_T("Normal L2"));
                    widget.w.kinsect->Append(_T("Wide L2"));
                    widget.w.kinsect->Append(_T("Long L2"));
                    widget.w.kinsect->Append(_T("Normal L3"));
                    widget.w.kinsect->Append(_T("Wide L3"));
                    widget.w.kinsect->Append(_T("Long L3"));
                    widget.w.kinsect->Append(_T("Normal L4"));
                    widget.w.kinsect->Append(_T("Wide L4"));
                    widget.w.kinsect->Append(_T("Long L4"));
                    widget.w.kinsect->Append(_T("Normal L5"));
                    widget.w.kinsect->Append(_T("Wide L5"));
                    widget.w.kinsect->Append(_T("Long L5"));
                    widget.w.kinsect->Thaw();
                    widget.w.text[9]->SetLabel(_T("Shot type"));
                    break;
                case 0x12:
                    widget.w.kinsect->Freeze();
                    widget.w.kinsect->Clear();
                    for(uint16_t i = 0; i < 50; i++)
                    {
                        switch(i)
                        {
                            case 6: widget.w.kinsect->Append(_T("Purple/Red/Green")); break;
                            case 38: widget.w.kinsect->Append(_T("Purple/Cyan/Yellow")); break;
                            case 40: widget.w.kinsect->Append(_T("Purple/Red/Orange")); break;
                            case 10: widget.w.kinsect->Append(_T("Purple/Red/Cyan")); break;
                            case 18: widget.w.kinsect->Append(_T("Purple/Blue/Green")); break;
                            case 2: widget.w.kinsect->Append(_T("Purple/Red/Blue")); break;
                            default: widget.w.kinsect->Append(wxString(std::string(PlusLib::int2str(i+1) + " ???").c_str(), wxConvUTF8)); break;
                        }
                    }
                    widget.w.kinsect->Thaw();
                    widget.w.text[9]->SetLabel(_T("Song"));
                    break;
                    break;
                case 0x13:
                    widget.w.kinsect->Freeze();
                    widget.w.kinsect->Clear();
                    widget.w.kinsect->Append(arena_list[20]);
                    widget.w.kinsect->Thaw();
                    widget.w.text[9]->SetLabel(_T("Kinsect"));
                    break;
                case 0x14:
                    widget.w.kinsect->Freeze();
                    widget.w.kinsect->Clear();
                    widget.w.kinsect->Append(_T("Impact"));
                    widget.w.kinsect->Append(_T("Element"));
                    widget.w.kinsect->Thaw();
                    widget.w.text[9]->SetLabel(_T("Phial"));
                    break;
                case 0x08: case 0x0a:
                    widget.w.kinsect->Freeze();
                    widget.w.kinsect->Clear();
                    for(uint32_t i = 0; i < 0x05; i++)
                        widget.w.kinsect->Append(wxString(std::string("Value #" + PlusLib::int2str(i)).c_str(), wxConvUTF8));
                    widget.w.kinsect->Thaw();
                    widget.w.text[9]->SetLabel(_T("Def. bonus (?)"));
                    break;
                case 0x07: case 0x09:
                    widget.w.kinsect->Freeze();
                    widget.w.kinsect->Clear();
                    for(uint32_t i = 0; i < 256; i++)
                        widget.w.kinsect->Append(wxString(PlusLib::int2str(i).c_str(), wxConvUTF8));
                    widget.w.kinsect->Thaw();
                    widget.w.text[9]->SetLabel(_T("Raw bonus (?)"));
                    break;
                case 0x0d: case 0x11:
                    widget.w.kinsect->Freeze();
                    widget.w.kinsect->Clear();
                    for(uint32_t i = 0; i < 256; i++)
                        widget.w.kinsect->Append(wxString(PlusLib::int2str(i).c_str(), wxConvUTF8));
                    widget.w.kinsect->Thaw();
                    widget.w.text[9]->SetLabel(_T("Aff. bonus (?)"));
                    break;
                default: // bow
                    widget.w.kinsect->Freeze();
                    widget.w.kinsect->Clear();
                    for(uint32_t i = 0; i < 256; i++)
                        widget.w.kinsect->Append(wxString(PlusLib::int2str(i).c_str(), wxConvUTF8));
                    widget.w.kinsect->Thaw();
                    widget.w.text[9]->SetLabel(_T("Unknown"));
                    break;
            }
            widget.w.kinsect->SetSelection(equip[15]);



            switch(equip[19])
            {
                case 0x00: widget.w.honing->SetSelection(0); break;
                case 0x40: widget.w.honing->SetSelection(1); break;
                case 0x80: widget.w.honing->SetSelection(2); break;
                case 0xC0: widget.w.honing->SetSelection(3); break;
            }
        }
    }
}

void EquipDialog::showWidget(uint32_t id, bool show)
{
    if(show)
    {
        switch(id)
        {
            case 0:
                for(uint32_t i = 0; i < 19; i++) widget.w.text[i]->Show();
                for(uint32_t i = 0; i < 8; i++) widget.w.kinsect_stat[i]->Show();
                widget.w.lvl->Show();
                widget.w.state->Show();
                widget.w.flag->Show();
                widget.w.rarity->Show();
                widget.w.attack->Show();
                widget.w.sharpness->Show();
                widget.w.attribute_id->Show();
                widget.w.attribute_value->Show();
                widget.w.honing->Show();
                widget.w.kinsect->Show();
                widget.w.kinsect_lvl->Show();
                break;
            case 1:
                for(uint32_t i = 0; i < 8; i++) widget.a.text[i]->Show();
                widget.a.lvl->Show();
                widget.a.state->Show();
                widget.a.flag->Show();
                widget.a.rarity->Show();
                widget.a.defense->Show();
                widget.a.resistance->Show();
                widget.a.color->Show();
                widget.a.color_set->Show();
                break;
            case 2:
                for(uint32_t i = 0; i < 5; i++) widget.t.text[i]->Show();
                widget.t.slot->Show();
                for(uint32_t i = 0; i < 2; i++)
                {
                    widget.t.skill[i]->Show();
                    widget.t.skill_value[i]->Show();
                }
                break;
        }
    }
    else
    {
        switch(id)
        {
            case 0:
                for(uint32_t i = 0; i < 19; i++) widget.w.text[i]->Hide();
                for(uint32_t i = 0; i < 8; i++) widget.w.kinsect_stat[i]->Hide();
                widget.w.lvl->Hide();
                widget.w.state->Hide();
                widget.w.flag->Hide();
                widget.w.rarity->Hide();
                widget.w.attack->Hide();
                widget.w.sharpness->Hide();
                widget.w.attribute_id->Hide();
                widget.w.attribute_value->Hide();
                widget.w.honing->Hide();
                widget.w.kinsect->Hide();
                widget.w.kinsect_lvl->Hide();
                break;
            case 1:
                for(uint32_t i = 0; i < 8; i++) widget.a.text[i]->Hide();
                widget.a.lvl->Hide();
                widget.a.state->Hide();
                widget.a.flag->Hide();
                widget.a.rarity->Hide();
                widget.a.defense->Hide();
                widget.a.resistance->Hide();
                widget.a.color->Hide();
                widget.a.color_set->Hide();
                break;
            case 2:
                for(uint32_t i = 0; i < 5; i++) widget.t.text[i]->Hide();
                widget.t.slot->Hide();
                for(uint32_t i = 0; i < 2; i++)
                {
                    widget.t.skill[i]->Hide();
                    widget.t.skill_value[i]->Hide();
                }
                break;
        }
    }
}

void EquipDialog::clearWidget()
{
    for(uint32_t i = 0; i < 8; i++) widget.w.kinsect_stat[i]->ChangeValue(_T("0"));
    widget.w.lvl->ChangeValue(_T("0"));
    widget.w.state->SetSelection(0);
    widget.w.flag->SetSelection(0);
    widget.w.rarity->SetSelection(0);
    widget.w.attack->SetSelection(0);
    widget.w.sharpness->SetSelection(0);
    widget.w.attribute_id->SetSelection(0);
    widget.w.attribute_value->SetSelection(0);
    widget.w.honing->SetSelection(0);
    widget.w.kinsect->SetSelection(0);
    widget.w.kinsect_lvl->SetSelection(0);

    widget.a.lvl->ChangeValue(_T("0"));
    widget.a.state->SetSelection(0);
    widget.a.flag->SetSelection(0);
    widget.a.rarity->SetSelection(0);
    widget.a.defense->SetSelection(0);
    widget.a.resistance->SetSelection(0);
    widget.a.color->ChangeValue(_T("0"));
    widget.a.color_set->ChangeValue(_T("0"));

    widget.t.slot->SetSelection(0);
    for(uint32_t i = 0; i < 2; i++)
    {
        widget.t.skill[i]->SetSelection(0);
        widget.t.skill_value[i]->ChangeValue(_T("0"));
    }
}
