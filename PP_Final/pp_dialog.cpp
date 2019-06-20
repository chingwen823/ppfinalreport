#include <QtMath>
#include "pp_dialog.h"
#include "public.h"

PP_Dialog::PP_Dialog(QWidget *parent)
    : QDialog(parent)
{
    btn_save = new QPushButton("Save", this);
    connect(btn_save, SIGNAL(clicked(bool)), this, SLOT(accept()));

    btn_cancel = new QPushButton("Cancel", this);
    connect(btn_cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));

    groupbox = new QGroupBox(this);
    groupboxLayout = new QGridLayout(groupbox);
    groupbox->setLayout(groupboxLayout);

    layout = new QGridLayout(this);
    layout->addWidget(groupbox, 0, 0, 1, 2);
    layout->addWidget(btn_save, 1, 0);
    layout->addWidget(btn_cancel, 1, 1);

    setLayout(layout);
}

PP_Dialog::~PP_Dialog()
{
    FREE(groupbox);

    FREE(btn_save);
    FREE(btn_cancel);

    FREE(layout);

    while (radioBtnList.size()) {
        QRadioButton *btn = radioBtnList.takeFirst();
        FREE(btn);
    }
    FREE(groupboxLayout);
}

#define COLUME_PER_ROW 5
int PP_Dialog::exec(QString title, QStringList radioText)
{
    int column_per_row = COLUME_PER_ROW;
    if (radioText.count() > column_per_row*column_per_row) {
        column_per_row = qSqrt(radioText.count())+1;
    }

    groupbox->setTitle(title);

    QStringList::const_iterator ci;
    radioBtnList.clear();
    int row=0, column=0;
    for (ci=radioText.constBegin(); ci!=radioText.constEnd(); ++ci) {
        QRadioButton *btn = new QRadioButton(*ci, groupbox);
        radioBtnList.append(btn);
        ++row;
        if (row > column_per_row) {
            row %= column_per_row;
            ++column;
        }
        connect(btn, SIGNAL(clicked(bool)), btn_save, SLOT(setEnabled(bool)));
        groupboxLayout->addWidget(btn, row, column);
        qDebug() << (*ci);
    }
    btn_save->setEnabled(false);

    int ret = QDialog::exec();

    int radioNum=-1, count=0;
    while (radioBtnList.size()) {
        QRadioButton *btn = radioBtnList.takeFirst();
        if (btn->isChecked()) {
            radioNum = count;
        }
        FREE(btn);
        ++count;
    }

    if (ret != QDialog::Accepted) {
        radioNum = -1;
    }

    return radioNum;
}
