/********************************************************************************
** Form generated from reading UI file 'toolwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLWINDOW_H
#define UI_TOOLWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ToolWindow
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QVBoxLayout *verticalLayout_2;
    QListWidget *lswClusters;
    QLabel *labelInfo;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn2D;
    QPushButton *btnDelete;
    QPushButton *btn3D;
    QPushButton *btnSave;
    QLineEdit *lineEditInfo;

    void setupUi(QDialog *ToolWindow)
    {
        if (ToolWindow->objectName().isEmpty())
            ToolWindow->setObjectName(QString::fromUtf8("ToolWindow"));
        ToolWindow->setWindowModality(Qt::ApplicationModal);
        ToolWindow->resize(868, 530);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ToolWindow->sizePolicy().hasHeightForWidth());
        ToolWindow->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(ToolWindow);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        graphicsView = new QGraphicsView(ToolWindow);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setMinimumSize(QSize(640, 480));
        graphicsView->setMaximumSize(QSize(640, 480));
        graphicsView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::CrossCursor)));
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout->addWidget(graphicsView);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lswClusters = new QListWidget(ToolWindow);
        lswClusters->setObjectName(QString::fromUtf8("lswClusters"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lswClusters->sizePolicy().hasHeightForWidth());
        lswClusters->setSizePolicy(sizePolicy1);
        lswClusters->setMinimumSize(QSize(200, 0));
        lswClusters->setMaximumSize(QSize(150, 16777215));

        verticalLayout_2->addWidget(lswClusters);

        labelInfo = new QLabel(ToolWindow);
        labelInfo->setObjectName(QString::fromUtf8("labelInfo"));
        QFont font;
        font.setPointSize(12);
        labelInfo->setFont(font);
        labelInfo->setFrameShape(QFrame::NoFrame);
        labelInfo->setLineWidth(1);
        labelInfo->setTextFormat(Qt::AutoText);
        labelInfo->setWordWrap(true);

        verticalLayout_2->addWidget(labelInfo);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btn2D = new QPushButton(ToolWindow);
        btn2D->setObjectName(QString::fromUtf8("btn2D"));
        sizePolicy.setHeightForWidth(btn2D->sizePolicy().hasHeightForWidth());
        btn2D->setSizePolicy(sizePolicy);
        btn2D->setMinimumSize(QSize(60, 60));
        btn2D->setMaximumSize(QSize(150, 60));

        horizontalLayout->addWidget(btn2D);

        btnDelete = new QPushButton(ToolWindow);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));
        btnDelete->setMinimumSize(QSize(32, 32));
        btnDelete->setMaximumSize(QSize(32, 32));

        horizontalLayout->addWidget(btnDelete);

        btn3D = new QPushButton(ToolWindow);
        btn3D->setObjectName(QString::fromUtf8("btn3D"));
        sizePolicy.setHeightForWidth(btn3D->sizePolicy().hasHeightForWidth());
        btn3D->setSizePolicy(sizePolicy);
        btn3D->setMinimumSize(QSize(60, 60));
        btn3D->setMaximumSize(QSize(150, 60));

        horizontalLayout->addWidget(btn3D);


        verticalLayout_2->addLayout(horizontalLayout);

        btnSave = new QPushButton(ToolWindow);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        sizePolicy.setHeightForWidth(btnSave->sizePolicy().hasHeightForWidth());
        btnSave->setSizePolicy(sizePolicy);
        btnSave->setMinimumSize(QSize(200, 40));
        btnSave->setMaximumSize(QSize(40, 16777215));

        verticalLayout_2->addWidget(btnSave);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 1, 1);

        lineEditInfo = new QLineEdit(ToolWindow);
        lineEditInfo->setObjectName(QString::fromUtf8("lineEditInfo"));
        lineEditInfo->setCursor(QCursor(Qt::ArrowCursor));
        lineEditInfo->setReadOnly(true);

        gridLayout->addWidget(lineEditInfo, 1, 0, 1, 2);


        retranslateUi(ToolWindow);

        QMetaObject::connectSlotsByName(ToolWindow);
    } // setupUi

    void retranslateUi(QDialog *ToolWindow)
    {
        ToolWindow->setWindowTitle(QCoreApplication::translate("ToolWindow", "Tool Window", nullptr));
        labelInfo->setText(QCoreApplication::translate("ToolWindow", "INFO", nullptr));
        btn2D->setText(QString());
        btnDelete->setText(QCoreApplication::translate("ToolWindow", "D", nullptr));
        btn3D->setText(QString());
        btnSave->setText(QCoreApplication::translate("ToolWindow", "SAVE", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ToolWindow: public Ui_ToolWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLWINDOW_H
