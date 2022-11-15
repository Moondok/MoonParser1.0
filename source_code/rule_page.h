#ifndef RULE_PAGE_H
#define RULE_PAGE_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include "util.h"
#include <QPushButton>
#include "functional_page.h"
#include <QToolBar>

class rule_page : public QMainWindow
{
    Q_OBJECT
public:
    explicit rule_page(QWidget *parent = nullptr);
private:
    QLabel * label= nullptr;
    QPushButton * continueBtn=nullptr;
    QPushButton * exitBtn=nullptr;




signals:
    void return_signal();


};

#endif // RULE_PAGE_H
