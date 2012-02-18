#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/dnd.h>
#include <wx/aboutdlg.h>
#include <wx/font.h>
#include <wx/notebook.h>
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
    wxRadioBox* nl_major_sections;
    wxRadioBox* case_all_kw;
    wxCheckBox* use_original_text;

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

    void create_menubar();
    void create_buttons(wxWindow* parent_window , wxSizer* parent_sizer);
    void create_options(wxNotebook*);
    void create_textarea(wxSizer* parent);

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

    EVT_BUTTON(idFormat, Notepad::OnFormat)
    EVT_BUTTON(idUnformat, Notepad::OnUnformat)
END_EVENT_TABLE()


#define ADD_NEWCHECKBOX(var_checkbox , parent_window , sizer , title , default_val) \
    var_checkbox = new wxCheckBox(parent_window, -1 , _(title) );\
    var_checkbox->SetValue(default_val);\
    sizer->Add(var_checkbox,0,0,0)\

Notepad::Notepad() : wxFrame(NULL, wxID_ANY, wxT("wx Free SQL Formatter"), wxDefaultPosition, wxSize(650,500))
{
    // create
    wxBoxSizer* sizerh = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* left_sizer = new wxBoxSizer(wxVERTICAL);
    wxNotebook* nb = new wxNotebook( this, wxID_ANY);

    this->SetSizer(sizerh);
    sizerh->Add(left_sizer,0,0,0);
    Notepad::create_buttons(this,left_sizer);
    left_sizer->Add(nb,0,0,0);
    Notepad::create_options(nb);
    Notepad::create_menubar();
    Notepad::create_textarea(sizerh);

    // (with drag and drop support)
    dnd_target* drop_target = new dnd_target(this->text_area);
    this->text_area->SetDropTarget(drop_target);

    SetIcon(wxIcon(fsqlf_right));
}



void Notepad::create_menubar()
{
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
}



void Notepad::create_buttons(wxWindow* parent_window , wxSizer* parent_sizer)
{
    parent_sizer->Add(new wxButton(parent_window, idFormat, wxT("Format")), 0, 0, 0);
    this->b_unformat = new wxButton(parent_window, idUnformat, wxT("Unformat"));
    parent_sizer->Add(this->b_unformat, 0, 0, 0);
}



void Notepad::create_options(wxNotebook* nb)
{
    wxPanel* parent_panel = new wxPanel(nb);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    parent_panel->SetSizer(sizer);
    nb->AddPage(parent_panel, _("Options"));

    // Radio buttons - new lines in SELECT clause
    wxString sel_comma_nl_choices[3];
    sel_comma_nl_choices[0] = _("None");
    sel_comma_nl_choices[1] = _("Before");
    sel_comma_nl_choices[2] = _("After");

    sel_comma_nl = new wxRadioBox(parent_panel, -1, _("New line:[comma]"), wxDefaultPosition, wxDefaultSize, 3, sel_comma_nl_choices,1,wxRA_SPECIFY_COLS);
    sel_comma_nl->SetSelection(1);
    sizer->Add(sel_comma_nl,0,0,0);

    // Check boxes for : OR , AND , SELECT
    wxStaticBoxSizer* nl_other_sizer = new wxStaticBoxSizer(
                     new wxStaticBox(parent_panel, -1, _("New line:[other]"))
                     , wxVERTICAL);
    sizer->Add(nl_other_sizer,0,0,0);

    ADD_NEWCHECKBOX(nl_after_select,parent_panel,nl_other_sizer,"[select] after",true);
    ADD_NEWCHECKBOX(nl_before_or   ,parent_panel,nl_other_sizer,"[or] before"   ,false);
    ADD_NEWCHECKBOX(nl_after_or    ,parent_panel,nl_other_sizer,"[or] after"    ,false);
    ADD_NEWCHECKBOX(nl_before_and  ,parent_panel,nl_other_sizer,"[and] before"  ,true);
    ADD_NEWCHECKBOX(nl_after_and   ,parent_panel,nl_other_sizer,"[and] after"   ,false);
    
    wxString nl_major_sections_choices[3];
    nl_major_sections_choices[0] = _("Use Config File");
    nl_major_sections_choices[1] = _("1 New Line");
    nl_major_sections_choices[2] = _("2 New Lines");
    nl_major_sections = new wxRadioBox(parent_panel, -1, _("Major sections"), wxDefaultPosition, wxDefaultSize, 3, nl_major_sections_choices,1,wxRA_SPECIFY_COLS);
    nl_major_sections->SetSelection(2);
    sizer->Add(nl_major_sections,0,0,0);

    ADD_NEWCHECKBOX(use_original_text,parent_panel,sizer,"Use original keyword text",false);
    
    // CASE settings
    wxString case_all_kw_choices[4];
    case_all_kw_choices[0] = _("None (aBc)");
    case_all_kw_choices[1] = _("Upper (ABC)");
    case_all_kw_choices[2] = _("Lower (abc)");
    case_all_kw_choices[3] = _("Init (Abc)");
    case_all_kw = new wxRadioBox(parent_panel, -1, _("Keyword case"), wxDefaultPosition, wxDefaultSize, 4, case_all_kw_choices,1,wxRA_SPECIFY_COLS);
    case_all_kw->SetSelection(1);
    sizer->Add(case_all_kw,0,0,0);
}



void Notepad::create_textarea(wxSizer* parent)
{
    this->text_area = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_MULTILINE);
    parent->Add(this->text_area,1,wxEXPAND,0);
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

    switch( this->nl_major_sections->GetSelection() ){
        case 0: break;
        case 1: cmd << wxT("  --newline-major-sections 1") ; break;
        case 2: cmd << wxT("  --newline-major-sections 2")  ; break;
    }

    switch( this->use_original_text->GetValue() ){
        case 0: cmd << wxT("  --keyword-text default")  ; break;
        case 1: cmd << wxT("  --keyword-text original") ; break;
    }

    switch( this->case_all_kw->GetSelection() ){
        case 0: cmd << wxT("  --keyword-case none")    ; break;
        case 1: cmd << wxT("  --keyword-case upper")   ; break;
        case 2: cmd << wxT("  --keyword-case lower")   ; break;
        case 3: cmd << wxT("  --keyword-case initcap") ; break;
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
