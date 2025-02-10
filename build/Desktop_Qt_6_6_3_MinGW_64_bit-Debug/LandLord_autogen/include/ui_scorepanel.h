/********************************************************************************
** Form generated from reading UI file 'scorepanel.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCOREPANEL_H
#define UI_SCOREPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScorePanel
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *myScore;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *leftScore;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *rightScore;
    QLabel *label_9;

    void setupUi(QWidget *ScorePanel)
    {
        if (ScorePanel->objectName().isEmpty())
            ScorePanel->setObjectName("ScorePanel");
        ScorePanel->resize(269, 102);
        gridLayout = new QGridLayout(ScorePanel);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(ScorePanel);
        label->setObjectName("label");
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221")});
        font.setPointSize(12);
        font.setBold(true);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        myScore = new QLabel(ScorePanel);
        myScore->setObjectName("myScore");
        myScore->setFont(font);
        myScore->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        myScore->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(myScore, 0, 1, 1, 1);

        label_3 = new QLabel(ScorePanel);
        label_3->setObjectName("label_3");
        label_3->setFont(font);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        label_4 = new QLabel(ScorePanel);
        label_4->setObjectName("label_4");
        label_4->setFont(font);
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_4->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        leftScore = new QLabel(ScorePanel);
        leftScore->setObjectName("leftScore");
        leftScore->setFont(font);
        leftScore->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        leftScore->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(leftScore, 1, 1, 1, 1);

        label_6 = new QLabel(ScorePanel);
        label_6->setObjectName("label_6");
        label_6->setFont(font);
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_6->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_6, 1, 2, 1, 1);

        label_7 = new QLabel(ScorePanel);
        label_7->setObjectName("label_7");
        label_7->setFont(font);
        label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_7->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_7, 2, 0, 1, 1);

        rightScore = new QLabel(ScorePanel);
        rightScore->setObjectName("rightScore");
        rightScore->setFont(font);
        rightScore->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        rightScore->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(rightScore, 2, 1, 1, 1);

        label_9 = new QLabel(ScorePanel);
        label_9->setObjectName("label_9");
        label_9->setFont(font);
        label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_9->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_9, 2, 2, 1, 1);


        retranslateUi(ScorePanel);

        QMetaObject::connectSlotsByName(ScorePanel);
    } // setupUi

    void retranslateUi(QWidget *ScorePanel)
    {
        ScorePanel->setWindowTitle(QCoreApplication::translate("ScorePanel", "Form", nullptr));
        label->setText(QCoreApplication::translate("ScorePanel", "\346\210\221", nullptr));
        myScore->setText(QString());
        label_3->setText(QCoreApplication::translate("ScorePanel", "\345\210\206", nullptr));
        label_4->setText(QCoreApplication::translate("ScorePanel", "\345\267\246\344\276\247\344\272\272\346\234\272", nullptr));
        leftScore->setText(QString());
        label_6->setText(QCoreApplication::translate("ScorePanel", "\345\210\206", nullptr));
        label_7->setText(QCoreApplication::translate("ScorePanel", "\345\217\263\344\276\247\344\272\272\346\234\272", nullptr));
        rightScore->setText(QString());
        label_9->setText(QCoreApplication::translate("ScorePanel", "\345\210\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScorePanel: public Ui_ScorePanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCOREPANEL_H
