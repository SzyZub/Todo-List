#include <wx/wx.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

class MyFrame : public wxFrame
{
public:
    wxListBox* taskBox;
    wxTextCtrl* write;
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Todo list", wxDefaultPosition, wxSize(800, 600)) 
    {
        Centre();
        wxFont f = this->GetFont();
        f.SetPointSize(18);
        this->SetFont(f);
        this->SetMinSize(wxSize(450, 300));       
    }
    void _mainFunc() {
        wxPanel* panel = new wxPanel(this, -1);
        wxBoxSizer* vertBox = new wxBoxSizer(wxVERTICAL);
        panel->SetSizer(vertBox);
        vertBox->Fit(panel);
        panel->Layout();
        wxBoxSizer* firstRow = new wxBoxSizer(wxHORIZONTAL);
        wxBoxSizer* secondRow = new wxBoxSizer(wxHORIZONTAL);
        wxBoxSizer* thirdRow = new wxBoxSizer(wxHORIZONTAL);
        write = new wxTextCtrl(panel, 3, "Tasks");
        firstRow->Add(write, 1, wxEXPAND, 0);
        vertBox->Add(firstRow, 1, wxEXPAND, 0);
        taskBox = new wxListBox(panel, 4, wxDefaultPosition, wxDefaultSize);
        _FillListBox();
        wxFont listFont = this->GetFont();
        listFont.SetPointSize(10);
        taskBox->SetFont(listFont);
        secondRow->Add(taskBox, 1, wxEXPAND, 0);
        vertBox->Add(secondRow, 3, wxEXPAND, 0);
        wxButton* add = new wxButton(panel, 5, "Add a task");
        wxButton* del = new wxButton(panel, 6, "Delete a task");
        thirdRow->Add(add, 1, wxEXPAND, 0);
        thirdRow->Add(del, 1, wxEXPAND, 0);
        vertBox->Add(thirdRow, 1, wxEXPAND, 0);
        add->Bind(wxEVT_BUTTON, &MyFrame::_AddTask, this);
        del->Bind(wxEVT_BUTTON, &MyFrame::_DeleteTask, this);
        Bind(wxEVT_CLOSE_WINDOW, &MyFrame::_OnClose, this);
    }
    void _AddTask(wxCommandEvent &event) {
        wxString task = write->GetValue().Trim();
        if (!task.IsEmpty()) {
            taskBox->Append(task);
        }
    }
    void _DeleteTask(wxCommandEvent& event) {
        int sel = taskBox->GetSelection();
        if (sel != wxNOT_FOUND) {
            taskBox->Delete(sel);
        }    
    }
    void _OnClose(wxCloseEvent& event) {
        wxFileOutputStream outputStream("listbox_data.txt");
        if (!outputStream.IsOk()) {
            wxLogError("Cannot save to file 'listbox_data.txt'.");
            return;
        } 
        wxTextOutputStream textStream(outputStream);
        for (int i = 0; i < taskBox->GetCount(); i++) {
            textStream << taskBox->GetString(i) << "\n";
        }
        event.Skip();
    }
    void _FillListBox() {
        if (!wxFileExists("listbox_data.txt")) {
            return;
        }
        wxFileInputStream inputStream("listbox_data.txt");
        if (!inputStream.IsOk()) {
            return;
        }
        wxTextInputStream textStream(inputStream);
        while (!inputStream.Eof()) {
            wxString line = textStream.ReadLine();
            if (!line.IsEmpty()) {
                taskBox->Append(line);
            }
        }
    }
};

class MyApp : public wxApp
{
public:
    bool OnInit() {
        MyFrame* frame = new MyFrame();
        frame->_mainFunc();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
