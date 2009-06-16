//---------------------------------------------------------------------------

#ifndef quadradoH
#define quadradoH

#include "cMesh.h"
#include "cWorld.h"
#include "cor.h"

class Quadrado : public cMesh{

        public:
                //Construtores
                Quadrado(cWorld* a_world, float _largura):cMesh(a_world){
                        largura = _largura;
                        criarQuadrado();
                }

                Quadrado(cWorld* a_world, float _largura, Cor* _cor):cMesh(a_world){
                        largura = _largura;
                        cor = _cor;
                        criarQuadrado();
                }

                //Atributos
                float largura;
                Cor* cor;

                //Método que seta a cor do quadrado
                void setCor(Cor* _cor){
                        this->getVertex(p1)->setColor(cor->vermelho,cor->verde,cor->azul,1);
                        this->getVertex(p2)->setColor(cor->vermelho,cor->verde,cor->azul,1);
                        this->getVertex(p3)->setColor(cor->vermelho,cor->verde,cor->azul,1);
                        this->getVertex(p4)->setColor(cor->vermelho,cor->verde,cor->azul,1);

                        this->computeAllNormals(true);
                }

                void aplicarTextura(){
                        this->getVertex(p1)->setTexCoord(0,0);
                        this->getVertex(p2)->setTexCoord(0,1);
                        this->getVertex(p3)->setTexCoord(1,0);
                        this->getVertex(p4)->setTexCoord(1,1);
                }

        private:
                int p1, p2, p3, p4;
                //Método que cria o quadrado utilizado nos construtores
                void criarQuadrado(){

                    if(!cor){
                        cor = new Cor(1.0,0,0);
                    }

                    this->useColors(true);

                    p1 = this->newVertex(-0.5*largura, -0.5*largura, 0);
                    p2 = this->newVertex( 0.5*largura, -0.5*largura, 0);
                    p3 = this->newVertex(-0.5*largura, 0.5*largura, 0);
                    p4 = this->newVertex( 0.5*largura, 0.5*largura, 0);

                    this->newTriangle(p1, p2, p3);
                    this->newTriangle(p2, p4, p3);

                    setCor(cor);
                    this->computeAllNormals(true);
                }

};
//---------------------------------------------------------------------------
#endif
