#include <QTextCursor>
#include <QTextLine>
#include <QTextBlock>
#include "pp_plaintextedit.h"
#include "public.h"

PP_PlainTextEdit::PP_PlainTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    dialog = new PP_Dialog(this);
    connect(this, SIGNAL(openDialog(int)), this, SLOT(showDialog(int)));
}

PP_PlainTextEdit::~PP_PlainTextEdit()
{
    FREE(dialog);
}

void PP_PlainTextEdit::showDialog(int ptListIdx)
{
    int res;
    res = dialog->exec(possibleTypoList.at(ptListIdx).orgText,
                       possibleTypoList.at(ptListIdx).candidate);
    qDebug() << "res: " << res;

    if (res>=0) {
        correctWord(ptListIdx, res);
    }
}

void PP_PlainTextEdit::correctWord(int ptListIdx, int candidateIdx)
{
    struct possibleTypo pt = possibleTypoList.at(ptListIdx);
    QTextCursor tc = textCursor();
    tc.setPosition(pt.start);
    tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, pt.end - pt.start);
//    tc.insertText(pt.candidate.at(candidateIdx));
    tc.insertHtml(pt.candidate.at(candidateIdx));
    setTextCursor(tc);
    possibleTypoList.removeAt(ptListIdx);
    return;
}

void PP_PlainTextEdit::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        QPlainTextEdit::mousePressEvent(e);

        QTextCursor tc = textCursor();
        for (int i=0; i<possibleTypoList.size(); ++i){
            if (tc.position() >= possibleTypoList.at(i).start &&
                    tc.position() <= possibleTypoList.at(i).end) {
                emit openDialog(i);
            }
        }
    }
    else if (e->button() == Qt::RightButton) {

    }
}

QString PP_PlainTextEdit::convertAnglebracket(const QString &htmlStr)
{
    QString output = tr("<font color='black'>")
            + htmlStr.toHtmlEscaped()
            + tr("</font>");
    return output.replace(" ","&nbsp;");
}

void PP_PlainTextEdit::loadFile(QString fileName)
{
    QFile result(fileName);

    if (!result.open(QIODevice::ReadOnly)) {
        qDebug() << "Error: Fail to open file, " << result.fileName();
        return;
    }
    clear();

//    QPalette p = palette();
//    p.setColor(QPalette::Base, Qt::white);
//    p.setColor(QPalette::Text, Qt::black);
//    setPalette(p);


    QTextStream res(&result);
    struct possibleTypo pt;
    possibleTypoList.clear();
    const int posStartShift=strlen(CANDIDATE_LEFT_BRACKET);
    const int posEndShift=strlen(CANDIDATE_RIGHT_BRACKET);
    bool isFirst=true;
    while(!res.atEnd()) {
        QString line = res.readLine();
        if (isFirst) {
            isFirst = false;
        }
        else {
            insertPlainText("\n");
        }
        for(;;) {
            int posStart = line.indexOf(CANDIDATE_LEFT_BRACKET);
            int posEnd = line.indexOf(CANDIDATE_RIGHT_BRACKET);
            if (posStart >= 0 && posEnd >= 0) {
                pt.candidate = line.mid(posStart+posStartShift, posEnd-posStart-posEndShift).split(',');
                int wordLen = pt.candidate.at(0).length();
                pt.start = toPlainText().count() + (posStart - wordLen);
                if (posStart-wordLen==0) ++pt.start;
                pt.orgText = line.mid(posStart-wordLen, wordLen);
                pt.end = pt.start + pt.orgText.length();
                possibleTypoList.append(pt);

                if (posStart) {
                    textCursor().insertHtml(convertAnglebracket(line.mid(0, posStart-wordLen)));
                }

                line = line.mid(posEnd+posEndShift);
                if (line.length()) {
                    textCursor().insertHtml("<font color='red'>"+pt.orgText+"</font>");
                }
                else {
                    textCursor().insertHtml("<font color='red'>"+pt.orgText+"</font>");
                }
                continue;
            }
            else {
                textCursor().insertHtml(convertAnglebracket(line));
                break;
            }
        }
    }
    result.close();

    return;
}
