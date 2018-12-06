https://www.cnblogs.com/haibindev/p/3434922.html
https://blog.csdn.net/xdwyyan/article/details/41721307
https://github.com/leixiaohua1020/simplest_ffmpeg_video_encoder/blob/master/simplest_ffmpeg_video_encoder/simplest_ffmpeg_video_encoder.cpp

/*
http://www.cnblogs.com/tinywan/p/6404411.html
encode:
H.264有四种画质级别,分别是baseline, extended, main, high：
1、Baseline Profile：基本画质。支持I/P 帧，只支持无交错（Progressive）和CAVLC；
2、Extended profile：进阶画质。支持I/P/B/SP/SI 帧，只支持无交错（Progressive）和CAVLC；(用的少)
3、Main profile：主流画质。提供I/P/B 帧，支持无交错（Progressive）和交错（Interlaced）， 也支持CAVLC 和CABAC 的支持；
4、High profile：高级画质。在main Profile 的基础上增加了8x8内部预测、自定义量化、 无损视频编码和更多的YUV 格式；
*/
