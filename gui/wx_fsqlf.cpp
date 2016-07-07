#include "wx_fsqlf.hpp"


extern "C" {
    #include "lib_fsqlf.h"
}


// define version if it was not passed as an argument in compilation command
#ifndef VERSION
#define VERSION "custom-version"
#endif


enum EventIds
{
    idFormat = wxID_HIGHEST,
    idUnformat,
    idUseConfigNlOther
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


void add_newcheckbox(wxCheckBox* &result_checkbox,
    wxSizer* parent_sizer,
    wxString title,
    bool default_val,
    bool enabled = true,
    int id = -1)
{
    result_checkbox = new wxCheckBox(parent_sizer->GetContainingWindow(), id, title);
    result_checkbox->SetValue(default_val);
    if (!enabled) result_checkbox->Disable();
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
    wxStaticBox * box = new wxStaticBox(parent_sizer->GetContainingWindow(),
            wxID_ANY, _("New line:[other]"));
    wxStaticBoxSizer* sizer = new wxStaticBoxSizer(box, wxVERTICAL);
    parent_sizer->Add(sizer,0,0,0);

    add_newcheckbox(nl_use_config  , sizer, _("Use config")    , true , true,
        idUseConfigNlOther);
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
    nl_major_sections = new wxRadioBox(parent_sizer->GetContainingWindow(),
        wxID_ANY, _("Major sections"), wxDefaultPosition, wxDefaultSize,
        NUM_CHOICES, choices, 1, wxRA_SPECIFY_COLS);
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
    case_all_kw = new wxRadioBox(parent_sizer->GetContainingWindow(), wxID_ANY,
        _("Keyword case"), wxDefaultPosition, wxDefaultSize,
        4, choices, 1, wxRA_SPECIFY_COLS);
    case_all_kw->SetSelection(1);
    parent_sizer->Add(case_all_kw,0,0,0);
}


void FsqlfGui::onFormat(wxCommandEvent &event)
{
    this->original_text = this->text_area->GetValue();

    // Init kwmap and set it according to gui-controls.
    fsqlf_kwmap_t kwmap;
    fsqlf_kwmap_init(&kwmap);
    fsqlf_kwmap_conffile_read_default(kwmap);
    switch (this->sel_comma_nl->GetSelection()) {
        case 1:
            fsqlf_kw_get(kwmap, "kw_comma")->before.new_line = 1;
            fsqlf_kw_get(kwmap, "kw_comma")->after.new_line  = 0;
            break;
        case 2:
            fsqlf_kw_get(kwmap, "kw_comma")->before.new_line = 0;
            fsqlf_kw_get(kwmap, "kw_comma")->after.new_line  = 1;
            break;
        case 3:
            fsqlf_kw_get(kwmap, "kw_comma")->before.new_line = 0;
            fsqlf_kw_get(kwmap, "kw_comma")->after.new_line  = 0;
            break;
    }

    if (this->nl_use_config->GetValue() == 0) {
        switch (this->nl_after_select->GetValue()) {
            case 0: fsqlf_kw_get(kwmap, "kw_select")->after.new_line = 0; break;
            case 1: fsqlf_kw_get(kwmap, "kw_select")->after.new_line = 0; break;
        }

        switch (this->nl_before_or->GetValue()) {
            case 0: fsqlf_kw_get(kwmap, "kw_or")->before.new_line = 0; break;
            case 1: fsqlf_kw_get(kwmap, "kw_or")->before.new_line = 1; break;
        }

        switch (this->nl_after_or->GetValue()) {
            case 0: fsqlf_kw_get(kwmap, "kw_or")->after.new_line = 0; break;
            case 1: fsqlf_kw_get(kwmap, "kw_or")->after.new_line = 1; break;
        }

        switch (this->nl_before_and->GetValue()) {
            case 0: fsqlf_kw_get(kwmap, "kw_and")->before.new_line = 0; break;
            case 1: fsqlf_kw_get(kwmap, "kw_and")->before.new_line = 1; break;
        }

        switch (this->nl_after_and->GetValue()) {
            case 0: fsqlf_kw_get(kwmap, "kw_and")->after.new_line = 0; break;
            case 1: fsqlf_kw_get(kwmap, "kw_and")->after.new_line = 1; break;
        }
    }

    switch (this->nl_major_sections->GetSelection()) {
        case 0: break; // Use config file.
        case 1: fsqlf_kwmap_set_major_clause_nl(kwmap, 1); break;
        case 2: fsqlf_kwmap_set_major_clause_nl(kwmap, 2); break;
    }

    switch (this->use_original_text->GetValue()) {
        case 0: fsqlf_kwmap_set_spelling(kwmap, FSQLF_KWSPELLING_USE_HARDCODED_DEFAULT); break;
        case 1: fsqlf_kwmap_set_spelling(kwmap, FSQLF_KWSPELLING_USE_ORIGINAL); break;
    }

    switch (this->case_all_kw->GetSelection()) {
        case 0: fsqlf_kwmap_set_case(kwmap, FSQLF_KWCASE_ORIGINAL); break;
        case 1: fsqlf_kwmap_set_case(kwmap, FSQLF_KWCASE_UPPER); break;
        case 2: fsqlf_kwmap_set_case(kwmap, FSQLF_KWCASE_LOWER); break;
        case 3: fsqlf_kwmap_set_case(kwmap, FSQLF_KWCASE_INITCAP); break;
    }

    // Actual formatting.
    wxCharBuffer buffer = this->original_text.ToUTF8();
    const char *input =  buffer.data();
    char *output;
    fsqlf_format_bytes(kwmap, input, strlen(input), &output);
    this->text_area->SetValue(wxString(output, wxConvUTF8));

    // Cleanup of kwmap.
    fsqlf_kwmap_destroy(kwmap);
}


void FsqlfGui::onUnformat(wxCommandEvent &event)
{
    if (this->original_text.IsEmpty()) return; // prevent deletion of everything
    this->text_area->Clear();
    this->text_area->SetValue(this->original_text);
}


void FsqlfGui::onUseConfigNlOther(wxCommandEvent &event)
{
    if (event.IsChecked()) {
        this->nl_after_select->Disable();
        this->nl_before_or->Disable();
        this->nl_after_or->Disable();
        this->nl_before_and->Disable();
        this->nl_after_and->Disable();
    } else {
        this->nl_after_select->Enable();
        this->nl_before_or->Enable();
        this->nl_after_or->Enable();
        this->nl_before_and->Enable();
        this->nl_after_and->Enable();
    }
}
