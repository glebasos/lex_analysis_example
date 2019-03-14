#include "manager.h"
#include "analysis.h"
#include <QPushButton>
#include <QLabel>
#include <QListView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCheckBox>



Manager::Manager(QWidget *parent) : QWidget(parent)
{

}

QWidget *Manager::getWidgets(void) {

    QPushButton *procButton;
    QCheckBox *checkBoxComment;
    QCheckBox *checkBoxError;
    /* Компановка виджета */
    QVBoxLayout *combLayout = new QVBoxLayout();

    QWidget *combWidget = new QWidget();

    Analysis *anal = new Analysis();

    combLayout->addWidget(anal);
    combWidget->setLayout(combLayout);

    procButton = anal->findChild<QPushButton*>("processButton");
    checkBoxComment = anal->findChild<QCheckBox*>("checkBox");
    checkBoxError = anal->findChild<QCheckBox*>("checkBox_2");
    connect(procButton, SIGNAL(released()), anal, SLOT(buttPressed()));
    connect(checkBoxComment, SIGNAL(stateChanged(int)), anal, SLOT(on_checkBox_stateChanged(int)));
    connect(checkBoxError, SIGNAL(stateChanged(int)), anal, SLOT(checkErrorChanged(int)));


    return combWidget;
}
