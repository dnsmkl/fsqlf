#ifndef DND_TARGET_HPP
#define DND_TARGET_HPP

#include <wx/wx.h>
#include <wx/dnd.h>



// copy paste from `wx_fsqlf.hpp`
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


//endif DND_TARGET_HPP
#endif
