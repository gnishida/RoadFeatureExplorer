/********************************************************************************
** Form generated from reading UI file 'ControlWidget.ui'
**
** Created: Fri Jan 24 13:44:55 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLWIDGET_H
#define UI_CONTROLWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlWidget
{
public:
    QWidget *dockWidgetContents;
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *lineEditEdgeType;
    QLabel *label_2;
    QLineEdit *lineEditEdgeLanes;
    QLabel *label_3;
    QLineEdit *lineEditEdgeOneWay;
    QGroupBox *groupBox_2;
    QLabel *label_4;
    QLineEdit *lineEditSimplifyThreshold;
    QPushButton *pushButtonSimplify;
    QGroupBox *groupBox_3;
    QPushButton *pushButtonRemoveShortDeadend;
    QLabel *label_5;
    QLineEdit *lineEditRemoveShortDeadendThreshold;
    QGroupBox *groupBox_4;
    QPushButton *pushButtonClean;
    QGroupBox *groupBox_5;
    QLabel *label_6;
    QLineEdit *lineEditVertexDesc;
    QLabel *label_7;
    QLineEdit *lineEditVertexPos;
    QLabel *label_8;
    QLineEdit *lineEditVertexNeighbors;
    QGroupBox *groupBox_6;
    QPushButton *pushButtonReduce;

    void setupUi(QDockWidget *ControlWidget)
    {
        if (ControlWidget->objectName().isEmpty())
            ControlWidget->setObjectName(QString::fromUtf8("ControlWidget"));
        ControlWidget->resize(192, 831);
        ControlWidget->setMinimumSize(QSize(192, 240));
        ControlWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 130, 171, 111));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 31, 16));
        lineEditEdgeType = new QLineEdit(groupBox);
        lineEditEdgeType->setObjectName(QString::fromUtf8("lineEditEdgeType"));
        lineEditEdgeType->setGeometry(QRect(80, 20, 81, 20));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 50, 51, 16));
        lineEditEdgeLanes = new QLineEdit(groupBox);
        lineEditEdgeLanes->setObjectName(QString::fromUtf8("lineEditEdgeLanes"));
        lineEditEdgeLanes->setGeometry(QRect(80, 50, 81, 20));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 80, 51, 16));
        lineEditEdgeOneWay = new QLineEdit(groupBox);
        lineEditEdgeOneWay->setObjectName(QString::fromUtf8("lineEditEdgeOneWay"));
        lineEditEdgeOneWay->setGeometry(QRect(80, 80, 81, 20));
        groupBox_2 = new QGroupBox(dockWidgetContents);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 320, 171, 91));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 20, 46, 21));
        lineEditSimplifyThreshold = new QLineEdit(groupBox_2);
        lineEditSimplifyThreshold->setObjectName(QString::fromUtf8("lineEditSimplifyThreshold"));
        lineEditSimplifyThreshold->setGeometry(QRect(80, 20, 81, 20));
        pushButtonSimplify = new QPushButton(groupBox_2);
        pushButtonSimplify->setObjectName(QString::fromUtf8("pushButtonSimplify"));
        pushButtonSimplify->setGeometry(QRect(10, 50, 151, 31));
        groupBox_3 = new QGroupBox(dockWidgetContents);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 490, 171, 91));
        pushButtonRemoveShortDeadend = new QPushButton(groupBox_3);
        pushButtonRemoveShortDeadend->setObjectName(QString::fromUtf8("pushButtonRemoveShortDeadend"));
        pushButtonRemoveShortDeadend->setGeometry(QRect(10, 50, 151, 31));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 20, 46, 21));
        lineEditRemoveShortDeadendThreshold = new QLineEdit(groupBox_3);
        lineEditRemoveShortDeadendThreshold->setObjectName(QString::fromUtf8("lineEditRemoveShortDeadendThreshold"));
        lineEditRemoveShortDeadendThreshold->setGeometry(QRect(80, 20, 81, 20));
        groupBox_4 = new QGroupBox(dockWidgetContents);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 250, 171, 61));
        pushButtonClean = new QPushButton(groupBox_4);
        pushButtonClean->setObjectName(QString::fromUtf8("pushButtonClean"));
        pushButtonClean->setGeometry(QRect(10, 20, 151, 31));
        groupBox_5 = new QGroupBox(dockWidgetContents);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 10, 171, 111));
        label_6 = new QLabel(groupBox_5);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 20, 31, 16));
        lineEditVertexDesc = new QLineEdit(groupBox_5);
        lineEditVertexDesc->setObjectName(QString::fromUtf8("lineEditVertexDesc"));
        lineEditVertexDesc->setGeometry(QRect(80, 20, 81, 20));
        label_7 = new QLabel(groupBox_5);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 50, 51, 16));
        lineEditVertexPos = new QLineEdit(groupBox_5);
        lineEditVertexPos->setObjectName(QString::fromUtf8("lineEditVertexPos"));
        lineEditVertexPos->setGeometry(QRect(80, 50, 81, 20));
        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 80, 51, 16));
        lineEditVertexNeighbors = new QLineEdit(groupBox_5);
        lineEditVertexNeighbors->setObjectName(QString::fromUtf8("lineEditVertexNeighbors"));
        lineEditVertexNeighbors->setGeometry(QRect(80, 80, 81, 20));
        groupBox_6 = new QGroupBox(dockWidgetContents);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(10, 420, 171, 61));
        pushButtonReduce = new QPushButton(groupBox_6);
        pushButtonReduce->setObjectName(QString::fromUtf8("pushButtonReduce"));
        pushButtonReduce->setGeometry(QRect(10, 20, 151, 31));
        ControlWidget->setWidget(dockWidgetContents);

        retranslateUi(ControlWidget);

        QMetaObject::connectSlotsByName(ControlWidget);
    } // setupUi

    void retranslateUi(QDockWidget *ControlWidget)
    {
        groupBox->setTitle(QApplication::translate("ControlWidget", "Edge", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ControlWidget", "Type", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ControlWidget", "Lanes", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ControlWidget", "One Way", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("ControlWidget", "Simplify", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ControlWidget", "Threshold", 0, QApplication::UnicodeUTF8));
        pushButtonSimplify->setText(QApplication::translate("ControlWidget", "Simplify", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("ControlWidget", "Remove Short Dead-End", 0, QApplication::UnicodeUTF8));
        pushButtonRemoveShortDeadend->setText(QApplication::translate("ControlWidget", "Remove Short Dead-End", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ControlWidget", "Threshold", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("ControlWidget", "Clean", 0, QApplication::UnicodeUTF8));
        pushButtonClean->setText(QApplication::translate("ControlWidget", "Clean", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("ControlWidget", "Vertex", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ControlWidget", "ID", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ControlWidget", "Location", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("ControlWidget", "Neighbors", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("ControlWidget", "Reduce", 0, QApplication::UnicodeUTF8));
        pushButtonReduce->setText(QApplication::translate("ControlWidget", "Reduce", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ControlWidget);
    } // retranslateUi

};

namespace Ui {
    class ControlWidget: public Ui_ControlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLWIDGET_H
