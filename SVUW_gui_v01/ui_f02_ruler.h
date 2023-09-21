/********************************************************************************
** Form generated from reading UI file 'f02_ruler.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_F02_RULER_H
#define UI_F02_RULER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_f02_Ruler
{
public:
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView_RulerFame;
    QTableWidget *tableWidget_RulerPoints;
    QGridLayout *gridLayout_4;
    QPushButton *pushButton_Stop;
    QPushButton *pushButton_Reload;

    void setupUi(QWidget *f02_Ruler)
    {
        if (f02_Ruler->objectName().isEmpty())
            f02_Ruler->setObjectName(QString::fromUtf8("f02_Ruler"));
        f02_Ruler->resize(806, 621);
        gridLayout_3 = new QGridLayout(f02_Ruler);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
        graphicsView_RulerFame = new QGraphicsView(f02_Ruler);
        graphicsView_RulerFame->setObjectName(QString::fromUtf8("graphicsView_RulerFame"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView_RulerFame->sizePolicy().hasHeightForWidth());
        graphicsView_RulerFame->setSizePolicy(sizePolicy);
        graphicsView_RulerFame->setAlignment(Qt::AlignCenter);
        graphicsView_RulerFame->setDragMode(QGraphicsView::NoDrag);

        gridLayout->addWidget(graphicsView_RulerFame, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 2);

        tableWidget_RulerPoints = new QTableWidget(f02_Ruler);
        if (tableWidget_RulerPoints->columnCount() < 5)
            tableWidget_RulerPoints->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_RulerPoints->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_RulerPoints->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_RulerPoints->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_RulerPoints->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_RulerPoints->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        if (tableWidget_RulerPoints->rowCount() < 2)
            tableWidget_RulerPoints->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_RulerPoints->setVerticalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_RulerPoints->setVerticalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_RulerPoints->setItem(0, 0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_RulerPoints->setItem(0, 1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_RulerPoints->setItem(0, 2, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_RulerPoints->setItem(0, 3, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_RulerPoints->setItem(1, 0, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget_RulerPoints->setItem(1, 1, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_RulerPoints->setItem(1, 2, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_RulerPoints->setItem(1, 3, __qtablewidgetitem14);
        tableWidget_RulerPoints->setObjectName(QString::fromUtf8("tableWidget_RulerPoints"));
        tableWidget_RulerPoints->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableWidget_RulerPoints->sizePolicy().hasHeightForWidth());
        tableWidget_RulerPoints->setSizePolicy(sizePolicy1);
        tableWidget_RulerPoints->setMinimumSize(QSize(0, 0));
        tableWidget_RulerPoints->setMaximumSize(QSize(400, 90));
        tableWidget_RulerPoints->setAutoFillBackground(false);
        tableWidget_RulerPoints->setAutoScroll(true);
        tableWidget_RulerPoints->setDragEnabled(false);
        tableWidget_RulerPoints->setAlternatingRowColors(false);
        tableWidget_RulerPoints->setShowGrid(true);
        tableWidget_RulerPoints->setSortingEnabled(false);
        tableWidget_RulerPoints->setCornerButtonEnabled(true);
        tableWidget_RulerPoints->horizontalHeader()->setVisible(true);
        tableWidget_RulerPoints->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget_RulerPoints->horizontalHeader()->setMinimumSectionSize(25);
        tableWidget_RulerPoints->horizontalHeader()->setDefaultSectionSize(71);
        tableWidget_RulerPoints->horizontalHeader()->setHighlightSections(true);
        tableWidget_RulerPoints->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget_RulerPoints->horizontalHeader()->setStretchLastSection(false);
        tableWidget_RulerPoints->verticalHeader()->setVisible(true);
        tableWidget_RulerPoints->verticalHeader()->setCascadingSectionResizes(true);
        tableWidget_RulerPoints->verticalHeader()->setMinimumSectionSize(15);
        tableWidget_RulerPoints->verticalHeader()->setDefaultSectionSize(30);
        tableWidget_RulerPoints->verticalHeader()->setHighlightSections(true);
        tableWidget_RulerPoints->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget_RulerPoints->verticalHeader()->setStretchLastSection(false);

        gridLayout_2->addWidget(tableWidget_RulerPoints, 4, 1, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        pushButton_Stop = new QPushButton(f02_Ruler);
        pushButton_Stop->setObjectName(QString::fromUtf8("pushButton_Stop"));

        gridLayout_4->addWidget(pushButton_Stop, 1, 0, 1, 1);

        pushButton_Reload = new QPushButton(f02_Ruler);
        pushButton_Reload->setObjectName(QString::fromUtf8("pushButton_Reload"));

        gridLayout_4->addWidget(pushButton_Reload, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout_4, 4, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);


        retranslateUi(f02_Ruler);

        QMetaObject::connectSlotsByName(f02_Ruler);
    } // setupUi

    void retranslateUi(QWidget *f02_Ruler)
    {
        f02_Ruler->setWindowTitle(QCoreApplication::translate("f02_Ruler", "DockWidget", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_RulerPoints->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("f02_Ruler", "x", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_RulerPoints->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("f02_Ruler", "y", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_RulerPoints->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("f02_Ruler", "z", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_RulerPoints->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("f02_Ruler", "\321\200\320\260\321\201\321\201\321\202\320\276\321\217\320\275\320\270\320\265", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_RulerPoints->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("f02_Ruler", "mouse xy", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_RulerPoints->verticalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("f02_Ruler", "01", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_RulerPoints->verticalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("f02_Ruler", "02", nullptr));

        const bool __sortingEnabled = tableWidget_RulerPoints->isSortingEnabled();
        tableWidget_RulerPoints->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_RulerPoints->item(0, 0);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("f02_Ruler", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_RulerPoints->item(0, 1);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("f02_Ruler", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_RulerPoints->item(0, 2);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("f02_Ruler", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_RulerPoints->item(0, 3);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("f02_Ruler", "-", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_RulerPoints->item(1, 0);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("f02_Ruler", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget_RulerPoints->item(1, 1);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("f02_Ruler", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget_RulerPoints->item(1, 2);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("f02_Ruler", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget_RulerPoints->item(1, 3);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("f02_Ruler", "0", nullptr));
        tableWidget_RulerPoints->setSortingEnabled(__sortingEnabled);

#if QT_CONFIG(statustip)
        tableWidget_RulerPoints->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        tableWidget_RulerPoints->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(accessibility)
        tableWidget_RulerPoints->setAccessibleName(QString());
#endif // QT_CONFIG(accessibility)
#if QT_CONFIG(accessibility)
        tableWidget_RulerPoints->setAccessibleDescription(QString());
#endif // QT_CONFIG(accessibility)
        pushButton_Stop->setText(QCoreApplication::translate("f02_Ruler", "Stop", nullptr));
        pushButton_Reload->setText(QCoreApplication::translate("f02_Ruler", "Reload", nullptr));
    } // retranslateUi

};

namespace Ui {
    class f02_Ruler: public Ui_f02_Ruler {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_F02_RULER_H
