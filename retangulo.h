//---------------------------------------------------------------------------

#ifndef retanguloH
#define retanguloH

#include "cMesh.h"
#include "cWorld.h"
#include "cor.h"

class Retangulo : public cMesh{

        public:
                //Construtores
                Retangulo(cWorld* a_world, float _largura, float _altura):cMesh(a_world){
                        largura = _largura;
                        altura = _altura;
                        criarRetangulo();
                }

                Retangulo(cWorld* a_world, float _largura, float _altura, Cor* _cor):cMesh(a_world){
                        largura = _largura;
                        altura = _altura;
                        cor = _cor;
                        criarRetangulo();
                }

                //Atributos
                float largura, altura;
                Cor* cor;

                //Método que seta a cor do retangulo
                void setCor(Cor* _cor){
                        this->getVertex(p1)->setColor(cor->vermelho,cor->verde,cor->azul,1);
                        this->getVertex(p2)->setColor(cor->vermelho,cor->verde,cor->azul,1);
                        this->getVertex(p3)->setColor(cor->vermelho,cor->verde,cor->azul,1);
                        this->getVertex(p4)->setColor(cor->vermelho,cor->verde,cor->azul,1);

                        this->computeAllNormals(true);
                }

        private:
                int p1, p2, p3, p4;
                //Método que cria o quadrado utilizado nos construtores
                void criarRetangulo(){

                    if(!cor){
                        cor = new Cor(1.0,0,0);
                    }

                    this->useColors(true);

                    p1 = this->newVertex(-0.5*largura, -0.5*altura, 0);
                    p2 = this->newVertex( 0.5*largura, -0.5*altura, 0);
                    p3 = this->newVertex(-0.5*largura, 0.5*altura, 0);
                    p4 = this->newVertex( 0.5*largura, 0.5*altura, 0);

                    this->newTriangle(p1, p2, p3);
                    this->newTriangle(p2, p4, p3);

                    setCor(cor);
                    this->computeAllNormals(true);

                }

};
//---------------------------------------------------------------------------
#endif
