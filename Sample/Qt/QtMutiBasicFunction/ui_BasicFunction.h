/********************************************************************************** Form generated from reading UI file 'BasicFunction.ui'**** Created by: Qt User Interface Compiler version 5.7.0**** WARNING! All changes made in this file will be lost when recompiling UI file!********************************************************************************/#ifndef UI_BASICFUNCTION_H#define UI_BASICFUNCTION_H#include <QtCore/QVariant>#include <QtWidgets/QAction>#include <QtWidgets/QApplication>#include <QtWidgets/QButtonGroup>#include <QtWidgets/QGraphicsView>#include <QtWidgets/QGridLayout>#include <QtWidgets/QGroupBox>#include <QtWidgets/QHBoxLayout>#include <QtWidgets/QHeaderView>#include <QtWidgets/QLabel>#include <QtWidgets/QPushButton>#include <QtWidgets/QScrollArea>#include <QtWidgets/QSpacerItem>#include <QtWidgets/QVBoxLayout>#include <QtWidgets/QWidget>#include <customcombobox.h>QT_BEGIN_NAMESPACEclass Ui_QBasicFunction{public:    QHBoxLayout *horizontalLayout;    QScrollArea *scrollArea;    QWidget *scrollAreaWidgetContents;    QVBoxLayout *verticalLayout;    QLabel *label_fps;    QGroupBox *General;    QGridLayout *gridLayout;    QPushButton *toolButton_Scan;    QPushButton *pushButton_Open;    QPushButton *pushButton_Start;    customComboBox *comboBox_Devices;    QSpacerItem *verticalSpacer;    QGridLayout *gridLayout_2;    QGraphicsView *graphicsView_PreView1;    QGraphicsView *graphicsView_PreView2;    QGraphicsView *graphicsView_PreView3;    QGraphicsView *graphicsView_PreView4;    void setupUi(QWidget *QBasicFunction)    {        if (QBasicFunction->objectName().isEmpty())            QBasicFunction->setObjectName(QStringLiteral("QBasicFunction"));        QBasicFunction->resize(1316, 787);        horizontalLayout = new QHBoxLayout(QBasicFunction);        horizontalLayout->setSpacing(2);        horizontalLayout->setContentsMargins(11, 11, 11, 11);        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));        horizontalLayout->setContentsMargins(1, 1, 1, 1);        scrollArea = new QScrollArea(QBasicFunction);        scrollArea->setObjectName(QStringLiteral("scrollArea"));        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);        sizePolicy.setHorizontalStretch(0);        sizePolicy.setVerticalStretch(0);        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());        scrollArea->setSizePolicy(sizePolicy);        scrollArea->setMinimumSize(QSize(500, 0));        scrollArea->setWidgetResizable(true);        scrollAreaWidgetContents = new QWidget();        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 498, 783));        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);        verticalLayout->setSpacing(6);        verticalLayout->setContentsMargins(11, 11, 11, 11);        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));        label_fps = new QLabel(scrollAreaWidgetContents);        label_fps->setObjectName(QStringLiteral("label_fps"));        verticalLayout->addWidget(label_fps);        General = new QGroupBox(scrollAreaWidgetContents);        General->setObjectName(QStringLiteral("General"));        gridLayout = new QGridLayout(General);        gridLayout->setSpacing(6);        gridLayout->setContentsMargins(11, 11, 11, 11);        gridLayout->setObjectName(QStringLiteral("gridLayout"));        toolButton_Scan = new QPushButton(General);        toolButton_Scan->setObjectName(QStringLiteral("toolButton_Scan"));        gridLayout->addWidget(toolButton_Scan, 0, 1, 1, 2);        pushButton_Open = new QPushButton(General);        pushButton_Open->setObjectName(QStringLiteral("pushButton_Open"));        gridLayout->addWidget(pushButton_Open, 2, 1, 1, 1);        pushButton_Start = new QPushButton(General);        pushButton_Start->setObjectName(QStringLiteral("pushButton_Start"));        gridLayout->addWidget(pushButton_Start, 2, 2, 1, 1);        comboBox_Devices = new customComboBox(General);        comboBox_Devices->setObjectName(QStringLiteral("comboBox_Devices"));        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);        sizePolicy1.setHorizontalStretch(0);        sizePolicy1.setVerticalStretch(0);        sizePolicy1.setHeightForWidth(comboBox_Devices->sizePolicy().hasHeightForWidth());        comboBox_Devices->setSizePolicy(sizePolicy1);        gridLayout->addWidget(comboBox_Devices, 1, 1, 1, 2);        verticalLayout->addWidget(General);        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);        verticalLayout->addItem(verticalSpacer);        scrollArea->setWidget(scrollAreaWidgetContents);        horizontalLayout->addWidget(scrollArea);        gridLayout_2 = new QGridLayout();        gridLayout_2->setSpacing(6);        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));        graphicsView_PreView1 = new QGraphicsView(QBasicFunction);        graphicsView_PreView1->setObjectName(QStringLiteral("graphicsView_PreView1"));        gridLayout_2->addWidget(graphicsView_PreView1, 0, 0, 1, 1);        graphicsView_PreView2 = new QGraphicsView(QBasicFunction);        graphicsView_PreView2->setObjectName(QStringLiteral("graphicsView_PreView2"));        gridLayout_2->addWidget(graphicsView_PreView2, 0, 1, 1, 1);        graphicsView_PreView3 = new QGraphicsView(QBasicFunction);        graphicsView_PreView3->setObjectName(QStringLiteral("graphicsView_PreView3"));        gridLayout_2->addWidget(graphicsView_PreView3, 1, 0, 1, 1);        graphicsView_PreView4 = new QGraphicsView(QBasicFunction);        graphicsView_PreView4->setObjectName(QStringLiteral("graphicsView_PreView4"));        gridLayout_2->addWidget(graphicsView_PreView4, 1, 1, 1, 1);        horizontalLayout->addLayout(gridLayout_2);        retranslateUi(QBasicFunction);        QMetaObject::connectSlotsByName(QBasicFunction);    } // setupUi    void retranslateUi(QWidget *QBasicFunction)    {        QBasicFunction->setWindowTitle(QApplication::translate("QBasicFunction", "QBasicFunction", 0));        label_fps->setText(QApplication::translate("QBasicFunction", "fps", 0));        General->setTitle(QApplication::translate("QBasicFunction", "General", 0));        toolButton_Scan->setText(QApplication::translate("QBasicFunction", "Scan", 0));        pushButton_Open->setText(QApplication::translate("QBasicFunction", "Open", 0));        pushButton_Start->setText(QApplication::translate("QBasicFunction", "Start", 0));    } // retranslateUi};namespace Ui {    class QBasicFunction: public Ui_QBasicFunction {};} // namespace UiQT_END_NAMESPACE#endif // UI_BASICFUNCTION_H