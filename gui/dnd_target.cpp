#include "dnd_target.hpp"



bool dnd_target::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames){
    this->text_area->LoadFile(filenames[0]);
};


dnd_target::dnd_target(wxTextCtrl* text_area){
    this->text_area = text_area;
};
