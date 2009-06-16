//===========================================================================
/*

    \author:    <http://www.chai3d.org>
    \author:    Remis Balaniuk
    \version    1.1
    \date       10/2005
*/
//===========================================================================
//---------------------------------------------------------------------------

#ifndef bolaH
#define bolaH
//---------------------------------------------------------------------------

#include "dynamicObject.h"

#define SLICES 15

class bola : public dynamicObject {

public:
        bola(dynamicWorld *mundo, float densidade, float raio);

        void criaBola();

        float _densidade,_raio;

};

#endif
