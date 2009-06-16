//---------------------------------------------------------------------------

#ifndef cuboH
#define cuboH

#include "cMesh.h"
#include "cWorld.h"
#include "quadrado.h"

class Cubo : public cMesh{

        public:
                //Construtores
                Cubo(cWorld* a_world, float _largura):cMesh(a_world){
                        largura = _largura;
                        criarCubo();
                }

                Cubo(cWorld* a_world, float _largura, Cor* _cor):cMesh(a_world){
                        largura = _largura;
                        cor = _cor;
                        criarCubo();
                }

                //Atributos
                int largura;
                Cor* cor;
                Quadrado *faces[6];

                Quadrado* getFace(int nFace){
                        if(nFace >= 0 ||  nFace <= 5)
                                return faces[nFace];

                        return NULL;
                }

        private:
                //Método que cria o cubo
                void criarCubo(){

                        this->useColors(true);
                        this->showNormals(true);

                        for(int i=0;i<6;i++) {
                                faces[i] = new Quadrado(this->getParentWorld(),largura,this->cor);
                                this->addChild(faces[i]);

                                switch (i) {
                                        case 0:
                                                faces[i]->translate(0,0,largura/2.0);
                                                break;

                                        case 1:
                                                faces[i]->translate(0,0,-largura/2.0);
                                                faces[i]->rotate(cVector3d(0,1,0),3.1416);
                                                break;
                                        case 2:
                                                faces[i]->rotate(cVector3d(1,0,0),-3.1416/2.0);
                                                faces[i]->translate(0,largura/2.0,0);
                                                break;

                                        case 3:
                                                faces[i]->rotate(cVector3d(1,0,0),3.1416/2.0);
                                                faces[i]->translate(0,-largura/2.0,0);
                                                break;

                                        case 4:
                                                faces[i]->rotate(cVector3d(0,1,0),3.1416/2.0);
                                                faces[i]->translate(largura/2.0,0,0);
                                                break;
                                                
                                        case 5:
                                                faces[i]->rotate(cVector3d(0,1,0),-3.1416/2.0);
                                                faces[i]->translate(-largura/2.0,0,0);
                                                break;
                                }
                        }
                }

};

//---------------------------------------------------------------------------
#endif