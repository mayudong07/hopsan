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
//! @file   GUIPort.h
//! @author Flumes <flumes@lists.iei.liu.se>
//! @date   2010-01-01
//!
//! @brief Contains the GUIPort class
//!
//$Id$

#ifndef PORT_H
#define PORT_H

#include <QGraphicsSvgItem>
//#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsWidget>
#include <QSharedPointer>

#include "common.h"
#include "GUIPortAppearance.h"
#include "CoreAccess.h"

//Forward declarations
class ModelObject;
class SystemContainer;
class ContainerObject;
class Connector;
class PlotWindow;

enum PortDirectionT {TOPBOTTOM, LEFTRIGHT};

class Port :public QGraphicsWidget
{
    Q_OBJECT
public:
    Port(QString name, qreal xpos, qreal ypos, PortAppearance* pPortAppearance, ModelObject *pParent = 0);
    ~Port();

    ContainerObject *getParentContainerObjectPtr();
    ModelObject *getGuiModelObject();
    QString getGuiModelObjectName();

    QString getPortName() const;
    void setDisplayName(const QString name);

    QPointF getCenterPos();
    qreal getPortRotation();
    PortDirectionT getPortDirection();
    void setCenterPos(qreal x, qreal y);
    void setCenterPosByFraction(qreal x, qreal y);
    void setRotation(qreal angle);

    void magnify(bool blowup);
    void show();
    void hide();

    virtual QString getPortType(const CoreSystemAccess::PortTypeIndicatorT ind=CoreSystemAccess::ACTUALPORTTYPE);
    virtual QString getNodeType();

    void getStartValueDataNamesValuesAndUnits(QVector<QString> &rNames, QVector<double> &rValues, QVector<QString> &rUnits);
    void getStartValueDataNamesValuesAndUnits(QVector<QString> &rNames, QVector<QString> &rValuesTxt, QVector<QString> &rUnits);
//    void setStartValueDataByNames(QVector<QString> names, QVector<double> values);
//    bool setStartValueDataByNames(QVector<QString> names, QVector<QString> valuesTxt);


    bool getLastNodeData(QString dataName, double& rData);

    virtual void addConnection(Connector *pConnector);
    virtual void removeConnection(Connector *pConnector);
    QVector<Connector*> getAttachedConnectorPtrs() const; //!< @todo should this be virtual also
    virtual QVector<Port *> getConnectedPorts();
    bool isConnected();

    virtual Port* getRealPort();

    ModelObject *mpParentGuiModelObject; //!< @todo make private

public slots:
    void showIfNotConnected(bool doShow=true);
    void setVisible(bool value);
    PlotWindow* plot(QString dataName, QString dataUnit=QString(), QColor desiredCurveColor=QColor());
    void plotToPlotWindow(PlotWindow *pPlotWindow, QString dataName, QString dataUnit=QString(), int axisY=0);
    void refreshPortOverlayPosition();
    void refreshPortGraphics(const CoreSystemAccess::PortTypeIndicatorT int_ext_act=CoreSystemAccess::ACTUALPORTTYPE);

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void openRightClickMenu(QPoint screenPos);

    QVector<Connector*> mConnectedConnectors;

protected slots:
    void refreshPortOverlayScale(qreal scale);

private:
    void refreshPortMainGraphics();
    void refreshPortOverlayGraphics();

//    QColor myLineColor;
//    qreal myLineWidth;
//    QGraphicsLineItem *lineH;
//    QGraphicsLineItem *lineV;

    PortAppearance *mpPortAppearance;
    PortAppearance mPortAppearanceAfterLastRefresh;
    QString mPortDisplayName;

    qreal mMag;
    qreal mOverlaySetScale;
    bool mIsMagnified;

    QGraphicsTextItem *mpPortLabel;
    //QVector<QGraphicsSvgItem*> mvPortGraphicsOverlayPtrs;
    QGraphicsSvgItem *mpCQSIconOverlay;
    QGraphicsSvgItem *mpMultiPortIconOverlay;
    QGraphicsSvgItem *mpMainIcon;
};


class GroupPortCommonInfo
{
public:
    QVector<Connector*> mConnectedConnectors;
    QList<Port*> mSharedPorts;
};

typedef QSharedPointer<GroupPortCommonInfo>  SharedGroupInfoPtrT;

class GroupPort : public Port
{
public:
    GroupPort(QString name, qreal xpos, qreal ypos, PortAppearance* pPortAppearance, ModelObject *pParentObject);
    QString getPortType(const CoreSystemAccess::PortTypeIndicatorT ind=CoreSystemAccess::ACTUALPORTTYPE);
    QString getNodeType();

    void addConnection(Connector *pConnector);
    void removeConnection(Connector *pConnector);

    Port* getRealPort();
    QVector<Port *> getConnectedPorts();
    SharedGroupInfoPtrT getSharedGroupPortInfo();
    void setSharedGroupPortInfo(SharedGroupInfoPtrT sharedGroupPortInfo);

protected:
    SharedGroupInfoPtrT mSharedGroupPortInfo;

};

QPointF getOffsetPointfromPort(Port *pStartPortGUIPort, Port *pEndPort);

#endif // GUIPORT_H
