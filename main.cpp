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
    \author:    Remis Balaniuk
    \version    1.1
    \date       07/2005

    Esse exemplo mostra como criar uma hierarquia de objetos.
*/
//===========================================================================

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "main.h"
#pragma package(smart_init)
#pragma link "WMPLib_OCX"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
TForm1 *Form1;

cMesh* criaParalelepipedo(float largura, float altura, float profundidade, cWorld *world, cVector3d centro);

//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{

}

//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    init(1);
    WindowsMediaPlayer2->settings->playCount = 10;
    WindowsMediaPlayer2->URL = "ordered.mp3";
    WindowsMediaPlayer2->controls->play();
}

void TForm1::init(int dimensao){

        // cria uma nova cena
        world = new dynamicWorld(false);
        // define cor de fundo
        world->setBackgroundColor(0.0f,0.0f,0.0f);

        // cria uma fonte de luz e liga ao mundo
        light = new cLight(world);
        light->setEnabled(true);
        light->setPos(cVector3d(20,10,10));

        light2 = new cLight(world);
        light2->setEnabled(false);
        light2->setPos(cVector3d(-20,10,10));

        jogo = new Jogo(world, dimensao);

        // cria uma camera  no carro
        cameraMapa = new cCamera(world);
        cameraCarro = new cCamera(world);

        //posicionarCameras();

        jogo->carro->carroceria->addChild(cameraCarro);
        jogo->carro->carroceria->addChild(cameraMapa);

        // cria um display para apresentar a cena
        viewportCarro = new cViewport(Panel1->Handle, cameraCarro, true);
        viewportMapa = new cViewport(Panel2->Handle, cameraMapa, true);

        cameraCarro->set(cVector3d(0,25,8), cVector3d(0,0,0), cVector3d(0.0, 1.0, 1.0));
        cameraMapa->set(cVector3d(0,250,200), cVector3d(0,0,0), cVector3d(0.0, 0.0, 1.0));

        atualizarLabelFase();
        atualizarLabelAlvos();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
    // stop simulation
    // cleanup memory
    delete WindowsMediaPlayer1;
    delete WindowsMediaPlayer2;
    delete world;
    delete viewportCarro;
    delete viewportMapa;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    // loop de atualizacao da tela chamado pelo timer

    if (viewportCarro != NULL)
    {
        viewportCarro->render();
    }

    if (viewportMapa != NULL)
    {
        viewportMapa->render();
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
        //define um passo de rotacao de forma a completar uma volta em 1 segundo
        double passo=-2.0*3.1416*Timer2->Interval/100.0;
        jogo->carro->movimentar(passo);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Panel1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    mouseX = X;
    mouseY = Y;

    if(Shift.Contains(ssLeft)){
        atirou = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Panel1MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{

    if (Shift.Contains(ssRight))
    {
        double delta= -0.01 * (X - mouseX);
        jogo->carro->rotacionarEixos(delta);
    }

    mouseX = X;
    mouseY = Y;
}

//---------------------------------------------------------------------------


void __fastcall TForm1::Timer3Timer(TObject *Sender)
{
        world->computeGlobalPositions(false);

        if(atirou){
                tocarTiro();
                jogo->atirar();
                atirou = false;
        }

        jogo->bolaCanhao->integrate(0.02);
        if(jogo->verificarColisoes()){
                tocarExplosao();
        }
        atualizarLabelAlvos();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        double rotacao = 3.14/8;

        switch(Key){
                case 32://BarraEspaco
                        if(atirou) atirou = false;
                        else atirou = true;
                        break;
                case 87://W
                        Timer2->Enabled = true;
                        break;
                case 38://Up
                        Timer2->Enabled = true;
                        break;
                case 65://A
                        jogo->carro->rotacionarEixos(rotacao);
                        break;
                case 37://Left
                        jogo->carro->rotacionarEixos(rotacao);
                        break;
                case 68://D
                        rotacao *= (-1);
                        jogo->carro->rotacionarEixos(rotacao);
                        break;
                case 39://Right
                        rotacao *= (-1);
                        jogo->carro->rotacionarEixos(rotacao);
                        break;

        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        double rotacao = 3.14/8;
        double angulo;

        switch(Key){
                case 87://W
                        Timer2->Enabled = false;
                        jogo->carro->acertarEixos();
                        break;
                case 38://Up
                        Timer2->Enabled = false;
                        jogo->carro->acertarEixos();
                        break;
                case 65://A
                        rotacao *= (-1);
                        jogo->carro->rotacionarEixos(rotacao);
                        break;
                case 37://Left
                        rotacao *= (-1);
                        jogo->carro->rotacionarEixos(rotacao);
                        break;
                case 68://D
                        jogo->carro->rotacionarEixos(rotacao);
                        break;
                case 39://Right
                        jogo->carro->rotacionarEixos(rotacao);
                        break;
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer4Timer(TObject *Sender)
{

        if(!jogo->isTempoAcabado()){
                if(jogo->isTodosAlvosDestruidos()){
                        Timer4->Enabled = false;
                        Application->MessageBox("Você destruiu todos os prédios! Parabéns!","Level concluído",NULL);
                        subirNivel();
                        Timer4->Enabled = true;
                }
        }
        else{
                Timer4->Enabled = false;
                Application->MessageBox("Seu tempo acabou. GAME OVER!","Game Over",NULL);
                exit(1);
        }

        atualizarTempoJogo();
}
//---------------------------------------------------------------------------

void TForm1::subirNivel(){
        int dimensao = ((jogo->dimensao)+1);

        dynamicWorld* mundoAntigo = world;
        Jogo* jogoAntigo = jogo;


        Timer2->Enabled = false;
        init(dimensao);

        delete jogoAntigo;
        delete mundoAntigo;
}

void TForm1::atualizarLabelFase(){
        string* textoFase = new string("Fase: ");
        textoFase->append(intToString(jogo->dimensao));
        Label2->Caption = textoFase->c_str();
}

void TForm1::atualizarLabelAlvos(){
        string* textoFase = new string("Alvos Restantes: ");
        textoFase->append(intToString((jogo->predios->size() - jogo->numPrediosAtingidos)));
        Label4->Caption = textoFase->c_str();
}

void TForm1::atualizarTempoJogo(){
        //Converte o tempo de int para str
        string tempoStr = intToString(jogo->tempo);

        //Monta o texto a ser exibido
        string *texto = new string("Tempo: ");
        texto->append(tempoStr);
        texto->append(" segundos.");

        //Seta o texto no label
        Label1->Caption = texto->c_str();

        //Decrementa o tempo
        jogo->tempo--;
}

string TForm1::intToString(int inteiro){
        std::stringstream out;
        out << inteiro;
        return out.str();
}

void TForm1::tocarTiro(){
        tocar("tiro.wav");
}

void TForm1::tocarExplosao(){
        tocar("explosao.wav");
}

void TForm1::tocar(string arquivo){
        WindowsMediaPlayer1->URL = arquivo.c_str();
        WindowsMediaPlayer1->controls->play();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
        exit(1);
}
//---------------------------------------------------------------------------

