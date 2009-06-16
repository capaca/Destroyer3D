//---------------------------------------------------------------------------

#ifndef carroH
#define carroH

#include "dynamicObject.h"
#include "dynamicWorld.h"

#define RAIORODA 0.5
#define LARGURACARRO 2.5
#define COMPRIMENTOCARRO 7.0
#define DISTANCIAEIXOS 4.0
#define ANGULOLIMITE (3.14/8)//0.25*3.1416


class Carro : public dynamicObject {

        public:
                //Constructor & Destructor
                Carro(dynamicWorld *world);

                void movimentar(double passo);
                void rotacionarEixos(double delta);
                void acertarEixos();
                void parar();

                cGenericObject *chassi, *meio, *eixo1, *eixo2;
                cMesh *carroceria, *roda1, *roda2, *roda3, *roda4;
                //cVector3d min, max, meio, span;
                double size, scaleFactor, altura, steeringAngle, carAngle;

};

//---------------------------------------------------------------------------
#endif
