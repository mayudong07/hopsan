#ifndef COMPONENTPROPERTIESDIALOG3_H
#define COMPONENTPROPERTIESDIALOG3_H

#include <QtGui>
//#include "Dialogs/ModelObjectPropertiesDialog.h"
#include "CoreAccess.h"

class Component;
class ParameterSettingsLayout;
class MainWindow;

class RowAwareToolButton :public QToolButton
{
    Q_OBJECT
public:
    RowAwareToolButton(const int row);
    void setRow(const int row);

signals:
    void triggeredAtRow(int);

private:
    int mRow;

private slots:
    void clickedSlot();
};

class RowAwareCheckBox :public QCheckBox
{
    Q_OBJECT
public:
    RowAwareCheckBox(const int row);
    void setRow(const int row);

signals:
    void checkedAtRow(int, bool);

private:
    int  mRow;

private slots:
    void checkedSlot(const bool state);
};

class TableWidgetTotalSize : public QTableWidget
{
public:
    TableWidgetTotalSize(QWidget *pParent=0);
    QSize sizeHint() const;
    void setMaxVisibleRows(const int maxRows);
private:
    int mMaxVisibleRows;
};

class VariableTableWidget :public TableWidgetTotalSize
{
    Q_OBJECT
public:
    enum ColumnEnumT {Name, Alias, Unit, Description, Type, Value, Scale, Buttons, NumCols};
    VariableTableWidget(Component *pModelObject, QWidget *pParent);
    bool setStartValues();
    bool setAliasNames();

private slots:
    void resetDefaultValueAtRow(int row);
    void selectSystemParameterAtRow(int row);
    void makePortAtRow(int row, bool isPort);
    void cellChangedSlot(const int row, const int col);

private:
    void createTableRow(const int row, const CoreVariameterDescription &rData);
    void createSeparatorRow(const int row, const QString name);
    void selectValueTextColor(const int row);
    bool cleanAndVerifyParameterValue(QString &rValue, const QString type);
    //void setStartValue(const int row);
    bool setAliasName(const int row);
    Component *mpModelObject;
};

class ComponentPropertiesDialog3 : public QDialog
{
    Q_OBJECT

public:
    ComponentPropertiesDialog3(Component *pModelObject, QWidget *pParent=0);

protected slots:
    void okPressed();
    void editPortPos();

protected:
    bool setAliasNames();
    bool setVariableValues();
    void setName();
    void recompileCppFromDialog();
    virtual void closeEvent(QCloseEvent* event);
    virtual void reject();

private:
    QGridLayout* createNameAndTypeEdit();
    QDialogButtonBox* createButtonBox();
    QWidget* createHelpWidget();
    QWidget* createSourcodeBrowser(QString &rFilePath);
    void createEditStuff();

    Component *mpModelObject;
    QLineEdit *mpNameEdit;
    VariableTableWidget *mpVariableTableWidget;

    QSpinBox *mpInputPortsSpinBox;
    QSpinBox *mpOutputPortsSpinBox;
    QTextEdit *mpTextEdit;
};

#endif // COMPONENTPROPERTIESDIALOG3_H