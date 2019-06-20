#ifndef PUBLIC_H
#define PUBLIC_H

#include <qdebug.h>

#define FILE_VER    2
#if FILE_VER == 0
#define DB_PATH             "../database/ch3_utf8_dic.csv"
//#define INPUT_FILE_PATH     "../ch3_standard_utf8.csv"
#define INPUT_FILE_PATH     "../ch3_standard_utf8_wrong.csv"
#elif FILE_VER ==1
#define DB_PATH             "../database/all_utf8_dic.csv"
#define INPUT_FILE_PATH     "../all_utf8.csv"
#elif FILE_VER ==2
#define DB_PATH             "../database/dict_all.csv"
//#define INPUT_FILE_PATH     "../all_utf8.csv"
#define INPUT_FILE_PATH     "../data/ch3_standard_utf8.csv"
//#define INPUT_FILE_PATH     "../shortline_standard_utf8.csv"
#endif

#define OUTPUT_FILE_PATH    "../output.txt"


#define FREE(obj) if(obj) {\
    delete(obj); \
    obj = NULL; \
}

#define PTHREAD_CNT  4
#define CANDIDATE_LEFT_BRACKET  "[[["
#define CANDIDATE_RIGHT_BRACKET "]]]"

#endif // PUBLIC_H
