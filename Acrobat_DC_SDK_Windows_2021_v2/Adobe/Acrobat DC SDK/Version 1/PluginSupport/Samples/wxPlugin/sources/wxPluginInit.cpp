/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2008 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/
/** 
 \file wxPluginInit.cpp

 - Skeleton .cpp file for a plug-in. It implements the basic
   handshaking methods required for all plug-ins.
 - A convenient function is provided to add a menu item easily. 

*********************************************************************/

// Acrobat Headers.

#include "wx/init.h"
#include "wx/app.h"
#include "wx/frame.h"

#include "wxInit.h"
#include "dialogs.h"

#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif


/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/
// stuff for Menu set up 
typedef ACCB1 void ACCB2 (*menuCommand)(void *clientData);
static AVMenuItem menuItem = NULL;
ACCB1 ASBool ACCB2 PluginMenuItem(char* MyMenuItemTitle, char* MyMenuItemName, 
								   menuCommand menuFunction, AVComputeMarkedProc isMarkedProc = NULL);

// callback functions implemented in file "wxPlugin.cpp"
extern ACCB1 ASBool ACCB2 MyPluginSetmenu();

extern const char* MyPluginExtensionName;

AcrobatFrame* mainFrame;

/*-------------------------------------------------------
	Core Handshake Callbacks
-------------------------------------------------------*/

/**
	Callback invoked by the application to give the plug-in an opportunity to register 
	an HFTServer with the application.
	@return true to indicate the plug-in should continue loading.
*/
ACCB1 ASBool ACCB2 PluginExportHFTs(void)
{
	return true;
}

/** 
	The application calls this function to allow it to
	<ul>
	<li> Import plug-in supplied HFTs.
	<li> Replace functions in the HFTs you're using (where allowed).
	<li> Register to receive notification events.
	</ul>
*/
ACCB1 ASBool ACCB2 PluginImportReplaceAndRegister(void)
{
	return true;
}

/** 
	The main initialization routine.
	We register our action handler with the application.
	@return true to continue loading the plug-in
	@return false to cause plug-in loading to stop.
*/
/* PluginInit
** ------------------------------------------------------
**
** The main initialization routine.
**
** Return true to continue loading plug-in.
** Return false to cause plug-in loading to stop.
*/

ACCB1 ASBool ACCB2 PluginInit(void)
{
#ifndef MAC_PLATFORM
	/* The following code is not supported on Mac. */
	wxInitialize();
	mainFrame = new AcrobatFrame;
	wxTheApp->SetTopWindow(mainFrame);
	return MyPluginSetmenu();
#else
	return true;
#endif
}

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
#ifndef MAC_PLATFORM
	wxUninitialize();
#endif
	return true;
}

/**
	Return the unique ASAtom associated with your plug-in.
	@return the plug-ins name as an ASAtom.
*/
ASAtom GetExtensionName()
{
	return ASAtomFromString(MyPluginExtensionName);
}


/**
	Function that provides the initial interface between your plug-in and the application.
	This function provides the callback functions to the application that allow it to 
	register the plug-in with the application environment.

	Required Plug-in handshaking routine: <b>Do not change it's name!</b>
	
	@param handshakeVersion the version this plug-in works with. There are two versions possible, the plug-in version 
	and the application version. The application calls the main entry point for this plug-in with its version.
	The main entry point will call this function with the version that is earliest. 
	@param handshakeData OUT the data structure used to provide the primary entry points for the plug-in. These
	entry points are used in registering the plug-in with the application and allowing the plug-in to register for 
	other plug-in services and offer its own.
	@return true to indicate success, false otherwise (the plug-in will not load).
*/
ACCB1 ASBool ACCB2 PIHandshake(Uns32 handshakeVersion, void *handshakeData)
{
	if (handshakeVersion == HANDSHAKE_V0200) {
		/* Cast handshakeData to the appropriate type */
		PIHandshakeData_V0200 *hsData = (PIHandshakeData_V0200 *)handshakeData;

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
		hsData->initCallback = (void*)ASCallbackCreateProto(PIInitProcType, &PluginInit);

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

/*-------------------------------------------------------
	Menu Utility
-------------------------------------------------------*/

/**
	A convenient function to add a menu item under Acrobat SDK menu.
	@param MyMenuItemTitle IN String for the menu item's title.
	@param MyMenuItemName IN String for the menu item's internal name.
	@return true if successful, false if failed.
	@see AVAppGetMenubar
	@see AVMenuItemNew
	@see AVMenuItemSetExecuteProc
	@see AVMenuItemSetComputeEnabledProc
	@see AVMenubarAcquireMenuItemByName
	@see AVMenubarAcquireMenuByName
*/
ACCB1 ASBool ACCB2 PluginMenuItem(char* MyMenuItemTitle, char* MyMenuItemName, 
								  menuCommand menuFunction, AVComputeMarkedProc isMarkedProc)
{
	AVMenubar menubar = AVAppGetMenubar();
	AVMenu volatile commonMenu = NULL;

	if (!menubar)
		return false;

	DURING

	   	// Create our menuitem
		menuItem = AVMenuItemNew (MyMenuItemTitle, MyMenuItemName, NULL, true, NO_SHORTCUT, 0, NULL, gExtensionID);
		AVMenuItemSetExecuteProc (menuItem, ASCallbackCreateProto(AVExecuteProc, menuFunction), NULL);
		if(isMarkedProc != NULL)
			AVMenuItemSetComputeMarkedProc (menuItem, ASCallbackCreateProto(AVComputeMarkedProc, isMarkedProc), NULL);

		// Acquire the common "AcrobatSDK" menu.
		commonMenu = AVMenubarAcquireMenuByName (menubar, "ADBE:Acrobat_SDK");

		// if "AcrobatSDK" menu is not existing, create one.
		if (!commonMenu) {
			commonMenu = AVMenuNew ("Acrobat SDK", "ADBE:Acrobat_SDK", gExtensionID);
			AVMenubarAddMenu(menubar, commonMenu, APPEND_MENU);		
		}

		// add new menu item to Acrobat SDK menu
		AVMenuAddMenuItem (commonMenu, menuItem, APPEND_MENUITEM);

		AVMenuRelease (commonMenu);

	HANDLER
		if (commonMenu)
			AVMenuRelease (commonMenu);
		return false;
	END_HANDLER

	return true;
}

