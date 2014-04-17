#include "wx_fsqlf.hpp"
#include "dnd_target.hpp"

// define version if it was not passed as an argument in compilation command
#ifndef VERSION
#define VERSION "custom-version"
#endif



Notepad::Notepad() : wxFrame(NULL, wxID_ANY, wxT("wx Free SQL Formatter"), wxDefaultPosition, wxSize(650,500))
{
    wxBoxSizer* sizerh = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* left_sizer = new wxBoxSizer(wxVERTICAL);
    wxNotebook* nb = new wxNotebook( this, wxID_ANY);

    this->SetSizer(sizerh);
    sizerh->Add(left_sizer,0,0,0);
    Notepad::create_buttons(left_sizer);
    left_sizer->Add(nb,0,0,0);
    Notepad::create_options(nb);
    Notepad::create_menubar();
    Notepad::create_textarea(sizerh);

    // drag and drop support
    dnd_target* drop_target = new dnd_target(this->text_area);
    this->text_area->SetDropTarget(drop_target);

    SetIcon(wxIcon(fsqlf_right));
}



void Notepad::create_menubar()
{
    wxMenuBar* menu;
    wxMenu* file;
    wxMenu* edit;
    wxMenu* help;

    menu = new wxMenuBar();
    file = new wxMenu();
    edit = new wxMenu();
    help = new wxMenu();

    menu->Append(file, wxT("&File"));
    menu->Append(edit, wxT("&Edit"));
    menu->Append(help, wxT("&Help"));

    file->Append(idSave, wxT("&Save File\tCtrl-S"));
    file->Append(idOpen, wxT("&Open File\tCtrl-O"));
    file->AppendSeparator();
    file->Append(idExit, wxT("E&xit\tAlt-F4"));

    edit->Append(idCut   , wxT("Cu&t\tCtrl-X"));
    edit->Append(idCopy  , wxT("&Copy\tCtrl-C"));
    edit->Append(idPaste , wxT("&Paste\tCtrl-V"));
    edit->AppendSeparator();
    edit->Append(idSelectAll, wxT("Select &All\tCtrl-A"));
    edit->AppendSeparator();
    edit->Append(idFormat, wxT("&Format\tCtrl-F"));
    edit->Append(idUnformat, wxT("&Unformat\tCtrl-U"));

    help->Append(idAbout, wxT("&About...\tAlt-F1"));

    this->SetMenuBar(menu);
}



void Notepad::create_buttons(wxSizer* parent_sizer)
{
    parent_sizer->Add(new wxButton(parent_sizer->GetContainingWindow(), idFormat, wxT("Format")), 0, 0, 0);
    parent_sizer->Add(new wxButton(parent_sizer->GetContainingWindow(), idUnformat, wxT("Unformat")), 0, 0, 0);
}



#define ADD_NEWCHECKBOX(var_checkbox , parent_window , sizer , title , default_val) \
    var_checkbox = new wxCheckBox(parent_window, -1 , _(title) );\
    var_checkbox->SetValue(default_val);\
    sizer->Add(var_checkbox,0,0,0)

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
    this->text_area->SetFont(wxFont(10, wxTELETYPE, wxNORMAL, wxNORMAL));
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
}


void Notepad::OnUnformat(wxCommandEvent &event){
    if(this->original_text.IsEmpty()) return; // prevent deletion of everything
    this->text_area->Clear();
    this->text_area->SetValue(this->original_text);
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

void Notepad::OnMaxLen(wxCommandEvent &event){
    new wxMessageDialog(this, _("Maximum Limit Reached"), _(""), wxOK|wxCENTRE );
}



void Notepad::OnAbout(wxCommandEvent &event){
    wxAboutDialogInfo info;
    info.SetName(_("Free SQL Formatter"));
    info.SetVersion(_(VERSION));
    info.SetDescription(_T("Free SQL Formatter beautifies SQL code. It is particularly useful in case one has to deal with machine generated SQL code"));
    info.SetCopyright(_T("(C) 2011,2012,2013,2014  Danas Mikelinskas <danas.mikelinskas@gmail.com>"));
    info.SetLicence(_( LICENSE_TEXT ));
    wxAboutBox(info);
}
