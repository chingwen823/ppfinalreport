#include <QCoreApplication>
#include <QFile>
#include <QFont>
#include <QTime>
#include "mainwindow.h"
#include "findtypo.h"
#include "public.h"

#include <unistd.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    containerWidget = new QWidget(scrollArea);

    start_findtypo = new QPushButton("Start", containerWidget);
    start_findtypo->setFixedSize(120, 40);
    QObject::connect(start_findtypo, SIGNAL(clicked(bool)), this, SLOT(run_findtypo()));

    plainTextEdit = new PP_PlainTextEdit(containerWidget);

    layout = new QGridLayout(containerWidget);
    layout->addWidget(start_findtypo, 0, 0);
    layout->addWidget(plainTextEdit, 1, 0, 1, 10);


    containerWidget->setLayout(layout);
    scrollArea->setWidget(containerWidget);
    setCentralWidget(scrollArea);
    setMinimumSize(400, 300);
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    FREE(containerWidget);
    FREE(scrollArea);
    FREE(layout);
    FREE(plainTextEdit);

    FREE(start_findtypo);
}

void MainWindow::run_findtypo()
{
    start_findtypo->setEnabled(false);
    //start_findtypo->repaint();
    QCoreApplication::processEvents();

    FindTypo findtypo;

//#define TEST_FOR_PP
#ifdef TEST_FOR_PP
    QTime time;
    QStringList filenames= {"ch3_standard_utf8.csv",
                            "ch4_standard_utf8.csv",
                            "ch5_standard_utf8.csv",
                            "ch6_standard_utf8.csv",
                            "ch7_standard_utf8.csv",
                            "ch8_standard_utf8.csv",
                            "shortline_standard_utf8.csv"};
    QStringList::const_iterator ci;
    for (ci=filenames.constBegin(); ci != filenames.constEnd(); ++ci){
        qDebug("file name: %s", (*ci).toLocal8Bit().constData());
        for (int i=0; i<3; ++i){
            time.start();
            findtypo.run("../data/"+(*ci));
            qDebug("%2d, spending time: %5.3f s", i, time.elapsed()/1000.0);
            sleep(5);
        }
        qDebug();
    }
#else
    QTime time;
    time.start();
    findtypo.run(INPUT_FILE_PATH, PTHREAD_CNT);
    qDebug("spending time: %5.3f s", time.elapsed()/1000.0);
#endif

    plainTextEdit->loadFile(QString(OUTPUT_FILE_PATH));

    start_findtypo->setEnabled(true);
    return;
}
