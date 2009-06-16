//---------------------------------------------------------------------------

#ifndef jogoH
#define jogoH

#include "dynamicWorld.h"
#include "bola.h"
#include "carro.h"
#include "paralelepipedo.h"
#include "quadrado.h"

class Jogo {

        public:
                //Contructor & Destructor
                Jogo(dynamicWorld *mundo, int dimensao);
                ~Jogo();
                
                dynamicWorld* mundo;
                int dimensao, numPrediosAtingidos;
                bola* bolaCanhao;
                vector<dynamicObject*> *predios;
                Carro* carro;

                //Deteccao de Colisao
                vector<cVector3d> *v1, *v2;
                vector<double> *val;
                vector<cGenericObject*> *objetos;

                //Flag de tiro
                bool atirou;

                //Dados dos predios
                int larguraQuadrado, larguraPista, largPar, translacao;

                //Tempo
                unsigned int tempo;

                //Métodos
                void init();
                void criarPredios();
                void criarChao();
                bool verificarColisoes();
                bool isTodosAlvosDestruidos();
                bool isTempoAcabado();
                void adicionarBolaCanhao();
                void posicionarCarro();
                void atirar();
                
};
//---------------------------------------------------------------------------
#endif
