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

#include "dynamicObject.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

cVector3d global2LocalPosition(cVector3d globalPosition, cMatrix3d rot, cVector3d pos);

cVector3d localtoGlobalPosition(cVector3d localPosition, cMatrix3d rot, cVector3d pos);


cVector3d computeNormal(cTriangle *tri) {

        cVector3d verts[3];

        verts[0]=tri->getVertex0()->getGlobalPos();
        verts[1]=tri->getVertex1()->getGlobalPos();
        verts[2]=tri->getVertex2()->getGlobalPos();

        // compute normal vector
        cVector3d normal, v01, v02;
        verts[1].subr(verts[0], v01);
        verts[2].subr(verts[0], v02);
        v01.crossr(v02, normal);
        if(normal.length()>0)
                normal.normalize();

        return normal;
}



dynamicObject::dynamicObject(dynamicWorld *world, double imass, bool irigid):
              cMesh(world) {

        myworld = world;
        mass = imass;
        rigid=irigid;

        inertiaMatrix.identity();
        inertiaMatrixInv.identity();

        force.zero();
        torque.zero();
        acceleration.zero();
        angAcceleration.zero();
        velocity.zero();
        angularVelocity.zero();
        testPoints.clear();
        clear();

        restitution = 0;
        noGravityStep = false;

        collisionDetectionUseVertices = true;


}

// init should be called by the subclass constructor once the mesh was created
void dynamicObject::init() {

        centerOfMass = getCenterOfMass();

        computeBoundaryBox(true);
        computeAllNormals(true);
        createSphereTreeCollisionDetector(true,true);

        // compute the collision test points
        computeTestPoints();

}

void dynamicObject::computeTestPoints() {

        double radius = cDistance(getBoundaryMax(),getBoundaryMin())/2.0;

        double angle1,angle2,steps=TESTCOLLISIONSLICES/2.0;
        int i,j,quadrante;
        cVector3d direction;

        for(i=0;i<steps;i++) {
                quadrante=0;
                angle1=i*0.5*3.1416/steps;
                direction.x=sin(angle1)*radius;
                for(j=0;j<(steps-i)*4;j++) {
                        angle2=j*0.5*3.1416/(steps-i);
                        direction.z=sin(angle2)*cos(angle1)*radius;
                        direction.y=cos(angle2)*cos(angle1)*radius;

                        createTestPoint(direction);

                        if(i>0) {
                                // repeat for the other hemisphere
                                cVector3d inverso = direction;
                                inverso.x*=-1.0;
                                createTestPoint(inverso);
                       }
                }
        }
}

void dynamicObject:: createTestPoint(cVector3d direction) {

        cVector3d colPoint,segmA = cAdd(centerOfMass,direction);
        cGenericObject *obj;
        cTriangle *tri;
        double dist=BIGNUMBER;

        // projects a point on the object surface following a direction
        // from the center passed as input

        computeCollisionDetection(segmA,centerOfMass, obj, tri, colPoint,dist, true, -1);

        testPoints.push_back(colPoint);

}

bool dynamicObject::testCollision(cMesh *otherOne, cVector3d& contactNormal, cVector3d& contactPoint, double& penetration,cGenericObject **touched) {

        int i,cont=0;
        cVector3d test,myNormal,myContactMax,myContactMin,contact;
        cGenericObject *obj;
        cTriangle *tri;
        double dist=BIGNUMBER,mypene=0;
        myContactMax=cVector3d(-BIGNUMBER,-BIGNUMBER,-BIGNUMBER);
        myContactMin=cVector3d(BIGNUMBER,BIGNUMBER,BIGNUMBER);
        myNormal.zero();

        for(i=0;i<getNumVertices()+testPoints.size();i++) {

                if(i<getNumVertices())
                        //test collision between a vertex and the other mesh
                        test = getVertex(i)->getGlobalPos();
                else
                        //test collision between a test point and the other mesh
                        test = local2GlobalPosition(testPoints[i-getNumVertices()]);

                // dist must be set to a high value ...
                if(otherOne->computeCollisionDetection(centerOfMass,test,
                                obj, tri, contact,dist, true, -1)) {
                        cont++;
                        mypene += dist;
                        *touched = obj;
                        myContactMax = cVector3d(myContactMax.x>contact.x?myContactMax.x:contact.x,
                                                 myContactMax.y>contact.y?myContactMax.y:contact.y,
                                                 myContactMax.z>contact.z?myContactMax.z:contact.z);
                        myContactMin = cVector3d(myContactMin.x<contact.x?myContactMin.x:contact.x,
                                                 myContactMin.y<contact.y?myContactMin.y:contact.y,
                                                 myContactMin.z<contact.z?myContactMin.z:contact.z);
                        myNormal = cAdd(myNormal,computeNormal(tri));
                }

        }

        if(cont) {
                contactPoint = cMul(1/2.0,cAdd(myContactMax,myContactMin));
                contactNormal = cNormalize(myNormal);
                penetration = mypene/(double)cont;
        }

        return cont;

}

// test collisions between vertices and test points of this mesh and a given mesh
int dynamicObject::testCollision(cMesh *otherOne,
                     std::vector<cVector3d> *contactNormal,
                     std::vector<cVector3d> *contactPoint,
                     std::vector<double> *penetration,
                     std::vector<cGenericObject *>*touched) {

        int i,j,cont=0;
        cVector3d test,contact;
        std::vector<cTriangle*> tris;
        std::vector<bool> frontContact;

        contactNormal->clear();
        contactPoint->clear();
        penetration->clear();
        touched->clear();
        tris.clear();
        frontContact.clear();

        cGenericObject *obj;
        cTriangle *tri;
        double dist=BIGNUMBER;
        int tests = testPoints.size(),testVertices=0;
        cMesh *esse = this;

        if(collisionDetectionUseVertices)  {
                testVertices = getNumVertices();
                if(testVertices==0) { // 3ds loaded from file
                        if(esse=dynamic_cast<cMesh*>(getChild(0)))
                                testVertices = esse->getNumVertices();
                        else
                                esse = this;
                }
                tests += testVertices;
        }

        for(i=0;i<tests;i++) {

                if(i<testVertices)
                       //test collision between a vertex and the other mesh
                        test = esse->getVertex(i)->getGlobalPos();
                else

                        //test collision between a test point and the other mesh
                        test = local2GlobalPosition(testPoints[i-testVertices]);


                // dist must be set to a high value ...
                dist=BIGNUMBER;
                //cVector3d testLastPosition = cMul(cTrans(lastRotation),cAdd(test,cMul(-lastDt,velocity)));
                cVector3d testLastPosition = cAdd(centerOfMass,cMul(-lastDt,velocity));
                
                if(otherOne->computeCollisionDetection(centerOfMass,test,
                                obj, tri, contact,dist, false, -1)) {

                        int pos=-1;
                        cVector3d trinormal = computeNormal(tri);

                        double pe = cDot(cSub(test,contact),trinormal);

                        for(j=0;j<touched->size();j++) {
                                // test if this object was already tested for contact
                                if((*touched)[j]==obj)
                                        pos=j;

                        }
                        if(pos<0) {
                                // new contact
                                pos=cont;
                                cont++;

                                tris.push_back(tri);
                                penetration->push_back(pe);
                                contactNormal->push_back(trinormal);
                                contactPoint->push_back(contact);
                                touched->push_back(obj);
                                frontContact.push_back(false);

                                if(i<testVertices)  // vertex face collision
                                        // stop collision detection
                                        i=tests;

                       }

                       if((i<tests)&&(!frontContact[pos]))  {
                                // test if this contact is better

                                if(pe<(*penetration)[pos]) {
                                        (*penetration)[pos] = pe;
                                        (*contactNormal)[pos] = trinormal;
                                        (*contactPoint)[pos] = contact;
                                        tris[pos] = tri;

                                }


                                // test if this is a frontal collision:
                                // if the angle between the collision normal and the test direction is less than the
                                // angle between 2 succesive tests then the collision can be frontal
                                // try the collision straight inside the triangle (would be the deepest penetration)

                                cVector3d testDirection = cNormalize(cSub(centerOfMass,test));
                                double ang = 2*(0.5*cDistance(testDirection,trinormal));

                                if(ang<0.5*3.1416/TESTCOLLISIONSLICES) {
                                        cVector3d end = cAdd(centerOfMass,
                                                 cMul(-1*cDistance(centerOfMass,test),trinormal));

                                        cVector3d lp1 = global2LocalPosition(centerOfMass,obj->getGlobalRot(), obj->getGlobalPos()),
                                                  lp2 = global2LocalPosition(cAdd(centerOfMass,cMul(-1,trinormal)),obj->getGlobalRot(), obj->getGlobalPos());

                                        dist=BIGNUMBER;
                                        if(tri->computeCollision(lp1,cSub(lp2,lp1),obj,tri, contact,dist)) {

                                                (*penetration)[pos]=-cDistance(end,contact);
                                                (*contactPoint)[pos]=localtoGlobalPosition(contact,obj->getGlobalRot(),obj->getGlobalPos());
                                                frontContact[pos] = true;
                                                tris[pos] = tri;
                                                // stop collision detection (???)
                                                //i=tests;
                                        }
                                }
                       }
                }
        }

        return cont;
}

void dynamicObject::reposition(cVector3d newpos,cVector3d newvelo,cVector3d newangvelo) {

        centerOfMass = newpos;
        setPos(centerOfMass);
        cMatrix3d m;
        m.identity();
        setRot(m);
        computeGlobalPositions(false);
        computeAllNormals();

        force.zero();
        torque.zero();
        acceleration.zero();
        angAcceleration.zero();
        velocity=newvelo;
        angularVelocity=newangvelo;
}

void dynamicObject::integrateVelocity(double dt) {

        velocity0 = velocity;

        velocity = cAdd(velocity,cMul(dt,acceleration));

        // using some damping to dissipate energy (??)
        velocity.sub(cMul(DISSIPATION,velocity));

}


void dynamicObject::integratePosition(double dt) {

        centerOfMass0 = centerOfMass;

        centerOfMass = cAdd(centerOfMass,cMul(dt,velocity));

        setPos(centerOfMass);

}

void dynamicObject::calcAcceleration(double dt) {

        acceleration = cMul(1/mass,force);
        if(myworld->getGravity()&&(!noGravityStep))
                acceleration = cAdd(acceleration,cVector3d(0,-9.8,0));

        noGravityStep=false;

}

void dynamicObject::applyForce(cVector3d iforce, cVector3d posiforce) {

        // compute force e torque
        cVector3d r = cSub(posiforce,centerOfMass);


        if((r.length()>0)&&(iforce.length()>0)) {
                // F* = -||r||.(F • ||r||)
                force = cAdd(force,cMul(-1*cDot(iforce,cNormalize(r)),cNormalize(r)));

                // T = F × r
                torque = cAdd(torque,cCross(iforce,r));
        }

}

void dynamicObject::applyCenterOfMassForce(cVector3d iforce) {
        force = cAdd(force,iforce);
}

void dynamicObject::rollBackIntegration(double dt) {

        force = force0;
        torque = torque0;
        velocity = velocity0;
        angularVelocity = angularVelocity0;
        centerOfMass = centerOfMass0;
        setRot(lastRotMatrix);

        integrate(dt);

}

void dynamicObject::integrate(double dt) {

        // calculate accelerations
        calcAcceleration(dt);
        calcAngularAcceleration(dt);

        // integrate velocity (linear e angular)
        integrateVelocity(dt);
        integrateAngularVelocity(dt);

        // integrate position (linear e angular)
        integratePosition(dt);
        integrateOrientation(dt);

        updateGlobalPositions(false);
        computeAllNormals();

        // reset forces
        torque0 = torque;
        force0 = force;
        torque.zero();
        force.zero();

        lastDt = dt;

}

void dynamicObject::integrateOrientation(double dt) {

        lastGiro = cMul(dt,angularVelocity);

        lastRotMatrix = getRot();
        
        // rotate
        if(lastGiro.length()>0)   {
                //rotate(cNormalize(igiro),igiro.length());
                rotate(cVector3d(1,0,0),lastGiro.x);
                rotate(cVector3d(0,1,0),lastGiro.y);
                rotate(cVector3d(0,0,1),lastGiro.z);
        }

}

void dynamicObject::integrateAngularVelocity(double dt) {

        angularVelocity0 = angularVelocity;

        angularVelocity = cAdd(angularVelocity,cMul(dt,angAcceleration));

       // using some damping to dissipate energy (??)
       angularVelocity.sub(cMul(DISSIPATION,angularVelocity));


}

void dynamicObject::calcAngularAcceleration(double dt) {

       angAcceleration = cMul(inertiaMatrixInv,torque);
}


cVector3d dynamicObject::local2GlobalPosition(cVector3d localPosition) {

        cVector3d globalPosition;
        getRot().mulr(localPosition, globalPosition);
        globalPosition.add(getPos());

        return globalPosition;
}

cVector3d localtoGlobalPosition(cVector3d localPosition, cMatrix3d rot, cVector3d pos) {

        cVector3d globalPosition;
        rot.mulr(localPosition, globalPosition);
        globalPosition.add(pos);

        return globalPosition;
}

cVector3d global2LocalPosition(cVector3d globalPosition, cMatrix3d rot, cVector3d pos) {

        cVector3d localPosition;
        globalPosition.sub(pos);
        rot.trans();
        rot.mulr(globalPosition,localPosition);

        return localPosition;
}

cMatrix3d dynamicObject::getInertiaMatrix(bool transform2Global) {

        if(transform2Global) {

                cMatrix3d rotrans = getRot();
                rotrans.trans();

                cMatrix3d ret = cMul(cMul(rotrans,inertiaMatrix),getRot());
                return ret;

        }
        else
                return inertiaMatrix;

}

void dynamicObject::setInertiaMatrix(cMatrix3d matrix) {

        inertiaMatrix = inertiaMatrixInv = matrix;

        inertiaMatrixInv.invert();

}


