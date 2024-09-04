#include <wx/wx.h>
#include "LoginFrame.h"
#include "AdminFrame.h"


// Define the main application class
class MyApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        LoginFrame* loginFrame = new LoginFrame(wxT("Login"));
        loginFrame->Show(true);
        return true;
    }
};

// Implement the application
wxIMPLEMENT_APP(MyApp);