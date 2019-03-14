#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>

class Manager : public QWidget
{
    Q_OBJECT
public:
    explicit Manager(QWidget *parent = nullptr);

    QWidget *getWidgets(void);

signals:

public slots:
};

#endif // MANAGER_H
