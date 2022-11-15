#include "rule_page.h"

rule_page::rule_page(QWidget *parent) : QMainWindow(parent)
{
    QToolBar *toolbar=new QToolBar(this);
    addToolBar(toolbar);
    toolbar->setMovable(false);
    QAction * return_action=new QAction("返回");
    QAction * exit_action=new QAction("退出");

    toolbar->addAction(return_action);
    toolbar->addSeparator();
    toolbar->addAction(exit_action);

    connect(return_action,&QAction::triggered,this,&rule_page::return_signal);
    connect(exit_action,&QAction::triggered,this,&rule_page::close);


    const QString rule="                              说明\n\n 1.MoonParser语法分析器基于指定文法构造，文法文件为grammar.syn\n\n 2.分析过程中的FIRST集合，GOTO表将保存在程序的同一级目录下 \n\n 3.只有在词法分析之后才可调用语法分析";

    this->setFixedSize(WIDTH,HEIGHT);

    label=new QLabel(rule,this);
    label->setFont(QFont("楷体",15,15));
    label->setFixedSize(1000,600);
    label->move(100,50);


    continueBtn=new QPushButton("CONTINUE",this);
    connect(continueBtn,&QPushButton::clicked,this,[=](){
        functional_page* func_page=new functional_page;
        func_page->show();
        this->hide();
        connect(func_page,&functional_page::return_signal,this,[=](){

            func_page->hide();
            this->show();
        });
    });



    exitBtn=new QPushButton("EXIT",this);
    connect(exitBtn,&QPushButton::clicked,this,&QMainWindow::close);

    continueBtn->setFixedSize(150,75);
    exitBtn->setFixedSize(150,75);

    continueBtn->move(300,600);
    exitBtn->move(750,600);
}
