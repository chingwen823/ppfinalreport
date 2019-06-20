#include <QTextStream>
#include <pthread.h>
#include "findtypo.h"
#include "public.h"

FindTypo::FindTypo()
{
    database = new QFile(DB_PATH);
    input = NULL;

    if (!database->open(QIODevice::ReadOnly)) {
        qDebug("Error: Fail to open database.\n");
    }

    QTextStream in(database);
    QString line;

    dbStrList = new QStringList();
    while(!in.atEnd()) {
        line = in.readLine();
        dbStrList->append(line);
    }
    database->close();

    inFileStrList = NULL;
}


FindTypo::~FindTypo()
{
    FREE(database);

    FREE(input);

    FREE(dbStrList);
    FREE(inFileStrList);
}

bool FindTypo::run(QString fileName, int cpu_cores)
{
    input = new QFile(fileName);

    if (!input->open(QIODevice::ReadOnly)) {
        QString str = "Error: Fail to open input file, " + input->fileName();
        qDebug() << str;
        return false;
    }

#define PARALLEL
#ifdef PARALLEL
    parallel_compareWord(cpu_cores);
#else
    compareWord();
#endif

    input->close();

    return true;
}

void FindTypo::compareWord()
{
    QTextStream in(input);
    QString line, str;
    bool isFind;
    QStringList candidate;
    QFile output(OUTPUT_FILE_PATH);


    if (!output.open(QIODevice::WriteOnly | QFile::Text)) {
        QString str = "Error: Fail to open output file, " + input->fileName();
        qDebug() << str;
        return;
    }
    QTextStream out(&output);

    while(!in.atEnd()) {
        line = in.readLine();
        if (line.count()==0) {
            continue;
        }
        QStringList::const_iterator ci;
        isFind = false;
        candidate.clear();
        for (ci=dbStrList->constBegin(); ci!=dbStrList->constEnd(); ++ci) {
            QStringList list = line.split(',');
            if (list.count() > 1) {
                str = line.section(',', 1);
            }
            else {
                break;
            }
            if (str == *ci) {
                isFind = true;
                break;
            }
            else {
                if (str.count() == (*ci).count()) {
                    for (int i=0; i<str.count(); ++i) {
                        if (str[i] == (*ci)[i]) {
                            candidate.append(*ci);
                        }
                    }
                }
            }
        }
        if (isFind) {
            out << str;
        }
        else {
            QString outputLine;
            outputLine.append(str);
            if (candidate.count()>0){
                outputLine.append(CANDIDATE_LEFT_BRACKET);
                for (ci=candidate.constBegin(); ci!=candidate.constEnd(); ++ci) {
                    if (ci!=candidate.constBegin()) {
                        outputLine.append(",");
                    }
                    outputLine.append(*ci);
                }
                outputLine.append(CANDIDATE_RIGHT_BRACKET);
            }
            if (outputLine.count()){
                out << outputLine;
            }
            else {
                out << endl;
            }
        }
    }

    output.close();
    return;
}

void FindTypo::parallel_compareWord(int cpu_cores)
{
//    int cpu_cores=PTHREAD_CNT;
    pthread_t *thread_handles=NULL;
    struct _arg_parallel_cw_r *arg_pcwr=NULL;
    QTextStream in(input);
    QFile output(OUTPUT_FILE_PATH);
    int line_count=0;
    int i;

    if (!output.open(QIODevice::WriteOnly | QFile::Text)) {
        QString str = "Error: Fail to open output file, " + input->fileName();
        qDebug() << str;
        return;
    }
    QTextStream out(&output);

    inFileStrList = new QStringList();
    while(!in.atEnd()) {
        QString line = in.readLine();
        inFileStrList->append(line);
        ++line_count;
    }

    thread_handles = (pthread_t*)malloc(cpu_cores*sizeof(pthread_t));
    arg_pcwr = new _arg_parallel_cw_r[cpu_cores];

    int lpos=0, lcnt=line_count/cpu_cores;
    for (i=0; i<cpu_cores; ++i) {
        if (i) {
            arg_pcwr[i].st_pos = lpos = lpos + arg_pcwr[i-1].line_count;
            arg_pcwr[i].line_count = lcnt;
        }
        else {
            arg_pcwr[i].st_pos = lpos;
            arg_pcwr[i].line_count = lcnt + line_count%cpu_cores;
        }
        arg_pcwr[i].obj = (void*)this;

        pthread_create(thread_handles+i, NULL, parallel_compareWord_start, (void*)(arg_pcwr+i));
    }

    for(i=0; i<cpu_cores; ++i) {
        char *str;
        pthread_join(thread_handles[i], (void**)&str);
        if (str) {
            out << QString(str);
            delete [] str;
        }
    }
    output.close();
    delete [] arg_pcwr;
    return;
}

void* FindTypo::parallel_compareWord_start(void *arg)
{
    struct _arg_parallel_cw_r *ptr = static_cast<struct _arg_parallel_cw_r *>(arg);
    return static_cast<class FindTypo*>(ptr->obj)->parallel_compareWord_routine(ptr);
}

void* FindTypo::parallel_compareWord_routine(struct _arg_parallel_cw_r *arg)
{
    struct _arg_parallel_cw_r arg_pcwr;
    QString line, str, out;
    bool isFind;
    QStringList candidate;
    QStringList strList(*inFileStrList), dbList(*dbStrList);
    QStringList::const_iterator ci;
    int i;

//    qDebug("Pthread ID: %lu", pthread_self());

    memcpy(&arg_pcwr, arg, sizeof(struct _arg_parallel_cw_r));

    for (i=0; i<arg_pcwr.line_count; ++i) {
        line = strList.at(arg_pcwr.st_pos+i);
        if (line.count()==0) {
            continue;
        }
        isFind = false;
        candidate.clear();
        for (ci=dbList.constBegin(); ci!=dbList.constEnd(); ++ci) {
            QStringList list = line.split(',');
            if (list.count() > 1) {
                str = line.section(',', 1);
            }
            else {
                break;
            }
            if (str == *ci) {
                isFind = true;
                break;
            }
            else {
                if (str.count() == (*ci).count()) {
                    for (int i=0; i<str.count(); ++i) {
                        if (str[i] == (*ci)[i]) {
                            candidate.append(*ci);
                        }
                    }
                }
            }
        }
        if (isFind) {
            out.append(str);
        }
        else {
            QString outputLine;
            outputLine.append(str);
            if (candidate.count()>0){
                outputLine.append(CANDIDATE_LEFT_BRACKET);
                for (ci=candidate.constBegin(); ci!=candidate.constEnd(); ++ci) {
                    if (ci!=candidate.constBegin()) {
                        outputLine.append(",");
                    }
                    outputLine.append(*ci);
                }
                outputLine.append(CANDIDATE_RIGHT_BRACKET);
            }
            if (outputLine.count()){
                out.append(outputLine);
            }
            else {
                out.append("\n");
            }
        }
    }

    return (void*)qstrdup(out.toLocal8Bit().constData());
}
