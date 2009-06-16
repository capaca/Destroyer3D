//===========================================================================
/*

    \author:    <http://www.chai3d.org>
    \author:    Remis Balaniuk
    \version    1.1
    \date       10/2005
*/
//===========================================================================

//---------------------------------------------------------------------------


#pragma hdrstop

#include "paralelepipedo.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

paralelepipedo::paralelepipedo(dynamicWorld *mundo, float densidade, float largura, float altura, float profundidade) :
         dynamicObject(mundo,densidade*largura*altura*profundidade,true) {

        _densidade = densidade;
        _largura = largura;
        _altura  = altura;
        _profundidade = profundidade;

        // create the mesh
        criaParalelepipedo();

        init();

        double m = _densidade*_largura*_altura*_profundidade;

        // define inertia matrix
        double Ixx=m*(_altura*_altura + _profundidade*_profundidade)/3.0;
        double Iyy=m*(_largura*_largura + _profundidade*_profundidade)/3.0;
        double Izz=m*(_largura*_largura + _altura*_altura)/3.0;

        cMatrix3d  im;

        im.set(Ixx,0,0,
              0,Iyy,0,
              0,0,Izz);

        setInertiaMatrix(im);

        setRestitution(1);

}

void paralelepipedo::criaParalelepipedo() {

    float dx=0,dy=0,dz=0;

    // face dianteira
    p1 = newVertex(-0.5*_largura+dx, -0.5*_altura+dy, 0.5*_profundidade+dz);
    p2 = newVertex( 0.5*_largura+dx, -0.5*_altura+dy, 0.5*_profundidade+dz);
    p3 = newVertex(-0.5*_largura+dx, 0.5*_altura+dy, 0.5*_profundidade+dz);
    p4 = newVertex( 0.5*_largura+dx, 0.5*_altura+dy, 0.5*_profundidade+dz);
    newTriangle(p1, p2, p3);
    newTriangle(p2, p4, p3);

    // face traseira
    p5 = newVertex(-0.5*_largura+dx,-0.5*_altura+dy, -0.5*_profundidade+dz);
    p6 = newVertex( 0.5*_largura+dx,-0.5*_altura+dy, -0.5*_profundidade+dz);
    p7 = newVertex(-0.5*_largura+dx, 0.5*_altura+dy, -0.5*_profundidade+dz);
    p8 = newVertex( 0.5*_largura+dx, 0.5*_altura+dy, -0.5*_profundidade+dz);
    newTriangle(p5, p7, p6);
    newTriangle(p6, p7, p8);

    //p2
    p9 = newVertex( 0.5*_largura+dx, -0.5*_altura+dy, 0.5*_profundidade+dz);

    //p4
    p10 = newVertex( 0.5*_largura+dx, 0.5*_altura+dy, 0.5*_profundidade+dz);

    //p6
    p11 = newVertex( 0.5*_largura+dx,-0.5*_altura+dy, -0.5*_profundidade+dz);

    //p8
    p12 = newVertex( 0.5*_largura+dx, 0.5*_altura+dy, -0.5*_profundidade+dz);

    // face lateral direita
    newTriangle(p9,p11,p10);//(p2, p6, p4);
    newTriangle(p11,p12,p10);//(p6, p8, p4);

    //p5
    p13 = newVertex(-0.5*_largura+dx,-0.5*_altura+dy, -0.5*_profundidade+dz);

    //p1
    p14 = newVertex(-0.5*_largura+dx, -0.5*_altura+dy, 0.5*_profundidade+dz);

    //p3
    p15 = newVertex(-0.5*_largura+dx, 0.5*_altura+dy, 0.5*_profundidade+dz);

    //p7
    p16 = newVertex(-0.5*_largura+dx, 0.5*_altura+dy, -0.5*_profundidade+dz);

    // face lateral esquerda
    newTriangle(p13, p14, p15);
    newTriangle(p13, p15, p16);
    
    // face superior
    newTriangle(p3, p4, p8);
    newTriangle(p3, p8, p7);
    // face inferior
    newTriangle(p1, p5, p6);
    newTriangle(p1, p6, p2);

    // compute normals
    computeAllNormals(true);

}

void paralelepipedo::aplicarTextura(){
        this->getVertex(p1)->setTexCoord(0,0);
        this->getVertex(p3)->setTexCoord(0,1);
        this->getVertex(p2)->setTexCoord(1,0);
        this->getVertex(p4)->setTexCoord(1,1);

        this->getVertex(p5)->setTexCoord(0,0);
        this->getVertex(p7)->setTexCoord(0,1);
        this->getVertex(p6)->setTexCoord(1,0);
        this->getVertex(p8)->setTexCoord(1,1);

        this->getVertex(p9)->setTexCoord(0,0);
        this->getVertex(p10)->setTexCoord(0,1);
        this->getVertex(p11)->setTexCoord(1,0);
        this->getVertex(p12)->setTexCoord(1,1);

        this->getVertex(p14)->setTexCoord(0,0);
        this->getVertex(p15)->setTexCoord(0,1);
        this->getVertex(p13)->setTexCoord(1,0);
        this->getVertex(p16)->setTexCoord(1,1);
}
