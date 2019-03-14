#include "analysis.h"
#include "ui_analysis.h"
#include <QDebug>
#include <QMessageBox>

Analysis::Analysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Analysis)
{
    ui->setupUi(this);
    //m_TableHeader<<"#"<<"Name"<<"Text";
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Идентификатор") );
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("Тип") );
}

Analysis::~Analysis()
{
    delete ui;
}

void Analysis::errorOut(QString msg)
{
    ui->label->setText("Обнаружены ошибки");
    ui->label->setStyleSheet("QLabel { background-color : red; color : black; }");
    if( m_errorState == 2){
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row, column, new QTableWidgetItem(msg));
    ui->tableWidget->setItem(row, column+1, new QTableWidgetItem("Ошибочная лексема") );
    this->row++;
    }
    m_currentWord.clear();
    m_inObject=false;
}

void Analysis::commentOut(QString comment)
{

    if( m_commentState == 2){
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row, column, new QTableWidgetItem(comment));
    ui->tableWidget->setItem(row, column+1, new QTableWidgetItem("Комментарий") );
    this->row++;
    }
    m_currentWord.clear();
    m_inObject=false;
}

void Analysis::tableOut(QString msg, QString _type, int row, int column)
{
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row, column, new QTableWidgetItem(msg));
    ui->tableWidget->setItem(row, column+1, new QTableWidgetItem(_type) );
    this->row++;
    m_currentWord.clear();
    m_inObject=false;
}

void Analysis::proverka(QChar bukva)
{
    if(bukva=="("){
        m_currentWord.append(bukva);
        tableOut(m_currentWord, "Открывающая скобка", row, column);
         m_inObject = false;
    }
    else if (bukva == ")"){
        m_currentWord.append(bukva);
        tableOut(m_currentWord, "Закрывающая скобка", row, column);
         m_inObject = false;
    }
    else if(bukva == "*" || bukva == "/" || bukva == "+" || bukva == "-"){
        m_currentWord.append(bukva);
        tableOut(m_currentWord, "Знак операции", row, column);
         m_inObject = false;
    }
    else if(bukva == "_" || isalpha(bukva.unicode())){
        m_currentWord.append(bukva);
        m_state = 0;
        m_inObject = true;
    }
    else if (bukva==":") {
        m_currentWord.append(bukva);
        m_state = 2;
        m_inObject = true;
    }
    else if(bukva=="#"){
        m_state = 3;
        m_inObject = true;
    }
    else if(bukva=="'"){
        m_currentWord.append(bukva);
        m_state = 1;
        m_inObject = true;
    }
    else if (bukva != ";" && bukva != " "){
        errorOut(bukva);
        m_inObject = false;
    }
}



//    идентификатор _бц
//    'C' 'c'
//    *=-/
//    (
//    )
//    :=
// #comments#


void Analysis::buttPressed()
{
//    ui->tableWidget->clear();
//    ui->tableWidget->clearContents();
    ui->label->setText("Ошибки не обнаружены");
    ui->label->setStyleSheet("QLabel { background-color : green; color : black; }");
    ui->tableWidget->setRowCount(0);
    this->row = 0;
    qDebug() << "slot is ok";
    m_rawInput = this->ui->inputEdit->text();
    qDebug() << m_rawInput;
    QChar c;

     //0 - id; 1 - const; 2 - :=; 3 - +-*/; 4 ();
    m_inObject = false;

    for (int i = 0; i < m_rawInput.size(); i++){
        c = m_rawInput[i];
        m_iter = i;
        qDebug() << c;

        //.........................................первое вхождение.........................................
        if(!m_inObject){
            if(c == "_" || isalpha(c.unicode())){
                    m_currentWord.append(c);
                    m_state = 0;
                    m_inObject = true;
            }
            else if(c=="'"){
                    m_currentWord.append(c);
                    m_state = 1;
                    m_inObject = true;
            }
            else if (c == ":")
            {
                    m_currentWord.append(c);
                    m_state = 2;
                    m_inObject = true;
            }
            else if(c == "+" || c == "-" || c == "*" || c == "/"){
                m_currentWord.append(c);
                tableOut(m_currentWord, "Знак операции", row, column);
            }
            else if (c == "(") {
                m_currentWord.append(c);
                tableOut(m_currentWord, "Открывающая скобка", row, column);
            }
            else if (c == ")") {
                m_currentWord.append(c);
                tableOut(m_currentWord, "Закрывающая скобка", row, column);
            }
            else if(c=="#"){
                qDebug()<<"comment";
                m_state = 3;
                m_inObject = true;
            }
            else if (c!=";" && c!=" ") {
                qDebug() << "Im here";
                qDebug() << i;
                while(m_rawInput[i] !=" " || m_rawInput[i]!=";" || m_rawInput[i]!="(" || m_rawInput[i]!=")"
                      || m_rawInput[i]!="*" || m_rawInput[i]!="/" || m_rawInput[i]!="+" || m_rawInput[i]!="-" || m_rawInput[i]!="#"){
                    if(m_rawInput[i] ==" " || m_rawInput[i]==";" || m_rawInput[i]=="(" || m_rawInput[i]==")"
                            || m_rawInput[i]=="*" || m_rawInput[i]=="/" || m_rawInput[i]=="+" || m_rawInput[i]=="-" || m_rawInput[i]=="#"){
                        break;
                    }
                    qDebug() << "error" << m_rawInput[i];
                    m_currentWord.append(m_rawInput[i]);
                    i++;
                    m_iter = i;
                }
                qDebug() << "Error error: m_cur is  " << m_currentWord;
                errorOut(m_currentWord);
                proverka(m_rawInput[i]);

            }

        }
        else {
            switch (m_state) {
            case 0:
                while(m_rawInput[i]=="_" || isalpha(m_rawInput[i].unicode()) || isdigit(m_rawInput[i].unicode()))
                {
                    m_currentWord.append(m_rawInput[i]);
                    i++;
                }
                tableOut(m_currentWord, "Идентификатор", row, column);
                proverka(m_rawInput[i]);
                break;
            case 1:
                if(isalpha(c.unicode())){  //................................
                    m_currentWord.append(c);
                    i++;
                    if(m_rawInput[i]=="'"){
                        m_currentWord.append(m_rawInput[i]);
                        tableOut(m_currentWord, "Константа", row, column);
                    }
                    else{
                        while(m_rawInput[i] !=" " || m_rawInput[i]!=";" || m_rawInput[i]!="(" || m_rawInput[i]!=")"
                              || m_rawInput[i]!="*" || m_rawInput[i]!="/" || m_rawInput[i]!="+" || m_rawInput[i]!="-" || m_rawInput[i]!="#"){
                            if(m_rawInput[i] ==" " || m_rawInput[i]==";" || m_rawInput[i]=="(" || m_rawInput[i]==")"
                                    || m_rawInput[i]=="*" || m_rawInput[i]=="/" || m_rawInput[i]=="+" || m_rawInput[i]=="-" || m_rawInput[i]=="#"){
                                break;
                            }
                            qDebug() << m_rawInput[i];
                            m_currentWord.append(m_rawInput[i]);
                            i++;
                            m_iter = i;
                        }
                        errorOut(m_currentWord);
                        proverka(m_rawInput[i]);
                    }

                } //.....................................
                else{
                    while(m_rawInput[i] !=" " || m_rawInput[i]!=";" || m_rawInput[i]!="(" || m_rawInput[i]!=")"
                          || m_rawInput[i]!="*" || m_rawInput[i]!="/" || m_rawInput[i]!="+" || m_rawInput[i]!="-" || m_rawInput[i]!="#"){
                        if(m_rawInput[i] ==" " || m_rawInput[i]==";" || m_rawInput[i]=="(" || m_rawInput[i]==")"
                                || m_rawInput[i]=="*" || m_rawInput[i]=="/" || m_rawInput[i]=="+" || m_rawInput[i]=="-" || m_rawInput[i]=="#"){
                            break;
                        }
                        qDebug() << m_rawInput[i];
                        m_currentWord.append(m_rawInput[i]);
                        i++;
                        m_iter = i;
                    }
                    errorOut(m_currentWord);
                    proverka(m_rawInput[i]); //xxxxxxxxxxxxxxxxxxxx
                }
                break;
            case 2:
                if(c=="="){
                    m_currentWord.append(c);
                    tableOut(m_currentWord, "Присваивание", row, column);
                }
                else {
                    while(m_rawInput[i] !=" " || m_rawInput[i]!=";" || m_rawInput[i]!="(" || m_rawInput[i]!=")"
                          || m_rawInput[i]!="*" || m_rawInput[i]!="/" || m_rawInput[i]!="+" || m_rawInput[i]!="-" || m_rawInput[i]!="#"){
                        if(m_rawInput[i] ==" " || m_rawInput[i]==";" || m_rawInput[i]=="(" || m_rawInput[i]==")"
                                || m_rawInput[i]=="*" || m_rawInput[i]=="/" || m_rawInput[i]=="+" || m_rawInput[i]=="-" || m_rawInput[i]=="#"){
                            break;
                        }
                        qDebug() << m_rawInput[i];
                        m_currentWord.append(m_rawInput[i]);
                        i++;
                        m_iter = i;
                    }
                    errorOut(m_currentWord);
                    proverka(m_rawInput[i]);
                }
                break;
             case 3:
                qDebug() << "case 3" << m_rawInput[i];
                while(m_rawInput[i]!="#")
                {
                    m_currentWord.append(m_rawInput[i]);
                    i++;
                }
                commentOut(m_currentWord);
                proverka(m_rawInput[i]);
                m_inObject = false;
                break;
            }
        }

}
}


void Analysis::checkErrorChanged(int state)
{
    qDebug() << state;
    m_errorState = state;
}



void Analysis::on_checkBox_stateChanged(int arg1)
{
    m_commentState = arg1;
}
