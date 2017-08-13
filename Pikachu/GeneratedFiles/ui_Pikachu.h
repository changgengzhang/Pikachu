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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "renderview.h"

QT_BEGIN_NAMESPACE

class Ui_PikachuClass
{
public:
    QWidget *centralWidget;
    RenderView *renderView;
    QTabWidget *tabWidget_4;
    QWidget *tab_5;
    QPushButton *bgSetBtn;
    QPushButton *bgDelBtn;
    QTabWidget *bgOperationTab;
    QWidget *tab_6;
    QCheckBox *bgSmoothBtn;
    QSlider *bgSmoothSlider;
    QTabWidget *tabWidget;
    QWidget *tab_4;
    QTabWidget *tabWidget_2;
    QWidget *tab_2;
    QCheckBox *modelPolygonFaceBtn;
    QCheckBox *modelPolygonLineBtn;
    QCheckBox *modelPolygonPointBtn;
    QWidget *tab_7;
    QCheckBox *modelSmoothBtn;
    QCheckBox *modelEditBtn;
    QCheckBox *modelDivideBtn;
    QPushButton *modelLoadBtn;
    QPushButton *modelDelBtn;
    QTabWidget *sensitivityTab;
    QWidget *tab;
    QSlider *mouseSensitiveSlider;
    QDoubleSpinBox *mouseSensitiveSpinBox;

    void setupUi(QMainWindow *PikachuClass)
    {
        if (PikachuClass->objectName().isEmpty())
            PikachuClass->setObjectName(QStringLiteral("PikachuClass"));
        PikachuClass->resize(1273, 820);
        PikachuClass->setMinimumSize(QSize(1273, 820));
        PikachuClass->setMaximumSize(QSize(1273, 820));
        centralWidget = new QWidget(PikachuClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        renderView = new RenderView(centralWidget);
        renderView->setObjectName(QStringLiteral("renderView"));
        renderView->setGeometry(QRect(10, 10, 1000, 800));
        tabWidget_4 = new QTabWidget(centralWidget);
        tabWidget_4->setObjectName(QStringLiteral("tabWidget_4"));
        tabWidget_4->setGeometry(QRect(1020, 10, 241, 231));
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        bgSetBtn = new QPushButton(tab_5);
        bgSetBtn->setObjectName(QStringLiteral("bgSetBtn"));
        bgSetBtn->setGeometry(QRect(10, 10, 101, 31));
        bgDelBtn = new QPushButton(tab_5);
        bgDelBtn->setObjectName(QStringLiteral("bgDelBtn"));
        bgDelBtn->setGeometry(QRect(130, 10, 101, 31));
        bgOperationTab = new QTabWidget(tab_5);
        bgOperationTab->setObjectName(QStringLiteral("bgOperationTab"));
        bgOperationTab->setGeometry(QRect(10, 50, 221, 141));
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        bgSmoothBtn = new QCheckBox(tab_6);
        bgSmoothBtn->setObjectName(QStringLiteral("bgSmoothBtn"));
        bgSmoothBtn->setGeometry(QRect(10, 10, 61, 41));
        bgSmoothSlider = new QSlider(tab_6);
        bgSmoothSlider->setObjectName(QStringLiteral("bgSmoothSlider"));
        bgSmoothSlider->setGeometry(QRect(80, 20, 131, 22));
        bgSmoothSlider->setMinimum(1);
        bgSmoothSlider->setMaximum(99);
        bgSmoothSlider->setSingleStep(2);
        bgSmoothSlider->setValue(1);
        bgSmoothSlider->setSliderPosition(1);
        bgSmoothSlider->setOrientation(Qt::Horizontal);
        bgOperationTab->addTab(tab_6, QString());
        tabWidget_4->addTab(tab_5, QString());
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(1020, 260, 241, 231));
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        tabWidget_2 = new QTabWidget(tab_4);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(10, 60, 211, 131));
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        modelPolygonFaceBtn = new QCheckBox(tab_2);
        modelPolygonFaceBtn->setObjectName(QStringLiteral("modelPolygonFaceBtn"));
        modelPolygonFaceBtn->setGeometry(QRect(30, 20, 61, 31));
        modelPolygonLineBtn = new QCheckBox(tab_2);
        modelPolygonLineBtn->setObjectName(QStringLiteral("modelPolygonLineBtn"));
        modelPolygonLineBtn->setGeometry(QRect(120, 20, 61, 31));
        modelPolygonPointBtn = new QCheckBox(tab_2);
        modelPolygonPointBtn->setObjectName(QStringLiteral("modelPolygonPointBtn"));
        modelPolygonPointBtn->setGeometry(QRect(30, 60, 61, 31));
        tabWidget_2->addTab(tab_2, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        modelSmoothBtn = new QCheckBox(tab_7);
        modelSmoothBtn->setObjectName(QStringLiteral("modelSmoothBtn"));
        modelSmoothBtn->setGeometry(QRect(30, 20, 61, 31));
        modelEditBtn = new QCheckBox(tab_7);
        modelEditBtn->setObjectName(QStringLiteral("modelEditBtn"));
        modelEditBtn->setGeometry(QRect(30, 60, 61, 31));
        modelEditBtn->setCheckable(false);
        modelDivideBtn = new QCheckBox(tab_7);
        modelDivideBtn->setObjectName(QStringLiteral("modelDivideBtn"));
        modelDivideBtn->setGeometry(QRect(120, 20, 61, 31));
        tabWidget_2->addTab(tab_7, QString());
        modelLoadBtn = new QPushButton(tab_4);
        modelLoadBtn->setObjectName(QStringLiteral("modelLoadBtn"));
        modelLoadBtn->setGeometry(QRect(10, 10, 91, 31));
        modelDelBtn = new QPushButton(tab_4);
        modelDelBtn->setObjectName(QStringLiteral("modelDelBtn"));
        modelDelBtn->setGeometry(QRect(130, 10, 91, 31));
        tabWidget->addTab(tab_4, QString());
        sensitivityTab = new QTabWidget(centralWidget);
        sensitivityTab->setObjectName(QStringLiteral("sensitivityTab"));
        sensitivityTab->setGeometry(QRect(1020, 510, 241, 80));
        sensitivityTab->setTabShape(QTabWidget::Rounded);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        mouseSensitiveSlider = new QSlider(tab);
        mouseSensitiveSlider->setObjectName(QStringLiteral("mouseSensitiveSlider"));
        mouseSensitiveSlider->setGeometry(QRect(10, 20, 131, 22));
        mouseSensitiveSlider->setMinimum(0);
        mouseSensitiveSlider->setMaximum(100);
        mouseSensitiveSlider->setValue(10);
        mouseSensitiveSlider->setOrientation(Qt::Horizontal);
        mouseSensitiveSpinBox = new QDoubleSpinBox(tab);
        mouseSensitiveSpinBox->setObjectName(QStringLiteral("mouseSensitiveSpinBox"));
        mouseSensitiveSpinBox->setGeometry(QRect(160, 20, 62, 22));
        mouseSensitiveSpinBox->setMinimum(0);
        mouseSensitiveSpinBox->setMaximum(1);
        mouseSensitiveSpinBox->setSingleStep(0.01);
        mouseSensitiveSpinBox->setValue(0.1);
        sensitivityTab->addTab(tab, QString());
        PikachuClass->setCentralWidget(centralWidget);

        retranslateUi(PikachuClass);

        tabWidget_4->setCurrentIndex(0);
        bgOperationTab->setCurrentIndex(0);
        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);
        sensitivityTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PikachuClass);
    } // setupUi

    void retranslateUi(QMainWindow *PikachuClass)
    {
        PikachuClass->setWindowTitle(QApplication::translate("PikachuClass", "Pikachu", Q_NULLPTR));
        bgSetBtn->setText(QApplication::translate("PikachuClass", "Set Background", Q_NULLPTR));
        bgDelBtn->setText(QApplication::translate("PikachuClass", "Del Background", Q_NULLPTR));
        bgSmoothBtn->setText(QApplication::translate("PikachuClass", "Smooth", Q_NULLPTR));
        bgOperationTab->setTabText(bgOperationTab->indexOf(tab_6), QApplication::translate("PikachuClass", "Operation", Q_NULLPTR));
        tabWidget_4->setTabText(tabWidget_4->indexOf(tab_5), QApplication::translate("PikachuClass", "Background", Q_NULLPTR));
        modelPolygonFaceBtn->setText(QApplication::translate("PikachuClass", "Face", Q_NULLPTR));
        modelPolygonLineBtn->setText(QApplication::translate("PikachuClass", "Line", Q_NULLPTR));
        modelPolygonPointBtn->setText(QApplication::translate("PikachuClass", "Point", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QApplication::translate("PikachuClass", "Polygon", Q_NULLPTR));
        modelSmoothBtn->setText(QApplication::translate("PikachuClass", "Smooth", Q_NULLPTR));
        modelEditBtn->setText(QApplication::translate("PikachuClass", "Edit", Q_NULLPTR));
        modelDivideBtn->setText(QApplication::translate("PikachuClass", "Divide", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_7), QApplication::translate("PikachuClass", "Operation", Q_NULLPTR));
        modelLoadBtn->setText(QApplication::translate("PikachuClass", "Load Model", Q_NULLPTR));
        modelDelBtn->setText(QApplication::translate("PikachuClass", "Del Model", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("PikachuClass", "Model", Q_NULLPTR));
        sensitivityTab->setTabText(sensitivityTab->indexOf(tab), QApplication::translate("PikachuClass", "MouseSensitivity", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PikachuClass: public Ui_PikachuClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PIKACHU_H
