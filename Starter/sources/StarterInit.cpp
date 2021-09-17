/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 StarterInit.cpp

 - Skeleton .cpp file for a plug-in. It implements the basic
   handshaking methods required for all plug-ins.

*********************************************************************/

// Acrobat Headers.
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstring>
#include <shobjidl.h>
#include <locale>
#include <codecvt>
#endif
using std::cout;
using std::endl;
AVExecuteProc ExecProcPtr = NULL;
AVExecuteProc ExecProcPtrShowNotes = NULL;
AVExecuteProc ExecProcPtrCreateNotes = NULL;
std::stringstream pdfCorpus;
std::string pathToFile="";
/**
	Starter is a plug-in template that provides a minimal
	implementation for a plug-in. Developers may use this plug-in a
	basis for their plug-ins.
*/

/*-------------------------------------------------------
	Core Handshake Callbacks
-------------------------------------------------------*/

/* PluginExportHFTs
** ------------------------------------------------------
**/

/**
** Create and register the HFT's.
**
** @return true to continue loading plug-in,
** false to cause plug-in loading to stop.
*/
ACCB1 ASBool ACCB2 PluginExportHFTs(void)
{
	return true;
}

/* PluginImportReplaceAndRegister
** ------------------------------------------------------
** */
/**
	The application calls this function to allow it to
	<ul>
	<li> Import plug-in supplied HFTs.
	<li> Replace functions in the HFTs you're using (where allowed).
	<li> Register to receive notification events.
	</ul>

	@return true to continue loading plug-in,
	false to cause plug-in loading to stop.
*/
ACCB1 ASBool ACCB2 PluginImportReplaceAndRegister(void)
{
	return true;
}

/* NoteMakerPluggin
** ------------------------------------------------------
**/
/**
	The main initialization routine.

	@return true to continue loading the plug-in,
	false to cause plug-in loading to stop.
*/

ACCB1 void ACCB2 CreateNotesPluggin(void* data)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH];
	strcpy(szFileName, "Test.txt");
	ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = (LPCSTR)"All Files (*.*)";
	ofn.lpstrFile = (LPSTR)szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER;
	ofn.lpstrDefExt = (LPCSTR)"txt";

	if (GetSaveFileName(&ofn))
	{
	    pathToFile = ofn.lpstrFile;
		std::ofstream file(pathToFile);
		// save you file here, using pathToFile

	}
	else
	{
		puts("Something went wrong");
	}

}
ACCB1 void ACCB2 ShowMessage(void* data) {
	//i am testing github
	//char* name = pathToFile;
	if (pathToFile=="") {
		AVAlertNote("Note file is not created yet.....you can create via create notes menu item in the same menu");
	                     }
	else {
		
		HINSTANCE hInstance;
		PWSTR pCmdLine ;
		int nCmdShow;
		
			HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
				COINIT_DISABLE_OLE1DDE);
			if (SUCCEEDED(hr))
			{
				IFileOpenDialog* pFileOpen;

				// Create the FileOpenDialog object.
				hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
					IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

				if (SUCCEEDED(hr))
				{
					// Show the Open dialog box.
					hr = pFileOpen->Show(NULL);

					// Get the file name from the dialog box.
					if (SUCCEEDED(hr))
					{
						IShellItem* pItem;
						hr = pFileOpen->GetResult(&pItem);
						if (SUCCEEDED(hr))
						{
							LPWSTR pszFilePathoriginal;
							hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePathoriginal);

							// Display the file name to the user.
							if (SUCCEEDED(hr))
							{
								
								//return temp;
								//LPWSTR pszFilePath = 
								std::wstring pszFilePath;
								pszFilePath = pszFilePathoriginal;

								//setup converter
								using convert_type = std::codecvt_utf8<wchar_t>;
								std::wstring_convert<convert_type, wchar_t> converter;

								//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
								std::string converted_str = converter.to_bytes(pszFilePath);
								ShellExecute(GetDesktopWindow(), "open", converted_str.c_str(), NULL, NULL, SW_SHOWNORMAL);
								CoTaskMemFree(pszFilePathoriginal);
							}
							pItem->Release();
						}
					}
					pFileOpen->Release();
				}
				CoUninitialize();
			}
			return ;
		
	}
}
ACCB1 ASBool ACCB2 pdTextSelectEnumTextProc(void* procObj, PDFont font, ASFixed size, PDColorValue color, char* text, ASInt32 textLen)
{
	char stringBuffer[200];
	char nextLine = '\n';
	strcpy(stringBuffer, text);
	//strcpy(stringBuffer, &nextLine);
	//pdfCorpus << "KBDBG";
	pdfCorpus << stringBuffer;

	//pdfCorpus.str() += "\n";
	return true;
}
ACCB1 ASBool ACCB2 getHighlightedText(PDWordFinder wObj, PDWord wInfo, ASInt32 pgNum, void* clientData)
{
	char stringBuffer[100];
	AVDoc avDoc = AVAppGetActiveDoc();
	PDDoc currentPDDoc = AVDocGetPDDoc(avDoc);
	//CosDoc cd = PDDocGetCosDoc(currentPDDoc);
	PDAnnot annot;
	PDPage pdpage = PDDocAcquirePage(currentPDDoc, pgNum);
	ASInt32 numAnnots = PDPageGetNumAnnots(pdpage);
	ASFixedRect boundingRect; // bounding rectangle of the term
	char* annBuf;


	ASFile pdfFile = PDDocGetFile(currentPDDoc);
	ASPathName pdfPath = ASFileAcquirePathName(pdfFile);
	//fileSys = ASGetDefaultFileSys();

	ASPlatformPath platformPath;



	for (ASInt32 i = 0; i < numAnnots; i++) {
		annot = PDPageGetAnnot(pdpage, i);
		if (ASAtomFromString("Highlight") == PDAnnotGetSubtype(annot))
		{
			ASInt32 currentPageNumber = pgNum + 1;
			pdfCorpus << " Page Number: " << currentPageNumber << "\n\n";
			// Gets the annotation's rect
			PDAnnotGetRect(annot, &boundingRect);
			// Gets the text selection from the annotation's rect
			PDTextSelect textSelect = PDDocCreateTextSelect(currentPDDoc, pgNum, &boundingRect);
			// create a callback to get the text from highlighted bounding box
			PDTextSelectEnumText(textSelect, ASCallbackCreateProto(PDTextSelectEnumTextProc, &pdTextSelectEnumTextProc), &annBuf);
			pdfCorpus << " \n";
		}
	}
	return 0;
}
ACCB1 void ACCB2 AddToNotesPluggin(void* clientData)
{
	// get this plugin's name for display
	ASAtom NameAtom = ASExtensionGetRegisteredName(gExtensionID);
	const char* name = ASAtomGetString(NameAtom);
	char str[256];
	sprintf(str, "This menu item is added by plugin %s.\n", name);

	// try to get front PDF document 
	AVDoc avDoc = AVAppGetActiveDoc();

	//Display words of the pdf file. 
	PDDoc currentPDDoc = AVDocGetPDDoc(avDoc);
	AVPageView currentPageView = AVDocGetPageView(avDoc);
	//ASInt32 pageNum = AVPageViewGetPageNum(currentPageView);
	for (ASInt32 pageNum = 0; pageNum < PDDocGetNumPages(currentPDDoc); pageNum++) {
		//string name = "this is rohil ";
		//Create a PDWordFinderConfigRec object to customise how text is extracted
		PDWordFinderConfigRec pConfig;
		//Set the DWordFinderConfigRec object's attributes
		memset(&pConfig, 0, sizeof(PDWordFinderConfigRec));
		pConfig.recSize = sizeof(PDWordFinderConfigRec);
		pConfig.ignoreCharGaps = true;
		pConfig.ignoreLineGaps = true;
		pConfig.noAnnots = true;
		pConfig.noEncodingGuess = true;

		//Create a PDWordFinder object
		PDWordFinder pdWordFinder = PDDocCreateWordFinderEx(currentPDDoc, WF_LATEST_VERSION, false, &pConfig);
		//Create a callback function
		PDWordProc wordProc = NULL;
		wordProc = ASCallbackCreateProto(PDWordProc, &getHighlightedText);

		//Extract and display words highlighted 
		PDWordFinderEnumWords(pdWordFinder, pageNum, wordProc, &name);
		PDWordFinderDestroy(pdWordFinder);
	}
	std::string strs = pdfCorpus.str();
	/*strs += "\n";
	strs += "                                 *********************************************************************                                               ";
	strs += "\n";*/
	char* cstr = new char[strs.length() + 1];
	std::strcpy(cstr, strs.c_str());
	//AVAlertNote(cstr);
	//open file for writing
	std::ofstream fw(pathToFile, std::ios_base::app);
	//check if file was successfully opened for writing
	if (fw.is_open())
	{
		//AVAlertNote("file is opened");
		//store array contents to text file
		char* p = std::strtok(cstr, " ");
		while (p != 0)
		{
			fw << p << " ";
			p = std::strtok(NULL, " ");
		}
		fw << "\n" << "                                   ***********************************************                                         " << "\n";
		fw.close();
		pdfCorpus.str(std::string());
	}




	if (avDoc == NULL) {
		// if no doc is loaded, make a message.
		strcat(str, "There is no PDF document loaded in Acrobat.");
	}
	else {
		// if a PDF is open, get its number of pages
		PDDoc pdDoc = AVDocGetPDDoc(avDoc);
		ASInt32 numPages = PDDocGetNumPages(pdDoc);
		//int NumPages = PDDocGetNumPages(pdDoc);
		sprintf(str, "%sThe active PDF document has %d pages.", str, numPages);
	}

}





ACCB1 ASBool ACCB2 NoteMakerPluggin(void)

{

	//Callback function
	ExecProcPtr = ASCallbackCreateProto(AVExecuteProc, &AddToNotesPluggin);
	ExecProcPtrShowNotes = ASCallbackCreateProto(AVExecuteProc, &ShowMessage);
	ExecProcPtrCreateNotes = ASCallbackCreateProto(AVExecuteProc, &CreateNotesPluggin);
	//Declare menu variables
	AVMenubar Themenubar = NULL;
	AVMenu NoteMaker = NULL;
	AVMenuItem AddToNotes = NULL;
	AVMenuItem ShowNotes = NULL;
	AVMenuItem CreateNotes = NULL;

	//Retrieve the menu bar in Adobe Reader or Acrobat
	Themenubar = AVAppGetMenubar();
	//Create a new menu
	NoteMaker = AVMenuNew("Note Maker", "ADBE:NoteMaker", gExtensionID);
	if (NoteMaker == NULL)
	{
		AVAlertNote("Unable to create the menu");
		AVMenuRelease(NoteMaker);
		return false;
	}
	//Create a add to note menu command
	AddToNotes = AVMenuItemNew("Add to note", "ADBE:AddToNotes", NULL, true,
		NO_SHORTCUT, 0, NULL, gExtensionID);
	if (AddToNotes == NULL)
	{
		AVAlertNote("Unable to create the menu command");
		AVMenuItemRelease(AddToNotes);
		return false;
	}
	AVMenuItemSetExecuteProc(AddToNotes, ExecProcPtr, NULL);



	//adding show notes menu command
	ShowNotes = AVMenuItemNew("Show Notes", "ADBE:ShowNotes", NULL, true,
		NO_SHORTCUT, 0, NULL, gExtensionID);
	if (ShowNotes == NULL)
	{
		AVAlertNote("Unable to create the menu command");
		AVMenuItemRelease(ShowNotes);
		return false;
	}
	AVMenuItemSetExecuteProc(ShowNotes, ExecProcPtrShowNotes, NULL);


	//adding show notes menu command
	CreateNotes = AVMenuItemNew("Create Notes", "ADBE:CreateNotes", NULL, true,
		NO_SHORTCUT, 0, NULL, gExtensionID);
	if (CreateNotes == NULL)
	{
		AVAlertNote("Unable to create the menu command");
		AVMenuItemRelease(CreateNotes);
		return false;
	}
	AVMenuItemSetExecuteProc(CreateNotes, ExecProcPtrCreateNotes, NULL);
	//Attach the menu item to the menu and the menu to
	//the menu bar
	AVMenuAddMenuItem(NoteMaker, AddToNotes, 0);
	AVMenuAddMenuItem(NoteMaker, CreateNotes, 0);
    AVMenuAddMenuItem(NoteMaker, ShowNotes, APPEND_MENUITEM);
	AVMenubarAddMenu(Themenubar, NoteMaker, APPEND_MENU);
	//Release the typedef instances
	AVMenuItemRelease(ShowNotes);
	AVMenuItemRelease(AddToNotes);
	AVMenuRelease(NoteMaker);
	return true;
}

/* PluginUnload
** ------------------------------------------------------
**/
/**
	The unload routine.
	Called when your plug-in is being unloaded when the application quits.
	Use this routine to release any system resources you may have
	allocated.

	Returning false will cause an alert to display that unloading failed.
	@return true to indicate the plug-in unloaded.
*/
ACCB1 ASBool ACCB2 PluginUnload(void)
{
	ASCallbackDestroy(ExecProcPtr);
	return true;
}

/* GetExtensionName
** ------------------------------------------------------
*/
/**
	Returns the unique ASAtom associated with your plug-in.
	@return the plug-in's name as an ASAtom.
*/
ASAtom GetExtensionName()
{
	return ASAtomFromString("ADBE:Starter");	/* Change to your extension's name */
}

/** PIHandshake
	function provides the initial interface between your plug-in and the application.
	This function provides the callback functions to the application that allow it to
	register the plug-in with the application environment.

	Required Plug-in handshaking routine: <b>Do not change its name!</b>

	@param handshakeVersion the version this plug-in works with. There are two versions possible, the plug-in version
	and the application version. The application calls the main entry point for this plug-in with its version.
	The main entry point will call this function with the version that is earliest.
	@param handshakeData OUT the data structure used to provide the primary entry points for the plug-in. These
	entry points are used in registering the plug-in with the application and allowing the plug-in to register for
	other plug-in services and offer its own.
	@return true to indicate success, false otherwise (the plug-in will not load).
*/
ACCB1 ASBool ACCB2 PIHandshake(Uns32 handshakeVersion, void* handshakeData)
{
	if (handshakeVersion == HANDSHAKE_V0200) {
		/* Cast handshakeData to the appropriate type */
		PIHandshakeData_V0200* hsData = (PIHandshakeData_V0200*)handshakeData;

		/* Set the name we want to go by */
		hsData->extensionName = GetExtensionName();

		/* If you export your own HFT, do so in here */
		hsData->exportHFTsCallback = (void*)ASCallbackCreateProto(PIExportHFTsProcType, &PluginExportHFTs);

		/*
		** If you import plug-in HFTs, replace functionality, and/or want to register for notifications before
		** the user has a chance to do anything, do so in here.
		*/
		hsData->importReplaceAndRegisterCallback = (void*)ASCallbackCreateProto(PIImportReplaceAndRegisterProcType,
			&PluginImportReplaceAndRegister);

		/* Perform your plug-in's initialization in here */
		hsData->initCallback = (void*)ASCallbackCreateProto(PIInitProcType, &NoteMakerPluggin);

		/* Perform any memory freeing or state saving on "quit" in here */
		hsData->unloadCallback = (void*)ASCallbackCreateProto(PIUnloadProcType, &PluginUnload);

		/* All done */
		return true;

	} /* Each time the handshake version changes, add a new "else if" branch */

	/*
	** If we reach here, then we were passed a handshake version number we don't know about.
	** This shouldn't ever happen since our main() routine chose the version number.
	*/
	return false;
}