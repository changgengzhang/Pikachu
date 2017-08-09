/********************************************************************************
** Form generated from reading UI file 'Pikachu.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PIKACHU_H
#define UI_PIKACHU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include "renderview.h"

QT_BEGIN_NAMESPACE

class Ui_PikachuClass
{
public:
    QWidget *centralWidget;
    RenderView *renderView;

    void setupUi(QMainWindow *PikachuClass)
    {
        if (PikachuClass->objectName().isEmpty())
            PikachuClass->setObjectName(QStringLiteral("PikachuClass"));
        PikachuClass->resize(1005, 820);
        centralWidget = new QWidget(PikachuClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        renderView = new RenderView(centralWidget);
        renderView->setObjectName(QStringLiteral("renderView"));
        renderView->setGeometry(QRect(10, 10, 800, 800));
        PikachuClass->setCentralWidget(centralWidget);

        retranslateUi(PikachuClass);

        QMetaObject::connectSlotsByName(PikachuClass);
    } // setupUi

    void retranslateUi(QMainWindow *PikachuClass)
    {
        PikachuClass->setWindowTitle(QApplication::translate("PikachuClass", "Pikachu", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PikachuClass: public Ui_PikachuClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PIKACHU_H
