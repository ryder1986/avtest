#include <iostream>
#include "test.h"
#include "unistd.h"
using namespace std;
FILE *fp_send1;

int read_buffer1(unsigned char *buf, int buf_size ){
    if(!feof(fp_send1)){
        int true_size=fread(buf,1,buf_size,fp_send1);
        return true_size;
    }else{
        return -1;
    }
}

int main()
{



    RtmpSender sender;
    fp_send1 = fopen("/root/cuc_ieschool.h264", "rb");

    //初始化并连接到服务器
  //  RTMP264_Connect("rtmp://localhost/live/test1");
    sender.RTMP264_Connect("rtmp://192.168.1.219/live/test1");

    //发送
    sender.RTMP264_Send(read_buffer1);

    //断开连接并释放相关资源
    sender.RTMP264_Close();

    cout << "Hello World!" << endl;
    usleep(10000000);
    return 0;
}

