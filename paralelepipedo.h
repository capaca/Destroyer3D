//===========================================================================
/*

    \author:    <http://www.chai3d.org>
    \author:    Remis Balaniuk
    \version    1.1
    \date       10/2005
*/
//===========================================================================
//---------------------------------------------------------------------------

#ifndef paralelepipedoH

#define paralelepipedoH
//---------------------------------------------------------------------------

#include "dynamicObject.h"

class paralelepipedo : public dynamicObject {

public:
        paralelepipedo(dynamicWorld *mundo, float densidade, float largura, float altura, float profundidade);

        void criaParalelepipedo();

        void aplicarTextura();

        float _densidade,_altura, _largura, _profundidade;

        //Face Frontal
        int p1, p2, p3, p4;

        //Face traseira
        int p5, p6, p7, p8;

        //Face Direita
        int p9,p10,p11,p12;

        //Face Esquerda
        int p13,p14,p15,p16;

};

#endif
 