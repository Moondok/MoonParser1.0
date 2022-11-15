#include "functional_page.h"
#include <QTime>
#include <QCoreApplication>
#include <sstream>
#include <QTimer>
#include <QEventLoop>

functional_page::functional_page(QWidget *parent) : QMainWindow(parent)
{
    draw_ui();

    QToolBar *toolbar=new QToolBar(this);
    addToolBar(toolbar);
    toolbar->setMovable(false);
    QAction * return_action=new QAction("返回");
    QAction * exit_action=new QAction("退出");

    toolbar->addAction(return_action);
    toolbar->addSeparator();
    toolbar->addAction(exit_action);

    connect(return_action,&QAction::triggered,this,&functional_page::return_signal);
    connect(exit_action,&QAction::triggered,this,&functional_page::close);

    set_btns();

    painting_borad->installEventFilter(this);
}

void functional_page::paintEvent(QPaintEvent *)
{
    if(begin_draw_syntax)
    {

        text_browser->hide();
        painting_borad->show();


    }

       // painter.drawLine(10,70,10,1000);

        //painter.drawLine(810,70,810,1000);

        //painter.drawLine(1010,70,1010,1000);


}

void functional_page::draw_ui()
{
    QHBoxLayout * hlayout1=new QHBoxLayout;
    QHBoxLayout* hlayout2=new QHBoxLayout;
    QHBoxLayout * hlayout3=new QHBoxLayout;

    this->setFixedSize(WIDTH,HEIGHT);
    this->left_panel.setFixedSize(1180,740);
    this->left_panel.move(10,70);
    //this->setCentralWidget(&left_panel);

    left_title=new QLabel("控制面板");
    left_title->setFixedSize(130,40);
    left_title->setFont(QFont("隶书",15,15));

    right_title=new QLabel("演示区");
    right_title->setFixedSize(100,40);
    right_title->setFont(QFont("隶书",15,15));

    textEdit=new QTextEdit();
    textEdit->setFixedSize(330,60);
    choose_file_btn=new QPushButton("选择文件");
    choose_file_btn->setFixedHeight(60);
    hlayout1->addWidget(textEdit);
    hlayout1->addWidget(choose_file_btn);

    lex_btn=new QPushButton("词法分析");
    syn_btn=new QPushButton("语法分析");
    lex_btn->setFixedWidth(200);
    lex_btn->setFixedHeight(60);
    syn_btn->setFixedWidth(200);
    syn_btn->setFixedHeight(60);
    hlayout2->addWidget(lex_btn);
    hlayout2->addWidget(syn_btn);

    open_syn_btn=new QPushButton("查看语法");
    reset_btn=new QPushButton("重置");

    open_syn_btn->setFixedWidth(200);
    open_syn_btn->setFixedHeight(60);
    reset_btn->setFixedWidth(200);
    reset_btn->setFixedHeight(60);
    hlayout3->addWidget(open_syn_btn);
    hlayout3->addWidget(reset_btn);

    output_title=new QLabel("控制输出");
    output_title->setFixedSize(130,40);
    output_title->setFont(QFont("隶书",15,15));

    output_browser=new QTextBrowser();
    QVBoxLayout* leftLayout=new QVBoxLayout();
    leftLayout->addStretch();
    leftLayout->addWidget(left_title,1,Qt::AlignHCenter);

    leftLayout->addSpacing(70);
    leftLayout->addStretch();
    leftLayout->addLayout(hlayout1,1);
    leftLayout->addSpacing(70);
    leftLayout->addStretch();
    leftLayout->addLayout(hlayout2,1);
    leftLayout->addSpacing(70);
    leftLayout->addStretch();
    leftLayout->addLayout(hlayout3,1);
    leftLayout->addSpacing(70);
    leftLayout->addStretch();
    leftLayout->addWidget(output_title,1,Qt::AlignHCenter);

    leftLayout->addWidget(output_browser);

    text_browser=new QTextBrowser;

    painting_borad=new QWidget;

    QVBoxLayout * rightLayout= new QVBoxLayout();
    rightLayout->addWidget(right_title,3,Qt::AlignHCenter);
    rightLayout->addWidget(text_browser);
    rightLayout->addWidget(painting_borad);
    text_browser->hide();

    layout=new QHBoxLayout();

    layout->addLayout(leftLayout,2);

    layout->addLayout(rightLayout,3);
    layout->setSpacing(10);

    left_panel.setParent(this);
    this->left_panel.setLayout(layout);

    QPalette pal(this->painting_borad->palette());
    pal.setColor(QPalette::Background, QColor("#FFFAFA"));
    this->painting_borad->setAutoFillBackground(true);
    this->painting_borad->setPalette(pal);
}

void functional_page::set_btns()
{
    connect(choose_file_btn,&QPushButton::clicked,this,[=](){
        file_name=QFileDialog::getOpenFileName(this,"select file");
        textEdit->setText(file_name);
        QFile f(file_name);
        f.open(QIODevice::ReadOnly);
        QByteArray content=f.readAll();
        painting_borad->hide();
        text_browser->show();
        text_browser->setText(content);

        is_file=true;
        output_browser->setText("程序文件已经读入并显示");
    });

    connect(lex_btn,&QPushButton::clicked,this,[=](){
        if(!is_file)
           // QMessageBox::Warning(this,"错误","请先选择文件");
            QMessageBox::critical(this, "错误","请先选择文件");
        else
        {
           int re=lexical_analyse();
           if(re==RIGHT_STATUS)
           {
                is_lex=true;
                output_browser->setText("Lexical Analyse success.");
                QFile f("token_result.txt");
                f.open(QIODevice::ReadOnly);
                QByteArray content=f.readAll();
                painting_borad->hide();
                text_browser->show();
                text_browser->setText(content);
           }
           else
           {
                output_browser->setText("Lexical Analyse fail.");
           }

        }

    });

    connect(syn_btn,&QPushButton::clicked,this,[=](){
        if(is_lex==false)
            QMessageBox::critical(this, "错误","请先完成词法分析");
        else
        {
            syn_btn->setDisabled(true);
            open_syn_btn->setDisabled(true);
            choose_file_btn->setDisabled(true);
            lex_btn->setDisabled(true);
            reset_btn->setDisabled(true);
            int re=syntax_analyse();
            if(re==SYNTAX_SUCCESS)
            {
                output_browser->setText("syntax Analyse success. the holistic precedure will be shown on the right side.");
                draw_syntax_analyse();

            }
            else
            {
                QString str="syntax Analyse fail.the nearest error occurs in line "+QString::number(re)+" !";
                output_browser->setText(str);
            }
            syn_btn->setDisabled(false);
            open_syn_btn->setDisabled(false);
            choose_file_btn->setDisabled(false);
            lex_btn->setDisabled(false);
            reset_btn->setDisabled(false);
        }

    });
    connect(open_syn_btn,&QPushButton::clicked,this,[=](){
        QString name ="grammer.syn";
        QFile f(name);
        f.open(QIODevice::ReadOnly);
        QByteArray content=f.readAll();
        painting_borad->hide();
        text_browser->show();

        text_browser->clear();
        text_browser->setText(content);

        output_browser->setText("语法文件已显示");
    });
    connect(reset_btn,&QPushButton::clicked,this,[=](){
        text_browser->clear();
        output_browser->clear();
        is_file=false;
        is_lex=false;
        textEdit->clear();
    });
}

int functional_page::lexical_analyse()
{
    LexicalAnalyzer analyzer;

        //读
        std::string in_path =file_name.toStdString();//"./Wrong_Test1.txt";
        std::ifstream infile(in_path);
        if (!infile.is_open()) {
            cout << "can't open " << in_path << "\n";
            exit(-1);
        }

        //string in_code="include iostream int a=1;";
        std::stringstream in_stream;
        in_stream<<infile.rdbuf();
        infile.close();

        std::string in_code,out_code;
        in_code=in_stream.str();
        int sign;
        int pre_line = 1, pre_col = 0;
        sign=analyzer.analyze(in_code,out_code,pre_line,pre_col);
        //预处理错误
        if(sign==PreProcess_NOT){
            //cout<<"Preprocess ERROR\n";
            //cout << pre_line << ' ' << pre_col-1 << "\n";
            return PreProcess_NOT;
        }

        //cout <<out_code << "\n\n";
        //其他错误
        if(sign!=RIGHT_STATUS){
            //cout<<"error"<<"\n";
            return sign;
        }

        //正确
        {
            //cout<<"lexical analyze complete\n";
        }
        return sign;
}

int functional_page::syntax_analyse()
{
    Parser parser;
    parser.read_grammer(".\\grammer.syn");
    parser.get_all_symbol_first();
    parser.get_item_group_list();

    parser.get_LR1_table();
    parser.print_LR1_table();

    parser.print_DFA();

    //std::cout<<'\n'<<parser.check(".\token_result.txt");
    int re=parser.check("token_result.txt");
    return re;
}

void functional_page::draw_syntax_analyse()
{
    begin_draw_syntax=true;
    update();
    std::fstream stack_file;
    std::string tmp;
    int line=0;
    stack_file.open("stack.tmp",std::ios::in);
    while(stack_file.eof()==false)
    {

        getline(stack_file,tmp);
        if(tmp.size()==0)
            break;
        line++;
        std::stringstream sstr(tmp);
        char sign;
        sstr>>sign;


        if(sign=='p')
        {
            int i;
            std::string str;

            sstr>>i;
            sstr>>str;
            //std::cout<<line<<": "<<i<<" "<<str<<"\n";
            if(str.size()==0)
                std::cout<<"funckkkk!"<<std::endl;
            id_stack.emplace_back(i);
            content_stack.emplace_back(str);

        }
        else if(sign=='b')
        {
            id_stack.pop_back();
            content_stack.pop_back();
        }
        else if(sign=='@')
        {
            sstr>>first_token;
           // std::cout<<first_token<<"\n";
        }


        QTime time;
        time.start();
        while(time.elapsed() < 200)             //等待时间流逝5秒钟
            QCoreApplication::processEvents();   //处理事件
       painting_borad->update();

    }
    //getline(stack_file);
   // std::cout<<"final id size"<<id_stack.size()<<"\n";
   // std::cout<<"final content size"<<content_stack.size()<<"\n\n";
    //for(unsigned int i=0;i<id_stack.size();i++)
       // std::cout<<id_stack.at(i)<<" ";

    //std::cout<<"\n";
    //for(unsigned int i=0;i<content_stack.size();i++)
    //    std::cout<<i<<" \""<<content_stack.at(i)<<"\" ";

}

bool functional_page::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == painting_borad && event->type() == QEvent::Paint&& begin_draw_syntax==true)
    {
        QPainter painter(painting_borad);
        QPen pen;
        pen.setWidth(3);
        painter.setPen(pen);

        painter.drawLine(100,120,100,600);
        painter.drawLine(300,120,300,600);
        painter.drawLine(500,120,500,600);
        painter.drawLine(100,600,500,600);
        painter.drawText(QRectF(100,600,200,50),Qt::AlignHCenter|Qt::AlignVCenter,"state stack");
        painter.drawText(QRectF(300,600,200,50),Qt::AlignHCenter|Qt::AlignVCenter,"symbol stack");
        painter.drawText(QRectF(100,20,500,80),Qt::AlignLeft|Qt::AlignVCenter,"the first token in sequence is:"+QString::fromStdString(first_token));

        if(id_stack.size()>=9)
        {
            unsigned int index=id_stack.size();
            for(int i=0;i<9;i++)
            {

                painter.drawText(QRectF(100,550-50*i,200,50),Qt::AlignHCenter|Qt::AlignVCenter,QString::number(id_stack.at(index-9+i)));
            }
        }
        else
        {
            unsigned int index=id_stack.size();
            for(unsigned int i=0;i<9;i++)
            {

                if(i<index)
                    painter.drawText(QRectF(100,550-50*i,200,50),Qt::AlignHCenter|Qt::AlignVCenter,QString::number(id_stack.at(i)));
                else if(i>=index)
                    painter.drawText(QRectF(100,550-50*i,200,50),Qt::AlignHCenter|Qt::AlignVCenter,"                ");
            }
        }

        if(content_stack.size()>=9)
        {
            unsigned int index=content_stack.size();
            for(int i=0;i<9;i++)
            {

                painter.drawText(QRectF(300,550-50*i,200,50),Qt::AlignHCenter|Qt::AlignVCenter,QString::fromStdString(content_stack.at(index-9+i)));
            }
        }
        else
        {
            unsigned int index=content_stack.size();
            for(unsigned int i=0;i<9;i++)
            {
                 if(i<index)
                painter.drawText(QRectF(300,550-50*i,200,50),Qt::AlignHCenter|Qt::AlignVCenter,QString::fromStdString(content_stack.at(i)));
                 else
                     painter.drawText(QRectF(300,550-50*i,200,50),Qt::AlignHCenter|Qt::AlignVCenter,"             ");
            }
        }
    }

    return QWidget::eventFilter(watched,event);
}

