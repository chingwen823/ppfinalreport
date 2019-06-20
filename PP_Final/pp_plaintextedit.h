#ifndef PP_PLAINTEXTEDIT_H
#define PP_PLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QMouseEvent>
#include <QString>
#include <QStringList>
#include <QList>
#include "pp_dialog.h"

struct possibleTypo{
    int start;
    int end;
    QString orgText;
    QStringList candidate;
};

class PP_PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    PP_PlainTextEdit(QWidget *parent=NULL);
    ~PP_PlainTextEdit();
    void loadFile(QString fileName);
signals:
    void openDialog(int ptListIdx);

protected:
    void mousePressEvent(QMouseEvent *e);

private slots:
    void showDialog(int ptListIdx);

private:
    void correctWord(int ptListIdx, int candidateIdx);
    QString convertAnglebracket(const QString &htmlStr);

private:
    PP_Dialog *dialog;
    QList<struct possibleTypo> possibleTypoList;

};

#endif // PP_PLAINTEXTEDIT_H
