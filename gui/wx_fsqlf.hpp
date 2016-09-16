#ifndef WX_FSQLF_HPP
#define WX_FSQLF_HPP


#include <wx/app.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dir.h>
#include <wx/font.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/statbox.h>
#include "basic_notepad.hpp"


class FsqlfGui: public BasicNotepad
{
public:
    FsqlfGui();

private:
    wxString original_text;

    wxRadioBox *sel_comma_nl, *case_all_kw, *nl_major_sections;
    wxCheckBox* nl_after_select;
    wxCheckBox *nl_use_config;
    wxCheckBox *nl_before_or, *nl_after_or;
    wxCheckBox *nl_before_and, *nl_after_and;
    wxCheckBox* use_original_text;

    void create_buttons(wxSizer* parent_sizer);
    void create_options(wxNotebook*);
    void create_options_nl_comma(wxSizer*);
    void create_options_nl_keywords(wxSizer*);
    void create_options_nl_major_sections(wxSizer*);
    void create_options_text(wxSizer* sizer);

    void onUnformat(wxCommandEvent &event);
    void onFormat(wxCommandEvent &event);
    void onUseConfigNlOther(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
};


class MainApp : public wxApp
{
public:
    virtual bool OnInit() {
        FsqlfGui *main = new FsqlfGui();
        main->Show(true);

        return true;
    }
};


IMPLEMENT_APP(MainApp)


//endif WX_FSQLF_HPP
#endif
