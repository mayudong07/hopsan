/*-----------------------------------------------------------------------------

 Copyright 2017 Hopsan Group

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

 The full license is available in the file GPLv3.
 For details about the 'Hopsan Group' or information about Authors and
 Contributors see the HOPSANGROUP and AUTHORS files that are located in
 the Hopsan source code root directory.

-----------------------------------------------------------------------------*/

#include <QGridLayout>
#include <QEvent>
#include <QKeyEvent>

#include "EditorWidget.h"
#include "Handlers/FileHandler.h"
#include "Configuration.h"
#include "Utilities/HighlightingUtilities.h"

EditorWidget::EditorWidget(Configuration *pConfiguration, QWidget *parent) :
    QWidget(parent)
{
    mpConfiguration = pConfiguration;

    //Create widgets
    mpNotEditableLabel = new QLabel("<font color='darkred'><h3><b>This file cannot be edited from within HoLC.</b></h3></font>", this);
    mpNotEditableLabel->hide();

    mpTextEdit = new TextEditor(this);
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    mpTextEdit->setFont(font);

    //Create layout
    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(mpNotEditableLabel,    0,0,1,2);
    pLayout->addWidget(mpTextEdit,          1,0,1,2);
    this->setLayout(pLayout);

    mpXmlHighlighter = new XmlHighlighter(0);
    mpCppHighlighter = new CppHighlighter(0);

    //Create connections
    connect(mpTextEdit, SIGNAL(textChanged()), this, SIGNAL(textChanged()));
    connect(mpConfiguration, SIGNAL(configChanged()), this, SLOT(update()));
}

void EditorWidget::setText(const QString &text, HighlighterTypeEnum type, bool editingEnabled)
{
    disconnect(mpTextEdit, SIGNAL(textChanged()), this, SIGNAL(textChanged()));

    mpTextEdit->setPlainText(text);

    if(type == XML)
    {
        mpCppHighlighter->setDocument(0);
        mpXmlHighlighter->setDocument(mpTextEdit->document());
    }
    else if(type == Cpp)
    {
        mpXmlHighlighter->setDocument(0);
        mpCppHighlighter->setDocument(mpTextEdit->document());
    }

    mpTextEdit->setPlainText(text);
    mpTextEdit->setReadOnly(!editingEnabled);
    mpNotEditableLabel->setVisible(!editingEnabled);

    connect(mpTextEdit, SIGNAL(textChanged()), this, SIGNAL(textChanged()));
}

QString EditorWidget::getText() const
{
    return mpTextEdit->toPlainText();
}

void EditorWidget::clear()
{
    mpTextEdit->clear();
}

void EditorWidget::update()
{
    if(mpConfiguration->getUseTextWrapping())
    {
        mpTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    }
    else
    {
        mpTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    }
    QWidget::update();
}
