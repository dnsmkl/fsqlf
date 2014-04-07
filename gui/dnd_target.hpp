#ifndef DND_TARGET_HPP
#define DND_TARGET_HPP

#include <wx/wx.h>
#include <wx/dnd.h>



class dnd_target : public wxFileDropTarget{
    private:
    wxTextCtrl* text_area;
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames);

    public:
    dnd_target(wxTextCtrl* text_area);
};


//endif DND_TARGET_HPP
#endif
