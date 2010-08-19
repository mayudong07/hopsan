/*
 * This file is part of OpenModelica.
 *
 * Copyright (c) 1998-CurrentYear, Linköping University,
 * Department of Computer and Information Science,
 * SE-58183 Linköping, Sweden.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 
 * AND THIS OSMC PUBLIC LICENSE (OSMC-PL). 
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES RECIPIENT'S  
 * ACCEPTANCE OF THE OSMC PUBLIC LICENSE.
 *
 * The OpenModelica software and the Open Source Modelica
 * Consortium (OSMC) Public License (OSMC-PL) are obtained
 * from Linköping University, either from the above address,
 * from the URLs: http://www.ida.liu.se/projects/OpenModelica or  
 * http://www.openmodelica.org, and in the OpenModelica distribution. 
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH
 * IN THE BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS
 * OF OSMC-PL.
 *
 * See the full OSMC Public License conditions for more details.
 *
 */

/*
 * HopsanGUI
 * Fluid and Mechatronic Systems, Department of Management and Engineering, Linköping University
 * Main Authors 2009-2010:  Robert Braun, Björn Eriksson, Peter Nordin
 * Contributors 2009-2010:  Mikael Axin, Alessandro Dell'Amico, Karl Pettersson, Ingo Staack
 */

#include "CoreSystemAccess.h"
#include <QString>
#include <QVector>

using namespace std;
using namespace hopsan;

CoreSystemAccess::CoreSystemAccess()
{
    //Create new Core system component
    mpCoreComponentSystem = HopsanEssentials::getInstance()->CreateComponentSystem();
}

bool CoreSystemAccess::connect(QString compname1, QString portname1, QString compname2, QString portname2)
{
    //*****Core Interaction*****
    return mpCoreComponentSystem->connect(compname1.toStdString(), portname1.toStdString(), compname2.toStdString(), portname2.toStdString());
    //**************************
}

bool CoreSystemAccess::disconnect(QString compname1, QString portname1, QString compname2, QString portname2)
{
    //*****Core Interaction*****
    return mpCoreComponentSystem->disconnect(compname1.toStdString(), portname1.toStdString(), compname2.toStdString(), portname2.toStdString());
    //**************************
}

void CoreSystemAccess::setDesiredTimeStep(double timestep)
{
    mpCoreComponentSystem->setDesiredTimestep(timestep);
}

double CoreSystemAccess::getDesiredTimeStep()
{
    return mpCoreComponentSystem->getDesiredTimeStep();
}

void CoreSystemAccess::setRootTypeCQS(const QString cqs_type, bool doOnlyLocalSet)
{
    mpCoreComponentSystem->setTypeCQS(cqs_type.toStdString());
}

void CoreSystemAccess::setSubSystemTypeCQS(QString systemName, const string cqs_type, bool doOnlyLocalSet)
{
    mpCoreComponentSystem->getSubComponentSystem(systemName.toStdString())->setTypeCQS(cqs_type, doOnlyLocalSet);
}

//QString GUIRootSystem::getSystemTypeCQS(QString systemName)
//{
//    return QString::fromStdString(mpCoreComponentSystem->getSubComponentSystem(systemName.toStdString())->getTypeCQSString());
//}

QString CoreSystemAccess::getTypeCQS(QString componentName)
{
    return QString::fromStdString(mpCoreComponentSystem->getComponent(componentName.toStdString())->getTypeCQSString());
}

void CoreSystemAccess::setRootSystemName(QString name)
{
    mpCoreComponentSystem->setName(name.toStdString());
}

//QString GUIRootSystem::setSystemName(QString systemname, QString name, bool doOnlyLocalRename)
//{
//    ComponentSystem *pTempComponentSystem = mpCoreComponentSystem->getSubComponentSystem(systemname.toStdString());
//    pTempComponentSystem->setName(name.toStdString(), doOnlyLocalRename);
//    return QString::fromStdString(pTempComponentSystem->getName());
//}

QString CoreSystemAccess::renameSubComponent(QString componentName, QString name)
{
    Component *pTempComponent = mpCoreComponentSystem->getComponent(componentName.toStdString());
    pTempComponent->setName(name.toStdString());
    return QString::fromStdString(pTempComponent->getName());
}

QString CoreSystemAccess::getRootSystemName()
{
   // qDebug() << "getNAme from core root: " << QString::fromStdString(mpCoreComponentSystem->getName());
    return QString::fromStdString(mpCoreComponentSystem->getName());
}

double CoreSystemAccess::getCurrentTime()
{
    return *(mpCoreComponentSystem->getTimePtr());
}

void CoreSystemAccess::stop()
{
    mpCoreComponentSystem->stop();
}

QString CoreSystemAccess::getPortType(QString componentName, QString portName)
{
    //qDebug() << "name for port fetch " << componentName << " " << portName;
    Component *pComponent = mpCoreComponentSystem->getComponent(componentName.toStdString());
    if(pComponent)
    {
        Port *pPort = pComponent->getPort(portName.toStdString());
        if(pPort)
            return QString(pPort->getPortTypeString().c_str());
    }
    return QString();
//    return QString(mpCoreComponentSystem->getComponent(componentName.toStdString())->getPort(portName.toStdString())->getPortTypeString().c_str());
}

QString CoreSystemAccess::getNodeType(QString componentName, QString portName)
{
    return QString(mpCoreComponentSystem->getComponent(componentName.toStdString())->getPort(portName.toStdString())->getNodeType().c_str());
}

void CoreSystemAccess::setParameter(QString componentName, QString parameterName, double value)
{
    mpCoreComponentSystem->getComponent(componentName.toStdString())->setParameterValue(parameterName.toStdString(), value);
}

void CoreSystemAccess::removeSubComponent(QString componentName, bool doDelete)
{
    mpCoreComponentSystem->removeSubComponent(componentName.toStdString(), doDelete);
}

//void GUIRootSystem::removeSystem(QString name)
//{
//    mpCoreComponentSystem->removeSubComponent(mpCoreComponentSystem->getSubComponentSystem(name.toStdString()), true);
//}

//! @brief This function loads a system from a file without adding any graphics
//! It calls the core load function and adds all contents into the specified subsystem
//! @param [in] sysname The name of the subsystem in which to add loaded contents
//! @param [in] filepath The path to the file to load from
void CoreSystemAccess::loadSystemFromFileCoreOnly(QString sysname, QString filepath)
{
    mpCoreComponentSystem->getSubComponentSystem(sysname.toStdString())->loadSystemFromFile(filepath.toStdString());
}


vector<double> CoreSystemAccess::getTimeVector(QString componentName, QString portName)
{
    vector<double>* ptr = (mpCoreComponentSystem->getComponent(componentName.toStdString())->getPort(portName.toStdString())->getTimeVectorPtr());
    if (ptr != 0)
    {
        return *ptr;
    }
    else
    {
        //Return empty dummy
        vector<double> dummy;
        return dummy;
    }
}


bool CoreSystemAccess::isSimulationOk()
{
    return mpCoreComponentSystem->isSimulationOk();
}


void CoreSystemAccess::initialize(double mStartTime, double mFinishTime)
{
    mpCoreComponentSystem->initialize(mStartTime, mFinishTime);
}


void CoreSystemAccess::simulate(double mStartTime, double mFinishTime, simulationMethod type)
{
    if(type == MULTICORE)
    {
        qDebug() << "Staring multicore simulation";
        mpCoreComponentSystem->simulateMultiThreaded(mStartTime, mFinishTime);
    }
    else
    {
        qDebug() << "Staring singlecore simulation";
        mpCoreComponentSystem->simulateSingleThreaded(mStartTime, mFinishTime);
    }
}


void CoreSystemAccess::finalize(double mStartTime, double mFinishTime)
{
    mpCoreComponentSystem->finalize(mStartTime, mFinishTime);
}

QString CoreSystemAccess::createComponent(QString type, QString name)
{
    Component *pCoreComponent = HopsanEssentials::getInstance()->CreateComponent(type.toStdString());
    mpCoreComponentSystem->addComponent(pCoreComponent);
    if (!name.isEmpty())
    {
        pCoreComponent->setName(name.toStdString());
    }
    return QString::fromStdString(pCoreComponent->getName());
}

QString CoreSystemAccess::createSubSystem(QString name)
{
    ComponentSystem *pTempComponentSystem = HopsanEssentials::getInstance()->CreateComponentSystem();
    mpCoreComponentSystem->addComponent(pTempComponentSystem);
    if (!name.isEmpty())
    {
        pTempComponentSystem->setName(name.toStdString());
    }
    return QString::fromStdString(pTempComponentSystem->getName());
}

QVector<QString> CoreSystemAccess::getParameterNames(QString componentName)
{
    QVector<QString> names;
    //*****Core Interaction*****
    vector<string> core_names = mpCoreComponentSystem->getComponent(componentName.toStdString())->getParameterNames();
    vector<string>::iterator nit;
    //Copy and cast to qt datatypes
    for ( nit=core_names.begin(); nit!=core_names.end(); ++nit)
    {
        names.push_back(QString::fromStdString(*nit));
    }
    //**************************

    return names;
}

QString CoreSystemAccess::getParameterUnit(QString componentName, QString parameterName)
{
    return QString::fromStdString(mpCoreComponentSystem->getComponent(componentName.toStdString())->getParameterUnit(parameterName.toStdString()));
}

QString CoreSystemAccess::getParameterDescription(QString componentName, QString parameterName)
{
    return QString::fromStdString(mpCoreComponentSystem->getComponent(componentName.toStdString())->getParameterDescription(parameterName.toStdString()));
}

double CoreSystemAccess::getParameterValue(QString componentName, QString parameterName)
{
    return mpCoreComponentSystem->getComponent(componentName.toStdString())->getParameterValue(parameterName.toStdString());
}

void CoreSystemAccess::deleteSystemPort(QString portname)
{
    mpCoreComponentSystem->deleteSystemPort(portname.toStdString());
}

QString CoreSystemAccess::addSystemPort(QString portname)
{
    //qDebug() << "add system port: " << portname;
    return QString::fromStdString(mpCoreComponentSystem->addSystemPort(portname.toStdString())->getPortName());
}

QString CoreSystemAccess::renameSystemPort(QString oldname, QString newname)
{
    return QString::fromStdString(mpCoreComponentSystem->renameSystemPort(oldname.toStdString(), newname.toStdString()));
}

//! @todo how to handle fetching from systemports, component names will not be found
void CoreSystemAccess::getPlotDataNamesAndUnits(const QString compname, const QString portname, QVector<QString> &rNames, QVector<QString> &rUnits)
{
    vector<string> corenames, coreunits;
    mpCoreComponentSystem->getComponent(compname.toStdString())->getPort(portname.toStdString())->getNodeDataNamesAndUnits(corenames, coreunits);

    rNames.clear();
    rUnits.clear();

    //Copy into QT datatype vector (assumes bothe received vectors same length (should always be same)
    for (size_t i=0; i<corenames.size(); ++i)
    {
        rNames.push_back(QString::fromStdString(corenames[i]));
        rUnits.push_back(QString::fromStdString(coreunits[i]));
    }
}

void CoreSystemAccess::getPlotData(const QString compname, const QString portname, const QString dataname, QVector<double> &rData)
{
    //*****Core Interaction*****
    int dataId = mpCoreComponentSystem->getComponent(compname.toStdString())->getPort(portname.toStdString())->getNodeDataIdFromName(dataname.toStdString());
    if (dataId >= 0)
    {
        vector< vector<double> > *pData = mpCoreComponentSystem->getComponent(compname.toStdString())->getPort(portname.toStdString())->getDataVectorPtr();

        //Ok lets copy all of the data to a QT vector
        rData.clear();
        rData.resize(pData->size()); //Allocaate memory
        for (size_t i=0; i<pData->size(); ++i) //Denna loop ar inte klok
        {
            rData[i] = pData->at(i).at(dataId);
        }
    }
    //**************************
}


bool CoreSystemAccess::isPortConnected(QString componentName, QString portName)
{
    return mpCoreComponentSystem->getComponent(componentName.toStdString())->getPort(portName.toStdString())->isConnected();
}
