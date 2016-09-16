#ifndef DND_TARGET_HPP
#define DND_TARGET_HPP


#include <wx/dnd.h>
#include <wx/textctrl.h>


// Drag and drop target
// Note:
// Notepad it self can not ingerit from wxFileDropTarget
// because this causes segfault at exit
// (See also: http://stackoverflow.com/a/7096992/788634)
class DndTarget: public wxFileDropTarget
{
    private:
    wxTextCtrl* text_area;
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames);

    public:
    DndTarget(wxTextCtrl* text_area);
};


//endif DND_TARGET_HPP
#endif
