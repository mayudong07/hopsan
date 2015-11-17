/*-----------------------------------------------------------------------------
 This source file is a part of Hopsan

 Copyright (c) 2009 to present year, Hopsan Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 For license details and information about the Hopsan Group see the files
 GPLv3 and HOPSANGROUP in the Hopsan source code root directory

 For author and contributor information see the AUTHORS file
-----------------------------------------------------------------------------*/

//!
//! @file   SystemParametersWidget.h
//! @author Flumes <flumes@lists.iei.liu.se>
//! @date   2010-11-20
//!
//! @brief Contains a System parameter widget class
//!
//$Id$

#ifndef SYSTEMPARAMETERSWIDGET_H
#define SYSTEMPARAMETERSWIDGET_H

#include <QComboBox>
#include <QItemDelegate>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QPushButton>
#include <QPointer>

#include "CoreAccess.h"

// Forward Declarations
class ContainerObject;

class ParameterTypeComboBox : public QComboBox
{
public:
    ParameterTypeComboBox(QWidget *pParentWidget=0) : QComboBox(pParentWidget)
    {
        addItem("double");
        addItem("integer");
        addItem("bool");
        addItem("string");
    }
};

class ParamTypeComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


class SysParamTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SysParamTableModel(ContainerObject *pContainerObject, QObject *pParent=0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    void setContainer(ContainerObject *pContainerObject);
    bool addOrSetParameter(CoreParameterData &rParameterData);
    void getFullParameterData(const QModelIndex &index, CoreParameterData &rParameterData);
    bool hasParameter(const QString name);

public slots:
    void sortByColumn(int c);

protected:
    void removeParameter(const int row);

    QPointer<ContainerObject> mpContainerObject;
    QVector<CoreParameterData> mParameterData;
};


class SystemParametersWidget : public QWidget
{
    Q_OBJECT
public:
    SystemParametersWidget(QWidget *pParent=0);
    QPointer<ContainerObject> getRepresentedContainerObject();

public slots:
    void update();
    void update(ContainerObject* pNewContainer);
    void openAddParameterDialog();
    void openEditParameterDialog();
    void highlightComponents(QModelIndex index);

protected slots:
    bool addParameter();
    void addParameterAndCloseDialog();
    void removeSelected();
    void closeDialog();

private:
    QPointer<ContainerObject> mpContainerObject;
    QTableView *mpSysParamTableView;
    QSortFilterProxyModel *mpProxyModel;
    SysParamTableModel *mpModel;

    QDialog *mpAddParameterDialog=nullptr;
    QLineEdit *mpNewParamNameEdit;
    QLineEdit *mpNewParamValueEdit;
    QLineEdit *mpNewParamDescriptionEdit;
    QLineEdit *mpNewParamUnitQuantityEdit;
    QComboBox *mpNewParamTypeBox;

    QPushButton *mpAddButton;
    QPushButton *mpEditButton;
    QPushButton *mpRemoveButton;
};

#endif // SYSTEMPARAMETERSWIDGET_H
