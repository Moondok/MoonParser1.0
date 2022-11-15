#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QDebug>
#include "rule_page.h"
#include "functional_page.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLabel label;
    QPushButton* startBtn=nullptr;
    QPushButton*exitBtn=nullptr;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
