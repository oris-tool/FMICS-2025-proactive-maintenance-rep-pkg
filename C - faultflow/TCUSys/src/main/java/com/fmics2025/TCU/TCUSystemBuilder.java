/*
 * This program is part of the ORIS Tool.
 * Copyright (C) 2011-2023 The ORIS Authors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

package com.fmics2025.TCU;


import it.unifi.stlab.faultflow.launcher.builders.SystemBuilder;
import com.fmics2025.TCU.TCUSystemBuilder;
import it.unifi.stlab.faultflow.model.knowledge.composition.ComponentType;
import it.unifi.stlab.faultflow.model.knowledge.composition.CompositionPortType;
import it.unifi.stlab.faultflow.model.knowledge.composition.SystemType;
import it.unifi.stlab.faultflow.model.knowledge.propagation.*;

import java.util.ArrayList;
import java.util.HashMap;

public class TCUSystemBuilder extends SystemBuilder {

    private static TCUSystemBuilder single_instance;

    private TCUSystemBuilder() {
        faultModes = new HashMap<>();
        failureModes = new HashMap<>();
        errorModes = new HashMap<>();
        propagationPortTypes = new ArrayList<>();

        // Definizione composizione del sistema

        system = new SystemType("TCUSystem_SYS");
        ComponentType az = new ComponentType("AZ");
        ComponentType it = new ComponentType("IT");
        ComponentType ca = new ComponentType("CA");
        ComponentType cf = new ComponentType("CF");
        ComponentType gs = new ComponentType("GS");
        ComponentType tcu = new ComponentType("TCU");

        system.addComponent(az,
                it, ca, cf, tcu, gs);
        system.setTopLevelComponent(az);

        CompositionPortType cp1 = new CompositionPortType(az, it);
        CompositionPortType cp2 = new CompositionPortType(az, ca);
        CompositionPortType cp3 = new CompositionPortType(az, cf);
        CompositionPortType cp4 = new CompositionPortType(az, tcu);
        CompositionPortType cp5 = new CompositionPortType(az, gs);


        az.addCompositionPorts(cp1, cp2, cp3, cp4, cp5);

        // Definizione di Fault Mode Endogeni

        InternalFaultMode enFM_F1 = new InternalFaultMode("F1");
        enFM_F1.setTimeToFaultPDF("exp(0.000464826)"); //0.00150 0.0006 
        InternalFaultMode enFM_F2 = new InternalFaultMode("F2");
        enFM_F2.setTimeToFaultPDF("exp(0.001050015)");
        InternalFaultMode enFM_F3 = new InternalFaultMode("F3");
        enFM_F3.setTimeToFaultPDF("exp(0.000788023)"); //0.00142
        InternalFaultMode enFM_F4 = new InternalFaultMode("F4");
        enFM_F4.setTimeToFaultPDF("exp(0.00115)");
        InternalFaultMode enFM_F5 = new InternalFaultMode("F5");
        enFM_F5.setTimeToFaultPDF("exp(0.00015296)"); //0.0004
        InternalFaultMode enFM_F6 = new InternalFaultMode("F6");
        enFM_F6.setTimeToFaultPDF("exp(0.00029008)"); //0.00041
        InternalFaultMode enFM_F7 = new InternalFaultMode("F7");
        enFM_F7.setTimeToFaultPDF("exp(0.00027872)"); //0.00053
        InternalFaultMode enFM_F8 = new InternalFaultMode("F8");
        enFM_F8.setTimeToFaultPDF("exp(0.0000606299)"); //0.00037
        InternalFaultMode enFM_F9 = new InternalFaultMode("F9");
        enFM_F9.setTimeToFaultPDF("exp(0.00012828)"); //0.00088
        InternalFaultMode enFM_F10 = new InternalFaultMode("F10");
        enFM_F10.setTimeToFaultPDF("exp(0.00024449)"); //0.00037

        faultModes.put(enFM_F1.getName(), enFM_F1);
        faultModes.put(enFM_F2.getName(), enFM_F2);
        faultModes.put(enFM_F3.getName(), enFM_F3);
        faultModes.put(enFM_F4.getName(), enFM_F4);
        faultModes.put(enFM_F5.getName(), enFM_F5);
        faultModes.put(enFM_F6.getName(), enFM_F6);
        faultModes.put(enFM_F7.getName(), enFM_F7);
        faultModes.put(enFM_F8.getName(), enFM_F8);
        faultModes.put(enFM_F9.getName(), enFM_F9);
        faultModes.put(enFM_F10.getName(), enFM_F10);

        // Definizione di Fault Mode Esogeni

        ExternalFaultMode exFM_ITF1 = new ExternalFaultMode("ITF1");
        ExternalFaultMode exFM_CAF1 = new ExternalFaultMode("CAF1");
        ExternalFaultMode exFM_CFF1 = new ExternalFaultMode("CFF1");
        ExternalFaultMode exFM_TCUF1 = new ExternalFaultMode("TCUF1");
        ExternalFaultMode exFM_GSF1 = new ExternalFaultMode("GSF1");
        ExternalFaultMode exFM_AZOR2 = new ExternalFaultMode("AZOR2");
        ExternalFaultMode exFM_AZOR3 = new ExternalFaultMode("AZOR3");
        ExternalFaultMode exFM_AZOR4 = new ExternalFaultMode("AZOR4");

        
        faultModes.put(exFM_ITF1.getName(), exFM_ITF1);
        faultModes.put(exFM_CAF1.getName(), exFM_CAF1);
        faultModes.put(exFM_CFF1.getName(), exFM_CFF1);
        faultModes.put(exFM_TCUF1.getName(), exFM_TCUF1);
        faultModes.put(exFM_GSF1.getName(), exFM_GSF1);
        faultModes.put(exFM_AZOR2.getName(), exFM_AZOR2);
        faultModes.put(exFM_AZOR3.getName(), exFM_AZOR3);
        faultModes.put(exFM_AZOR4.getName(), exFM_AZOR4);



        //Definizione delle Failure Mode



        FailureMode fM_ITF = new FailureMode("ITFailure");
        ErrorMode eM_ITAND1 = new ErrorMode("IT_prop");
        eM_ITAND1.addInputFaultMode(enFM_F1, enFM_F2);
        eM_ITAND1.setOutgoingFailure(fM_ITF);
        eM_ITAND1.setEnablingCondition("F1 && F2", faultModes);
        eM_ITAND1.setPDF("dirac(0)");
        errorModes.put(eM_ITAND1.getName(), eM_ITAND1);
        failureModes.put(fM_ITF.getDescription(), fM_ITF);

        it.addErrorMode(eM_ITAND1);

        FailureMode fM_CAF = new FailureMode("CAFailure");
        ErrorMode eM_CAAND1 = new ErrorMode("CA_prop");
        eM_CAAND1.addInputFaultMode(enFM_F3, enFM_F4);
        eM_CAAND1.setOutgoingFailure(fM_CAF);
        eM_CAAND1.setEnablingCondition("F3 && F4", faultModes);
        eM_CAAND1.setPDF("dirac(0)");
        errorModes.put(eM_CAAND1.getName(), eM_CAAND1);
        failureModes.put(fM_CAF.getDescription(), fM_CAF);

        ca.addErrorMode(eM_CAAND1);

        FailureMode fM_CFF = new FailureMode("CFFailure");
        ErrorMode eM_CFAND1 = new ErrorMode("CF_prop");
        eM_CFAND1.addInputFaultMode(enFM_F5, enFM_F6);
        eM_CFAND1.setOutgoingFailure(fM_CFF);
        eM_CFAND1.setEnablingCondition("F5 && F6", faultModes);
        eM_CFAND1.setPDF("dirac(0)");
        errorModes.put(eM_CFAND1.getName(), eM_CFAND1);
        failureModes.put(fM_CFF.getDescription(), fM_CFF);

        cf.addErrorMode(eM_CFAND1);

        FailureMode fM_TCUF = new FailureMode("TCUFailure");
        ErrorMode eM_TCUAND1 = new ErrorMode("TCU_prop");
        eM_TCUAND1.addInputFaultMode(enFM_F7, enFM_F8);
        eM_TCUAND1.setOutgoingFailure(fM_TCUF);
        eM_TCUAND1.setEnablingCondition("F7 && F8", faultModes);
        eM_TCUAND1.setPDF("dirac(0)");
        errorModes.put(eM_TCUAND1.getName(), eM_TCUAND1);
        failureModes.put(fM_TCUF.getDescription(), fM_TCUF);

        tcu.addErrorMode(eM_TCUAND1);

        FailureMode fM_GSF = new FailureMode("GSFailure");
        ErrorMode eM_GSAND1 = new ErrorMode("GS_prop");
        eM_GSAND1.addInputFaultMode(enFM_F9, enFM_F10);
        eM_GSAND1.setOutgoingFailure(fM_GSF);
        eM_GSAND1.setEnablingCondition("F9 && F10", faultModes);
        eM_GSAND1.setPDF("dirac(0)");
        errorModes.put(eM_GSAND1.getName(), eM_GSAND1);
        failureModes.put(fM_GSF.getDescription(), fM_GSF);

        gs.addErrorMode(eM_GSAND1);

        FailureMode fM_AZF1 = new FailureMode("AZFailure");
        ErrorMode eM_AZOR1 = new ErrorMode("AZOR1");
        eM_AZOR1.addInputFaultMode(exFM_ITF1, exFM_AZOR2);
        eM_AZOR1.setOutgoingFailure(fM_AZF1);
        eM_AZOR1.setEnablingCondition("ITF1 || AZOR2", faultModes);
        eM_AZOR1.setPDF("dirac(0)");
        errorModes.put(eM_AZOR1.getName(), eM_AZOR1);
        failureModes.put(fM_AZF1.getDescription(), fM_AZF1);

        az.addErrorMode(eM_AZOR1);

        FailureMode fM_AZF2 = new FailureMode("AZFailure2");
        ErrorMode eM_AZOR2 = new ErrorMode("AZOR2");
        eM_AZOR2.addInputFaultMode(exFM_CAF1, exFM_AZOR3);
        eM_AZOR2.setOutgoingFailure(fM_AZF2);
        eM_AZOR2.setEnablingCondition("CAF1 || AZOR3", faultModes);
        eM_AZOR2.setPDF("dirac(0)");
        errorModes.put(eM_AZOR2.getName(), eM_AZOR2);
        failureModes.put(fM_AZF2.getDescription(), fM_AZF2);

        az.addErrorMode(eM_AZOR2);

        FailureMode fM_AZF3 = new FailureMode("AZFailure3");
        ErrorMode eM_AZOR3 = new ErrorMode("AZOR3");
        eM_AZOR3.addInputFaultMode(exFM_CFF1, exFM_AZOR4);
        eM_AZOR3.setOutgoingFailure(fM_AZF3);
        eM_AZOR3.setEnablingCondition("CFF1 || AZOR4", faultModes);
        eM_AZOR3.setPDF("dirac(0)");
        errorModes.put(eM_AZOR3.getName(), eM_AZOR3);
        failureModes.put(fM_AZF3.getDescription(), fM_AZF3);

        az.addErrorMode(eM_AZOR3);

        FailureMode fM_AZF4 = new FailureMode("AZFailure4");
        ErrorMode eM_AZOR4 = new ErrorMode("AZOR4");
        eM_AZOR4.addInputFaultMode(exFM_TCUF1, exFM_GSF1);
        eM_AZOR4.setOutgoingFailure(fM_AZF4);
        eM_AZOR4.setEnablingCondition("TCUF1 || GSF1", faultModes);
        eM_AZOR4.setPDF("dirac(0)");
        errorModes.put(eM_AZOR4.getName(), eM_AZOR4);
        failureModes.put(fM_AZF4.getDescription(), fM_AZF4);

        az.addErrorMode(eM_AZOR4);

        it.addPropagationPort(
                new PropagationPortType(fM_ITF, exFM_ITF1, az));
        propagationPortTypes.addAll(it.getPropagationPorts());
        ca.addPropagationPort(
                new PropagationPortType(fM_CAF, exFM_CAF1, az));
        propagationPortTypes.addAll(ca.getPropagationPorts());
        cf.addPropagationPort(
                new PropagationPortType(fM_CFF, exFM_CFF1, az));
        propagationPortTypes.addAll(cf.getPropagationPorts());
        tcu.addPropagationPort(
                new PropagationPortType(fM_TCUF, exFM_TCUF1, az));
        propagationPortTypes.addAll(tcu.getPropagationPorts());
        gs.addPropagationPort(
                new PropagationPortType(fM_GSF, exFM_GSF1, az));
        propagationPortTypes.addAll(gs.getPropagationPorts());
        
        az.addPropagationPort(                
                new PropagationPortType(fM_AZF4, exFM_AZOR4, az));
        propagationPortTypes.addAll(gs.getPropagationPorts());
        az.addPropagationPort(
                new PropagationPortType(fM_AZF3, exFM_AZOR3, az));
        propagationPortTypes.addAll(gs.getPropagationPorts());

        az.addPropagationPort(
                new PropagationPortType(fM_AZF2, exFM_AZOR2, az));
        propagationPortTypes.addAll(gs.getPropagationPorts());

    }



    public static TCUSystemBuilder getInstance() {
        if (single_instance == null)
            single_instance = new TCUSystemBuilder();
        return single_instance;
    }

    public SystemType getSystem() {
        return system;
    }
}
