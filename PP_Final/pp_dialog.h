#ifndef PP_DIALOG_H
#define PP_DIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QString>
#include <QList>

class PP_Dialog : public QDialog
{
public:
    PP_Dialog(QWidget *parent=NULL);
    ~PP_Dialog();
    int exec(QString title, QStringList radioText);

private:
    void setGroupboxTitle(QString title);

private:
    QGridLayout *layout;
    QPushButton *btn_save;
    QPushButton *btn_cancel;
    QGroupBox *groupbox;
    QList<QRadioButton*> radioBtnList;
    QGridLayout *groupboxLayout;
};

#endif // PP_DIALOG_H
