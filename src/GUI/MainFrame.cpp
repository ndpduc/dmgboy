/*
 This file is part of gbpablog.

 gbpablog is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 gbpablog is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with gbpablog.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include "MainFrame.h"
#include "AboutDialog.h"
#include "IDControls.h"
#include "../Settings.h"
#include "../Pad.h"
#include "Xpm/open.xpm"
#include "Xpm/play.xpm"
#include "Xpm/pause.xpm"
#include "Xpm/stop.xpm"
#include "Xpm/gb16.xpm"
#include "Xpm/gb32.xpm"

IMPLEMENT_CLASS(MainFrame, wxFrame)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(wxID_EXIT, MainFrame::OnFileExit)
EVT_MENU(wxID_OPEN, MainFrame::OnFileOpen)
EVT_MENU(wxID_PREFERENCES, MainFrame::OnSettings)
EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
EVT_MENU(ID_START, MainFrame::OnPlay)
EVT_MENU(ID_PAUSE, MainFrame::OnPause)
EVT_MENU(ID_STOP, MainFrame::OnStop)
EVT_UPDATE_UI( ID_START, MainFrame::OnPlayUpdate )
EVT_UPDATE_UI( ID_PAUSE, MainFrame::OnPauseUpdate )
EVT_UPDATE_UI( ID_STOP, MainFrame::OnStopUpdate )
EVT_IDLE(MainFrame::OnIdle)
END_EVENT_TABLE()

MainFrame::MainFrame()
{
    // Create the MainFrame
    this->Create(0, ID_MAINFRAME, wxT("gbpablog"), wxDefaultPosition,
           wxDefaultSize, wxCAPTION | wxSYSTEM_MENU |
           wxMINIMIZE_BOX | wxCLOSE_BOX);
	
	wxIconBundle * icons = new wxIconBundle(wxIcon(gb16_xpm));
	icons->AddIcon(wxIcon(gb32_xpm));
	this->SetIcons(*icons);

    this->CreateMenuBar();
	this->CreateToolBar();

	settingsDialog = new SettingsDialog(this);
	settingsDialog->CentreOnScreen();
	settingsDialog->LoadFromFile();
	SettingsSetNewValues(settingsDialog->settings);
	PadSetKeys(SettingsGetInput());	

    // create the MainPanel
    panel = new MainPanel(this);


    video = new Video(panel);
	cpu = new CPU(video);

	cartridge = NULL;

	emuState = NotStartedYet;

	this->SetClientSize(GB_SCREEN_W*SettingsGetWindowZoom(), GB_SCREEN_H*SettingsGetWindowZoom());
}

MainFrame::~MainFrame()
{
	this->Clean();
}

void MainFrame::CreateMenuBar()
{
	// create the main menubar
    mb = new wxMenuBar();

    // create the file menu
    wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(wxID_OPEN, wxT("&Open\tCtrl+O"));
	fileMenu->Append(wxID_EXIT, wxT("E&xit"));

    // add the file menu to the menu bar
    mb->Append(fileMenu, wxT("&File"));

	// create the emulation menu
    wxMenu *emulationMenu = new wxMenu;
	emulationMenu->Append(wxID_PREFERENCES, wxT("&Settings\tCtrl+E"));
    emulationMenu->Append(ID_START, wxT("&Start\tCtrl+S"));
	emulationMenu->Append(ID_PAUSE, wxT("&Pause\tCtrl+P"));
	emulationMenu->Append(ID_STOP, wxT("S&top\tCtrl+T"));

    // add the file menu to the menu bar
    mb->Append(emulationMenu, wxT("&Emulation"));

    // create the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About"));

    // add the help menu to the menu bar
    mb->Append(helpMenu, wxT("&Help"));

    // add the menu bar to the MainFrame
    this->SetMenuBar(mb);
}

void MainFrame::CreateToolBar()
{

	toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER);
	wxBitmap bmpOpen(open_xpm);
	toolBar->AddTool(wxID_OPEN, bmpOpen, wxT("Open"));

	toolBar->AddSeparator();

	wxBitmap bmpPlay(play_xpm);
	toolBar->AddTool(ID_START, bmpPlay, wxT("Start"));

	wxBitmap bmpPause(pause_xpm);
	toolBar->AddTool(ID_PAUSE, bmpPause, wxT("Pause"));

	wxBitmap bmpStop(stop_xpm);
	toolBar->AddTool(ID_STOP, bmpStop, wxT("Stop"));

	toolBar->Realize();
	this->SetToolBar(toolBar);
}

void MainFrame::OnFileOpen(wxCommandEvent &) {

	wxFileDialog* openDialog = new wxFileDialog(this, wxT("Choose a gameboy rom to open"), wxEmptyString, wxEmptyString,
												wxT("Gameboy roms (*.gb; *.zip)|*.gb;*.zip"),
												wxFD_OPEN, wxDefaultPosition);

	// Creates a "open file" dialog
	if (openDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
		this->ChangeFile(openDialog->GetPath());

	// Clean up after ourselves
	openDialog->Destroy();
}

void MainFrame::ChangeFile(const wxString fileName)
{
	BYTE * buffer = NULL;
	unsigned long size = 0;
	bool isZip = false;
	
	if (!wxFileExists(fileName))
	{
		wxMessageBox(wxT("The file:\n")+fileName+wxT("\ndoesn't exist"), wxT("Error"));
		return;
	}
	
	wxString fileLower = fileName.Lower();
	if (fileLower.EndsWith(wxT(".zip")))
	{
		isZip = true;
		this->LoadZip(fileName, &buffer, &size);
		if ((buffer == NULL) || (size == 0))
			return;
	}
	else if (!fileLower.EndsWith(wxT(".gb")))
	{
		wxMessageBox(wxT("Only gb and zip files allowed!"), wxT("Error"));
		return;
	}

	
	// Si ha llegado aquí es que es un archivo permitido
	cpu->Reset();
	if (cartridge)
		delete cartridge;
	
	if (isZip) {
		cartridge = new Cartridge(buffer, size);
	}else {
		cartridge = new Cartridge(std::string(fileName.mb_str()));
	}
	
	
	cpu->LoadCartridge(cartridge);
	emuState = Playing;
}

/*
 * Carga un fichero comprimido con zip y busca una rom de gameboy (un fichero con extension gb).
 * Si existe mas de una rom solo carga la primera. Si se ha encontrado, la rom se devuelve en un buffer
 * junto con su tamaño, sino las variables se dejan intactas
 */
void MainFrame::LoadZip(const wxString zipPath, BYTE ** buffer, unsigned long * size)
{
	wxString fileInZip, fileLower;
	wxZipEntry* entry;
	wxFFileInputStream in(zipPath);
	wxZipInputStream zip(in);
	while (entry = zip.GetNextEntry())
	{
		fileInZip = entry->GetName();
		
		fileLower = fileInZip.Lower();
		if (fileLower.EndsWith(wxT(".gb")))
		{
			*size = zip.GetSize();
			*buffer = new BYTE[*size];
			zip.Read(*buffer, *size);
			delete entry;
			return;
		}
		else
		{
			delete entry;
			continue;
		}
	}
	
	// Archivo no encontrado
	wxMessageBox(wxT("GameBoy rom not found in the file:\n")+zipPath, wxT("Error"));
	return;
}

void MainFrame::OnFileExit(wxCommandEvent &)
{
	this->Close();
}

void MainFrame::Clean()
{
	emuState = Stopped;
	delete cpu;
	delete video;
	if (cartridge)
		delete cartridge;
	if (settingsDialog)
		settingsDialog->Destroy();
}

/*
 * Abre un dialogo de configuracion. Cuando se cierra se encarga de aplicar ciertos cambios a la emulacion
 */
void MainFrame::OnSettings(wxCommandEvent &)
{
	enumEmuStates lastState = emuState;
	if (emuState == Playing)
		emuState = Paused;


    if (settingsDialog->ShowModal() == wxID_OK)
	{
		SettingsSetNewValues(settingsDialog->settings);
		panel->ChangePalette(SettingsGetGreenScale());
		panel->ChangeSize();
		this->SetClientSize(GB_SCREEN_W*SettingsGetWindowZoom(), GB_SCREEN_H*SettingsGetWindowZoom());
		PadSetKeys(SettingsGetInput());
	}

	emuState = lastState;
}

void MainFrame::OnAbout(wxCommandEvent &)
{
	AboutDialog(this);
}

void MainFrame::OnPlay(wxCommandEvent &)
{
	emuState = Playing;
}

void MainFrame::OnPause(wxCommandEvent &)
{
	if (emuState == Playing)
		emuState = Paused;
	else if (emuState == Paused)
		emuState = Playing;
}

void MainFrame::OnStop(wxCommandEvent &)
{
	cpu->Reset();
	panel->OnRefreshScreen();
	emuState = Stopped;
}

void MainFrame::OnPlayUpdate(wxUpdateUIEvent& event)
{
	if ((emuState == NotStartedYet) || (emuState == Playing)){
		event.Enable(false);
	}
	else {
		event.Enable(true);
	}

}

void MainFrame::OnPauseUpdate(wxUpdateUIEvent& event)
{
	if ((emuState == NotStartedYet) || (emuState == Stopped)){
		event.Enable(false);
	}
	else {
		event.Enable(true);
	}

}

void MainFrame::OnStopUpdate(wxUpdateUIEvent& event)
{
	if ((emuState == Stopped)||(emuState == NotStartedYet)) {
		event.Enable(false);
	}
	else {
		event.Enable(true);
	}

}

/**
 * Esta funcion se encarga de dar la orden de hacer aproximadamente los calculos
 * de la emulacion de un frame, pero lo hara cuando la interfaz haya terminado de
 * procesar todos los eventos. Se quiere que pinte los frames cada 16.6 milisegundos
 * (60 frames por segundo). Para ello se tiene en cuenta el tiempo que hace que se
 * llamo por ultima vez a esta funcion y el tiempo que ha tardado la emulacion en
 * calcularse.
 */
void MainFrame::OnIdle(wxIdleEvent &event)
{
	long duration = 16;

	long lastDuration = swFrame.Time();
	swFrame.Start();

	long delay = 0;

	if (lastDuration > duration)
		delay = lastDuration - duration;

    if (emuState == Playing)
	{
		swExecution.Start();

		cpu->UpdatePad();

		// Valor obtenido comparandolo con otros emuladores
		cpu->Run(113000);

		delay += swExecution.Time();
	}

	if (delay > duration)
		delay = duration;

	event.RequestMore(true);
    wxMilliSleep(duration-delay);
}
