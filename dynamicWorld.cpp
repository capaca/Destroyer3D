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

#include "dynamicWorld.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

cVector3d positionTransform(cVector3d origPos,const cMatrix3d rot, cVector3d trans);

dynamicWorld::dynamicWorld(bool grav) {

        gravity = grav;

}

bool dynamicWorld::getGravity() {

        return gravity;

}

void dynamicWorld::setGravity(bool grav) {

        gravity = grav;

}

void dynamicWorld::integrate(double dt) {

        dynamicObject *child;

        lastDt= dt;
        // check if there are external forces being applied to objects
        checkForces();

        for(int i=0;i<getNumChildren();i++) {

                // if this is a dynamicObject
                if(child=dynamic_cast<dynamicObject*>(getChild(i))) {

                        child->integrate(dt);

                }
        }
}

void dynamicWorld::checkForces() {

        checkHapticForces();

        checkCollisions();

}

void dynamicWorld:: checkHapticForces() {

        cCamera *child;
        cMeta3dofPointer *grandchild;

        for(int i=0;i<getNumChildren();i++) {

                // if this is a cCamera
                if(child=dynamic_cast<cCamera*>(getChild(i))) {

                    for(int j=0;j<child->getNumChildren();j++) {

                        // if this is a cMeta3dofPointer
                        if(grandchild=dynamic_cast<cMeta3dofPointer*>(child->getChild(j))) {

                            cProxyPointForceAlgo *proxy=grandchild->getProxy();
                            dynamicObject *touched = dynamic_cast<dynamicObject*>(proxy->getContactObject());

                            if(touched)
                                touched->applyForce(proxy->getLastGlobalForce(),
                                                proxy->getContactPoint());
                        }
                    }
                }
        }
}

// check collisions between dynamicObjects and other meshes

void dynamicWorld::checkCollisions() {

        dynamicObject *child1;
        cMesh *child2;

        // assuming that all meshes in the world have a sphereTree collision detector
        for(int i=0;i<getNumChildren();i++) {
           // if this is a dynamicObject
           if(child1=dynamic_cast<dynamicObject*>(getChild(i))) {

                // check collisions with the next cMeshes on the list to avoid
                // repeating a same mesh-mesh combination twice
                for(int j=i+1;j<getNumChildren();j++) {

                        if(child2=dynamic_cast<cMesh*>(getChild(j)))

                                checkMeshMeshCollision(child1,child2);
                }
           }
        }
}

void dynamicWorld::checkMeshMeshCollision(dynamicObject *mesh1, cMesh *mesh2) {

        //double pene;
        //cVector3d normal,contact;
        //cGenericObject *touched;
        std::vector<cVector3d> *normal= new std::vector<cVector3d>;
        std::vector<cVector3d> *contact = new std::vector<cVector3d>;
        std::vector<double> *pene = new std::vector<double>;
        std::vector<cGenericObject *> *touched = new std::vector<cGenericObject *>;
        int touchs;

        double  r1 = cDistance(mesh1->getBoundaryMax(),mesh1->getBoundaryMin())/2.0,
                r2 = cDistance(mesh2->getBoundaryMax(),mesh2->getBoundaryMin())/2.0;

        if(cDistance(mesh1->getCenterOfMass(true),mesh2->getCenterOfMass(true))<(r1+r2)) {

                if(touchs=mesh1->testCollision(mesh2,normal,contact,pene,touched)) {

                    cVector3d vai = mesh1->getVelocity(),
                              wai = mesh1->getAngularVelocity();

                    double ma = mesh1->getMass();
                    cMatrix3d MIa = mesh1->getInertiaMatrix(true);

                    for(int i=0;i<touchs;i++) {
                      // the touched objects can be mesh2 itself or one of its child
                      // test if this touched object is a dynamic object
                      dynamicObject *obj2 = dynamic_cast<dynamicObject*>((*touched)[i]);
                      if(obj2) {

                        // if both are rigid use impulse
                        if((mesh1->rigid)&&(obj2->rigid)) {

                            cVector3d vaf,vbf,waf,wbf;
                            cVector3d vbi = obj2->getVelocity(),
                                      wbi = obj2->getAngularVelocity(),
                                      ra = cSub((*contact)[i],mesh1->getDynCenterOfMass()),
                                      rb = cSub((*contact)[i],obj2->getDynCenterOfMass());

                            CollisionResponse(0.5*(mesh1->getRestitution()+obj2->getRestitution()),
                                        ma,obj2->getMass(),
                                        MIa,obj2->getInertiaMatrix(true),
                                        ra,rb,(*normal)[i],vai,vbi,wai,wbi,
                                        vaf,vbf,waf,wbf,(*pene)[i]);

                            mesh1->setVelocity(vaf);
                            obj2->setVelocity(vbf);
                            mesh1->setAngularVelocity(waf);
                            obj2->setAngularVelocity(wbf);
                        }
                      }
                      else {
                        // collided with a static mesh
                        if(mesh1->rigid) {

                            cVector3d vaf,vbf,waf,wbf;
                            cVector3d vbi,
                                      ra = cSub((*contact)[i],mesh1->getDynCenterOfMass()),
                                      rb = cSub((*contact)[i],mesh2->getCenterOfMass()),
                                      wbi;
                            vbi.zero();
                            wbi.zero();
                            cMatrix3d heavy;
                            heavy.set(BIGNUMBER,0,0,
                                      0,BIGNUMBER,0,
                                      0,0,BIGNUMBER);

                            double rest;
                            if((*touched)[i]->m_userData!=NULL)
                                rest = (mesh1->getRestitution()+(*(double*)(*touched)[i]->m_userData))*0.5;
                            else
                                rest = mesh1->getRestitution();

                            CollisionResponse(rest,ma,BIGNUMBER,
                                        MIa,heavy,ra,rb,
                                        (*normal)[i],vai,vbi,wai,wbi,
                                        vaf,vbf,waf,wbf,(*pene)[i]);

                            mesh1->setVelocity(vaf);
                            mesh1->setAngularVelocity(waf);
                            mesh1->preventGravity();
                        }
                    }
                }
              }
           }
}

/**
Adapted from:
http://www.euclideanspace.com/physics/dynamics/collision/twod/

This function calulates the velocities after a 3D collision vaf, vbf, waf and wbf from information about the colliding bodies
@param double e coefficient of restitution which depends on the nature of the two colliding materials
@param double ma total mass of body a
@param double mb total mass of body b
@param matrix IaInverse: inverse of the inertia tensor for body a in absolute coordinates (if this is known in local body coordinates it must
                 be converted before this is called).
@param matrix IbInverse: inverse of the inertia tensor for body b in absolute coordinates (if this is known in local body coordinates it must
                 be converted before this is called).
@param vector ra position of collision point relative to centre of mass of body a in absolute coordinates (if this is
                 known in local body coordinates it must be converted before this is called).
@param vector rb position of collision point relative to centre of mass of body b in absolute coordinates (if this is
                 known in local body coordinates it must be converted before this is called).
@param vector n normal to collision point, the line along which the impulse acts.
@param vector vai initial velocity of centre of mass on object a
@param vector vbi initial velocity of centre of mass on object b
@param vector wai initial angular velocity of object a
@param vector wbi initial angular velocity of object b
@param vector vaf final velocity of centre of mass on object a
@param vector vbf final velocity of centre of mass on object a
@param vector waf final angular velocity of object a
@param vector wbf final angular velocity of object b
*/



void dynamicWorld::CollisionResponse(double e,double ma,double mb,
                                     cMatrix3d MIa,cMatrix3d MIb,
                                     cVector3d ra,cVector3d rb,cVector3d n,
                                     cVector3d vai, cVector3d vbi,
                                     cVector3d wai, cVector3d wbi,
                                     cVector3d& vaf, cVector3d& vbf,
                                     cVector3d& waf, cVector3d& wbf,
                                     double pene) {


        cMatrix3d Ia = MIa, Ib=MIb;
        Ia.invert();
        Ib.invert();

        cVector3d vpa = cAdd(vai,cCross(wai,ra));
        cVector3d vpb = cAdd(vbi,cCross(wbi,rb));
        cVector3d vab = cSub(vpa,vpb);
        double normalV = cDot(vab,n);

        if(normalV>0) {
                // objects are receding
                vaf = vai;
                waf = wai;
                vbf = vbi;
                wbf = wbi;
                return;
        }

        double linearMoment = cMul(ma,vai).length() + cMul(mb,vbi).length();
        double angularMoment = cMul(MIa,wai).length() + cMul(MIb,wbi).length();

        cVector3d normal = cNormalize(n);
        cVector3d angularVelChangea  = normal;

        // start calculating the change in angular rotation of a
        angularVelChangea.cross(ra);
        angularVelChangea = cMul(Ia,angularVelChangea);
        cVector3d vaLinDueToR = cCross(angularVelChangea,ra);

        // calculate the linear velocity of collision point on a due to rotation of a
        double scalar = 1/ma + cDot(vaLinDueToR,normal);
        cVector3d angularVelChangeb = normal;

        // start calculating the change in angular rotation of b
        angularVelChangeb.cross(rb);
        angularVelChangeb = cMul(Ib,angularVelChangeb);
        cVector3d vbLinDueToR = cCross(angularVelChangeb,rb);

        // calculate the linear velocity of collision point on b due to rotation of b
        scalar += 1/mb + cDot(vbLinDueToR,normal);
        //double Jmod = (e+1)*cSub(vai,vbi).length()/scalar;

        double Jmod = -(e+1)*cDot(vab,normal)/scalar;
        cVector3d J = cMul(Jmod,normal);
        cVector3d impulseA = cMul(1/ma,J);

        /*double Jmod = -(1+e)*cDot(cSub(vai,vbi),normal)/
                (cDot(normal,normal)*(1/ma+1/mb)+pow(cDot(ra,normal),2)*/
        vaf = cAdd(vai, impulseA);
        angularVelChangea = cMul(Ia,cCross(J,ra));
        waf = cSub(wai, angularVelChangea);
        cVector3d impulseB = cMul(1/mb,J);

        if(mb<BIGNUMBER) { // I used a big mass to represent static objects but numerical instabilities calculate a tiny impulse that makes a big moment
                vbf = cSub(vbi, impulseB);

                angularVelChangeb = cMul(Ib,cCross(J,rb));
                wbf = cAdd(wbi, angularVelChangeb);
        }
        else  {
                vbf.zero();
                wbf.zero();

        }


        double linearMomentf = cMul(ma,vaf).length() + cMul(mb,vbf).length();
        double angularMomentf = cMul(MIa,waf).length() + cMul(MIb,wbf).length();

        double ratio;
        ratio = e*(angularMoment+linearMoment)/(angularMomentf+linearMomentf);
        vaf.mul(ratio);
        vbf.mul(ratio);
        waf.mul(ratio);
        wbf.mul(ratio);
}

/*

Original code:

CollisionResponce(double e,double ma,double mb,matrix Ia,matrix Ib,vector ra,vector rb,vector n,
    vector vai, vector vbi, vector wai, vector wbi, vector vaf, vector vbf, vector waf, vector wbf) {
  matrix IaInverse = Ia.inverse();
  vector normal = n.normalise();
  vector angularVelChangea  = normal.copy(); // start calculating the change in abgular rotation of a
  angularVelChangea.cross(ra);
  IaInverse.transform(angularVelChangea);
  vector vaLinDueToR = angularVelChangea.copy().cross(ra);  // calculate the linear velocity of collision point on a due to rotation of a
  double scalar = 1/ma + vaLinDueToR.dot(normal);
  matrix IbInverse = Ib.inverse();
  vector angularVelChangeb = normal.copy(); // start calculating the change in abgular rotation of b
  angularVelChangeb.cross(rb);
  IbInverse.transform(angularVelChangeb);
  vector vbLinDueToR = angularVelChangeb.copy().cross(rb);  // calculate the linear velocity of collision point on b due to rotation of b
  scalar += 1/mb + vbLinDueToR.dot(normal);
  double Jmod = (e+1)*(vai-vbi).magnitude()/scalar;
  vector J = normal.mul(Jmod);
  vaf = vai - J.mul(1/ma);
  vbf = vbi - J.mul(1/mb);
  waf = wai - angularVelChangea;
  wbf = wbi - angularVelChangeb;
} */

cVector3d positionTransform(cVector3d origPos,const cMatrix3d rot, cVector3d trans) {

        cVector3d globalPosition;
        rot.mulr(origPos, globalPosition);
        globalPosition.add(trans);

        return globalPosition;
}


