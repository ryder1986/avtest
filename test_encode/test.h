#ifndef TEST_H
#define TEST_H
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
class H264Encoder{
public:
    H264Encoder(int picture_size)
    {

        //    av_register_all();//register


        pFormatCtx = avformat_alloc_context();
        //Guess Format
        fmt = av_guess_format(NULL, "ds.h264", NULL);
        pFormatCtx->oformat = fmt;

        //Method 2.
        //avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file);
        //fmt = pFormatCtx->oformat;


        //Open output URL
        if (avio_open(&pFormatCtx->pb,"test.h264", AVIO_FLAG_READ_WRITE) < 0){
            printf("Failed to open output file! \n");
            //return -1;
        }

        video_st = avformat_new_stream(pFormatCtx, 0);

        framecnt=0;
        loop=0;
        //Param that must set
        pCodecCtx = video_st->codec;
        //pCodecCtx->codec_id =AV_CODEC_ID_HEVC;
        pCodecCtx->codec_id = AV_CODEC_ID_H264;
        pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
        pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
        pCodecCtx->width = 480;
        pCodecCtx->height = 272;
        pCodecCtx->bit_rate = 400000;
        pCodecCtx->gop_size=250;

        pCodecCtx->time_base.num = 1;
        pCodecCtx->time_base.den = 25;

        //H264
        //pCodecCtx->me_range = 16;
        //pCodecCtx->max_qdiff = 4;
        //pCodecCtx->qcompress = 0.6;
        pCodecCtx->qmin = 10;
        pCodecCtx->qmax = 51;

        //Optional Param
        pCodecCtx->max_b_frames=3;

        // Set Option
        AVDictionary *param = 0;
        //H.264
        if(pCodecCtx->codec_id == AV_CODEC_ID_H264) {
            av_dict_set(&param, "preset", "slow", 0);
            av_dict_set(&param, "tune", "zerolatency", 0);
            //av_dict_set(&param, "profile", "main", 0);
        }
        //H.265
        if(pCodecCtx->codec_id == AV_CODEC_ID_H265){
            av_dict_set(&param, "preset", "ultrafast", 0);
            av_dict_set(&param, "tune", "zero-latency", 0);
        }

        //Show some Information
        //  av_dump_format(pFormatCtx, 0, out_file, 1);

        pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
        if (!pCodec){
            printf("Can not find encoder! \n");
            //  return -1;
        }
        if (avcodec_open2(pCodecCtx, pCodec,&param) < 0){
            printf("Failed to open encoder! \n");
            //  return -1;
        }
        pFrame = av_frame_alloc();
        //  int  picture_size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
        uint8_t *  picture_buf = (uint8_t *)av_malloc(picture_size);
        avpicture_fill((AVPicture *)pFrame, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);


        /////
        pFrame->width=480;
        pFrame->height=272;
        pFrame->format=AV_PIX_FMT_YUVJ420P;
        ///




        //Write File Header
        avformat_write_header(pFormatCtx,NULL);

        av_new_packet(&pkt,picture_size);



    }
    ~H264Encoder()
    {

    }

    int encode( uint8_t* picture_buf)
    {
        int y_size = pCodecCtx->width * pCodecCtx->height;

        pFrame->data[0] = picture_buf;              // Y
        pFrame->data[1] = picture_buf+ y_size;      // U
        pFrame->data[2] = picture_buf+ y_size*5/4;  // V
        //PTS
        //pFrame->pts=i;
        pFrame->pts=loop*(video_st->time_base.den)/((video_st->time_base.num)*25);
        loop++;
        int got_picture=0;
        //Encode
        int ret = avcodec_encode_video2(pCodecCtx, &pkt,pFrame, &got_picture);
        if(ret < 0){
            printf("Failed to encode! \n");
            return -1;
        }
        if (got_picture==1){
            printf("Succeed to encode frame: %5d\tsize:%5d\n",framecnt,pkt.size);
            framecnt++;
            pkt.stream_index = video_st->index;
            // ret = av_write_frame(pFormatCtx, &pkt);
            //av_free_packet(&pkt);
            return 0;
        }
        return -1;
    }
    AVPacket& get_pkt()
    {
        return pkt;
    }

private:
    AVFrame* pFrame;//a frame data(yuv)
    //  uint8_t* picture_buf;//yuv
    AVStream* video_st;
    int framecnt;
    AVPacket pkt;
    AVCodecContext* pCodecCtx;
    AVCodec* pCodec;
    int loop;

    AVOutputFormat* fmt;
    AVFormatContext* pFormatCtx;
};
class YuvFile{
public:
    YuvFile()
    {
        width=480;
        height=272;
    }

    ~YuvFile()
    {

    }

    int open()
    {
        in_file  = fopen("/root/test.yuv", "rb");   //Input raw YUV data
        //  in_file  = fopen("/root/ds_480x272.yuv", "rb");   //Input raw YUV data
        return 0;
    }
    int read(uint8_t* picture_buf)
    {
        if (fread(picture_buf, 1, width*height*3/2, in_file) <= 0){
            printf("Failed to read raw data! \n");
            return -1;
        }else if(feof(in_file)){
            printf("file end\n");
            return 0;
        }
        return 0;
    }

    void close()
    {
        fclose(in_file);
    }
    friend class TestH264;
private:
    FILE *in_file;
    int width;
    int height;

};
class H264File{
public:
    H264File()
    {
        strcpy(out_file,"/root/test.264");
        pFormatCtx = avformat_alloc_context();
        //Guess Format
        fmt = av_guess_format(NULL, out_file, NULL);
        pFormatCtx->oformat = fmt;

        //Write File Header
        //  avformat_write_header(pFormatCtx,NULL);

    }

    ~H264File()
    {

    }

    int open()
    {
        if (avio_open(&pFormatCtx->pb,out_file, AVIO_FLAG_READ_WRITE) < 0){
            printf("Failed to open output file! \n");
            return -1;
        }
        return 0;
    }
    int write(AVPacket &pkt)
    {
        int ret=0;
        ret = av_write_frame(pFormatCtx, &pkt);
    }

    void close()
    {

        //        //Flush Encoder
        //        int ret = flush_encoder(pFormatCtx,0);
        //        if (ret < 0) {
        //            printf("Flushing encoder failed\n");
        //            return -1;
        //        }

        //Write file trailer
        av_write_trailer(pFormatCtx);

        //        //Clean
        //        if (video_st){
        //            avcodec_close(video_st->codec);
        //            av_free(pFrame);
        //            av_free(picture_buf);
        //        }
        avio_close(pFormatCtx->pb);
        avformat_free_context(pFormatCtx);

    }
private:

    AVFormatContext* pFormatCtx;
    AVOutputFormat* fmt;
    char out_file[100];
};
class TestH264{
public:
    TestH264():h264_encoder(yuv_file.width*yuv_file.height*3/2)
    {

    }
    ~TestH264()
    {

    }

    int run()
    {

        if(yuv_file.open()){
            return -1;
        }

        if(h264_file.open()){
            yuv_file.close();
            return -1;
        }
        uint8_t buf[yuv_file.width*yuv_file.height*3/2];
        // while()
        while(1){
            if(yuv_file.read(buf)){
                printf("yuv done\n");fflush(NULL);

                break;
            }

            if(!h264_encoder.encode(buf)){
                AVPacket &pk=h264_encoder.get_pkt();
                h264_file.write(pk);
            }
            printf("process a yuv \n");fflush(NULL);
        }

        h264_file.close();
        yuv_file.close();

    }
private:
    H264File h264_file;
    YuvFile yuv_file;
    H264Encoder h264_encoder;
};

#endif // TEST_H

