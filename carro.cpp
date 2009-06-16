//---------------------------------------------------------------------------


#pragma hdrstop

#include "carro.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Carro::Carro(dynamicWorld* world):dynamicObject(world,0.5*LARGURACARRO*COMPRIMENTOCARRO,true) {

        steeringAngle = 0;

        //Instancia o chassi e o meio
        meio = new cGenericObject();
        chassi = new cGenericObject();
        meio->addChild(chassi);

        //Adiciona o meio ao carro propriamente dito
        this->addChild(meio);

        //Inicializa a roda1
        roda1 = new cMesh(world);
        roda1->loadFromFile("roda_simples.obj");
        roda1->computeBoundaryBox(true);
        cVector3d min = roda1->getBoundaryMin();
        cVector3d max = roda1->getBoundaryMax();
        cVector3d meio = cMul(-0.5,cAdd(min,max));
        cVector3d span = cSub(max, min);

        for(int i=0;i<roda1->getNumVertices(true);i++)
                roda1->getVertex(i,true)->translate(meio);

        double size = cMax(span.x, cMax(span.y, span.z));
        double scaleFactor = 2*RAIORODA / size;
        roda1->scale(scaleFactor);
        chassi->addChild(roda1);
        roda1->translate(0,RAIORODA,LARGURACARRO/2.0);
        roda1->rotate(cVector3d(1,0,0),3.1415/2.0);

        //Instancia e inicializa a roda2
        roda2 = new cMesh(world);
        roda2->loadFromFile("roda_simples.obj");

        for(int i=0;i<roda2->getNumVertices(true);i++)
                roda2->getVertex(i,true)->translate(meio);

        roda2->scale(scaleFactor);
        chassi->addChild(roda2);
        roda2->translate(0,RAIORODA,-LARGURACARRO/2.0);
        roda2->rotate(cVector3d(1,0,0),-3.1415/2.0);

        //Instanci os eixos do carro
        eixo1 = new cGenericObject();
        chassi->addChild(eixo1);
        eixo2 = new cGenericObject();
        chassi->addChild(eixo2);

        //Instancia e inicializa a roda3
        roda3 = new cMesh(world);
        roda3->loadFromFile("roda_simples.obj");
        for(int i=0;i<roda3->getNumVertices(true);i++)
                roda3->getVertex(i,true)->translate(meio);

        roda3->scale(scaleFactor);
        eixo1->addChild(roda3);
        eixo1->translate(DISTANCIAEIXOS,RAIORODA,LARGURACARRO/2.0);
        roda3->rotate(cVector3d(1,0,0),3.1415/2.0);

        //Instancia e inicializa a roda4
        roda4 = new cMesh(world);
        roda4->loadFromFile("roda_simples.obj");
        for(int i=0;i<roda4->getNumVertices(true);i++)
                roda4->getVertex(i,true)->translate(meio);

        roda4->scale(scaleFactor);
        eixo2->addChild(roda4);
        eixo2->translate(DISTANCIAEIXOS,RAIORODA,-LARGURACARRO/2.0);
        roda4->rotate(cVector3d(1,0,0),-3.1415/2.0);

        //Instancia e inicializa a carroceria
        carroceria = new cMesh(world);
        carroceria->loadFromFile("ferrari.3ds");
        chassi->addChild(carroceria);
        carroceria->computeBoundaryBox(true);
        min = carroceria->getBoundaryMin();
        max = carroceria->getBoundaryMax();
        span = cSub(max, min);
        meio = cMul(-0.5,cAdd(min,max));

        for(int i=0;i<carroceria->getNumVertices(true);i++)
                carroceria->getVertex(i,true)->translate(meio);
        size = cMax(span.x, cMax(span.y, span.z));
        scaleFactor = COMPRIMENTOCARRO / size;
        carroceria->scale(scaleFactor);
        carroceria->rotate(cVector3d(0,1,0),3.1415/2.0);
        carroceria->rotate(cVector3d(0,0,1),3.1415/2.0);

        //recalcula dimensões
        carroceria->computeBoundaryBox(true);
        min = carroceria->getBoundaryMin();
        max = carroceria->getBoundaryMax();
        span = cSub(max, min);
        double altura= cMin(span.x, cMin(span.y, span.z));

        // posiciona carroceria: assumindo que altura do chão é 0.3* raio da roda
        // levanta meia altura para chao ficar no plano x,z
        carroceria->translate(DISTANCIAEIXOS/1.85,altura*0.5+RAIORODA*0.3,0);
        carroceria->useColors(true, true);
        carroceria->useMaterial(false,true);
}

void Carro::movimentar(double passo){

        roda1->rotate(cVector3d(0,0,1),passo);
        roda2->rotate(cVector3d(0,0,1),passo);
        roda3->rotate(cVector3d(0,0,1),passo);
        roda4->rotate(cVector3d(0,0,1),passo);
        double dangle = -passo*RAIORODA*tan(steeringAngle)/DISTANCIAEIXOS;
        carAngle+= dangle;
        meio->rotate(cVector3d(0,1,0),dangle);
        this->translate(-passo*RAIORODA*cos(carAngle),0,passo*RAIORODA*sin(carAngle));
}

void Carro::rotacionarEixos(double delta){
        if(fabs(steeringAngle+delta)<=ANGULOLIMITE) {
                steeringAngle += delta;
                eixo1->rotate(cVector3d(0,1,0),delta);
                eixo2->rotate(cVector3d(0,1,0),delta);
        }
}

void Carro::acertarEixos(){
        double angulo = steeringAngle*(-1);
        eixo1->rotate(cVector3d(0,1,0),angulo);
        eixo2->rotate(cVector3d(0,1,0),angulo);
        steeringAngle = 0.0;
}
