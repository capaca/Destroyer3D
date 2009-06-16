//===========================================================================
/*
    This file is part of the CHAI 3D visualization and haptics libraries.
    Copyright (C) 2003-2004 by CHAI 3D. All rights reserved.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License("GPL") version 2
    as published by the Free Software Foundation.

    For using the CHAI 3D libraries with software that can not be combined
    with the GNU GPL, and for taking advantage of the additional benefits
    of our support services, please contact CHAI 3D about acquiring a
    Professional Edition License.

    \author:    <http://www.chai3d.org>
    \author:    Francois Conti
    \version    1.1
    \date       06/2004
*/
//===========================================================================

//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "WMPLib_OCX.h"
#include <OleCtrls.hpp>
#include <sstream>
//---------------------------------------------------------------------------
#include "CCamera.h"
#include "CViewport.h"
#include "CLight.h"
#include "CWorld.h"
#include "CMesh.h"
#include "CTriangle.h"
#include "CVertex.h"
#include "CMaterial.h"
#include "CMatrix3d.h"
#include "CVector3d.h"
#include "CString.h"
#include "CCollisionAABB.h"
#include "CCollisionSpheres.h"
#include "CPrecisionClock.h"
#include "cShapeSphere.h"
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <AppEvnts.hpp>
#include <CustomizeDlg.hpp>;
#include "CDelta3dofPointer.h"
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <Graphics.hpp>
#include "CPhantom3dofPointer.h"
#include "CMeta3dofPointer.h"
#include "Cor.h"
#include "quadrado.h"
#include "retangulo.h"
#include "cubo.h"
#include "paralelepipedo.h"
#include "dynamicWorld.h"
#include "dynamicObject.h"
#include "bola.h"
#include "carro.h"
#include "jogo.h"


//---------------------------------------------------------------------------

#define RAIORODA 0.5
#define LARGURACARRO 2.5
#define COMPRIMENTOCARRO 7.0
#define DISTANCIAEIXOS 4.0
#define ANGULOLIMITE (3.14/8)//0.25*3.1416
//---------------------------------------------------------------------------

class TForm1 : public TForm
{
__published:
    TPanel *Panel1;
    TPanel *Panel4;
    TTimer *Timer1;
    TTimer *Timer2;
    TPanel *Panel2;
    TTimer *Timer3;
    TTimer *Timer4;
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label4;
        TWindowsMediaPlayer *WindowsMediaPlayer1;
        TWindowsMediaPlayer *WindowsMediaPlayer2;

    //Contructor & Destructor
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);


    //Threads
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall Timer2Timer(TObject *Sender);
    void __fastcall Timer3Timer(TObject *Sender);
    void __fastcall Timer4Timer(TObject *Sender);

    //Mouse Events
    void __fastcall Panel1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);

    void __fastcall Panel1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);

    //Keyboard Events
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:
    // virtual world
    dynamicWorld* world;

    // camera
    cCamera* cameraMapa, *cameraCarro;

    // light source
    cLight *light, *light2;

    // viewport
    cViewport *viewportCarro, *viewportMapa;

    //Jogo
    Jogo* jogo;

    //Flag
    bool atirou;

    //Mouse
    int mouseX,mouseY;

    void init(int dimensao);
    void atualizarTempoJogo();
    void subirNivel();
    void atualizarLabelFase();
    void atualizarLabelAlvos();
    void tocar(string arquivo);
    void tocarTiro();
    void tocarExplosao();
    string intToString(int inteiro);

public:
    __fastcall TForm1(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
