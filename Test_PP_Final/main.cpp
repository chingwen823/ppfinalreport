#include "../PP_Final/findtypo.h"

int main(int arcg, char *argv[])
{
    (void)arcg;
    int cores=atoi(argv[1]);
    char *fileName=argv[2];
    FindTypo findtypo;

    findtypo.run(fileName, cores);

    return 0;
}
