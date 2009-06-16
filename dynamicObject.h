//===========================================================================
/*

    \author:    <http://www.chai3d.org>
    \author:    Remis Balaniuk
    \version    1.1
    \date       10/2005
*/
//===========================================================================
//---------------------------------------------------------------------------

#ifndef dynamicObjectH
#define dynamicObjectH
//---------------------------------------------------------------------------

class dynamicObject;

#include <cMesh.h>
#include "dynamicWorld.h"

#include <cCollisionSpheres.h>

#define DISSIPATION 0.0
#define TESTCOLLISIONSLICES 9 // indicates the granularity of the test points cloud

class dynamicObject : public cMesh {
public:
        dynamicObject(dynamicWorld *world, double imass, bool rigid);

        void init();

        void integrate(double dt);

        void rollBackIntegration(double dt);

        bool testCollision(cMesh *otherOne, cVector3d& contactNormal, cVector3d& contactPoint, double& penetration,cGenericObject **touched);

        int testCollision(cMesh *otherOne,
                     std::vector<cVector3d> *contactNormal,
                     std::vector<cVector3d> *contactPoint,
                     std::vector<double> *penetration,
                     std::vector<cGenericObject *>*touched);

        void applyForce(cVector3d force, cVector3d posiforce);

        void applyCenterOfMassForce(cVector3d force);

        void reposition(cVector3d newpos,cVector3d newvelo,cVector3d newangvelo);

        cVector3d getVelocity() { return velocity; }

        cVector3d getAngularVelocity() { return  angularVelocity; }

        cVector3d setVelocity(cVector3d ivelo) {  velocity = ivelo; }

        cVector3d setAngularVelocity(cVector3d ivelo) { angularVelocity = ivelo; }

        cVector3d getDynCenterOfMass() { return centerOfMass; }

        double getMass() { return mass; }

        double setMass(double m) { mass = m;}

        double getRestitution() { return restitution; }

        void setRestitution(double rest) { restitution = rest; }

        cMatrix3d getInertiaMatrix(bool transform2Global);

        void setInertiaMatrix(cMatrix3d matrix);

        cVector3d local2GlobalPosition(cVector3d localPosition);

        void preventGravity() { noGravityStep = true; }

        bool rigid,collisionDetectionUseVertices;

private:

        void integrateVelocity(double dt);

        void integratePosition(double dt);

        void calcAcceleration(double dt);

        void integrateOrientation(double dt);

        void integrateAngularVelocity(double dt);

        void calcAngularAcceleration(double dt);

        void computeTestPoints();

        void createTestPoint(cVector3d direction);

        cVector3d force,torque,acceleration,angAcceleration,centerOfMass,
                  angularVelocity,velocity;

        cVector3d centerOfMass0,angularVelocity0,velocity0,force0,torque0,
                lastGiro;
        cMatrix3d lastRotMatrix;

        double mass,lastDt;

        double restitution;
        
        cMatrix3d inertiaMatrix,inertiaMatrixInv;

        dynamicWorld *myworld;

        vector<cVector3d> testPoints;

        bool noGravityStep;


};


#endif
 