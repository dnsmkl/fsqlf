#ifndef BASIC_NOTEPAD_HPP
#define BASIC_NOTEPAD_HPP


#include <wx/wx.h>

#include "dnd_target.hpp"


class BasicNotepad: public wxFrame
{
protected:
    wxTextCtrl* text_area;
    DndTarget* dnd_target;
protected:
    wxMenu *file_menu, *edit_menu, *help_menu;

public:
    BasicNotepad(const wxString title);
    void create_textarea(wxSizer* parent);

private:
    void create_menubar();

    void onSave(wxCommandEvent &event);
    void onOpen(wxCommandEvent &event);
    void onExit(wxCommandEvent &event);
    void onCopy(wxCommandEvent &event);
    void onCut(wxCommandEvent &event);
    void onPaste(wxCommandEvent &event);
    void onSelectAll(wxCommandEvent &event);
    void onAbout(wxCommandEvent &event);
    void onMaxLen(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
};


//endif BASIC_NOTEPAD_HPP
#endif
