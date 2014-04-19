#ifndef WX_FSQLF_HPP
#define WX_FSQLF_HPP

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/dnd.h>
#include <wx/aboutdlg.h>
#include <wx/font.h>
#include <wx/notebook.h>

#include "fsqlf_right.xpm"
#include "license_text.h"



class Notepad : public wxFrame {
    public:
    Notepad();

    private:
    wxTextCtrl* text_area;
    wxString original_text;

    wxRadioBox *sel_comma_nl, *case_all_kw, *nl_major_sections;
    wxCheckBox* nl_after_select;
    wxCheckBox *nl_use_config, *nl_before_or, *nl_after_or, *nl_before_and, *nl_after_and;
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
    void OnMaxLen(wxCommandEvent &event);

    void create_menubar();
    void create_buttons(wxSizer* parent_sizer);
    void create_options(wxNotebook*);
    void create_options_nl_comma(wxSizer*);
    void create_options_nl_keywords(wxSizer*);
    void create_options_nl_major_sections(wxSizer*);
    void create_options_text(wxSizer* sizer);
    void create_textarea(wxSizer* parent);

    enum EventIds{
        idSave = 1000, idOpen, idExit, idAbout
        , idFormat, idUnformat
        , idCut, idCopy, idPaste, idSelectAll
        , idMaxLen
    };

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

    EVT_TEXT_MAXLEN(idMaxLen, Notepad::OnMaxLen)
END_EVENT_TABLE()


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



//endif WX_FSQLF_HPP
#endif
