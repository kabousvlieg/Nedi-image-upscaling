///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 20 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __NONAME_H__
#define __NONAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/choicebk.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame1
///////////////////////////////////////////////////////////////////////////////
class MyFrame1 : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText2;
		wxButton* m_buttonRaw;
		wxCheckBox* m_checkBoxRaw;
		wxButton* m_buttonDownscale;
		wxCheckBox* m_checkBoxGrayScale;
		wxButton* m_buttonUpscale;
		wxCheckBox* m_checkBoxColorize;
		wxButton* m_buttonEdgeEnhance;
		wxCheckBox* m_checkBoxEdgeEnhance;
		wxChoicebook* m_choicebook2;
		wxPanel* m_panel_Edge_enhance;
		wxPanel* m_panelGaussian;
		wxButton* m_buttonGaussian;
		wxPanel* m_panelSharpen;
		wxButton* m_buttonSharpen;
		wxPanel* m_panelUnsharpMask;
		wxButton* m_buttonUnsharpMask;
		wxPanel* m_panel_E_Zoom;
		wxRadioButton* m_radioBtnNoZoom;
		wxRadioButton* m_radioBtn2xZoom;
		wxRadioButton* radioBtn4xZoom;
		wxRadioButton* radioBtn8xZoom;
		wxStaticText* m_staticText16;
		wxStaticText* m_staticTextHorisontalPixels;
		wxStaticText* m_staticText18;
		wxStaticText* m_staticTextVerticalPixels;
		wxStaticText* m_staticText20;
		wxStaticText* m_staticTextFramerate;
		wxStaticText* m_staticText22;
		wxStaticText* m_staticTextPipeline;
		wxStaticText* m_staticText24;
		wxStaticText* m_staticTextColourSpace;
		
		// Virtual event handlers, overide them in your derived class
		virtual void m_buttonRawOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_checkBox10OnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonDownscaleOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_checkBoxGrayScaleOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonUpscaleOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_checkBoxColorizeOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonEdgeEnhanceOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_checkBoxEdgeEnhanceOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonGaussianOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonSharpenOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonUnsharpMaskOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_radioBtnNoZoomOnRadioButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_radioBtn2xZoomOnRadioButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void radioBtn4xZoomOnRadioButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void radioBtn8xZoomOnRadioButton( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MyFrame1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,500 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MyFrame1();
	
};

#endif //__NONAME_H__
