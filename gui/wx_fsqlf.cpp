#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/dnd.h>
#include <wx/aboutdlg.h>
#include <wx/font.h>
#include "fsqlf_right.xpm"
#include "license_text.h"

// define version if it was not passed as an argument in compilation command
#ifndef VERSION
#define VERSION "custom-version"
#endif


// set monospace font for all content of this->text_area
#define MONOSPACE_FONT                                                                                  \
    wxFont font(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetPointSize(), wxFONTFAMILY_TELETYPE, wxNORMAL, wxNORMAL);                     \
    wxTextAttr text_style(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT), wxNullColour, font);    \
    this->text_area->SetStyle(0,this->text_area->GetLastPosition(),text_style)


class dnd_target : public wxFileDropTarget{
    private:
    wxTextCtrl* text_area;
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames);

    public:
    dnd_target(wxTextCtrl* text_area);
};

bool dnd_target::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames){
    this->text_area->LoadFile(filenames[0]);
    MONOSPACE_FONT;
};

dnd_target::dnd_target(wxTextCtrl* text_area){
    this->text_area = text_area;
};





class Notepad : public wxFrame {
    public:
    Notepad();

    private:
    wxMenuBar* menu;
    wxMenu* file;
    wxMenu* edit;
    wxMenu* help;

    wxTextCtrl* text_area;
    wxTextCtrl* textRight;
    wxString original_text;
    wxButton* b_unformat;

    wxRadioBox* sel_comma_nl;
    wxCheckBox* nl_after_select;
    wxCheckBox *nl_before_or, *nl_after_or, *nl_before_and, *nl_after_and;

    void OnUnformat(wxCommandEvent &event);
    void OnFormat(wxCommandEvent &event);
    void OnSave(wxCommandEvent &event);
    void OnOpen(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event)    {    this->Destroy();    };
    void OnCopy(wxCommandEvent &event);
    void OnCut(wxCommandEvent &event);
    void OnPaste(wxCommandEvent &event);
    void OnSelectAll(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    enum MenuControls{ idSave = 1000, idOpen, idExit, idFormat, idUnformat, idCut, idCopy, idPaste, idSelectAll, idAbout  };

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(Notepad, wxFrame)
    EVT_MENU(idSave, Notepad::OnSave)
    EVT_MENU(idOpen, Notepad::OnOpen)
    EVT_MENU(idExit, Notepad::OnExit)

    EVT_MENU(idCut       , Notepad::OnCut)
    EVT_MENU(idCopy      , Notepad::OnCopy)
    EVT_MENU(idPaste     , Notepad::OnPaste)
    EVT_MENU(idSelectAll , Notepad::OnSelectAll)
    EVT_MENU(idFormat    , Notepad::OnFormat)
    EVT_MENU(idUnformat  , Notepad::OnUnformat)

    EVT_MENU(idAbout, Notepad::OnAbout)

    EVT_BUTTON(idSave, Notepad::OnSave)
    EVT_BUTTON(idOpen, Notepad::OnOpen)
    EVT_BUTTON(idFormat, Notepad::OnFormat)
    EVT_BUTTON(idUnformat, Notepad::OnUnformat)
END_EVENT_TABLE()



Notepad::Notepad() : wxFrame(NULL, wxID_ANY, wxT("wx Free SQL Formatter"), wxDefaultPosition, wxSize(650,500))
{
    // Menu
    this->menu = new wxMenuBar();
    this->file = new wxMenu();
    this->edit = new wxMenu();
    this->help = new wxMenu();

    this->menu->Append(this->file, wxT("&File"));
    this->menu->Append(this->edit, wxT("&Edit"));
    this->menu->Append(this->help, wxT("&Help"));

    this->file->Append(idSave, wxT("&Save File\tCtrl-S"));
    this->file->Append(idOpen, wxT("&Open File\tCtrl-O"));
    this->file->AppendSeparator();
    this->file->Append(idExit, wxT("E&xit\tAlt-F4"));

    this->edit->Append(idCut   , wxT("Cu&t\tCtrl-X"));
    this->edit->Append(idCopy  , wxT("&Copy\tCtrl-C"));
    this->edit->Append(idPaste , wxT("&Paste\tCtrl-V"));
    this->edit->AppendSeparator();
    this->edit->Append(idSelectAll, wxT("Select &All\tCtrl-A"));
    this->edit->AppendSeparator();
    this->edit->Append(idFormat, wxT("&Format\tCtrl-F"));
    this->edit->Append(idUnformat, wxT("&Unformat\tCtrl-U"));

    this->help->Append(idAbout, wxT("&About...\tAlt-F1"));

    this->SetMenuBar(menu);

    // Buttons on the left
    wxBoxSizer *sizerv = new wxBoxSizer(wxVERTICAL); // buttons on the left
    sizerv->Add(new wxButton(this, idSave, wxT("Save")), 0, 0, 0);
    sizerv->Add(new wxButton(this, idOpen, wxT("Open")), 0, 0, 0);
    sizerv->Add(new wxButton(this, idFormat, wxT("Format")), 0, 0, 0);
    this->b_unformat = new wxButton(this, idUnformat, wxT("Unformat"));
    sizerv->Add(this->b_unformat, 0, 0, 0);


    sizerv->Add( new wxStaticText(this, -1, _("SELECT section:")) );

    // Radio buttons - new lines in SELECT clause
    wxString sel_comma_nl_choices[3];
    sel_comma_nl_choices[0] = _("None");
    sel_comma_nl_choices[1] = _("Before comma");
    sel_comma_nl_choices[2] = _("After comma");

    sel_comma_nl = new wxRadioBox(this, -1, _("New lines"), wxDefaultPosition, wxDefaultSize, 3, sel_comma_nl_choices,1,wxRA_SPECIFY_COLS);
    sel_comma_nl->SetSelection(1);
    sizerv->Add(sel_comma_nl,0,0,0);

    // Check box - new line after SELECT
    nl_after_select = new wxCheckBox(this, -1 , _("New line after select") );
    nl_after_select->SetValue(true);
    sizerv->Add(nl_after_select,0,0,0);
    
    nl_before_or = new wxCheckBox(this, -1 , _("New line before or") );
    nl_before_or->SetValue(false);
    sizerv->Add(nl_before_or,0,0,0);
    
    nl_after_or = new wxCheckBox(this, -1 , _("New line after or") );
    nl_after_or->SetValue(false);
    sizerv->Add(nl_after_or,0,0,0);
    
    nl_before_and = new wxCheckBox(this, -1 , _("New line before and") );
    nl_before_and->SetValue(false);
    sizerv->Add(nl_before_and,0,0,0);
    
    nl_after_and = new wxCheckBox(this, -1 , _("New line after and") );
    nl_after_and->SetValue(false);
    sizerv->Add(nl_after_and,0,0,0);

    // Text area on the right
    wxBoxSizer *sizerh = new wxBoxSizer(wxHORIZONTAL);
    sizerh->Add(sizerv,0,0,0);
    this->text_area = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_MULTILINE);
    sizerh->Add(this->text_area,1,wxEXPAND,0);
    this->SetSizer(sizerh);
    // (with drag and drop support)
    dnd_target* drop_target = new dnd_target(this->text_area);
    this->text_area->SetDropTarget(drop_target);


    SetIcon(wxIcon(fsqlf_right));
}



void Notepad::OnFormat(wxCommandEvent &event)
{
    this->original_text = this->text_area->GetValue();
    #ifdef _WIN32
        #define EXECUTABLE_FILE "fsqlf.exe"
        #define EXECUTION_PREFIX
    #else
        #define EXECUTABLE_FILE "fsqlf"
        #define EXECUTION_PREFIX "./"
    #endif

    #define TMP_INPUT_FILE  "tmp_fsqlf_in.txt"
    #define TMP_OUTPUT_FILE "tmp_fsqlf_out.txt"

    wxString cmd;
    cmd = wxT( EXECUTION_PREFIX EXECUTABLE_FILE " " TMP_INPUT_FILE " " TMP_OUTPUT_FILE );
    switch( this->sel_comma_nl->GetSelection() ){
        case 0: cmd << wxT("  --select-comma-newline none")   ; break;
        case 1: cmd << wxT("  --select-comma-newline before") ; break;
        case 2: cmd << wxT("  --select-comma-newline after")  ; break;
    }

    switch( this->nl_after_select->GetValue() ){
        case 0: cmd << wxT("  --select-newline-after 0") ; break;
        case 1: cmd << wxT("  --select-newline-after 1") ; break;
    }
    
    switch( this->nl_before_or->GetValue() ){
        case 0: cmd << wxT("  --newline-or-before 0") ; break;
        case 1: cmd << wxT("  --newline-or-before 1") ; break;
    }
    
    switch( this->nl_after_or->GetValue() ){
        case 0: cmd << wxT("  --newline-or-after 0") ; break;
        case 1: cmd << wxT("  --newline-or-after 1") ; break;
    }
    
    switch( this->nl_before_and->GetValue() ){
        case 0: cmd << wxT("  --newline-and-before 0") ; break;
        case 1: cmd << wxT("  --newline-and-before 1") ; break;
    }
    
    switch( this->nl_after_and->GetValue() ){
        case 0: cmd << wxT("  --newline-and-after 0") ; break;
        case 1: cmd << wxT("  --newline-and-after 1") ; break;
    }

    wxDir dir(wxGetCwd());
    if(  !dir.HasFiles(wxT(EXECUTABLE_FILE))  )
    {
        wxMessageBox(wxT("Formatter executable file not found: " EXECUTABLE_FILE),wxT("Error"), wxOK | wxICON_INFORMATION, this);
        return;
    }

    this->text_area->SaveFile(wxT(TMP_INPUT_FILE));

    if( system(cmd.mb_str()) )
    {   // non zero status
        wxMessageBox(cmd << wxT("\n returned non zero code"),wxT("Error"), wxOK | wxICON_INFORMATION, this);
        return;
    }
    this->text_area->LoadFile(wxT(TMP_OUTPUT_FILE));
    MONOSPACE_FONT;

    if( !wxRemoveFile(wxT(TMP_INPUT_FILE))  ) wxMessageBox(wxT("Failed to remove temporary file " TMP_INPUT_FILE),wxT("Error") , wxOK | wxICON_INFORMATION, this);
    if( !wxRemoveFile(wxT(TMP_OUTPUT_FILE)) ) wxMessageBox(wxT("Failed to remove temporary file " TMP_OUTPUT_FILE),wxT("Error"), wxOK | wxICON_INFORMATION, this);
}



void Notepad::OnSave( wxCommandEvent &event ){
    wxFileDialog *saveDialog = new wxFileDialog(this, wxT("Save File~"), wxT(""), wxT(""), wxT("SQL (*.sql)|*.sql|All (*.*)|*.*"), wxSAVE);
    if( wxID_OK == saveDialog->ShowModal() ) this->text_area->SaveFile(saveDialog->GetPath());
}


void Notepad::OnOpen( wxCommandEvent &event ){
    wxFileDialog *openDialog = new wxFileDialog(this, wxT("Open File~"), wxT(""), wxT(""), wxT("SQL (*.sql)|*.sql|All (*.*)|*.*"), wxOPEN);
    if( wxID_OK == openDialog->ShowModal() ) this->text_area->LoadFile(openDialog->GetPath());
    MONOSPACE_FONT;
}


void Notepad::OnUnformat(wxCommandEvent &event){
    if(this->original_text.IsEmpty()) return; // prevent deletion of everything
    this->text_area->Clear();
    this->text_area->SetValue(this->original_text);
    MONOSPACE_FONT;
}

void Notepad::OnCut(wxCommandEvent &event){
    this->text_area->Cut();
}

void Notepad::OnCopy(wxCommandEvent &event){
    this->text_area->Copy();
}

void Notepad::OnPaste(wxCommandEvent &event){
    this->text_area->Paste();
}

void Notepad::OnSelectAll(wxCommandEvent &event){
    this->text_area->SetSelection(-1, -1);
}



void Notepad::OnAbout(wxCommandEvent &event){
    wxAboutDialogInfo info;
    info.SetName(_("Free SQL Formatter"));
    info.SetVersion(_(VERSION));
    info.SetDescription(_T("Free SQL Formatter beautifies SQL code. It is particularly useful in case one has to deal with machine generated SQL code"));
    info.SetCopyright(_T("(C) 2011 Danas Mikelinskas <danas.mikelinskas@gmail.com>"));
    info.SetLicence(_( LICENSE_TEXT ));
    wxAboutBox(info);
}





class MainApp : public wxApp
{
    public:
    virtual bool OnInit(){
        Notepad *main = new Notepad();
        main->Show(true);

        return true;
    }
};

IMPLEMENT_APP(MainApp)
