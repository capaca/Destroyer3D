//---------------------------------------------------------------------------


#pragma hdrstop

#include "jogo.h"
#define LARGURAQUADRADO 50
#define LARGURAPISTA 50
#define LARGURAPARALELEPIPEDO 0

//---------------------------------------------------------------------------

#pragma package(smart_init)

Jogo::Jogo(dynamicWorld *mundo_, int dimensao_){

        mundo = mundo_;
        dimensao = dimensao_;
        init();
}

Jogo::~Jogo(){
    //delete bolaCanhao;
    delete predios;
    delete v1;
    delete v2;
    delete val;
    delete objetos;
}
//---------------------------------------------------------------------------
void Jogo::init(){

        larguraQuadrado = LARGURAQUADRADO;
        larguraPista = LARGURAPISTA;
        largPar = LARGURAPARALELEPIPEDO;
        translacao = larguraQuadrado+larguraPista;
        v1 = new vector<cVector3d>();
        v2 = new vector<cVector3d>();
        val = new vector<double>();
        objetos = new vector<cGenericObject*>();
        tempo = 8*dimensao*dimensao;
        numPrediosAtingidos = 0;

        carro = new Carro(mundo);
        mundo->addChild(carro);

        posicionarCarro();
        adicionarBolaCanhao();

        criarPredios();
        criarChao();
}
//---------------------------------------------------------------------------
void Jogo::criarPredios(){
        predios = new vector<dynamicObject*>();

        cTexture2D* texturaTijolos = new cTexture2D();
        texturaTijolos->loadFromFile("tijolos.bmp");

        for(int i=0; i<dimensao; i++){
                for(int j=0; j<dimensao; j++){
                        largPar = (rand() % 4 + 1) * 6;

                        paralelepipedo *paralelepipedo1 = new paralelepipedo(mundo,0.8,largPar,largPar*1.5,largPar);
                        paralelepipedo1->setTexture(texturaTijolos,true);
                        paralelepipedo1->aplicarTextura();
                        predios->push_back(paralelepipedo1);
                        mundo->addChild(paralelepipedo1);
                        paralelepipedo1->setPos(translacao*j,0,translacao*i);
                        paralelepipedo1->translate(cVector3d(0,(largPar*1.5)/2,0));
                }
        }

}
//---------------------------------------------------------------------------
void Jogo::criarChao(){
        int dimensaoChao = dimensao*5;

        cMesh* chao = new cMesh(mundo);
        int largQuad = 40;

        cTexture2D* texturaGrama = new cTexture2D();
        texturaGrama->loadFromFile("moss2.bmp");


        for(int i=0; i<dimensaoChao; i++){
                for(int j=0; j<dimensaoChao; j++){
                        Quadrado *quadrado1 = new Quadrado(mundo,largQuad,new Cor(1,1,1));
                        quadrado1->setTexture(texturaGrama,false);
                        quadrado1->aplicarTextura();
                        quadrado1->rotate(cVector3d(1,0,0),-(3.14/2));
                        quadrado1->translate(cVector3d(((largQuad-0.5)*j),0,((largQuad-0.5)*i)));
                        chao->addChild(quadrado1);
                }
        }

        mundo->addChild(chao);
}
//---------------------------------------------------------------------------
bool Jogo::verificarColisoes()
{
        //Testa a colisao
        vector<dynamicObject*>::iterator itPredios = predios->begin();

        while(itPredios!=predios->end()){
                if(bolaCanhao->testCollision(*itPredios,v1,v2,val,objetos)>0 &&
                   (*itPredios)->getShow() && bolaCanhao->getShow()){

                        cGenericObject* predio = *itPredios;

                        predio->setShow(false,false);

                        bolaCanhao->setShow(false,true);
                        bolaCanhao->setVelocity(cVector3d(0,0,0));
                        
                        numPrediosAtingidos++;
                        return true;
                }

                ++itPredios;
        }

        return false;
}
//---------------------------------------------------------------------------
void Jogo::adicionarBolaCanhao() {
        cMaterial material1;
        material1.m_ambient.set( 0.5, 0.0, 0.0, 1.0 );
        material1.m_diffuse.set( 0.8, 0.6, 0.6, 1.0 );
        material1.m_specular.set( 0.9, 0.9, 0.9, 1.0 );
        material1.setShininess(100);

        bolaCanhao = new bola(mundo, 0.01,1.2);
        bolaCanhao->m_material = material1;
        bolaCanhao->setShow(false);
        bolaCanhao->preventGravity();
        mundo->addChild(bolaCanhao);

}
//---------------------------------------------------------------------------
bool Jogo::isTodosAlvosDestruidos(){
        return (predios->size()>0 && predios->size() == numPrediosAtingidos);
}
//---------------------------------------------------------------------------
bool Jogo::isTempoAcabado(){
        return tempo<=0;
}
//---------------------------------------------------------------------------
void Jogo::posicionarCarro() {
        int trans = (larguraQuadrado+larguraPista)/2;
        carro->translate(0,0,trans);
}
//---------------------------------------------------------------------------
void Jogo::atirar(){
        this->bolaCanhao->setVelocity(cVector3d(0,0,0));
        this->bolaCanhao->setShow(true);
        cVector3d nforca= cVector3d(1600,0,0),forca;
        cMatrix3d rot=this->carro->meio->getGlobalRot();
        rot.mulr(nforca,forca);

        cVector3d posicaoGlobal = this->carro->carroceria->getGlobalPos();
        cVector3d vetorZerado = cVector3d(0,0,0);
        this->bolaCanhao->reposition(posicaoGlobal,vetorZerado,vetorZerado);
        this->bolaCanhao->applyCenterOfMassForce(forca);
}
