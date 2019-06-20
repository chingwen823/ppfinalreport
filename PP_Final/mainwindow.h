#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include "pp_plaintextedit.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
private slots:
    void run_findtypo();

private:
    QWidget *containerWidget;
    QScrollArea *scrollArea;
    QGridLayout *layout;
    PP_PlainTextEdit *plainTextEdit;
    QPushButton *start_findtypo;
};

#endif // MAINWINDOW_H
