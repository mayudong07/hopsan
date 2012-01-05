/*-----------------------------------------------------------------------------
 This source file is part of Hopsan NG

 Copyright (c) 2011 
    Mikael Axin, Robert Braun, Alessandro Dell'Amico, Björn Eriksson,
    Peter Nordin, Karl Pettersson, Petter Krus, Ingo Staack

 This file is provided "as is", with no guarantee or warranty for the
 functionality or reliability of the contents. All contents in this file is
 the original work of the copyright holders at the Division of Fluid and
 Mechatronic Systems (Flumes) at Linköping University. Modifying, using or
 redistributing any part of this file is prohibited without explicit
 permission from the copyright holders.
-----------------------------------------------------------------------------*/

//!
//! @file   GUIContainerPort.cpp
//! @author Flumes <flumes@lists.iei.liu.se>
//! @date   2010-01-01
//!
//! @brief Contains the ContainerPort class
//!
//$Id$

#include "GUIContainerPort.h"
#include "GUIContainerObject.h"
#include "GUIPort.h"
#include "Dialogs/ContainerPortPropertiesDialog.h"
#include "MainWindow.h"

ContainerPort::ContainerPort(QPointF position, qreal rotation, ModelObjectAppearance* pAppearanceData, ContainerObject *pParentContainer, selectionStatus startSelected, graphicsType gfxType)
        : ModelObject(position, rotation, pAppearanceData, startSelected, gfxType, pParentContainer, pParentContainer)
{
    mIsSystemPort = (pParentContainer->type() == SYSTEMCONTAINER); //determine if I am a system port
    this->mHmfTagName = HMF_SYSTEMPORTTAG;
    //Sets the ports
    createPorts();
    refreshDisplayName();
}

ContainerPort::~ContainerPort()
{
    qDebug() << "GuiContainerPort destructor: " << this->getName();
    if (mIsSystemPort)
    {
        mpParentContainerObject->getCoreSystemAccessPtr()->deleteSystemPort(this->getName());
    }
    else
    {
        mpParentContainerObject->getCoreSystemAccessPtr()->unReserveUniqueName(this->getName());
    }
}

//! @brief Help function to create ports in the SystemPort Object when it is created
void ContainerPort::createPorts()
{
    //A system port only contains one port, which should be first in the map, ignore any others (should not be any more)
    PortAppearanceMapT::iterator i = mModelObjectAppearance.getPortAppearanceMap().begin();

    //Check if a systemport name is given in appearance data (for example if the systemport is loaded from file)
    //In that case override the default port name with this name
    QString desiredportname;
    if (mModelObjectAppearance.getName().isEmpty())
    {
        desiredportname = i.key();
    }
    else
    {
        desiredportname = mModelObjectAppearance.getName();
    }

    qreal x = i.value().x;
    qreal y = i.value().y;

    //! @todo should make this function select a systemport icon not undefined
    i.value().selectPortIcon("", "", "Undefined"); //Dont realy need to write undefined here, could be empty, (just to make it clear)


    if (mIsSystemPort)
    {
        qDebug() << ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,Adding systemport with name: " << desiredportname;
        mModelObjectAppearance.setName(mpParentContainerObject->getCoreSystemAccessPtr()->addSystemPort(desiredportname));
        qDebug() << ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,resulting in name from core: " << mModelObjectAppearance.getName();
        mpPort = new Port(mModelObjectAppearance.getName(), x*boundingRect().width(), y*boundingRect().height(), &(i.value()), this);
    }
    else
    {
        qDebug() << ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,Adding groupport with desired name: " << desiredportname;
        mModelObjectAppearance.setName(mpParentContainerObject->getCoreSystemAccessPtr()->reserveUniqueName(desiredportname));
        mpPort = new GroupPort(mModelObjectAppearance.getName(), x*boundingRect().width(), y*boundingRect().height(), &(i.value()), this);
        SharedGroupInfoPtrT shp(new GroupPortCommonInfo);
        static_cast<GroupPort*>(mpPort)->setSharedGroupPortInfo(shp);
    }

    mPortListPtrs.append(mpPort);
}


//! Returns a string with the GUIObject type.
QString ContainerPort::getTypeName()
{
    return HOPSANGUICONTAINERPORTTYPENAME;
}

//! @brief Sets the name of the modelobject ContainerPort and the contained GUIPort
//! Note, this function will NOT change the core name of the component
void ContainerPort::setDisplayName(QString name)
{
    mModelObjectAppearance.setName(name);
    mPortListPtrs[0]->setDisplayName(name);
    refreshDisplayName();
}

//! @brief ContainerPorts shal only save their port name if they are systemports, if they are group ports no core data should be saved
void ContainerPort::saveCoreDataToDomElement(QDomElement &rDomElement)
{
    if (mIsSystemPort)
    {
        rDomElement.setAttribute(HMF_NAMETAG, getName());
    }
}

//! @brief Opens the properties dialog
void ContainerPort::openPropertiesDialog()
{
    ContainerPortPropertiesDialog *pDialog = new ContainerPortPropertiesDialog(this, gpMainWindow);
    pDialog->exec();
    delete pDialog;
}


//! @brief Event when double clicking on container port icon.
void ContainerPort::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mouseDoubleClickEvent(event);
    openPropertiesDialog();
}


int ContainerPort::type() const
{
    return Type;
}
