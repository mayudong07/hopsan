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
//! @file   loadFunctions.h
//! @author Flumes <flumes@lists.iei.liu.se>
//! @date   2010-01-01
//!
//! @brief Contains classes and functions used to recreate models from load data
//!
//$Id$

#ifndef LOADFUNCTIONS_H
#define LOADFUNCTIONS_H

#include "Utilities/XMLUtilities.h"
#include "common.h"


//Forward Declarations
class LibraryWidget;
class ModelObject;
class ContainerObject;
class TextBoxWidget;


ModelObject* loadModelObject(QDomElement &rDomElement, LibraryWidget* pLibrary, ContainerObject* pContainer, undoStatus undoSettings=UNDO);

ModelObject* loadContainerPortObject(QDomElement &rDomElement, LibraryWidget* pLibrary, ContainerObject* pContainer, undoStatus undoSettings=UNDO);

bool loadConnector(QDomElement &rDomElement, ContainerObject* pContainer, undoStatus undoSettings=UNDO);

void loadParameterValue(QDomElement &rDomElement, ModelObject* pObject, undoStatus undoSettings=UNDO);

void loadStartValue(QDomElement &rDomElement, ModelObject* pObject, undoStatus undoSettings=UNDO);

void loadSystemParameter(QDomElement &rDomElement, double hmfVersion, ContainerObject* pContainer);

void loadFavoriteVariable(QDomElement &rDomElement, ContainerObject* pContainer);

void loadPlotAlias(QDomElement &rDomElement, ContainerObject* pContainer);

TextBoxWidget* loadTextBoxWidget(QDomElement &rDomElement, ContainerObject *pContainer, undoStatus undoSettings=UNDO);

#endif // LOADFUNCTIONS_H
