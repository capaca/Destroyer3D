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

#include "bola.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

bola::bola(dynamicWorld *mundo, float densidade, float raio) :
         dynamicObject(mundo,densidade*4.0*3.1416*raio*raio*raio,true) {

        _densidade = densidade;
        _raio=raio;

        // cria vertices
        criaBola();

        // inicializa propriedades do objeto dinamico
        init();

        double m = _densidade*4*3.1416*raio*raio*raio;

        // define matriz de inercia
        double I=2*m*raio*raio/5.0;

        cMatrix3d im;
        im.set(I,0,0,
               0,I,0,
               0,0,I);
        setInertiaMatrix(im);

        // restitution 
        setRestitution(1);

        collisionDetectionUseVertices = false;

}

void bola::criaBola() {

    double angle1,angle2;
    int i,j,steps=SLICES/2,esse1,esse2,quadrante=0;
    int verts[SLICES][2*SLICES];
    cVector3d direction,inverso;

    for(i=0;i<steps;i++) {
        quadrante=0;
        angle1=i*0.5*3.1416/steps;
        direction.x=sin(angle1)*_raio;
        for(j=0;j<(steps-i)*4;j++) {
                angle2=j*0.5*3.1416/(steps-i);
                direction.z=sin(angle2)*cos(angle1)*_raio;
                direction.y=cos(angle2)*cos(angle1)*_raio;
                inverso = direction;

                esse1 = verts[steps+i][j] = newVertex(direction);
                if(i>0) {
                        inverso.x*=-1.0;

                        esse2 = verts[steps-i][j] = newVertex(inverso);

                        newTriangle(esse1,verts[steps+i-1][j+quadrante],verts[steps+i-1][j+quadrante+1] );
                        newTriangle(esse2,verts[steps-i+1][j+quadrante+1],verts[steps-i+1][j+quadrante]);

                        int pos= j%(steps-i);
                        if(j>0) {
                                newTriangle(esse1,verts[steps+i][j-1],verts[steps+i-1][j+quadrante] );
                                newTriangle(esse2,verts[steps-i+1][j+quadrante] ,verts[steps-i][j-1]);
                        }
                        if((pos==0)&&(j>0)) {
                                quadrante++;
                                newTriangle(esse1,verts[steps+i-1][j+quadrante],verts[steps+i-1][j+quadrante+1] );
                                newTriangle(esse2,verts[steps-i+1][j+quadrante+1],verts[steps-i+1][j+quadrante]);
                        }
                 }
        }
        if(i>0)   {
                newTriangle(verts[steps+i][0],verts[steps+i][(steps-i)*4-1],verts[steps+i-1][(steps-i)*4+quadrante] );
                newTriangle(verts[steps+i][0],verts[steps+i-1][(steps-i)*4+quadrante],verts[steps+i-1][0]);
                newTriangle(verts[steps-i][0],verts[steps-i+1][(steps-i)*4+quadrante] ,verts[steps-i][(steps-i)*4-1]);
                newTriangle(verts[steps-i][0],verts[steps-i+1][0],verts[steps-i+1][(steps-i)*4+quadrante] );
        }
    }

    esse1 = newVertex(_raio,0,0);
    newTriangle(esse1,verts[SLICES-2][0],verts[SLICES-2][1]);
    newTriangle(esse1,verts[SLICES-2][1],verts[SLICES-2][2]);
    newTriangle(esse1,verts[SLICES-2][2],verts[SLICES-2][3]);
    newTriangle(esse1,verts[SLICES-2][3],verts[SLICES-2][0]);
    esse1 = newVertex(-_raio,0,0);
    newTriangle(esse1,verts[1][1],verts[1][0]);
    newTriangle(esse1,verts[1][2],verts[1][1]);
    newTriangle(esse1,verts[1][3],verts[1][2]);
    newTriangle(esse1,verts[1][0],verts[1][3]);

    // compute normals
    computeAllNormals(true);

}