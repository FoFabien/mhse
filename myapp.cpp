// On appelle le fichier de déclaration de notre classe
#include "myapp.hpp"
#include "mainframe.hpp"
#include "version.h"
#include <string>

// La macro pour rediriger l'exécution vers notre classe
IMPLEMENT_APP(MyApp);

// L'implémentation de la méthode OnInit
bool MyApp::OnInit()
{
    std::string app_name = "MHQE v" + std::string(AutoVersion::FULLVERSION_STRING) + " " + std::string(AutoVersion::STATUS);
    // On crée un objet MainFrame
    MainFrame *frm=new MainFrame(wxString(app_name.c_str(),wxConvUTF8));
    // On fait en sorte que la fenêtre soit visible
    frm->Show();
    // On retourne true pour que le programme ne se termine pas immédiatement
    return true;
}
