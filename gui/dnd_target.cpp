#include "dnd_target.hpp"


bool DndTarget::OnDropFiles(wxCoord x, wxCoord y,
    const wxArrayString &filenames)
{
    this->text_area->LoadFile(filenames[0]);
}


DndTarget::DndTarget(wxTextCtrl* text_area)
{
    this->text_area = text_area;
    this->text_area->SetDropTarget(this);
}
