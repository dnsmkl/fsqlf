#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/dnd.h>


class dnd_target : public wxFileDropTarget{
    private:
    wxTextCtrl* text_area;
    
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames)
    {
        this->text_area->LoadFile(filenames[0]);
    };
    
    
    public:
    dnd_target(wxTextCtrl* text_area)
    {
        this->text_area = text_area;
    };
};



class Notepad : public wxFrame {
    public:
    Notepad(); // our default constructor


    private:
    wxMenuBar* menu;
    wxMenu* file;//menu
    wxTextCtrl* text_area;
    wxTextCtrl* textRight;
    wxString original_text;
    wxButton* b_unformat;

    void OnUnformat(wxCommandEvent &event);
    void OnFormat(wxCommandEvent &event);
    void OnSave(wxCommandEvent &event);
    void OnOpen(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event)    {    this->Destroy();    };
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames);

    enum MenuControls{    idSave = 1000, idOpen, idExit, idFormat, idUnformat    };

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(Notepad, wxFrame)
    EVT_MENU(idSave, Notepad::OnSave)
    EVT_MENU(idOpen, Notepad::OnOpen)
    EVT_MENU(idExit, Notepad::OnExit)
    EVT_BUTTON(idSave, Notepad::OnSave)
    EVT_BUTTON(idOpen, Notepad::OnOpen)
    EVT_BUTTON(idFormat, Notepad::OnFormat)
    EVT_BUTTON(idUnformat, Notepad::OnUnformat)
END_EVENT_TABLE()



Notepad::Notepad() : wxFrame(NULL, wxID_ANY, wxT("wxNotepad"), wxDefaultPosition, wxSize(650,500)) {
    this->menu = new wxMenuBar(); // instantiate our menu bar
    this->file = new wxMenu(); // instantiate our file menu for our menu bar
    this->file->Append(idSave, wxT("&Save File\tCtrl-S")); // add a save option to the menu bar
    this->file->Append(idOpen, wxT("&Open File\tCtrl-O"));
    this->file->AppendSeparator(); // add a separator (between our file I/O options and our exit option
    this->file->Append(idExit, wxT("E&xit\tCtrl-F4")); // add an exit option to the file menu
    this->menu->Append(file, wxT("&File")); // add the file menu to the menubar, and give it the title "File"
    this->SetMenuBar(menu); // set our menu bar to be visible on the application


    wxBoxSizer *sizerv = new wxBoxSizer(wxVERTICAL); // buttons on the left
        sizerv->Add(new wxButton(this, idSave, wxT("Save")), 0, 0, 0);
        sizerv->Add(new wxButton(this, idOpen, wxT("Open")), 0, 0, 0);
        sizerv->Add(new wxButton(this, idFormat, wxT("Format")), 0, 0, 0);
        this->b_unformat = new wxButton(this, idUnformat, wxT("Unformat"));
        sizerv->Add(this->b_unformat, 0, 0, 0);

    wxBoxSizer *sizerh = new wxBoxSizer(wxHORIZONTAL);
        sizerh->Add(sizerv,0,0,0);
        this->text_area = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_MULTILINE);
        sizerh->Add(this->text_area,1,wxEXPAND,0);
        this->SetSizer(sizerh);

    dnd_target* drop_target = new dnd_target(this->text_area);
        this->text_area->SetDropTarget(drop_target);

}

void Notepad::OnUnformat(wxCommandEvent &event){
    this->text_area->Clear();
    this->text_area->SetValue(this->original_text);
}



void Notepad::OnFormat(wxCommandEvent &event){
    #ifdef _WIN32
        #define EXECUTABLE_FILE "fsqlf.exe"
        #define EXECUTION_PREFIX
    #else
        #define EXECUTABLE_FILE "fsqlf"
        #define EXECUTION_PREFIX "./"
    #endif

    wxDir dir(wxGetCwd());
    if( !dir.HasFiles(wxT(EXECUTABLE_FILE)) ){
        wxMessageBox(wxT("Formater executable file not found: " EXECUTABLE_FILE),wxT("Error"), wxOK | wxICON_INFORMATION, this);
        return;
    }

    #define TMP_INPUT_FILE  "tmp_fsqlf_in.txt"
    #define TMP_OUTPUT_FILE "tmp_fsqlf_out.txt"
    #define CMD   EXECUTION_PREFIX EXECUTABLE_FILE " " TMP_INPUT_FILE " " TMP_OUTPUT_FILE

    this->text_area->SaveFile(wxT(TMP_INPUT_FILE));
    if( system( CMD ) != 0 ){
        wxMessageBox(wxT("Command '" CMD "' returned non zero code"),wxT("Error"), wxOK | wxICON_INFORMATION, this);
        return;
    }
    this->text_area->LoadFile(wxT(TMP_OUTPUT_FILE));

    if( !wxRemoveFile(wxT(TMP_INPUT_FILE))  ) wxMessageBox(wxT("Failed to remove temporary file " TMP_INPUT_FILE),wxT("Error"), wxOK | wxICON_INFORMATION, this);
    if( !wxRemoveFile(wxT(TMP_OUTPUT_FILE)) ) wxMessageBox(wxT("Failed to remove temporary file " TMP_OUTPUT_FILE),wxT("Error"), wxOK | wxICON_INFORMATION, this);
}

void Notepad::OnSave(wxCommandEvent &event) {
    wxFileDialog *saveDialog = new wxFileDialog(this, wxT("Save File~"), wxT(""), wxT("")
                                                , wxT("SQL (*.sql)|*.sql|All (*.*)|*.*"), wxSAVE);

    if( wxID_OK == saveDialog->ShowModal() ) this->text_area->SaveFile(saveDialog->GetPath());
}

void Notepad::OnOpen(wxCommandEvent &event) {
    wxFileDialog *openDialog = new wxFileDialog(this, wxT("Open File~"), wxT(""), wxT("")
                                                , wxT("SQL (*.sql)|*.sql|All (*.*)|*.*"), wxOPEN);

    if( wxID_OK == openDialog->ShowModal() ) this->text_area->LoadFile(openDialog->GetPath());
}






class MainApp : public wxApp {
    public:
    virtual bool OnInit(){
        Notepad *main = new Notepad();
        main->Show(true);

        return true;
    }
};

IMPLEMENT_APP(MainApp)
