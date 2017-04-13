// On appelle le fichier de d�claration de notre classe
#include "myapp.hpp"
#include "mainframe.hpp"
#include "version.h"
#include <string>

// La macro pour rediriger l'ex�cution vers notre classe
IMPLEMENT_APP(MyApp);

// L'impl�mentation de la m�thode OnInit
bool MyApp::OnInit()
{
    std::string app_name = "MHQE v" + std::string(AutoVersion::FULLVERSION_STRING) + " " + std::string(AutoVersion::STATUS);
    // On cr�e un objet MainFrame
    MainFrame *frm=new MainFrame(wxString(app_name.c_str(),wxConvUTF8));
    // On fait en sorte que la fen�tre soit visible
    frm->Show();
    // On retourne true pour que le programme ne se termine pas imm�diatement
    return true;
}
