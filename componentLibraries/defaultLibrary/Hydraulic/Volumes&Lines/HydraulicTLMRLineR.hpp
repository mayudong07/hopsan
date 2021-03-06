/*-----------------------------------------------------------------------------

 Copyright 2017 Hopsan Group

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.


 The full license is available in the file LICENSE.
 For details about the 'Hopsan Group' or information about Authors and
 Contributors see the HOPSANGROUP and AUTHORS files that are located in
 the Hopsan source code root directory.

-----------------------------------------------------------------------------*/

//!
//! @file   HydraulicTLMRLineR.hpp
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2009-12-21
//!
//! @brief Contains a Hydraulic Transmission Line Component with Resistors in the ends
//!
//$Id$

#ifndef HYDRAULICTLMRLINER_HPP_INCLUDED
#define HYDRAULICTLMRLINER_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup HydraulicComponents
    //!
    class HydraulicTLMRlineR : public ComponentC
    {

    private:
        double *mpAlpha, *mpZc, *mpR1, *mpR2;
        double mTimeDelay;

        double *mpND_p1, *mpND_q1, *mpND_c1, *mpND_Zc1, *mpND_p2, *mpND_q2, *mpND_c2, *mpND_Zc2;

        Delay mDelayedC1;
        Delay mDelayedC2;
        Port *mpP1, *mpP2;

    public:
        static Component *Creator()
        {
            return new HydraulicTLMRlineR();
        }

        void configure()
        {
            mpP1 = addPowerPort("P1", "NodeHydraulic");
            mpP2 = addPowerPort("P2", "NodeHydraulic");

            addInputVariable("alpha", "Low pass coefficient", "-", 0.0, &mpAlpha);
            addInputVariable("Z_c", "Characteristic Impedance", "Ns/m^5",  1.0e9, &mpZc);
            addInputVariable("R_1", "Resistance 1", "Ns/m^5",  0.5, &mpR1);
            addInputVariable("R_2", "Resistance 2", "Ns/m^5",  0.5, &mpR2);

            addConstant("deltat", "Time delay", "s",   0.1, mTimeDelay);

            setDefaultStartValue(mpP1, NodeHydraulic::Flow, 0.0);
            setDefaultStartValue(mpP1, NodeHydraulic::Pressure, 1.0e5);
            setDefaultStartValue(mpP2, NodeHydraulic::Flow, 0.0);
            setDefaultStartValue(mpP2, NodeHydraulic::Pressure, 1.0e5);
        }


        void initialize()
        {
            mpND_p1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::Pressure);
            mpND_q1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::Flow);
            mpND_c1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::WaveVariable);
            mpND_Zc1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::CharImpedance);

            mpND_p2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::Pressure);
            mpND_q2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::Flow);
            mpND_c2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::WaveVariable);
            mpND_Zc2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::CharImpedance);

            double Zc, r1, r2;
            Zc = (*mpZc);
            r1 = (*mpR1);
            r2 = (*mpR2);

            //Write to nodes
            (*mpND_c1) = getDefaultStartValue(mpP1,NodeHydraulic::Pressure)+(Zc+r1)*getDefaultStartValue(mpP1,NodeHydraulic::Flow);
            (*mpND_Zc1) = Zc+r1;
            (*mpND_c2) = getDefaultStartValue(mpP2,NodeHydraulic::Pressure)+(Zc+r2)*getDefaultStartValue(mpP2,NodeHydraulic::Flow);
            (*mpND_Zc2) = Zc+r2;

            if (mTimeDelay-mTimestep < 0)
            {
                addWarningMessage("TimeDelay must be >= Ts");
                //stopSimulation();
            }

            //Init delay
            // We use -Ts to make the delay one step shorter as the TLM already have one built in timstep delay
            //! @todo for Td=Ts the delay will actually be 2Ts (the delay and inherited) need if check to avoid using delays if Td=Ts
            mDelayedC1.initialize(mTimeDelay-mTimestep, mTimestep, (*mpND_c1));
            mDelayedC2.initialize(mTimeDelay-mTimestep, mTimestep, (*mpND_c2));
        }


        void simulateOneTimestep()
        {
            //Declare local variables
            double p1, q1, c1, p2, q2, c2, c10, c20, alpha, Zc, r1, r2;

            //Read variables from nodes
            p1 = (*mpND_p1);
            q1 = (*mpND_q1);
            p2 = (*mpND_p2);
            q2 = (*mpND_q2);
            c1 = (*mpND_c1);
            c2 = (*mpND_c2);
            alpha = (*mpAlpha);
            Zc = (*mpZc);
            r1 = (*mpR1);
            r2 = (*mpR2);

            //Delay Line equations
            c10 = p2 + (Zc+r2) * q2;
            c20 = p1 + (Zc+r1) * q1;
            c1  = alpha*c1 + (1.0-alpha)*c10;
            c2  = alpha*c2 + (1.0-alpha)*c20;

            //Write new values to nodes
            (*mpND_c1) = mDelayedC1.update(c1);
            (*mpND_Zc1) = Zc + r1;
            (*mpND_c2) = mDelayedC2.update(c2);
            (*mpND_Zc2) = Zc + r2;
        }
    };
}

#endif // HYDRAULICTLMRLINER_HPP_INCLUDED
