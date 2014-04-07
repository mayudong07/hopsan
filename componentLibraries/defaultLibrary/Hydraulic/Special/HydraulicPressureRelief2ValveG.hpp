#ifndef HYDRAULICPRESSURERELIEF2VALVEG_HPP_INCLUDED
#define HYDRAULICPRESSURERELIEF2VALVEG_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file HydraulicPressureRelief2ValveG.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Mon 7 Apr 2014 14:17:04
//! @brief A hydraulic pressure relief valve based on geometry
//! @ingroup HydraulicComponents
//!
//==This code has been autogenerated using Compgen==
//from 
/*{, C:, HopsanTrunk, ComponentLibraries, defaultLibrary, Hydraulic, \
Special}/PressureRelief2ValveG.nb*/

using namespace hopsan;

class HydraulicPressureRelief2ValveG : public ComponentQ
{
private:
     double rho;
     double visc;
     double Dv;
     double Bv;
     double Mv;
     double Xvmax;
     double Cq;
     double phi;
     double ks;
     double p0;
     Port *mpP1;
     Port *mpP2;
     double delayParts1[9];
     double delayParts2[9];
     double delayParts3[9];
     double delayParts4[9];
     double delayParts5[9];
     Matrix jacobianMatrix;
     Vec systemEquations;
     Matrix delayedPart;
     int i;
     int iter;
     int mNoiter;
     double jsyseqnweight[4];
     int order[5];
     int mNstep;
     //Port P1 variable
     double p1;
     double q1;
     double T1;
     double dE1;
     double c1;
     double Zc1;
     //Port P2 variable
     double p2;
     double q2;
     double T2;
     double dE2;
     double c2;
     double Zc2;
//==This code has been autogenerated using Compgen==
     //inputVariables
     double pref;
     //outputVariables
     double xv;
     double dxv;
     //LocalExpressions variables
     double Av;
     double w;
     double kf;
     double ke;
     //Expressions variables
     //Port P1 pointer
     double *mpND_p1;
     double *mpND_q1;
     double *mpND_T1;
     double *mpND_dE1;
     double *mpND_c1;
     double *mpND_Zc1;
     //Port P2 pointer
     double *mpND_p2;
     double *mpND_q2;
     double *mpND_T2;
     double *mpND_dE2;
     double *mpND_c2;
     double *mpND_Zc2;
     //Delay declarations
//==This code has been autogenerated using Compgen==
     //inputVariables pointers
     double *mppref;
     //inputParameters pointers
     double *mprho;
     double *mpvisc;
     double *mpDv;
     double *mpBv;
     double *mpMv;
     double *mpXvmax;
     double *mpCq;
     double *mpphi;
     double *mpks;
     double *mpp0;
     //outputVariables pointers
     double *mpxv;
     double *mpdxv;
     Delay mDelayedPart10;
     Delay mDelayedPart11;
     Delay mDelayedPart12;
     Delay mDelayedPart20;
     Delay mDelayedPart21;
     Delay mDelayedPart30;
     EquationSystemSolver *mpSolver;

public:
     static Component *Creator()
     {
        return new HydraulicPressureRelief2ValveG();
     }

     void configure()
     {
//==This code has been autogenerated using Compgen==

        mNstep=9;
        jacobianMatrix.create(5,5);
        systemEquations.create(5);
        delayedPart.create(6,6);
        mNoiter=2;
        jsyseqnweight[0]=1;
        jsyseqnweight[1]=0.67;
        jsyseqnweight[2]=0.5;
        jsyseqnweight[3]=0.5;


        //Add ports to the component
        mpP1=addPowerPort("P1","NodeHydraulic");
        mpP2=addPowerPort("P2","NodeHydraulic");
        //Add inputVariables to the component
            addInputVariable("pref","Reference pressure","Pa",1.e7,&mppref);

        //Add inputParammeters to the component
            addInputVariable("rho", "oil density", "kg/m3", 860.,&mprho);
            addInputVariable("visc", "viscosity ", "Ns/m2", 0.03,&mpvisc);
            addInputVariable("Dv", "Spool diameter", "m", 0.03,&mpDv);
            addInputVariable("Bv", "Damping", "N/(m s)", 0.03,&mpBv);
            addInputVariable("Mv", "Spool mass", "kg", 0.03,&mpMv);
            addInputVariable("Xvmax", "Max spool displacement", "m", \
0.03,&mpXvmax);
            addInputVariable("Cq", "Flow coefficient", " ", 0.67,&mpCq);
            addInputVariable("phi", "Stream angle", "rad", 0.03,&mpphi);
            addInputVariable("ks", "Spring constant", "N/m", 100.,&mpks);
            addInputVariable("p0", "Turbulent pressure trans.", "Pa", \
100000.,&mpp0);
        //Add outputVariables to the component
            addOutputVariable("xv","Spool position","m",0.,&mpxv);
            addOutputVariable("dxv","Spool velocity","m/s",0.,&mpdxv);

//==This code has been autogenerated using Compgen==
        //Add constantParameters
        mpSolver = new EquationSystemSolver(this,5);
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Port P1
        mpND_p1=getSafeNodeDataPtr(mpP1, NodeHydraulic::Pressure);
        mpND_q1=getSafeNodeDataPtr(mpP1, NodeHydraulic::Flow);
        mpND_T1=getSafeNodeDataPtr(mpP1, NodeHydraulic::Temperature);
        mpND_dE1=getSafeNodeDataPtr(mpP1, NodeHydraulic::HeatFlow);
        mpND_c1=getSafeNodeDataPtr(mpP1, NodeHydraulic::WaveVariable);
        mpND_Zc1=getSafeNodeDataPtr(mpP1, NodeHydraulic::CharImpedance);
        //Port P2
        mpND_p2=getSafeNodeDataPtr(mpP2, NodeHydraulic::Pressure);
        mpND_q2=getSafeNodeDataPtr(mpP2, NodeHydraulic::Flow);
        mpND_T2=getSafeNodeDataPtr(mpP2, NodeHydraulic::Temperature);
        mpND_dE2=getSafeNodeDataPtr(mpP2, NodeHydraulic::HeatFlow);
        mpND_c2=getSafeNodeDataPtr(mpP2, NodeHydraulic::WaveVariable);
        mpND_Zc2=getSafeNodeDataPtr(mpP2, NodeHydraulic::CharImpedance);

        //Read variables from nodes
        //Port P1
        p1 = (*mpND_p1);
        q1 = (*mpND_q1);
        T1 = (*mpND_T1);
        dE1 = (*mpND_dE1);
        c1 = (*mpND_c1);
        Zc1 = (*mpND_Zc1);
        //Port P2
        p2 = (*mpND_p2);
        q2 = (*mpND_q2);
        T2 = (*mpND_T2);
        dE2 = (*mpND_dE2);
        c2 = (*mpND_c2);
        Zc2 = (*mpND_Zc2);

        //Read inputVariables from nodes
        pref = (*mppref);

        //Read inputParameters from nodes
        rho = (*mprho);
        visc = (*mpvisc);
        Dv = (*mpDv);
        Bv = (*mpBv);
        Mv = (*mpMv);
        Xvmax = (*mpXvmax);
        Cq = (*mpCq);
        phi = (*mpphi);
        ks = (*mpks);
        p0 = (*mpp0);

        //Read outputVariables from nodes
        xv = (*mpxv);
        dxv = (*mpdxv);

//==This code has been autogenerated using Compgen==

        //LocalExpressions
        Av = 0.785398*Power(Dv,2);
        w = 3.14159*Dv*Sin(phi);
        kf = 2*Cq*(p1 - p2)*w*Cos(phi);
        ke = kf + ks;

        //Initialize delays
        delayParts1[1] = (-2*Av*Power(mTimestep,2)*p1 + \
2*Av*Power(mTimestep,2)*p2 + 2*Av*Power(mTimestep,2)*pref + \
2*ke*Power(mTimestep,2)*xv - 8*Mv*xv)/(2*Bv*mTimestep + ke*Power(mTimestep,2) \
+ 4*Mv);
        mDelayedPart11.initialize(mNstep,delayParts1[1]);
        delayParts1[2] = (-(Av*Power(mTimestep,2)*p1) + \
Av*Power(mTimestep,2)*p2 + Av*Power(mTimestep,2)*pref - 2*Bv*mTimestep*xv + \
ke*Power(mTimestep,2)*xv + 4*Mv*xv)/(2*Bv*mTimestep + ke*Power(mTimestep,2) + \
4*Mv);
        mDelayedPart12.initialize(mNstep,delayParts1[2]);
        delayParts2[1] = (Bv*dxv*mTimestep - 2*dxv*Mv - Av*mTimestep*p1 + \
Av*mTimestep*p2 + Av*mTimestep*pref + ke*mTimestep*xv)/(Bv*mTimestep + 2*Mv);
        mDelayedPart21.initialize(mNstep,delayParts2[1]);

        delayedPart[1][1] = delayParts1[1];
        delayedPart[1][2] = mDelayedPart12.getIdx(1);
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
        delayedPart[4][1] = delayParts4[1];
        delayedPart[5][1] = delayParts5[1];
     }
    void simulateOneTimestep()
     {
        Vec stateVar(5);
        Vec stateVark(5);
        Vec deltaStateVar(5);

        //Read variables from nodes
        //Port P1
        T1 = (*mpND_T1);
        c1 = (*mpND_c1);
        Zc1 = (*mpND_Zc1);
        //Port P2
        T2 = (*mpND_T2);
        c2 = (*mpND_c2);
        Zc2 = (*mpND_Zc2);

        //Read inputVariables from nodes
        pref = (*mppref);

        //LocalExpressions
        Av = 0.785398*Power(Dv,2);
        w = 3.14159*Dv*Sin(phi);
        kf = 2*Cq*(p1 - p2)*w*Cos(phi);
        ke = kf + ks;

        //Initializing variable vector for Newton-Raphson
        stateVark[0] = xv;
        stateVark[1] = dxv;
        stateVark[2] = q2;
        stateVark[3] = p1;
        stateVark[4] = p2;

        //Iterative solution using Newton-Rapshson
        for(iter=1;iter<=mNoiter;iter++)
        {
         //PressureRelief2ValveG
         //Differential-algebraic system of equation parts

          //Assemble differential-algebraic equations
          systemEquations[0] =xv - limit(-((Av*Power(mTimestep,2)*(-p1 + p2 + \
pref))/(2*Bv*mTimestep + ke*Power(mTimestep,2) + 4*Mv)) - delayedPart[1][1] - \
delayedPart[1][2],0.,Xvmax);
          systemEquations[1] =dxv - \
dxLimit(limit(-((Av*Power(mTimestep,2)*(-p1 + p2 + pref))/(2*Bv*mTimestep + \
ke*Power(mTimestep,2) + 4*Mv)) - delayedPart[1][1] - \
delayedPart[1][2],0.,Xvmax),0.,Xvmax)*(-((mTimestep*(Av*(-p1 + p2 + pref) + \
ke*xv))/(Bv*mTimestep + 2*Mv)) - delayedPart[2][1]);
          systemEquations[2] =q2 - \
1.4142135623730951*Cq*Sqrt(1/rho)*w*xv*signedSquareL(p1 - p2,p0);
          systemEquations[3] =p1 - lowLimit(c1 - q2*Zc1,0);
          systemEquations[4] =p2 - lowLimit(c2 + q2*Zc2,0);

          //Jacobian matrix
          jacobianMatrix[0][0] = 1;
          jacobianMatrix[0][1] = 0;
          jacobianMatrix[0][2] = 0;
          jacobianMatrix[0][3] = \
-((Av*Power(mTimestep,2)*dxLimit(-((Av*Power(mTimestep,2)*(-p1 + p2 + \
pref))/(2*Bv*mTimestep + ke*Power(mTimestep,2) + 4*Mv)) - delayedPart[1][1] - \
delayedPart[1][2],0.,Xvmax))/(2*Bv*mTimestep + ke*Power(mTimestep,2) + \
4*Mv));
          jacobianMatrix[0][4] = \
(Av*Power(mTimestep,2)*dxLimit(-((Av*Power(mTimestep,2)*(-p1 + p2 + \
pref))/(2*Bv*mTimestep + ke*Power(mTimestep,2) + 4*Mv)) - delayedPart[1][1] - \
delayedPart[1][2],0.,Xvmax))/(2*Bv*mTimestep + ke*Power(mTimestep,2) + 4*Mv);
          jacobianMatrix[1][0] = \
(ke*mTimestep*dxLimit(limit(-((Av*Power(mTimestep,2)*(-p1 + p2 + \
pref))/(2*Bv*mTimestep + ke*Power(mTimestep,2) + 4*Mv)) - delayedPart[1][1] - \
delayedPart[1][2],0.,Xvmax),0.,Xvmax))/(Bv*mTimestep + 2*Mv);
          jacobianMatrix[1][1] = 1;
          jacobianMatrix[1][2] = 0;
          jacobianMatrix[1][3] = \
-((Av*mTimestep*dxLimit(limit(-((Av*Power(mTimestep,2)*(-p1 + p2 + \
pref))/(2*Bv*mTimestep + ke*Power(mTimestep,2) + 4*Mv)) - delayedPart[1][1] - \
delayedPart[1][2],0.,Xvmax),0.,Xvmax))/(Bv*mTimestep + 2*Mv));
          jacobianMatrix[1][4] = \
(Av*mTimestep*dxLimit(limit(-((Av*Power(mTimestep,2)*(-p1 + p2 + \
pref))/(2*Bv*mTimestep + ke*Power(mTimestep,2) + 4*Mv)) - delayedPart[1][1] - \
delayedPart[1][2],0.,Xvmax),0.,Xvmax))/(Bv*mTimestep + 2*Mv);
          jacobianMatrix[2][0] = \
-1.4142135623730951*Cq*Sqrt(1/rho)*w*signedSquareL(p1 - p2,p0);
          jacobianMatrix[2][1] = 0;
          jacobianMatrix[2][2] = 1;
          jacobianMatrix[2][3] = \
-1.4142135623730951*Cq*Sqrt(1/rho)*w*xv*dxSignedSquareL(p1 - p2,p0);
          jacobianMatrix[2][4] = \
1.4142135623730951*Cq*Sqrt(1/rho)*w*xv*dxSignedSquareL(p1 - p2,p0);
          jacobianMatrix[3][0] = 0;
          jacobianMatrix[3][1] = 0;
          jacobianMatrix[3][2] = Zc1*dxLowLimit(c1 - q2*Zc1,0);
          jacobianMatrix[3][3] = 1;
          jacobianMatrix[3][4] = 0;
          jacobianMatrix[4][0] = 0;
          jacobianMatrix[4][1] = 0;
          jacobianMatrix[4][2] = -(Zc2*dxLowLimit(c2 + q2*Zc2,0));
          jacobianMatrix[4][3] = 0;
          jacobianMatrix[4][4] = 1;
//==This code has been autogenerated using Compgen==

          //Solving equation using LU-faktorisation
          mpSolver->solve(jacobianMatrix, systemEquations, stateVark, iter);
          xv=stateVark[0];
          dxv=stateVark[1];
          q2=stateVark[2];
          p1=stateVark[3];
          p2=stateVark[4];
          //Expressions
          q1 = -q2;
        }

        //Calculate the delayed parts
        delayParts1[1] = (-2*Av*Power(mTimestep,2)*p1 + \
2*Av*Power(mTimestep,2)*p2 + 2*Av*Power(mTimestep,2)*pref + \
2*ke*Power(mTimestep,2)*xv - 8*Mv*xv)/(2*Bv*mTimestep + ke*Power(mTimestep,2) \
+ 4*Mv);
        delayParts1[2] = (-(Av*Power(mTimestep,2)*p1) + \
Av*Power(mTimestep,2)*p2 + Av*Power(mTimestep,2)*pref - 2*Bv*mTimestep*xv + \
ke*Power(mTimestep,2)*xv + 4*Mv*xv)/(2*Bv*mTimestep + ke*Power(mTimestep,2) + \
4*Mv);
        delayParts2[1] = (Bv*dxv*mTimestep - 2*dxv*Mv - Av*mTimestep*p1 + \
Av*mTimestep*p2 + Av*mTimestep*pref + ke*mTimestep*xv)/(Bv*mTimestep + 2*Mv);

        delayedPart[1][1] = delayParts1[1];
        delayedPart[1][2] = mDelayedPart12.getIdx(0);
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
        delayedPart[4][1] = delayParts4[1];
        delayedPart[5][1] = delayParts5[1];

        //Write new values to nodes
        //Port P1
        (*mpND_p1)=p1;
        (*mpND_q1)=q1;
        (*mpND_dE1)=dE1;
        //Port P2
        (*mpND_p2)=p2;
        (*mpND_q2)=q2;
        (*mpND_dE2)=dE2;
        //outputVariables
        (*mpxv)=xv;
        (*mpdxv)=dxv;

        //Update the delayed variabels
        mDelayedPart11.update(delayParts1[1]);
        mDelayedPart12.update(delayParts1[2]);
        mDelayedPart21.update(delayParts2[1]);

     }
    void deconfigure()
    {
        delete mpSolver;
    }
};
#endif // HYDRAULICPRESSURERELIEF2VALVEG_HPP_INCLUDED
