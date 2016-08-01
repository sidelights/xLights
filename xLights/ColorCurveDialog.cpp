#include "ColorCurveDialog.h"
#include "xLightsMain.h"

//(*InternalHeaders(ColorCurveDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/dcbuffer.h>
#include <wx/file.h>
#include "ValueCurve.h"
#include <log4cpp/Category.hh>
#include <wx/colordlg.h>

wxDEFINE_EVENT(EVT_CCP_CHANGED, wxCommandEvent);

BEGIN_EVENT_TABLE(ColorCurvePanel, wxWindow)
EVT_MOTION(ColorCurvePanel::mouseMoved)
EVT_LEFT_DOWN(ColorCurvePanel::mouseLeftDown)
EVT_LEFT_UP(ColorCurvePanel::mouseLeftUp)
EVT_ENTER_WINDOW(ColorCurvePanel::mouseEnter)
EVT_LEAVE_WINDOW(ColorCurvePanel::mouseLeave)
EVT_PAINT(ColorCurvePanel::Paint)
EVT_MOUSE_CAPTURE_LOST(ColorCurvePanel::mouseCaptureLost)
END_EVENT_TABLE()

ColorCurvePanel::ColorCurvePanel(ColorCurve* cc, wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style)
    : wxWindow(parent, id, pos, size, style, "ID_VCP"), xlCustomControl()
{
    _cc = cc;
    Connect(wxEVT_LEFT_DOWN, (wxObjectEventFunction)&ColorCurvePanel::mouseLeftDown, 0, this);
    Connect(wxEVT_LEFT_UP, (wxObjectEventFunction)&ColorCurvePanel::mouseLeftUp, 0, this);
    Connect(wxEVT_ENTER_WINDOW, (wxObjectEventFunction)&ColorCurvePanel::mouseEnter, 0, this);
    Connect(wxEVT_LEAVE_WINDOW, (wxObjectEventFunction)&ColorCurvePanel::mouseLeave, 0, this);
    Connect(wxEVT_MOTION, (wxObjectEventFunction)&ColorCurvePanel::mouseMoved, 0, this);
    Connect(wxEVT_PAINT, (wxObjectEventFunction)&ColorCurvePanel::Paint, 0, this);
    Connect(wxEVT_MOUSE_CAPTURE_LOST, (wxObjectEventFunction)&ColorCurvePanel::mouseCaptureLost, 0, this);
    Connect(wxEVT_LEFT_DCLICK, (wxObjectEventFunction)&ColorCurvePanel::mouseLeftDClick, 0, this);
    wxWindowBase::SetBackgroundStyle(wxBG_STYLE_PAINT);
    _grabbedPoint = -1;
    _startPoint = -1;

    wxWindow::Refresh();
}

void ColorCurvePanel::Select(float x)
{
    _grabbedPoint = x;
    Refresh();
}

void ColorCurvePanel::Convert(float &x, wxMouseEvent& event) const
{
    wxSize size = GetSize();
    float startX = 0.0; // size.GetWidth() / 10.0;
    float bw = size.GetWidth(); //  *0.8;

    x = (event.GetX() - startX) / bw;
    x = ccSortableColorPoint::Normalise(x);
}

//(*IdInit(ColorCurveDialog)
const long ColorCurveDialog::ID_STATICTEXT1 = wxNewId();
const long ColorCurveDialog::ID_CHOICE1 = wxNewId();
const long ColorCurveDialog::ID_BUTTON3 = wxNewId();
const long ColorCurveDialog::ID_BUTTON4 = wxNewId();
const long ColorCurveDialog::ID_BUTTON1 = wxNewId();
const long ColorCurveDialog::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ColorCurveDialog,wxDialog)
	//(*EventTable(ColorCurveDialog)
	//*)
END_EVENT_TABLE()

ColorCurveDialog::ColorCurveDialog(wxWindow* parent, ColorCurve* cc, wxWindowID id,const wxPoint& pos,const wxSize& size)
{
    _cc = cc;

    //(*Initialize(ColorCurveDialog)
    wxFlexGridSizer* FlexGridSizer4;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer5;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer6;
    wxFlexGridSizer* FlexGridSizer1;

    Create(parent, id, _("Color Curve"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxRESIZE_BORDER|wxMAXIMIZE_BOX, _T("id"));
    SetClientSize(wxDefaultSize);
    Move(wxDefaultPosition);
    FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer1->AddGrowableCol(0);
    FlexGridSizer1->AddGrowableRow(0);
    FlexGridSizer6 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer6->AddGrowableCol(0);
    FlexGridSizer6->AddGrowableRow(0);
    FlexGridSizer1->Add(FlexGridSizer6, 1, wxALL|wxEXPAND, 2);
    FlexGridSizer2 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer2->AddGrowableCol(0);
    FlexGridSizer4 = new wxFlexGridSizer(0, 2, 0, 0);
    FlexGridSizer4->AddGrowableCol(1);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Blend Mode"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer4->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    Choice1 = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    Choice1->SetSelection( Choice1->Append(_("None")) );
    Choice1->Append(_("Gradient"));
    FlexGridSizer4->Add(Choice1, 1, wxALL|wxEXPAND, 2);
    FlexGridSizer2->Add(FlexGridSizer4, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 2);
    FlexGridSizer5 = new wxFlexGridSizer(0, 2, 0, 0);
    ButtonLoad = new wxButton(this, ID_BUTTON3, _("Load"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    FlexGridSizer5->Add(ButtonLoad, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonExport = new wxButton(this, ID_BUTTON4, _("Export"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    FlexGridSizer5->Add(ButtonExport, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer5, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
    Button_Ok = new wxButton(this, ID_BUTTON1, _("Ok"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    FlexGridSizer3->Add(Button_Ok, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button_Cancel = new wxButton(this, ID_BUTTON2, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    FlexGridSizer3->Add(Button_Cancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->Fit(this);
    FlexGridSizer1->SetSizeHints(this);

    Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&ColorCurveDialog::OnChoice1Select);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ColorCurveDialog::OnButtonLoadClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ColorCurveDialog::OnButtonExportClick);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ColorCurveDialog::OnButton_OkClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ColorCurveDialog::OnButton_CancelClick);
    Connect(wxEVT_SIZE,(wxObjectEventFunction)&ColorCurveDialog::OnResize);
    //*)

    Connect(wxID_ANY, wxEVT_CHAR_HOOK, wxKeyEventHandler(ColorCurveDialog::OnChar), (wxObject*)nullptr, this);

    _ccp = new ColorCurvePanel(_cc, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER);
    _ccp->SetMinSize(wxSize(300, 150));
    _ccp->SetType(_cc->GetType());
    FlexGridSizer6->Add(_ccp, 1, wxALL | wxEXPAND, 2);
    wxWindow::Layout();
    wxWindow::Fit();

    _backup = *_cc;

    Choice1->SetStringSelection(wxString(_cc->GetType().c_str()));
    Choice1->SetFocus();

    Connect(wxID_ANY, EVT_CCP_CHANGED, (wxObjectEventFunction)&ColorCurveDialog::OnCCPChanged, 0, this);

    ValidateWindow();
}

ColorCurveDialog::~ColorCurveDialog()
{
	//(*Destroy(ColorCurveDialog)
	//*)
}

void ColorCurveDialog::OnCCPChanged(wxCommandEvent& event)
{
    ValidateWindow();
}

void ColorCurveDialog::OnButton_OkClick(wxCommandEvent& event)
{
    EndDialog(wxID_OK);
}

void ColorCurveDialog::OnButton_CancelClick(wxCommandEvent& event)
{
    *_cc = _backup;
    EndDialog(wxID_CANCEL);
}

void ColorCurveDialog::OnChoice1Select(wxCommandEvent& event)
{
    _ccp->SetType(std::string(Choice1->GetStringSelection().c_str()));
    _cc->SetType(std::string(Choice1->GetStringSelection().c_str()));
    _ccp->Refresh();
    ValidateWindow();
}

#pragma region Mouse Control

void ColorCurvePanel::Undo()
{
    if (_undo.size() > 0)
    {
        ccSortableColorPoint pt = _undo.back();
        _undo.pop_back();

        if (pt.x < 0)
        {
            _cc->SetValueAt(pt.x * -1, pt.color);
        }
        else
        {
            _cc->DeletePoint(pt.x);
        }

        if (pt.DoNext())
        {
            Undo();
        }
    }
    NotifyChange();
    Reloaded();
}

void ColorCurvePanel::Reloaded()
{
    _grabbedPoint = -1;
    _startPoint = -1;
    SetToolTip("");
    if (HasCapture())
    {
        ReleaseCapture();
    }
    Refresh();
}

void ColorCurvePanel::SaveUndo(ccSortableColorPoint& point, bool del)
{
    if (del)
    {
        point.x *= -1;
        _undo.push_back(point);
        point.x *= -1;
    }
    else
    {
        _undo.push_back(point);
    }
}

void ColorCurvePanel::mouseLeftDClick(wxMouseEvent& event)
{
    float x;
    Convert(x, event);
    x = ccSortableColorPoint::Normalise(x);

    Select(x);

    if (!_cc->IsSetPoint(x))
    {
        _cc->SetValueAt(x, *wxBLACK);
    }
    ccSortableColorPoint* pt = _cc->GetPointAt(x);

    wxColourData cd;
    cd.SetColour(pt->color);
    wxColourDialog cdlg(this, &cd);

    if (cdlg.ShowModal() == wxID_OK)
    {
        wxColourData retData = cdlg.GetColourData();
        SaveUndo(*pt, true);
        _cc->SetValueAt(x, retData.GetColour());
    }
    Refresh();
    SetToolTip("");
}

void ColorCurvePanel::mouseLeftDown(wxMouseEvent& event)
{
    float x;
    Convert(x, event);
    x = ccSortableColorPoint::Normalise(x);

    _grabbedPoint = x;

    if (_cc->IsSetPoint(_grabbedPoint))
    {
        _startPoint = _grabbedPoint;
    }
    else
    {
        _startPoint = -1;
        _cc->SetValueAt(_grabbedPoint, *wxBLACK);
    }

    _minGrabbedPoint = _cc->FindMinPointLessThan(_grabbedPoint);
    _maxGrabbedPoint = _cc->FindMaxPointGreaterThan(_grabbedPoint);

    SetToolTip(wxString::Format("%.1f", _grabbedPoint * 100));

    CaptureMouse();
    Refresh();
}

void ColorCurvePanel::mouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    _startPoint = -1;
    SetToolTip("");
    Refresh();
}

void ColorCurvePanel::mouseLeftUp(wxMouseEvent& event)
{
    if (HasCapture())
    {
        float x;
        Convert(x, event);
        x = ccSortableColorPoint::Normalise(x);

        if (x <= _minGrabbedPoint)
        {
            x = _minGrabbedPoint;
        }
        else if (x > _maxGrabbedPoint)
        {
            x = _maxGrabbedPoint;
        }

        wxColor c = *wxBLACK;
        if (x != _grabbedPoint)
        {

            // find the original grabbed point in the grid
            if (_cc->IsSetPoint(_grabbedPoint))
            {
                c = _cc->GetValueAt(_grabbedPoint);
            }
            _cc->SetValueAt(x, c);
            if (_cc->IsSetPoint(_grabbedPoint))
            {
                _cc->DeletePoint(_grabbedPoint);
            }
        }

        ccSortableColorPoint* pt = _cc->GetPointAt(x);
        wxASSERT(pt != nullptr);
        SaveUndo(*pt, false);
        if (_startPoint >= 0)
        {
            ccSortableColorPoint ptdel(_startPoint, c, true);
            SaveUndo(ptdel, true);
        }

        _startPoint = -1;
        ReleaseMouse();
        NotifyChange();
    }
    Refresh();
    SetToolTip("");
}

void ColorCurvePanel::Delete()
{
    if (_grabbedPoint >= 0 && _cc->IsSetPoint(_grabbedPoint))
    {
        ccSortableColorPoint* pt = _cc->GetPointAt(_grabbedPoint);
        wxASSERT(pt != nullptr);
        SaveUndo(*pt, true);

        _cc->DeletePoint(_grabbedPoint);
        _grabbedPoint = -1;
        Refresh();
        NotifyChange();
    }
}

void ColorCurvePanel::mouseEnter(wxMouseEvent& event)
{
    SetCursor(wxCURSOR_CROSS);
    mouseMoved(event);
}

void ColorCurvePanel::mouseLeave(wxMouseEvent& event)
{
    SetCursor(wxCURSOR_DEFAULT);
}

void ColorCurvePanel::mouseMoved(wxMouseEvent& event)
{
    float x;
    Convert(x, event);
    x = ccSortableColorPoint::Normalise(x);

    if (_cc->NearPoint(x))
    {
        SetCursor(wxCURSOR_SIZEWE);
    }
    else
    {
        SetCursor(wxCURSOR_CROSS);
    }

    if (HasCapture())
    {
        if (x <= _minGrabbedPoint)
        {
            x = _minGrabbedPoint;
        }
        else if (x > _maxGrabbedPoint)
        {
            x = _maxGrabbedPoint;
        }

        if (x != _grabbedPoint)
        {
            wxColor c = _cc->GetValueAt(_grabbedPoint);

            if (_cc->IsSetPoint(_grabbedPoint))
            {
                _cc->SetValueAt(x, c);
            }
            else
            {
                _cc->SetValueAt(x, *wxBLACK);
            }
            if (_cc->IsSetPoint(_grabbedPoint))
            {
                _cc->DeletePoint(_grabbedPoint);
            }
            _grabbedPoint = x;

            Refresh();
            SetToolTip(wxString::Format("%.1f", _grabbedPoint * 100));
        }
    }
}
#pragma endregion Mouse Control

void ColorCurvePanel::Paint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC pdc(this);
    wxSize s = GetSize();
    if (_cc != nullptr)
    {
        wxBitmap bmp = _cc->GetImage(s.GetWidth(), s.GetHeight(), true);
        pdc.DrawBitmap(bmp, 0, 0);

        pdc.SetPen(wxPen(*wxRED, 3, wxPENSTYLE_SOLID));
        //pdc.SetLogicalFunction(wxXOR);

        std::list<ccSortableColorPoint> pts = _cc->GetPoints();

        if (pts.size() > 0)
        {
            std::list<ccSortableColorPoint>::iterator last = pts.begin();
            for (auto p = pts.begin()++; p != pts.end(); p++)
            {
                if (p->x == 1.0)
                {
                    pdc.DrawLine(p->x * s.GetWidth() - 2, 0, p->x * s.GetWidth() - 2, s.GetHeight());
                }
                else
                {
                    pdc.DrawLine(p->x * s.GetWidth(), 0, p->x * s.GetWidth(), s.GetHeight());
                }
            }
        }

        if (_grabbedPoint >= 0)
        {
            pdc.SetPen(wxPen(*wxBLUE, 3, wxPENSTYLE_SOLID));
            if (_grabbedPoint == 1.0)
            {
                pdc.DrawLine(_grabbedPoint * s.GetWidth() - 2, 0, _grabbedPoint * s.GetWidth() - 2, s.GetHeight());
            }
            else
            {
                pdc.DrawLine(_grabbedPoint * s.GetWidth(), 0, _grabbedPoint * s.GetWidth(), s.GetHeight());
            }
        }
    }
}

void ColorCurveDialog::ValidateWindow()
{
}

void ColorCurveDialog::OnChar(wxKeyEvent& event)
{
    wxChar uc = event.GetUnicodeKey();
    if (uc == WXK_DELETE)
    {
        _ccp->Delete();
    }
    else if ((uc == 'Z' || uc == 'z') && event.ControlDown())
    {
        _ccp->Undo();
    }
    _ccp->Refresh();
    Refresh();
}

void ColorCurveDialog::OnResize(wxSizeEvent& event)
{
    OnSize(event);
    _ccp->Refresh();
    Refresh();
}

void ColorCurveDialog::OnButtonLoadClick(wxCommandEvent& event)
{
    std::string id = _cc->GetId(); // save if because it will be overwritten

    wxString filename = wxFileSelector(_("Choose color curve file"), wxEmptyString, wxEmptyString, wxEmptyString, "Color Curve files (*.xcc)|*.xcc", wxFD_OPEN);
    if (filename.IsEmpty()) return;
    
    wxXmlDocument doc(filename);

    if (doc.IsOk())
    {
        wxXmlNode* root = doc.GetRoot();

        if (root->GetName() == "colorcurve")
        {
            wxString data = root->GetAttribute("data");
            wxString v = root->GetAttribute("SourceVersion");

            // Add any colorcurve version conversion logic here
            // Source version will be the program version that created the custom model

            _cc->Deserialise(data.ToStdString());
            _cc->SetId(id);
            _ccp->Reloaded();
            _ccp->Refresh();
        }
        else
        {
            wxMessageBox("Failure loading color curve file.");
        }
    }
    else
    {
        wxMessageBox("Failure loading color curve file.");
    }
}

void ColorCurveDialog::OnButtonExportClick(wxCommandEvent& event)
{
    wxLogNull logNo; //kludge: avoid "error 0" message from wxWidgets after new file is written
    wxString filename = wxFileSelector(_("Choose output file"), wxEmptyString, "ColorCurve", wxEmptyString, "Color Curves (*.xcc)|*.xcc", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (filename.IsEmpty()) return;

    wxFile f(filename);
    log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.info("Saving to xcc file %s.", std::string(filename.c_str()).c_str());

    if (!f.Create(filename, true) || !f.IsOpened())
    {
        logger_base.info("Unable to create file %s. Error %d\n", std::string(filename.c_str()).c_str(), f.GetLastError());
        wxMessageBox(wxString::Format("Unable to create file %s. Error %d\n", filename, f.GetLastError()));
        return;
    }

    wxString v = xlights_version_string;
    f.Write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<colorcurve \n");
    f.Write(wxString::Format("data=\"%s\" ", (const char *)_cc->Serialise().c_str()));
    f.Write(wxString::Format("SourceVersion=\"%s\" ", v));
    f.Write(" >\n");
    f.Write("</colorcurve>");
    f.Close();
}
