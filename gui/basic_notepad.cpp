#include <wx/aboutdlg.h>

#include "basic_notepad.hpp"

#include "fsqlf_right.xpm"
#include "license_text.h"


enum EventIds
{
    idSave = wxID_HIGHEST,
    idOpen,
    idExit,
    idAbout,
    idCut,
    idCopy,
    idPaste,
    idSelectAll,
    idMaxLen
};


BEGIN_EVENT_TABLE(BasicNotepad, wxFrame)
    EVT_MENU(idSave, BasicNotepad::onSave)
    EVT_MENU(idOpen, BasicNotepad::onOpen)
    EVT_MENU(idExit, BasicNotepad::onExit)

    EVT_MENU(idCut       , BasicNotepad::onCut)
    EVT_MENU(idCopy      , BasicNotepad::onCopy)
    EVT_MENU(idPaste     , BasicNotepad::onPaste)
    EVT_MENU(idSelectAll , BasicNotepad::onSelectAll)

    EVT_TEXT_MAXLEN(idMaxLen, BasicNotepad::onMaxLen)


    EVT_MENU(idAbout, BasicNotepad::onAbout)
END_EVENT_TABLE()


BasicNotepad::BasicNotepad(const wxString title):
    wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(650,500)),
    text_area(new wxTextCtrl(this, wxID_ANY, _(""), wxDefaultPosition,
        wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_MULTILINE)),
    dnd_target(new DndTarget(this->text_area)),
    file_menu(new wxMenu()),
    edit_menu(new wxMenu()),
    help_menu(new wxMenu())
{
    SetIcon(wxIcon(fsqlf_right));
    this->create_menubar();
}


void BasicNotepad::create_textarea(wxSizer* parent_sizer)
{
    text_area->SetFont(wxFont(10, wxTELETYPE, wxNORMAL, wxNORMAL));
    parent_sizer->Add(this->text_area,1,wxEXPAND,0);
}


void BasicNotepad::create_menubar()
{
    wxMenuBar* menu;

    menu = new wxMenuBar();

    menu->Append(file_menu, _("&File"));
    menu->Append(edit_menu, _("&Edit"));
    menu->Append(help_menu, _("&Help"));

    file_menu->Append(idSave, _("&Save File\tCtrl-S"));
    file_menu->Append(idOpen, _("&Open File\tCtrl-O"));
    file_menu->AppendSeparator();
    file_menu->Append(idExit, _("E&xit\tAlt-F4"));

    edit_menu->Append(idCut   , _("Cu&t\tCtrl-X"));
    edit_menu->Append(idCopy  , _("&Copy\tCtrl-C"));
    edit_menu->Append(idPaste , _("&Paste\tCtrl-V"));
    edit_menu->AppendSeparator();
    edit_menu->Append(idSelectAll, _("Select &All\tCtrl-A"));

    help_menu->Append(idAbout, _("&About...\tAlt-F1"));

    this->SetMenuBar(menu);
}


void BasicNotepad::onSave( wxCommandEvent &event)
{
    wxFileDialog *saveDialog = new wxFileDialog(this, _("Save File~"), _(""), _(""), _("SQL (*.sql)|*.sql|All (*.*)|*.*"), wxFD_SAVE);
    if (wxID_OK == saveDialog->ShowModal()) this->text_area->SaveFile(saveDialog->GetPath());
}


void BasicNotepad::onOpen( wxCommandEvent &event)
{
    wxFileDialog *openDialog = new wxFileDialog(this, _("Open File~"), _(""), _(""), _("SQL (*.sql)|*.sql|All (*.*)|*.*"), wxFD_OPEN);
    if (wxID_OK == openDialog->ShowModal()) this->text_area->LoadFile(openDialog->GetPath());
}


void BasicNotepad::onExit(wxCommandEvent &event)
{
    //this->Destroy();
}


void BasicNotepad::onCopy(wxCommandEvent &event)
{
    this->text_area->Copy();
}


void BasicNotepad::onCut(wxCommandEvent &event)
{
    this->text_area->Cut();
}


void BasicNotepad::onPaste(wxCommandEvent &event)
{
    this->text_area->Paste();
}


void BasicNotepad::onSelectAll(wxCommandEvent &event)
{
    this->text_area->SetSelection(-1, -1);
}


void BasicNotepad::onAbout(wxCommandEvent &event)
{
    wxAboutDialogInfo info;
    info.SetName(_("Free SQL Formatter"));
    info.SetVersion(_(VERSION));
    info.SetDescription(_T("Free SQL Formatter beautifies SQL code. It is particularly useful in case one has to deal with machine generated SQL code"));
    info.SetCopyright(_T("(C) 2011-2016 Danas Mikelinskas <danas.mikelinskas@gmail.com>"));
    info.SetLicence(wxString(LICENSE_TEXT, wxConvUTF8, LICENSE_TEXT_LEN));
    wxAboutBox(info);
}


void BasicNotepad::onMaxLen(wxCommandEvent &event)
{
    new wxMessageDialog(this, _("Maximum Limit Reached"), _(""), wxOK|wxCENTRE );
}
