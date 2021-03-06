/*
 * widgets.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: kobus
 */

#include <boost/thread/pthread/thread_data.hpp>
#include "widgets.h"


// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

TiDspParameters tiDspParams;
TiCameraParameters tiCameraParams;

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT,
    QUEUE_TIMER
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(MyFrame1, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame1::OnQuit)
    EVT_MENU(Minimal_About, MyFrame1::OnAbout)
    EVT_TIMER(QUEUE_TIMER, MyFrame1::OnQueueTimer)
wxEND_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
wxIMPLEMENT_APP_NO_MAIN(MyApp);
IMPLEMENT_WX_THEME_SUPPORT;


// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if ( !wxApp::OnInit() )
        return false;

    // create the main application window
    myFrame = new MyFrame1("Thermal imager pipeline");

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    myFrame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returneCannyd false here, the
    // application would exit immediately.
    return true;
}

void MyApp::InitQueues(Queues* qs)
{
	queues = qs;
	myFrame->InitQueues(qs);

    static const int INTERVAL = 1000; // milliseconds
    myFrame->qTimer = new wxTimer(myFrame, QUEUE_TIMER);
    myFrame->qTimer->Start(INTERVAL);
};



// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame1::MyFrame1(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
    /* -------------- Generated code (wxFormBuilder) ----------------------------------------------------------------*/
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer1;
    bSizer1 = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* bSizer2;
    bSizer2 = new wxBoxSizer( wxVERTICAL );

    wxGridSizer* gSizer1;
    gSizer1 = new wxGridSizer( 6, 2, 0, 0 );

    m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Output select"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText1->Wrap( -1 );
    m_staticText1->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, true, wxEmptyString ) );

    gSizer1->Add( m_staticText1, 0, wxALL, 5 );

    m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Active"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText2->Wrap( -1 );
    m_staticText2->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, true, wxEmptyString ) );

    gSizer1->Add( m_staticText2, 0, wxALL, 5 );

    m_buttonRaw = new wxButton( this, wxID_ANY, wxT("Raw"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer1->Add( m_buttonRaw, 0, wxALL, 5 );

    m_checkBoxRaw = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    m_checkBoxRaw->SetValue(true);
    m_checkBoxRaw->Enable( false );

    gSizer1->Add( m_checkBoxRaw, 0, wxALL, 5 );

    m_buttonDownscale = new wxButton( this, wxID_ANY, wxT("Downscale"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer1->Add( m_buttonDownscale, 0, wxALL, 5 );

    m_checkBoxGrayScale = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    m_checkBoxGrayScale->SetValue(true);
    m_checkBoxGrayScale->Enable( false );

    gSizer1->Add( m_checkBoxGrayScale, 0, wxALL, 5 );

    m_buttonUpscale = new wxButton( this, wxID_ANY, wxT("Upscale"), wxDefaultPosition, wxDefaultSize, 0 );
    m_buttonUpscale->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

    gSizer1->Add( m_buttonUpscale, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    m_checkBoxUpscale = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    m_checkBoxUpscale->SetValue(true);
    gSizer1->Add( m_checkBoxUpscale, 0, wxALL, 5 );

    m_buttonEdgeEnhance = new wxButton( this, wxID_ANY, wxT("Edge enhance"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer1->Add( m_buttonEdgeEnhance, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    m_checkBoxEdgeEnhance = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    gSizer1->Add( m_checkBoxEdgeEnhance, 0, wxALL, 5 );


    bSizer2->Add( gSizer1, 0, wxALIGN_LEFT|wxFIXED_MINSIZE, 5 );


    bSizer1->Add( bSizer2, 0, 0, 5 );

    wxBoxSizer* bSizer9;
    bSizer9 = new wxBoxSizer( wxVERTICAL );

    m_choicebook2 = new wxChoicebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
    m_panel_Edge_enhance = new wxPanel( m_choicebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer14;
    bSizer14 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* bSizer3;
    bSizer3 = new wxBoxSizer( wxHORIZONTAL );

    m_panelGaussian = new wxPanel( m_panel_Edge_enhance, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER|wxTAB_TRAVERSAL );
    m_panelGaussian->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT ) );

    wxBoxSizer* bSizer4;
    bSizer4 = new wxBoxSizer( wxVERTICAL );

    m_buttonGaussian = new wxButton( m_panelGaussian, wxID_ANY, wxT("Gaussian"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer4->Add( m_buttonGaussian, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );


    m_panelGaussian->SetSizer( bSizer4 );
    m_panelGaussian->Layout();
    bSizer4->Fit( m_panelGaussian );
    bSizer3->Add( m_panelGaussian, 1, wxEXPAND | wxALL, 5 );

    m_panelSharpen = new wxPanel( m_panel_Edge_enhance, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER|wxTAB_TRAVERSAL );
    m_panelSharpen->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

    wxBoxSizer* bSizer7;
    bSizer7 = new wxBoxSizer( wxVERTICAL );

    m_buttonSharpen = new wxButton( m_panelSharpen, wxID_ANY, wxT("Sharpen"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer7->Add( m_buttonSharpen, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );


    m_panelSharpen->SetSizer( bSizer7 );
    m_panelSharpen->Layout();
    bSizer7->Fit( m_panelSharpen );
    bSizer3->Add( m_panelSharpen, 1, wxEXPAND | wxALL, 5 );

    m_panelUnsharpMask = new wxPanel( m_panel_Edge_enhance, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER|wxTAB_TRAVERSAL );
    m_panelUnsharpMask->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

    wxBoxSizer* bSizer8;
    bSizer8 = new wxBoxSizer( wxVERTICAL );

    m_buttonUnsharpMask = new wxButton( m_panelUnsharpMask, wxID_ANY, wxT("Unsharp mask"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer8->Add( m_buttonUnsharpMask, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );


    m_panelUnsharpMask->SetSizer( bSizer8 );
    m_panelUnsharpMask->Layout();
    bSizer8->Fit( m_panelUnsharpMask );
    bSizer3->Add( m_panelUnsharpMask, 1, wxEXPAND | wxALL, 5 );


    bSizer14->Add( bSizer3, 1, wxEXPAND, 5 );


    m_panel_Edge_enhance->SetSizer( bSizer14 );
    m_panel_Edge_enhance->Layout();
    bSizer14->Fit( m_panel_Edge_enhance );
    m_choicebook2->AddPage( m_panel_Edge_enhance, wxT("Edge enhance"), false );
    m_panel_E_Zoom = new wxPanel( m_choicebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer161;
    bSizer161 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* bSizer17;
    bSizer17 = new wxBoxSizer( wxHORIZONTAL );

    m_radioBtnNoZoom = new wxRadioButton( m_panel_E_Zoom, wxID_ANY, wxT("None"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer17->Add( m_radioBtnNoZoom, 0, wxALL, 5 );

    m_radioBtn2xZoom = new wxRadioButton( m_panel_E_Zoom, wxID_ANY, wxT("2x"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer17->Add( m_radioBtn2xZoom, 0, wxALL, 5 );

    radioBtn4xZoom = new wxRadioButton( m_panel_E_Zoom, wxID_ANY, wxT("4x"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer17->Add( radioBtn4xZoom, 0, wxALL, 5 );

    radioBtn8xZoom = new wxRadioButton( m_panel_E_Zoom, wxID_ANY, wxT("8x"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer17->Add( radioBtn8xZoom, 0, wxALL, 5 );


    bSizer161->Add( bSizer17, 0, wxEXPAND, 5 );


    m_panel_E_Zoom->SetSizer( bSizer161 );
    m_panel_E_Zoom->Layout();
    bSizer161->Fit( m_panel_E_Zoom );
    m_choicebook2->AddPage( m_panel_E_Zoom, wxT("Upscale"), true );
    bSizer9->Add( m_choicebook2, 1, wxEXPAND, 5 );

    wxBoxSizer* bSizer10;
    bSizer10 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* bSizer11;
    bSizer11 = new wxBoxSizer( wxHORIZONTAL );

    m_staticText16 = new wxStaticText( this, wxID_ANY, wxT("Horisontal pixels:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText16->Wrap( -1 );
    bSizer11->Add( m_staticText16, 0, wxALL, 5 );

    m_staticTextHorisontalPixels = new wxStaticText( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextHorisontalPixels->Wrap( -1 );
    bSizer11->Add( m_staticTextHorisontalPixels, 0, wxALL, 5 );


    bSizer10->Add( bSizer11, 1, wxEXPAND, 5 );

    wxBoxSizer* bSizer12;
    bSizer12 = new wxBoxSizer( wxHORIZONTAL );

    m_staticText18 = new wxStaticText( this, wxID_ANY, wxT("Vertical pixels:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText18->Wrap( -1 );
    bSizer12->Add( m_staticText18, 0, wxALL, 5 );

    m_staticTextVerticalPixels = new wxStaticText( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextVerticalPixels->Wrap( -1 );
    bSizer12->Add( m_staticTextVerticalPixels, 0, wxALL, 5 );


    bSizer10->Add( bSizer12, 1, wxEXPAND, 5 );

    wxBoxSizer* bSizer13;
    bSizer13 = new wxBoxSizer( wxHORIZONTAL );

    m_staticText20 = new wxStaticText( this, wxID_ANY, wxT("Framerate"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText20->Wrap( -1 );
    bSizer13->Add( m_staticText20, 0, wxALL, 5 );

    m_staticTextFramerate = new wxStaticText( this, wxID_ANY, wxT("0 Hz"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextFramerate->Wrap( -1 );
    bSizer13->Add( m_staticTextFramerate, 0, wxALL, 5 );


    bSizer10->Add( bSizer13, 1, wxEXPAND, 5 );

    wxBoxSizer* bSizer141;
    bSizer141 = new wxBoxSizer( wxHORIZONTAL );

    m_staticText22 = new wxStaticText( this, wxID_ANY, wxT("Pipeline"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText22->Wrap( -1 );
    bSizer141->Add( m_staticText22, 0, wxALL, 5 );

    m_staticTextPipeline = new wxStaticText( this, wxID_ANY, wxT("0 Hz"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextPipeline->Wrap( -1 );
    bSizer141->Add( m_staticTextPipeline, 0, wxALL, 5 );


    bSizer10->Add( bSizer141, 1, wxEXPAND, 5 );

    wxBoxSizer* bSizer15;
    bSizer15 = new wxBoxSizer( wxHORIZONTAL );

    m_staticText24 = new wxStaticText( this, wxID_ANY, wxT("Colour space:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText24->Wrap( -1 );
    bSizer15->Add( m_staticText24, 0, wxALL, 5 );

    m_staticTextColourSpace = new wxStaticText( this, wxID_ANY, wxT("0 : 0"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextColourSpace->Wrap( -1 );
    bSizer15->Add( m_staticTextColourSpace, 0, wxALL, 5 );


    bSizer10->Add( bSizer15, 1, wxEXPAND, 5 );


    bSizer9->Add( bSizer10, 0, wxEXPAND, 5 );


    bSizer1->Add( bSizer9, 1, wxEXPAND, 5 );


    this->SetSizer( bSizer1 );
    this->Layout();

    // Connect Events
    m_buttonRaw->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonRawOnButtonClick ), NULL, this );
    m_checkBoxRaw->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MyFrame1::m_checkBox10OnCheckBox ), NULL, this );
    m_buttonDownscale->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonDownscaleOnButtonClick ), NULL, this );
    m_checkBoxGrayScale->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MyFrame1::m_checkBoxGrayScaleOnCheckBox ), NULL, this );
    m_buttonUpscale->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonUpscaleOnButtonClick ), NULL, this );
    m_checkBoxUpscale->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MyFrame1::m_checkBoxUpscaleOnCheckBox ), NULL, this );
    m_buttonEdgeEnhance->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonEdgeEnhanceOnButtonClick ), NULL, this );
    m_checkBoxEdgeEnhance->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MyFrame1::m_checkBoxEdgeEnhanceOnCheckBox ), NULL, this );
    m_buttonGaussian->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonGaussianOnButtonClick ), NULL, this );
    m_buttonSharpen->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonSharpenOnButtonClick ), NULL, this );
    m_buttonUnsharpMask->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonUnsharpMaskOnButtonClick ), NULL, this );
    m_radioBtnNoZoom->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MyFrame1::m_radioBtnNoZoomOnRadioButton ), NULL, this );
    m_radioBtn2xZoom->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MyFrame1::m_radioBtn2xZoomOnRadioButton ), NULL, this );
    radioBtn4xZoom->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MyFrame1::radioBtn4xZoomOnRadioButton ), NULL, this );
    radioBtn8xZoom->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MyFrame1::radioBtn8xZoomOnRadioButton ), NULL, this );
}

MyFrame1::~MyFrame1()
{
	/* -------------- Generated code (wxFormBuilder) ----------------------------------------------------------------*/
    m_buttonRaw->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonRawOnButtonClick ), NULL, this );
    m_checkBoxRaw->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MyFrame1::m_checkBox10OnCheckBox ), NULL, this );
    m_buttonDownscale->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonDownscaleOnButtonClick ), NULL, this );
    m_checkBoxGrayScale->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MyFrame1::m_checkBoxGrayScaleOnCheckBox ), NULL, this );
    m_buttonUpscale->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonUpscaleOnButtonClick ), NULL, this );
    m_checkBoxUpscale->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MyFrame1::m_checkBoxUpscaleOnCheckBox ), NULL, this );
    m_buttonEdgeEnhance->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonEdgeEnhanceOnButtonClick ), NULL, this );
    m_checkBoxEdgeEnhance->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MyFrame1::m_checkBoxEdgeEnhanceOnCheckBox ), NULL, this );
    m_buttonGaussian->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonGaussianOnButtonClick ), NULL, this );
    m_buttonSharpen->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonSharpenOnButtonClick ), NULL, this );
    m_buttonUnsharpMask->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonUnsharpMaskOnButtonClick ), NULL, this );
    m_radioBtnNoZoom->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MyFrame1::m_radioBtnNoZoomOnRadioButton ), NULL, this );
    m_radioBtn2xZoom->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MyFrame1::m_radioBtn2xZoomOnRadioButton ), NULL, this );
    radioBtn4xZoom->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MyFrame1::radioBtn4xZoomOnRadioButton ), NULL, this );
    radioBtn8xZoom->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MyFrame1::radioBtn8xZoomOnRadioButton ), NULL, this );
    /* -------------- Generated code (wxFormBuilder) ----------------------------------------------------------------*/
}



// event handlers
void MyFrame1::OnQueueTimer(wxTimerEvent& event)
{
    FromTiDspThread tiDspMsg;
    if(!(queues->fromTiDspToGui.empty()))
    {
        queues->fromTiDspToGui.pop(tiDspMsg);
        m_staticTextPipeline->SetLabel(wxString::Format(wxT("%i %s"), tiDspMsg.info.pipeLineFrameRate, " Hz"));
    }

    FromTiCameraThread tiCameraMsg;
    if (!(queues->fromTiCameraToGui.empty()))
    {
        queues->fromTiCameraToGui.pop(tiCameraMsg);
        m_staticTextFramerate->SetLabel(wxString::Format(wxT("%i %s"), tiCameraMsg.info.frameRate, " Hz"));
        m_staticTextHorisontalPixels->SetLabel(wxString::Format(wxT("%i"), tiCameraMsg.info.horisontalPixels));
        m_staticTextVerticalPixels->SetLabel(wxString::Format(wxT("%i"), tiCameraMsg.info.verticalPixels));
        m_staticTextColourSpace->SetLabel(wxString::Format(wxT("%i : %i"), tiCameraMsg.info.colourSpace, tiCameraMsg.info.bitDepth));
    }
    boost::this_thread::yield();
}


void MyFrame1::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    exit(0);
}

void MyFrame1::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
                 (
                    "Welcome to %s!\n"
                    "\n"
                    "This is the minimal wxWidgets sample\n"
                    "running under %s.",
                    wxVERSION_STRING,
                    wxGetOsDescription()
                 ),
                 "About wxWidgets minimal sample",
                 wxOK | wxICON_INFORMATION,
                 this);
}

void MyFrame1::m_buttonRawOnButtonClick( wxCommandEvent& event )
{
    tiDspParams.outputSelect = OutputSelect::Raw;
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::OutputSelectChange,
            .params = tiDspParams
    });
}

void MyFrame1::m_buttonEdgeEnhanceOnButtonClick( wxCommandEvent& event )
{
    tiDspParams.outputSelect = OutputSelect::EdgeEnhance;
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::OutputSelectChange,
            .params = tiDspParams
    });
}

void MyFrame1::m_checkBoxEdgeEnhanceOnCheckBox( wxCommandEvent& event )
{
	static EdgeEnhanceType previousType = EdgeEnhanceType::Gaussian;

	if (event.IsChecked())
	{
        tiDspParams.edgeEnhancementOn = true;
	}
	else
	{
		tiDspParams.edgeEnhancementOn = false;
	}
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::EdgeEnhanceChange,
            .params = tiDspParams
    });
}

void MyFrame1::m_checkBoxGrayScaleOnCheckBox( wxCommandEvent& event )
{

}

void MyFrame1::m_buttonDownscaleOnButtonClick( wxCommandEvent& event )
{
    tiDspParams.outputSelect = OutputSelect::Downscale;
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::OutputSelectChange,
            .params = tiDspParams
    });
}

void MyFrame1::m_buttonUpscaleOnButtonClick( wxCommandEvent& event )
{
    tiDspParams.outputSelect = OutputSelect::Upscale;
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::OutputSelectChange,
            .params = tiDspParams
    });
}

void MyFrame1::m_checkBoxUpscaleOnCheckBox( wxCommandEvent& event )
{
    if (event.IsChecked())
    {
        tiDspParams.upscaleOn = true;
    }
    else
        {
        tiDspParams.upscaleOn = false;
    }
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::UpscaleChange,
            .params = tiDspParams
    });
}

void MyFrame1::m_buttonGaussianOnButtonClick(wxCommandEvent& event)
{
    tiDspParams.edgeEnhanceType = EdgeEnhanceType::Gaussian;
    m_panelGaussian->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT ) );
    m_panelSharpen->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND ) );
    m_panelUnsharpMask->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND ) );
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::EdgeEnhanceChange,
            .params = tiDspParams
    });
}

void MyFrame1::m_buttonSharpenOnButtonClick(wxCommandEvent& event)
{
    tiDspParams.edgeEnhanceType = EdgeEnhanceType::Sharpen;
    m_panelGaussian->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND ) );
    m_panelSharpen->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT ) );
    m_panelUnsharpMask->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND ) );
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::EdgeEnhanceChange,
            .params = tiDspParams
    });
}

void MyFrame1::m_buttonUnsharpMaskOnButtonClick(wxCommandEvent& event)
{
    tiDspParams.edgeEnhanceType = EdgeEnhanceType::Unsharp_mask;
    m_panelGaussian->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND ) );
    m_panelSharpen->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND ) );
    m_panelUnsharpMask->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT ) );
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::EdgeEnhanceChange,
            .params = tiDspParams
    });
}

void MyFrame1::m_radioBtnNoZoomOnRadioButton( wxCommandEvent& event )
{
    tiDspParams.ezoom = Ezoom::NoZoom;
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::EzoomChange,
            .params = tiDspParams
    });
}

void MyFrame1::m_radioBtn2xZoomOnRadioButton( wxCommandEvent& event )
{
    tiDspParams.ezoom = Ezoom::x2;
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::EzoomChange,
            .params = tiDspParams
    });
}

void MyFrame1::radioBtn4xZoomOnRadioButton( wxCommandEvent& event )
{
    tiDspParams.ezoom = Ezoom::x4;
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::EzoomChange,
            .params = tiDspParams
    });
}

void MyFrame1::radioBtn8xZoomOnRadioButton( wxCommandEvent& event )
{
    tiDspParams.ezoom = Ezoom::x8;
    queues->fromGuiToTiDsp.push(ToTiDspThread{
            .reason = ToTiDspReason::EzoomChange,
            .params = tiDspParams
    });
}

void MyFrame1::m_checkBox10OnCheckBox(wxCommandEvent &event) {

}
