#include "wx/wx.h"
#include <wx/dialog.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/font.h>
#include <wx/string.h>

#include "sudoku.h"

//-------------------------
// Class/Function declaration
//-------------------------

class MyApp : public wxApp
{
public:
  virtual bool OnInit();
};


class MyFrame : public wxFrame
{
public:
    MyFrame();

    //wxObjects
    wxListBox *m_list1;
    wxGrid* sudokuGridTable;
    wxBoxSizer* sudokuSizerPanel;
    wxPanel* mainPanel;
    wxMenuBar* menuBar;
    wxMenu* fileMenu;
    wxMenu* helpMenu;

    sudoku* sudokuObj;

    // Event handlers
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSolve(wxCommandEvent& event);
    void OnEmpty(wxCommandEvent& event);

    enum
    {
        ID_SOLVE = 1042,
        ID_EMPTY = 1666
    };
    
    // This class handles events
    DECLARE_EVENT_TABLE();

private:
    int cellSize = 50;
    wxString tmp = "test";
    int twoDsudoku[9][9];
    int nrTmp;
};

//-------------------------
// Macro stuff
//-------------------------

// Event table for MyFrame
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(wxID_EXIT,  MyFrame::OnQuit)
    EVT_MENU(ID_SOLVE,  MyFrame::OnSolve)
    EVT_MENU(ID_EMPTY,  MyFrame::OnEmpty)
END_EVENT_TABLE()

// Implements MyApp& GetApp()
wxDECLARE_APP(MyApp);
// Give wxWidgets the means to create a MyApp object
wxIMPLEMENT_APP(MyApp);

//-------------------------
// function definition
//-------------------------

bool MyApp::OnInit()
{
    // Initialisiert App
    MyFrame* m_frame = new MyFrame();
    m_frame->Show();
    return true;
}

MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, "sudoku42" , wxPoint(30, 30), wxSize(640, 400), wxCLOSE_BOX | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCAPTION | wxSYSTEM_MENU | wxCLIP_CHILDREN)
{
    //----------------
    //Create the menu
    //----------------

    fileMenu = new wxMenu;
    helpMenu = new wxMenu;

    helpMenu->Append(wxID_ABOUT, "&About...\tF1", "Show about dialog");
    fileMenu->Append(wxID_EXIT, "&Exit\tAlt-X", "Quit this program");
    fileMenu->Append(ID_EMPTY, "&Empty\tCtrl-E", "Empty the Sudoku");
    fileMenu->Append(ID_SOLVE, "&Solve\tCtrl-S", "Solve the Sudoku");

    //--------
    //Menu bar
    //--------

    // Now append the freshly created menu to the menu bar...
    menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    //attach this menu bar to the frame
    SetMenuBar(menuBar);

    //-----------
    //Status bar
    //-----------

    CreateStatusBar(2);
    SetStatusText("Welcome to sudoku42");

    //-------------------------
    // Creates a new Panel child of myFrame
    //-------------------------

    mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

    //-------------------------
    // Creates a new sizer for the panel
    //-------------------------
    
    sudokuSizerPanel = new wxBoxSizer(wxVERTICAL);

    //-------------------------
    // Creates a new grid child of panel
    //-------------------------
    
    sudokuGridTable = new wxGrid(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxUINT8_MAX);

    // Grid
	sudokuGridTable->CreateGrid(9, 9);
	sudokuGridTable->EnableEditing(true);
	sudokuGridTable->EnableGridLines(true);
    sudokuGridTable->SetMargins(0, 0);
	sudokuGridTable->EnableDragGridSize(false);

    // Col
	sudokuGridTable->EnableDragColMove(false);
	sudokuGridTable->EnableDragColSize(false);
	sudokuGridTable->HideColLabels();

	// Row
	sudokuGridTable->EnableDragRowSize(false);
	sudokuGridTable->HideRowLabels();

    // Set sizes
    for (int i = 0; i < 9; i++)
    {
        sudokuGridTable->SetColSize(i, cellSize);
        sudokuGridTable->SetRowSize(i, cellSize);
    }

    for (int a = 0; a < 9; a++)
    {
        for (int  b = 0; b < 9; b++)
        {
            sudokuGridTable->SetCellFont(a, b, wxFontInfo(40));
        }
    }

	sudokuGridTable->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

    // add the grid to the sizer
	sudokuSizerPanel->Add(sudokuGridTable, 1, wxALL | wxEXPAND);
    // add the sizer to panel, layout and fit it
	mainPanel->SetSizer(sudokuSizerPanel);
	mainPanel->Layout();
	sudokuSizerPanel->Fit(this);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    //Blöng sound
    wxBell();

    //Message window
    wxString msg;
    msg.Printf("Hello and welcome to sudoku42. A weekend project by fidoiel made with C++ and %s", wxVERSION_STRING);
    wxMessageBox(msg, "About sudoku42", wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnSolve(wxCommandEvent& event)
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            tmp = sudokuGridTable->GetCellValue(row, col);
            nrTmp = wxAtoi(tmp);
            if (nrTmp > 9 || nrTmp < 0)
            {
                tmp = wxString::Format(wxT("Input error on Colum %i Row %i"), (int)(col+1), (int)(row+1));
                SetStatusText(tmp);
                return;
            }
            twoDsudoku[row][col] = nrTmp;
        }
    }
    
    sudokuObj = new sudoku(twoDsudoku);    
    SetStatusText("Solving sudoku...");
    sudokuObj->solve();

    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            tmp = wxString::Format(wxT("%i"), sudokuObj->sudokuGrid[row][col]);
            sudokuGridTable->SetCellValue(row, col, tmp);
        }
    }
    SetStatusText("Solution found");
}

void MyFrame::OnEmpty(wxCommandEvent& event)
{
    for (int a = 0; a < 9; a++)
    {
        for (int  b = 0; b < 9; b++)
        {
            sudokuGridTable->SetCellValue(a, b, "");
        }
    }
}
    

void MyFrame::OnQuit(wxCommandEvent& event)
{
    // Destroy the frame
    Close();
}