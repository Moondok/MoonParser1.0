#ifndef FUNCTIONAL_PAGE_H
#define FUNCTIONAL_PAGE_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QTextBrowser>
#include <QLayout>
#include <QGridLayout>
#include <QLabel>
#include "util.h"
#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QToolBar>
#include <string>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include "LexicalAnalyzer.h"
#include "common.h"
#include "parser.h"

class functional_page : public QMainWindow
{
    Q_OBJECT
public:
    explicit functional_page(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *watched, QEvent *event);
private:
    QHBoxLayout * layout=nullptr;


    QTextEdit * textEdit=nullptr;
    QPushButton* choose_file_btn=nullptr;  //click this to make the file certain
    QPushButton* lex_btn=nullptr;
    QPushButton* syn_btn=nullptr;
    QPushButton* open_syn_btn=nullptr;
    QPushButton* reset_btn=nullptr;

    QTextBrowser* text_browser=nullptr;

    QWidget * painting_borad=nullptr;

    QLabel  * right_title=nullptr;
    QLabel  * left_title=nullptr;

    QWidget left_panel;

    QLabel * output_title=nullptr;
    QTextBrowser * output_browser=nullptr;

    bool is_file=0;
    bool is_lex=0;
    bool begin_draw_syntax=0;
    QString file_name;
    std::string first_token="";

    std::vector<int> id_stack;
    std::vector<std::string> content_stack;

    void draw_ui();

    void set_btns();

    int lexical_analyse();

    int syntax_analyse();

    void draw_syntax_analyse();

    void show_paint();
signals:
    void return_signal();

};

#endif // FUNCTIONAL_PAGE_H
