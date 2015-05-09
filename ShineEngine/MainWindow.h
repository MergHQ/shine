#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#pragma once

class CMainWindow
{
public:
	CMainWindow();
	~CMainWindow();
	void Init();
	IGlobalSystem* gSys;
private:

}; 
extern IGlobalSystem* gSys; // Gives linker errors :( Some shit is up.

#endif