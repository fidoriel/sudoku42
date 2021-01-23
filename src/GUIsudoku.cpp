#include "wx/wx.h"
#include <wx/dialog.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/font.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/colour.h>
#include <wx/textfile.h>
#include <wx/msgdlg.h>
#include <wx/event.h>
#include <wx/app.h>

#include "sudoku.h"
#include <string>

#include "../icons/AppIcon.xpm"

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
    wxGrid* sudokuGridTable;
    wxFlexGridSizer* sudokuSizerPanel;
    wxBoxSizer* buttonSizerPanel;
    wxPanel* mainPanel;
    wxMenuBar* menuBar;
    wxTextFile* file;
    wxGridCellAttr* attr;

    wxMenu* fileMenu;
    wxMenu* helpMenu;
    wxMenu* sudokuMenu;

    wxButton* solveBtn;
    wxButton* emptyBtn;

    sudoku* sudokuObj;

    // Event handlers
    void OnQuit( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );
    void OnSolve( wxCommandEvent& event );
    void OnSave( wxCommandEvent& event );
    void OnSaveAs( wxCommandEvent& event );
    void OnOpen( wxCommandEvent& event );
    void OnEmpty( wxCommandEvent& event );
    void OnNewFrame( wxCommandEvent& event );
    void OnClose( wxCloseEvent& event );

    enum
    {
        ID_SOLVE = 1042,
        ID_EMPTY = 1666,
        ID_NEW = 1012
    };
    
    // This class handles events
    DECLARE_EVENT_TABLE();

private:
    int cellSize = 50;
    wxString tmp;
    wxString path;
    int twoDsudoku[ 9 ][ 9 ];
    int nrTmp;
    bool isSolved = false;
    bool isSaved = false;

    bool save();
    bool checkIfSudokuIsEmpty();
};

/*class CellRectangleBlackSector : public wxGridCellStringRenderer
{
public:
    virtual void Draw( wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected ) wxOVERRIDE;
};*/

//-------------------------
// Macro stuff
//-------------------------

// Event table for MyFrame
BEGIN_EVENT_TABLE( MyFrame, wxFrame )
    EVT_MENU( wxID_ABOUT, MyFrame::OnAbout )
    EVT_MENU( wxID_EXIT,  MyFrame::OnQuit )
    EVT_MENU( wxID_SAVE,  MyFrame::OnSave )
    EVT_MENU( wxID_SAVEAS,MyFrame::OnSaveAs )
    EVT_MENU( wxID_OPEN,  MyFrame::OnOpen )
    EVT_MENU( ID_SOLVE,   MyFrame::OnSolve )
    EVT_MENU( ID_EMPTY,   MyFrame::OnEmpty )
    EVT_MENU( ID_NEW,     MyFrame::OnNewFrame )

    EVT_BUTTON( ID_SOLVE, MyFrame::OnSolve )
    EVT_BUTTON( ID_EMPTY, MyFrame::OnEmpty )

    EVT_CLOSE( MyFrame::OnClose )
    EVT_END_SESSION( MyFrame::OnClose )
END_EVENT_TABLE()

// Implements MyApp& GetApp()
wxDECLARE_APP( MyApp );
// Give wxWidgets the means to create a MyApp object
wxIMPLEMENT_APP( MyApp );

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

class sudokuRenderer : public wxGridCellStringRenderer
{
public:
    virtual void Draw( wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected ) wxOVERRIDE;
};

MyFrame::MyFrame() : wxFrame( NULL, wxID_ANY, "sudoku42" , wxPoint( 30, 30 ), wxSize( 640, 400 ), wxCLOSE_BOX | wxMINIMIZE_BOX /*| wxMAXIMIZE_BOX */| wxCAPTION | wxSYSTEM_MENU | wxCLIP_CHILDREN )
{

    // add the appicon

    SetIcon( wxICON( AppIcon ) );

    //----------------
    //Create the menu
    //----------------

    fileMenu = new wxMenu;
    sudokuMenu = new wxMenu;
    helpMenu = new wxMenu;

    helpMenu->Append( wxID_ABOUT, "&About...\tF1", "Show about dialog" );
    fileMenu->Append( wxID_EXIT, "&Exit\tAlt-X", "Quit this program" );
    //fileMenu->Append( ID_NEW, "&New\tCtrl-N", "Open new window" );
    fileMenu->Append( wxID_OPEN, "&Open\tCtrl-O", "Open a .s42 file" );
    fileMenu->Append( wxID_SAVE, "&Save\tCtrl-S", "Save the sudoku template" );
    fileMenu->Append( wxID_SAVEAS, "&Save As...\tCtrl-Shift-S", "Save the sudoku template" );
    sudokuMenu->Append( ID_EMPTY, "&Empty\tCtrl-E", "Empty the Sudoku" );
    sudokuMenu->Append( ID_SOLVE, "&Solve\tCtrl-F", "Solve the Sudoku" );

    //--------
    //Menu bar
    //--------
    // Now append the freshly created menu to the menu bar...
    menuBar = new wxMenuBar();
    menuBar->Append( fileMenu, "&File" );
    menuBar->Append( sudokuMenu, "&Sudoku" );
    menuBar->Append( helpMenu, "&Help" );

    //attach this menu bar to the frame
    SetMenuBar( menuBar );

    //-----------
    //Status bar
    //-----------

    CreateStatusBar( 1 );
    SetStatusText( "Welcome to sudoku42" );

    //-------------------------
    // Creates a new Panel child of myFrame
    //-------------------------

    mainPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

    //-------------------------
    // Creates a new sizer for the panel
    //-------------------------
    
    sudokuSizerPanel = new wxFlexGridSizer( 3, 1, 0, 0 );
    buttonSizerPanel = new wxBoxSizer( wxHORIZONTAL );

    //-------------------------
    // Creates a new grid child of panel
    //-------------------------
    
    sudokuGridTable = new wxGrid( mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize );

    // Grid
    sudokuGridTable->CreateGrid( 9, 9 );

    sudokuGridTable->EnableEditing( true );
    sudokuGridTable->SetMargins( -2, -2 );
    sudokuGridTable->EnableDragGridSize( false );
    sudokuGridTable->SetMargins( 0, 0 );
    sudokuGridTable->EnableGridLines( false );
    sudokuGridTable->SetCellHighlightROPenWidth( 0 );
    sudokuGridTable->SetCellHighlightPenWidth( 0 );

    // Col
	sudokuGridTable->EnableDragColMove( false );
	sudokuGridTable->HideColLabels();

	// Row
	sudokuGridTable->HideRowLabels();
       
    // Set sizes
    attr = new wxGridCellAttr;
    attr->SetBackgroundColour( *wxWHITE );
    attr->SetTextColour( *wxBLACK );
    attr->SetFont( wxFontInfo( 40 ) );
    
    for ( int i = 0; i < 9; i++ )
    {
        sudokuGridTable->SetColSize( i, cellSize );
        sudokuGridTable->SetRowSize( i, cellSize );
    }

    for ( size_t col = 0; col < 9; col++ )
    {
        for ( size_t row = 0; row < 9; row++ )
        {
            sudokuGridTable->SetAttr( row, col, attr);
            sudokuGridTable->SetCellRenderer( row, col, new sudokuRenderer() );
        }
    }

	sudokuGridTable->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

    solveBtn = new wxButton( mainPanel, ID_SOLVE, "Solve", wxDefaultPosition, wxSize( 150, 30 ) );
    emptyBtn = new wxButton( mainPanel, ID_EMPTY, "Empty", wxDefaultPosition, wxSize( 150, 30 ) );

    // add the grid and buttons to the sizer
    buttonSizerPanel->Add( solveBtn, 0, wxALL );
    buttonSizerPanel->AddSpacer( 10 );
    buttonSizerPanel->Add( emptyBtn, 0, wxALL );

    sudokuSizerPanel->Add( sudokuGridTable, 0, wxALL | wxEXPAND, 10 );

    sudokuSizerPanel->Add( buttonSizerPanel, 0, wxALIGN_CENTER, 20 );
    // add the sizer to panel, layout and fit it
    sudokuSizerPanel->AddSpacer( 20 );
    mainPanel->SetSizer( sudokuSizerPanel );
    mainPanel->Layout();
	sudokuSizerPanel->Fit( this );
    sudokuSizerPanel->SetSizeHints( this );
}

void MyFrame::OnAbout( wxCommandEvent& event )
{
    //Blöng sound
    wxBell();

    //Message window
    wxString msg;
    msg.Printf( "Hello and welcome to sudoku42. A weekend project by fidoiel made with C++ and %s", wxVERSION_STRING );
    wxMessageBox( msg, "About sudoku42", wxOK | wxICON_INFORMATION, this );
}

void MyFrame::OnSave( wxCommandEvent& event )
{
    //Skip Event
    //event.Skip();

    if ( !this->path.IsEmpty() )
    {
        this->save();
    }
    else
    {
        this->OnSaveAs( event );
    }
}

void MyFrame::OnSaveAs( wxCommandEvent& event )
{
    //Skip Event
    //event.Skip();

    wxFileDialog saveDialog( this, "Save s42 file", wxEmptyString, "sudoku", "sudoku42 files ( *.s42 )|*.s42", wxFD_SAVE | wxFD_OVERWRITE_PROMPT );
    if ( saveDialog.ShowModal() == wxID_OK )
    {
        SetStatusText( "Saving file..." );
        this->path = saveDialog.GetPath();
        this->save();
    }
    else
    {
        //Nutzer entschied sich die datei nicht zu Speichern
        SetStatusText( "User canceled to save the file" );
        return;
    }
}

void MyFrame::OnOpen( wxCommandEvent& event )
{
    wxFileDialog openDialog( this, "Open s42 file", "", "", "sudoku42 files ( *.s42 )|*.s42", wxFD_OPEN | wxFD_FILE_MUST_EXIST );
    if ( openDialog.ShowModal() == wxID_OK )
    {
        SetStatusText( "Opening file..." );
        this->path = openDialog.GetPath();
    }
    else
    {
        //Nutzer entschied sich die datei nicht zu Speichern
        SetStatusText( "User canceled to open the file" );
        return;
    }

    this->file = new wxTextFile( this->path );
    this->file->Open();
    this->SetLabel( this->path );

    //übertrage Dateiinhalt in string und dann in Array
    std::string line = file->GetFirstLine().ToStdString();

    for ( int col = 0; col < 9; col++ )
    {
        this->twoDsudoku[ 0 ][ col ] = line[ col ] - 48;
    }

    for ( int row = 1; row < 9; row++ )
    {
        std::string line = this->file->GetNextLine().ToStdString();
        for ( int col = 0; col < 9; col++ )
        {
            this->twoDsudoku[ row ][ col ] = line[ col ] - 48;
        }
    }
    
    this->file->Close();

    //Übertrage Array in grid
    for ( int row = 0; row < 9; row++ )
    {
        for ( int col = 0; col < 9; col++ )
        {
            if ( twoDsudoku[ row ][ col ] )
            {
                tmp = wxString::Format( wxT( "%i" ), twoDsudoku[ row ][ col ] );
                sudokuGridTable->SetCellValue( row, col, tmp );
            }
        }
    }
    SetStatusText( "File succesfully opened" );
}

void MyFrame::OnNewFrame( wxCommandEvent& event )
{
    /*
    MyFrame* m_frame = new MyFrame();
    m_frame->Show();
    */
}

void MyFrame::OnSolve( wxCommandEvent& event )
{
    //perform accepted number test and transform grid to array
    for ( int row = 0; row < 9; row++ )
    {
        for ( int col = 0; col < 9; col++ )
        {
            tmp = sudokuGridTable->GetCellValue( row, col );
            nrTmp = wxAtoi( tmp );
            if ( nrTmp > 9 || nrTmp < 0 )
            {
                tmp = wxString::Format( wxT( "Input error on Colum %i Row %i" ), ( int )( col+1 ), ( int )( row+1 ) );
                SetStatusText( tmp );
                return;
            }
            twoDsudoku[ row ][ col ] = nrTmp;
        }
    }

    //Highlight given cells green
    for ( int row = 0; row < 9; row++ )
    {
        for ( int col = 0; col < 9; col++ )
        {
            if ( twoDsudoku[ row ][ col ] > 0 )
            {
                sudokuGridTable->SetCellBackgroundColour( row, col, *wxGREEN );
            }
        }
    }
    
    sudokuObj = new sudoku( twoDsudoku );

    //Perform input check
    for ( int row = 0; row < 9; row++ )
    {
        for ( int col = 0; col < 9; col++ )
        {
            if ( sudokuObj->sudokuGrid[ row ][ col ] > 0 )
            {
                int tmp = sudokuObj->sudokuGrid[ row ][ col ];
                sudokuObj->sudokuGrid[ row ][ col ] = 0;
                if( !sudokuObj->checkIfPossible( row, col, tmp ) )
                {
                    SetStatusText( "Input error" );
                    return;
                }
                sudokuObj->sudokuGrid[ row ][ col ] = tmp;
            }
        }
    }
    
        
    SetStatusText( "Solving sudoku..." );
    sudokuObj->solve();

    for ( int row = 0; row < 9; row++ )
    {
        for ( int col = 0; col < 9; col++ )
        {
            tmp = wxString::Format( wxT( "%i" ), sudokuObj->sudokuGrid[ row ][ col ] );
            sudokuGridTable->SetCellValue( row, col, tmp );
        }
    }
    tmp = wxString::Format( wxT( "Solution found. It took %i steps to do so." ), ( int )( sudokuObj->getSteps() ) );
    SetStatusText( tmp );
    this->isSolved = true;
    this->isSaved = false;
}

void MyFrame::OnEmpty( wxCommandEvent& event )
{
    this->isSolved = false;
    for ( int a = 0; a < 9; a++ )
    {
        for ( int  b = 0; b < 9; b++ )
        {
            sudokuGridTable->SetCellValue( a, b, "" );
            sudokuGridTable->SetCellBackgroundColour( a, b, *wxWHITE );
        }
    }
}

void MyFrame::OnQuit( wxCommandEvent& event )
{
    Close();
}

void MyFrame::OnClose( wxCloseEvent& event )
{
    if ( !this->checkIfSudokuIsEmpty() )
    {
        if ( !this->isSaved or this->path.IsEmpty() )
        {
            wxMessageDialog dialog( this, "Do you want to save the file?", "Disacard?", wxYES_NO | wxCANCEL | wxICON_INFORMATION );
            switch ( dialog.ShowModal() )
            {
            case wxID_YES:
                if ( !this->path.IsEmpty() )
                {
                    this->save();
                }
                else
                {
                    wxFileDialog saveDialog( this, "Save s42 file", wxEmptyString, "sudoku", "sudoku42 files ( *.s42 )|*.s42", wxFD_SAVE | wxFD_OVERWRITE_PROMPT );
                    if ( saveDialog.ShowModal() == wxID_OK )
                    {
                        SetStatusText( "Saving file..." );
                        this->path = saveDialog.GetPath();
                        this->save();
                    }
                    else
                    {
                        //Nutzer entschied sich die datei nicht zu Speichern
                        SetStatusText( "User canceled to save the file" );
                    }
                }
                break;

            case wxID_NO:
                break;

            case wxID_CANCEL:
                return;
                break;

            default:
                break;
            }
        }
    }
    event.Skip();
}

bool MyFrame::save()
{
    this->file = new wxTextFile( this->path );
    this->SetLabel( this->path );
    if ( this->file->Exists() )
    {
        this->file->Open();
        this->file->Clear();
        this->file->Write();
    }
    else
    {
        this->file->Create();
    }
    
    for ( int row = 0; row < 9; row++ )
    {
        for ( int col = 0; col < 9; col++ )
        {
            tmp = sudokuGridTable->GetCellValue( row, col );
            nrTmp = wxAtoi( tmp );

            if ( nrTmp > 9 || nrTmp < 0 )
            {
                tmp = wxString::Format( wxT( "File saving error. Input error on Colum %i Row %i." ), ( int )( col+1 ), ( int )( row+1 ) );
                SetStatusText( tmp );
                this->file->Close();
                return false;
            }
            if ( sudokuGridTable->GetCellBackgroundColour( row, col ).GetAsString( wxC2S_NAME ) != "white" && this->isSolved )
            {
                this->twoDsudoku[ row ][ col ] = nrTmp;
            }
            if ( sudokuGridTable->GetCellBackgroundColour( row, col ).GetAsString( wxC2S_NAME ) == "white" && this->isSolved )
            {
                this->twoDsudoku[ row ][ col ] = 0;
            }
            else
            {
                this->twoDsudoku[ row ][ col ] = nrTmp;
            }     
        }
    }

    std::string line( "" );

    for ( int row = 0; row < 9; row++ )
    {
        for ( int col = 0; col < 9; col++ )
        {
            line.append( std::to_string( this->twoDsudoku[ row ][ col ] ) );
        }
        this->file->AddLine( line );
        line = ( "" );
    }

    this->file->Write();
    this->file->Close();

    SetStatusText( "Saved file" );
    this->isSaved = true;
    return true;   
}

bool MyFrame::checkIfSudokuIsEmpty()
{
    for ( int row = 0; row < 9; row++ )
    {
        for ( int col = 0; col < 9; col++ )
        {
            if( wxAtoi( sudokuGridTable->GetCellValue( row, col ) ) )
            {
                return false;
            }
        }
    }
    return true;
}

void sudokuRenderer::Draw( wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected )
{
    wxGridCellStringRenderer::Draw( grid, attr, dc, rect, row, col, isSelected );

    dc.SetPen( *wxBLACK_PEN );
    dc.SetBackground( wxBrush( *wxGREEN, wxBRUSHSTYLE_SOLID ) );
    dc.SetBackgroundMode( wxBRUSHSTYLE_SOLID );
    wxPen pen = dc.GetPen();   

    int out = 12;
    int inner = 8/2;
    int line = 4/2;

    // Top
    switch ( row )
    {
    case 0:
        dc.SetPen( wxPen( *wxBLACK, out ) );
        break;

    case 3:
    case 6:
        dc.SetPen( wxPen( *wxBLACK, inner ) );
        break;
    
    default:
        dc.SetPen( wxPen( *wxBLACK, line ) );
        break;
    }
    dc.DrawLine( rect.GetTopLeft(), rect.GetTopRight() );

    // Left
    switch ( col )
    {
    case 0:
        dc.SetPen( wxPen( *wxBLACK, out ) );
        break;

    case 3:
    case 6:
        dc.SetPen( wxPen( *wxBLACK, inner ) );
        break;
    
    default:
        dc.SetPen( wxPen( *wxBLACK, line ) );
        break;
    }
    dc.DrawLine( rect.GetTopLeft(), rect.GetBottomLeft() );

    // Right
    switch ( col )
    {
    case 8:
        dc.SetPen( wxPen( *wxBLACK, out ) );
        break;

    case 2:
    case 5:
        dc.SetPen( wxPen( *wxBLACK, inner ) );
        break;
    
    default:
        dc.SetPen( wxPen( *wxBLACK, line ) );
        break;
    }
    dc.DrawLine( rect.GetTopRight(), rect.GetBottomRight() );

    // Bottom
    switch ( row )
    {
    case 8:
        dc.SetPen( wxPen( *wxBLACK, out ) );
        break;

    case 2:
    case 5:
        dc.SetPen( wxPen( *wxBLACK, inner ) );
        break;
    
    default:
        dc.SetPen( wxPen( *wxBLACK, line ) );
        break;
    }
    dc.DrawLine( rect.GetBottomLeft(), rect.GetBottomRight() );
}