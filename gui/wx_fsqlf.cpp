#include "wx_fsqlf.hpp"


// define version if it was not passed as an argument in compilation command
#ifndef VERSION
#define VERSION "custom-version"
#endif


enum EventIds
{
    idFormat = wxID_HIGHEST
    ,idUnformat
    ,idUseConfigNlOther
};


BEGIN_EVENT_TABLE(FsqlfGui, BasicNotepad)
    EVT_BUTTON(idFormat, FsqlfGui::onFormat)
    EVT_BUTTON(idUnformat, FsqlfGui::onUnformat)

    EVT_MENU(idFormat    , FsqlfGui::onFormat)
    EVT_MENU(idUnformat  , FsqlfGui::onUnformat)

    EVT_CHECKBOX(idUseConfigNlOther, FsqlfGui::onUseConfigNlOther)
END_EVENT_TABLE()


#define TITLE "wx Free SQL Formatter"


FsqlfGui::FsqlfGui(): BasicNotepad(_(TITLE))
{
    wxNotebook* nb = new wxNotebook(this, wxID_ANY);
    wxBoxSizer* sizerh = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* left_sizer = new wxBoxSizer(wxVERTICAL);

    this->SetSizer(sizerh);
    sizerh->Add(left_sizer,0,0,0);

    FsqlfGui::create_buttons(left_sizer);
    FsqlfGui::create_textarea(sizerh);

    left_sizer->Add(nb,0,0,0);
    FsqlfGui::create_options(nb);

    this->edit_menu->AppendSeparator();
    this->edit_menu->Append(idFormat, _("&Format\tCtrl-F"));
    this->edit_menu->Append(idUnformat, _("&Unformat\tCtrl-U"));
}


void FsqlfGui::create_buttons(wxSizer* parent_sizer)
{
    wxWindow* window = parent_sizer->GetContainingWindow();
    parent_sizer->Add(new wxButton(window, idFormat, _("Format")), 0, 0, 0);
    parent_sizer->Add(new wxButton(window, idUnformat, _("Unformat")), 0, 0, 0);
}


void FsqlfGui::create_options(wxNotebook* nb)
{
    wxPanel* parent_panel = new wxPanel(nb);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    parent_panel->SetSizer(sizer);
    nb->AddPage(parent_panel, _("Options"));

    create_options_nl_comma(sizer);
    create_options_nl_keywords(sizer);
    create_options_nl_major_sections(sizer);
    create_options_text(sizer);
}


void add_newcheckbox(wxCheckBox* &result_checkbox
    , wxSizer* parent_sizer
    , wxString title
    , bool default_val
    , bool enabled = true, int id = -1)
{
    result_checkbox = new wxCheckBox(parent_sizer->GetContainingWindow(), id, title);
    result_checkbox->SetValue(default_val);
    if( !enabled ) result_checkbox->Disable();
    parent_sizer->Add(result_checkbox, 0, 0, 0);
}


void FsqlfGui::create_options_nl_comma(wxSizer* parent_sizer)
{
    // Radio buttons - new lines in SELECT clause
    #define NUM_COMMA_NL_CHOICES (4)
    wxString choices[NUM_COMMA_NL_CHOICES];
    choices[0] = _("Use Config File");
    choices[1] = _("Before");
    choices[2] = _("After");
    choices[3] = _("None");

    sel_comma_nl = new wxRadioBox(parent_sizer->GetContainingWindow(), wxID_ANY
        , _("New line:[comma]"), wxDefaultPosition, wxDefaultSize
        , NUM_COMMA_NL_CHOICES, choices, 1, wxRA_SPECIFY_COLS);
    sel_comma_nl->SetSelection(0);
    parent_sizer->Add(sel_comma_nl,0,0,0);
}


void FsqlfGui::create_options_nl_keywords(wxSizer* parent_sizer)
{
    // Check boxes for : OR , AND , SELECT
    wxStaticBox * box = new wxStaticBox(parent_sizer->GetContainingWindow()
        , wxID_ANY, _("New line:[other]"));
    wxStaticBoxSizer* sizer = new wxStaticBoxSizer(box, wxVERTICAL);
    parent_sizer->Add(sizer,0,0,0);

    add_newcheckbox(nl_use_config  , sizer, _("Use config")    , true , true
        , idUseConfigNlOther);
    add_newcheckbox(nl_after_select, sizer, _("[select] after"), true , false);
    add_newcheckbox(nl_before_or   , sizer, _("[or] before")   , false, false);
    add_newcheckbox(nl_after_or    , sizer, _("[or] after")    , false, false);
    add_newcheckbox(nl_before_and  , sizer, _("[and] before")  , true , false);
    add_newcheckbox(nl_after_and   , sizer, _("[and] after")   , false, false);
}


void FsqlfGui::create_options_nl_major_sections(wxSizer* parent_sizer)
{
    #define NUM_CHOICES (3)
    wxString choices[NUM_CHOICES];
    choices[0] = _("Use Config File");
    choices[1] = _("1 New Line");
    choices[2] = _("2 New Lines");
    nl_major_sections = new wxRadioBox(parent_sizer->GetContainingWindow(), wxID_ANY
        , _("Major sections"), wxDefaultPosition, wxDefaultSize
        , NUM_CHOICES, choices, 1, wxRA_SPECIFY_COLS);
    nl_major_sections->SetSelection(0);
    parent_sizer->Add(nl_major_sections,0,0,0);
}


void FsqlfGui::create_options_text(wxSizer* parent_sizer)
{
    add_newcheckbox(use_original_text, parent_sizer, _("Use original keyword text"), false);

    // CASE settings
    wxString choices[4];
    choices[0] = _("None (aBc)");
    choices[1] = _("Upper (ABC)");
    choices[2] = _("Lower (abc)");
    choices[3] = _("Init (Abc)");
    case_all_kw = new wxRadioBox(parent_sizer->GetContainingWindow(), wxID_ANY
        , _("Keyword case"), wxDefaultPosition, wxDefaultSize
        , 4, choices, 1, wxRA_SPECIFY_COLS);
    case_all_kw->SetSelection(1);
    parent_sizer->Add(case_all_kw,0,0,0);
}


void FsqlfGui::onFormat(wxCommandEvent &event)
{
    this->original_text = this->text_area->GetValue();
    #ifdef _WIN32
        #define EXEC_FILE "fsqlf.exe"
        #define EXEC_PREFIX
    #else
        #define EXEC_FILE "fsqlf"
        #define EXEC_PREFIX "./"
    #endif

    #define TMP_INPUT_FILE  "tmp_fsqlf_in.txt"
    #define TMP_OUTPUT_FILE "tmp_fsqlf_out.txt"

    wxString cmd;
    cmd = _(EXEC_PREFIX EXEC_FILE " " TMP_INPUT_FILE " " TMP_OUTPUT_FILE);
    switch( this->sel_comma_nl->GetSelection() ){
        case 1: cmd << _("  --select-comma-newline before") ; break;
        case 2: cmd << _("  --select-comma-newline after")  ; break;
        case 3: cmd << _("  --select-comma-newline none")   ; break;
    }

    if( this->nl_use_config->GetValue() == 0 )
    {
        switch( this->nl_after_select->GetValue() )
        {
            case 0: cmd << _("  --select-newline-after 0") ; break;
            case 1: cmd << _("  --select-newline-after 1") ; break;
        }

        switch( this->nl_before_or->GetValue() )
        {
            case 0: cmd << _("  --newline-or-before 0") ; break;
            case 1: cmd << _("  --newline-or-before 1") ; break;
        }

        switch( this->nl_after_or->GetValue() )
        {
            case 0: cmd << _("  --newline-or-after 0") ; break;
            case 1: cmd << _("  --newline-or-after 1") ; break;
        }

        switch( this->nl_before_and->GetValue() )
        {
            case 0: cmd << _("  --newline-and-before 0") ; break;
            case 1: cmd << _("  --newline-and-before 1") ; break;
        }

        switch( this->nl_after_and->GetValue() )
        {
            case 0: cmd << _("  --newline-and-after 0") ; break;
            case 1: cmd << _("  --newline-and-after 1") ; break;
        }
    }

    switch( this->nl_major_sections->GetSelection() )
    {
        case 0: break;
        case 1: cmd << _("  --newline-major-sections 1") ; break;
        case 2: cmd << _("  --newline-major-sections 2")  ; break;
    }

    switch( this->use_original_text->GetValue() )
    {
        case 0: cmd << _("  --keyword-text default")  ; break;
        case 1: cmd << _("  --keyword-text original") ; break;
    }

    switch( this->case_all_kw->GetSelection() )
    {
        case 0: cmd << _("  --keyword-case none")    ; break;
        case 1: cmd << _("  --keyword-case upper")   ; break;
        case 2: cmd << _("  --keyword-case lower")   ; break;
        case 3: cmd << _("  --keyword-case initcap") ; break;
    }

    wxDir dir(wxGetCwd());
    if( !dir.HasFiles(_(EXEC_FILE)) )
    {
        wxMessageBox(_("Formatter executable file not found: " EXEC_FILE)
            , _("Error")
            , wxOK | wxICON_INFORMATION, this);
        return;
    }

    this->text_area->SaveFile(_(TMP_INPUT_FILE));

    if( system(cmd.mb_str()) )
    {   // non zero status
        wxMessageBox(cmd << _("\n returned non zero code")
            , _("Error")
            , wxOK | wxICON_INFORMATION, this);
        return;
    }
    this->text_area->LoadFile(_(TMP_OUTPUT_FILE));

    if( !wxRemoveFile(_(TMP_INPUT_FILE))  )
        wxMessageBox(_("Failed to remove temporary file " TMP_INPUT_FILE)
            ,_("Error")
            , wxOK | wxICON_INFORMATION, this);
    if( !wxRemoveFile(_(TMP_OUTPUT_FILE)) )
        wxMessageBox(_("Failed to remove temporary file " TMP_OUTPUT_FILE)
            , _("Error")
            , wxOK | wxICON_INFORMATION, this);
}


void FsqlfGui::onUnformat(wxCommandEvent &event)
{
    if(this->original_text.IsEmpty()) return; // prevent deletion of everything
    this->text_area->Clear();
    this->text_area->SetValue(this->original_text);
}


void FsqlfGui::onUseConfigNlOther(wxCommandEvent &event)
{
    if(event.IsChecked())
    {
        this->nl_after_select->Disable();
        this->nl_before_or->Disable();
        this->nl_after_or->Disable();
        this->nl_before_and->Disable();
        this->nl_after_and->Disable();
    }
    else
    {
        this->nl_after_select->Enable();
        this->nl_before_or->Enable();
        this->nl_after_or->Enable();
        this->nl_before_and->Enable();
        this->nl_after_and->Enable();
    }
}
