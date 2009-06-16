//===========================================================================
/*

    \author:    <http://www.chai3d.org>
    \author:    Remis Balaniuk
    \version    1.1
    \date       10/2005
*/
//===========================================================================
//---------------------------------------------------------------------------

#ifndef dynamicWorldH
#define dynamicWorldH

#include <cWorld.h>
#include <cMeta3dofPointer.h>
#include <cProxyPointForceAlgo.h>
#define BIGNUMBER (1e20)

class dynamicWorld;

#include "dynamicObject.h"
#include <cCollisionSpheres.h>

//---------------------------------------------------------------------------
class dynamicWorld : public cWorld {
public:
        dynamicWorld(bool grav);

        void integrate(double dt);

        bool getGravity();

        void setGravity(bool grav);

private:
        void checkForces();

        void checkHapticForces();

        void checkCollisions();

        void checkMeshMeshCollision(dynamicObject *mesh1, cMesh *mesh2);

        void CollisionResponse(double e,double ma,double mb,
                                     cMatrix3d Ia,cMatrix3d Ib,
                                     cVector3d ra,cVector3d rb,cVector3d n,
                                     cVector3d vai, cVector3d vbi,
                                     cVector3d wai, cVector3d wbi,
                                     cVector3d& vaf, cVector3d& vbf,
                                     cVector3d& waf, cVector3d& wbf,double pene);
        bool gravity;

        double lastDt;

};

#endif
 