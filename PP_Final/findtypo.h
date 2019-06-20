#ifndef FINDTYPO_H
#define FINDTYPO_H

#include <QFile>
#include <QString>
#include <QStringList>

struct _arg_parallel_cw_r {
    int st_pos;
    int line_count;
    void* obj;
};

class FindTypo
{
public:
    FindTypo();
    ~FindTypo();
    bool run(QString fileName, int cpu_cores=4);

private:
    void compareWord();
    void parallel_compareWord(int cpu_cores=4);
    static void* parallel_compareWord_start(void *arg);
    void* parallel_compareWord_routine(struct _arg_parallel_cw_r *arg);

private:
    QFile *database;
    QFile *input;
    QStringList *dbStrList;
    QStringList *inFileStrList;
};

#endif // FINDTYPO_H
