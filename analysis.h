#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QWidget>

namespace Ui {
class Analysis;
}

class Analysis : public QWidget
{
    Q_OBJECT

public:
    explicit Analysis(QWidget *parent = nullptr);
    ~Analysis();

private:
    Ui::Analysis *ui;
    QString m_rawInput;
    int m_state;
    int m_errorState = 0;
    int m_commentState = 0;
    int m_iter;
    void errorOut(QString msg);
    void commentOut(QString comment);
    void tableOut(QString msg, QString _type, int row, int column);
    int row = 0;
    int column = 0;
    QString m_currentWord;
    bool m_inObject = false;
    void proverka(QChar bukva);

private slots:
    void buttPressed(void);
    void checkErrorChanged(int state);
    void on_checkBox_stateChanged(int arg1);
};

#endif // ANALYSIS_H
