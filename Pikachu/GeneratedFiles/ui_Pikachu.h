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

QT_BEGIN_NAMESPACE

class Ui_PikachuClass
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget_4;
    QWidget *tab_5;
    QPushButton *bgSetBtn;
    QPushButton *bgDelBtn;
    QTabWidget *bgOperationTab;
    QWidget *tab_6;
    QCheckBox *bgSmoothBtn;
    QSlider *bgSmoothSlider;
    QTabWidget *tabWidget;
    QWidget *modelTab;
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
    QTabWidget *tabWidget_11;
    QWidget *tab_12;
    QPushButton *modelTextureDelBtn;
    QPushButton *modelTextureSetBtn;
    QCheckBox *modelTextureAverageBtn;
    QCheckBox *modelTextureShapPreserveBtn;
    QWidget *parameterizationTab;
    QPushButton *parameterizationMoelLoadBtn;
    QTabWidget *tabWidget_8;
    QWidget *tab_14;
    QCheckBox *parameterizationShowModelBtn;
    QCheckBox *parameterizationShowParamBtn;
    QTabWidget *tabWidget_9;
    QWidget *tab_11;
    QCheckBox *parameterizationBoundaryCircleBtn;
    QCheckBox *parameterizationBoundarySquareBtn;
    QTabWidget *tabWidget_10;
    QWidget *tab_15;
    QCheckBox *parameterizationInnerAverageBtn;
    QCheckBox *parameterizationShapPreserveBtn;
    QPushButton *parameterizationMoelDelBtn;
    QTabWidget *sensitivityTab;
    QWidget *tab;
    QSlider *mouseSensitiveSlider;
    QDoubleSpinBox *mouseSensitiveSpinBox;

    void setupUi(QMainWindow *PikachuClass)
    {
        if (PikachuClass->objectName().isEmpty())
            PikachuClass->setObjectName(QStringLiteral("PikachuClass"));
        PikachuClass->resize(1305, 920);
        PikachuClass->setMinimumSize(QSize(1305, 920));
        PikachuClass->setMaximumSize(QSize(1305, 920));
        centralWidget = new QWidget(PikachuClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget_4 = new QTabWidget(centralWidget);
        tabWidget_4->setObjectName(QStringLiteral("tabWidget_4"));
        tabWidget_4->setGeometry(QRect(1030, 10, 261, 171));
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        bgSetBtn = new QPushButton(tab_5);
        bgSetBtn->setObjectName(QStringLiteral("bgSetBtn"));
        bgSetBtn->setGeometry(QRect(10, 10, 101, 31));
        bgDelBtn = new QPushButton(tab_5);
        bgDelBtn->setObjectName(QStringLiteral("bgDelBtn"));
        bgDelBtn->setGeometry(QRect(140, 10, 101, 31));
        bgOperationTab = new QTabWidget(tab_5);
        bgOperationTab->setObjectName(QStringLiteral("bgOperationTab"));
        bgOperationTab->setGeometry(QRect(10, 50, 231, 81));
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
        tabWidget->setGeometry(QRect(1030, 200, 261, 351));
        modelTab = new QWidget();
        modelTab->setObjectName(QStringLiteral("modelTab"));
        tabWidget_2 = new QTabWidget(modelTab);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(10, 60, 231, 111));
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        modelPolygonFaceBtn = new QCheckBox(tab_2);
        modelPolygonFaceBtn->setObjectName(QStringLiteral("modelPolygonFaceBtn"));
        modelPolygonFaceBtn->setGeometry(QRect(30, 10, 61, 31));
        modelPolygonLineBtn = new QCheckBox(tab_2);
        modelPolygonLineBtn->setObjectName(QStringLiteral("modelPolygonLineBtn"));
        modelPolygonLineBtn->setGeometry(QRect(120, 10, 61, 31));
        modelPolygonPointBtn = new QCheckBox(tab_2);
        modelPolygonPointBtn->setObjectName(QStringLiteral("modelPolygonPointBtn"));
        modelPolygonPointBtn->setGeometry(QRect(30, 50, 61, 31));
        tabWidget_2->addTab(tab_2, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        modelSmoothBtn = new QCheckBox(tab_7);
        modelSmoothBtn->setObjectName(QStringLiteral("modelSmoothBtn"));
        modelSmoothBtn->setGeometry(QRect(30, 10, 61, 31));
        modelEditBtn = new QCheckBox(tab_7);
        modelEditBtn->setObjectName(QStringLiteral("modelEditBtn"));
        modelEditBtn->setGeometry(QRect(30, 50, 61, 31));
        modelEditBtn->setCheckable(false);
        modelDivideBtn = new QCheckBox(tab_7);
        modelDivideBtn->setObjectName(QStringLiteral("modelDivideBtn"));
        modelDivideBtn->setGeometry(QRect(120, 10, 61, 31));
        tabWidget_2->addTab(tab_7, QString());
        modelLoadBtn = new QPushButton(modelTab);
        modelLoadBtn->setObjectName(QStringLiteral("modelLoadBtn"));
        modelLoadBtn->setGeometry(QRect(10, 10, 101, 31));
        modelDelBtn = new QPushButton(modelTab);
        modelDelBtn->setObjectName(QStringLiteral("modelDelBtn"));
        modelDelBtn->setGeometry(QRect(140, 10, 101, 31));
        tabWidget_11 = new QTabWidget(modelTab);
        tabWidget_11->setObjectName(QStringLiteral("tabWidget_11"));
        tabWidget_11->setGeometry(QRect(10, 180, 231, 131));
        tab_12 = new QWidget();
        tab_12->setObjectName(QStringLiteral("tab_12"));
        modelTextureDelBtn = new QPushButton(tab_12);
        modelTextureDelBtn->setObjectName(QStringLiteral("modelTextureDelBtn"));
        modelTextureDelBtn->setGeometry(QRect(130, 10, 61, 21));
        modelTextureSetBtn = new QPushButton(tab_12);
        modelTextureSetBtn->setObjectName(QStringLiteral("modelTextureSetBtn"));
        modelTextureSetBtn->setGeometry(QRect(20, 10, 61, 21));
        modelTextureAverageBtn = new QCheckBox(tab_12);
        modelTextureAverageBtn->setObjectName(QStringLiteral("modelTextureAverageBtn"));
        modelTextureAverageBtn->setGeometry(QRect(20, 40, 61, 31));
        modelTextureShapPreserveBtn = new QCheckBox(tab_12);
        modelTextureShapPreserveBtn->setObjectName(QStringLiteral("modelTextureShapPreserveBtn"));
        modelTextureShapPreserveBtn->setGeometry(QRect(20, 70, 101, 31));
        tabWidget_11->addTab(tab_12, QString());
        tabWidget->addTab(modelTab, QString());
        parameterizationTab = new QWidget();
        parameterizationTab->setObjectName(QStringLiteral("parameterizationTab"));
        parameterizationMoelLoadBtn = new QPushButton(parameterizationTab);
        parameterizationMoelLoadBtn->setObjectName(QStringLiteral("parameterizationMoelLoadBtn"));
        parameterizationMoelLoadBtn->setGeometry(QRect(10, 10, 101, 31));
        tabWidget_8 = new QTabWidget(parameterizationTab);
        tabWidget_8->setObjectName(QStringLiteral("tabWidget_8"));
        tabWidget_8->setGeometry(QRect(10, 230, 231, 81));
        tab_14 = new QWidget();
        tab_14->setObjectName(QStringLiteral("tab_14"));
        parameterizationShowModelBtn = new QCheckBox(tab_14);
        parameterizationShowModelBtn->setObjectName(QStringLiteral("parameterizationShowModelBtn"));
        parameterizationShowModelBtn->setGeometry(QRect(20, 10, 61, 31));
        parameterizationShowParamBtn = new QCheckBox(tab_14);
        parameterizationShowParamBtn->setObjectName(QStringLiteral("parameterizationShowParamBtn"));
        parameterizationShowParamBtn->setGeometry(QRect(100, 10, 121, 31));
        tabWidget_8->addTab(tab_14, QString());
        tabWidget_9 = new QTabWidget(parameterizationTab);
        tabWidget_9->setObjectName(QStringLiteral("tabWidget_9"));
        tabWidget_9->setGeometry(QRect(10, 50, 231, 81));
        tab_11 = new QWidget();
        tab_11->setObjectName(QStringLiteral("tab_11"));
        parameterizationBoundaryCircleBtn = new QCheckBox(tab_11);
        parameterizationBoundaryCircleBtn->setObjectName(QStringLiteral("parameterizationBoundaryCircleBtn"));
        parameterizationBoundaryCircleBtn->setGeometry(QRect(20, 20, 61, 31));
        parameterizationBoundarySquareBtn = new QCheckBox(tab_11);
        parameterizationBoundarySquareBtn->setObjectName(QStringLiteral("parameterizationBoundarySquareBtn"));
        parameterizationBoundarySquareBtn->setGeometry(QRect(110, 20, 61, 31));
        tabWidget_9->addTab(tab_11, QString());
        tabWidget_10 = new QTabWidget(parameterizationTab);
        tabWidget_10->setObjectName(QStringLiteral("tabWidget_10"));
        tabWidget_10->setGeometry(QRect(10, 140, 231, 81));
        tab_15 = new QWidget();
        tab_15->setObjectName(QStringLiteral("tab_15"));
        parameterizationInnerAverageBtn = new QCheckBox(tab_15);
        parameterizationInnerAverageBtn->setObjectName(QStringLiteral("parameterizationInnerAverageBtn"));
        parameterizationInnerAverageBtn->setGeometry(QRect(20, 10, 61, 31));
        parameterizationShapPreserveBtn = new QCheckBox(tab_15);
        parameterizationShapPreserveBtn->setObjectName(QStringLiteral("parameterizationShapPreserveBtn"));
        parameterizationShapPreserveBtn->setGeometry(QRect(100, 10, 101, 31));
        tabWidget_10->addTab(tab_15, QString());
        parameterizationMoelDelBtn = new QPushButton(parameterizationTab);
        parameterizationMoelDelBtn->setObjectName(QStringLiteral("parameterizationMoelDelBtn"));
        parameterizationMoelDelBtn->setGeometry(QRect(140, 10, 101, 31));
        tabWidget->addTab(parameterizationTab, QString());
        sensitivityTab = new QTabWidget(centralWidget);
        sensitivityTab->setObjectName(QStringLiteral("sensitivityTab"));
        sensitivityTab->setGeometry(QRect(1030, 570, 261, 80));
        sensitivityTab->setTabShape(QTabWidget::Rounded);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        mouseSensitiveSlider = new QSlider(tab);
        mouseSensitiveSlider->setObjectName(QStringLiteral("mouseSensitiveSlider"));
        mouseSensitiveSlider->setGeometry(QRect(10, 20, 151, 22));
        mouseSensitiveSlider->setMinimum(0);
        mouseSensitiveSlider->setMaximum(100);
        mouseSensitiveSlider->setValue(10);
        mouseSensitiveSlider->setOrientation(Qt::Horizontal);
        mouseSensitiveSpinBox = new QDoubleSpinBox(tab);
        mouseSensitiveSpinBox->setObjectName(QStringLiteral("mouseSensitiveSpinBox"));
        mouseSensitiveSpinBox->setGeometry(QRect(180, 20, 62, 22));
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
        tabWidget_11->setCurrentIndex(0);
        tabWidget_8->setCurrentIndex(0);
        tabWidget_9->setCurrentIndex(0);
        tabWidget_10->setCurrentIndex(0);
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
        modelLoadBtn->setText(QApplication::translate("PikachuClass", "Load", Q_NULLPTR));
        modelDelBtn->setText(QApplication::translate("PikachuClass", "Del", Q_NULLPTR));
        modelTextureDelBtn->setText(QApplication::translate("PikachuClass", "Del", Q_NULLPTR));
        modelTextureSetBtn->setText(QApplication::translate("PikachuClass", "Set", Q_NULLPTR));
        modelTextureAverageBtn->setText(QApplication::translate("PikachuClass", "Averarge", Q_NULLPTR));
        modelTextureShapPreserveBtn->setText(QApplication::translate("PikachuClass", "Shap Preserve", Q_NULLPTR));
        tabWidget_11->setTabText(tabWidget_11->indexOf(tab_12), QApplication::translate("PikachuClass", "Texture", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(modelTab), QApplication::translate("PikachuClass", "    Model    ", Q_NULLPTR));
        parameterizationMoelLoadBtn->setText(QApplication::translate("PikachuClass", "Load", Q_NULLPTR));
        parameterizationShowModelBtn->setText(QApplication::translate("PikachuClass", "Model", Q_NULLPTR));
        parameterizationShowParamBtn->setText(QApplication::translate("PikachuClass", "Parameterization", Q_NULLPTR));
        tabWidget_8->setTabText(tabWidget_8->indexOf(tab_14), QApplication::translate("PikachuClass", "Show", Q_NULLPTR));
        parameterizationBoundaryCircleBtn->setText(QApplication::translate("PikachuClass", "Circle", Q_NULLPTR));
        parameterizationBoundarySquareBtn->setText(QApplication::translate("PikachuClass", "Square", Q_NULLPTR));
        tabWidget_9->setTabText(tabWidget_9->indexOf(tab_11), QApplication::translate("PikachuClass", "Boundary", Q_NULLPTR));
        parameterizationInnerAverageBtn->setText(QApplication::translate("PikachuClass", "Averarge", Q_NULLPTR));
        parameterizationShapPreserveBtn->setText(QApplication::translate("PikachuClass", "Shap Preserve", Q_NULLPTR));
        tabWidget_10->setTabText(tabWidget_10->indexOf(tab_15), QApplication::translate("PikachuClass", "Inner", Q_NULLPTR));
        parameterizationMoelDelBtn->setText(QApplication::translate("PikachuClass", "Del", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(parameterizationTab), QApplication::translate("PikachuClass", "Parameterization", Q_NULLPTR));
        sensitivityTab->setTabText(sensitivityTab->indexOf(tab), QApplication::translate("PikachuClass", "MouseSensitivity", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PikachuClass: public Ui_PikachuClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PIKACHU_H
