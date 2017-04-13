// Un petit "Guard-Block" afin d'�viter les inclusions multiples
#ifndef MYAPP_H_INCLUDED
#define MYAPP_H_INCLUDED

// Le header wxWidgets de base
#include <wx/wx.h>

// D�claration de notre classe d�riv�e de wxApp
class MyApp : public wxApp
{
    public:
        // Une seule m�thode pour l'instant
        virtual bool OnInit();
};

// On n'oublie pas de d�clarer cette classe comme �tant la classe principale
DECLARE_APP(MyApp);

// Et on n'oublie pas �galement de refermer le "Guard-Block"
#endif //MYAPP_H_INCLUDED
