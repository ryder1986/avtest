#include <iostream>
#include "test.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#ifdef __cplusplus
}
#endif
using namespace std;
#include <unistd.h>
int main()
{
    av_register_all();//register

    TestH264 t;
    t.run();
    cout << "Hello World!" << endl;
    usleep(10000000);
    return 0;
}

