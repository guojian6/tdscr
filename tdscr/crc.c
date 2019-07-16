/*
      crc 校验要点：
      1）单个数据生成相应的crc校验在该数据后添加相应的数据长度。
      2）生成多项式的最高位必须是1。例如：CRC-CCITT标准的16位生成多项式：g（x）= x16+x12+x1+1；阶数r = 16
      即：0x11021.最高位通常为1。
      3) 计算校验码时进行的除法运算是模2运算。
      4）生成校验码时用该数据位后面添加相应的位数除以生成多项式得到的余式就是该数据的校验码。
      例如：1的校验码可以进行如下运算：0x10000%0x11024=0x1024;21的校验码为：0x20000%0x11024=0x2048 ....
            0x10的校验码为：0x10 0000 % 0x11024 = 0x1231;0x11的校验码为：0x110000%0x11024= 0x0210...
      5)进行数据流校验时把得到的最高8位与下一个要参加校验的数据相与把得到的数据进行单字节运算，得到的
      余式的高位数据与上一轮的低位数据相与就是该数据流的校验码。
      6)如下数据流有三种方式：纯计算方式、半查表方式、纯查表方式。在对1000个数据校验的计算过程中
	  所消耗的时间比为：53ms : 22ms :19ms.消耗的字节空间比例为：0:64:512字节。在实际的使用中可以根据
	  不同的环境选择相应的检验方式。
*/
//#include<stdio.h>
//#include<conio.h>
//#include<time.h>
//#include<dos.h>
#include "def.h"
#include "all.h"
#define uchar   unsigned char
#define uint    unsigned int

/*
#define uchar unsigned char
#define uint unsigned int
struct Msg_Struct{
	uint a;
	uchar b;
	uchar c;
	uint d;
}*p;
struct Msg_Struct qq;
enum{
	MT_INIT=0,
	MT_LEN,
	MT_TYPE,
	MT_RVMSG,
	MT_RVVER
};
*/

//extern struct Msg_Stru *mp;

__flash uint h_num_table[16] = {0,0x1231,0x2462,0x3653,0x48c4,0x5af5,0x6ca6,0x7e97,
						0x9188,0x83b9,0xb5ea,0xa7db,0xd94c,0xcb7d,0xfd2e,0xef1f};

__flash uint l_num_table[16] = {     0,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
						0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef};
uint crc_fun(uchar *pbuf, uchar numc)
{//计算查表法
	//uint h_num_table[16] = {     0,0x0210,0x0420,0x0630,0x0840,0x0a50,0x0c60,0x0e70,
	//						0x1080,0x1290,0x14a0,0x16b0,0x18c0,0x1ad0,0x1ce0,0x1ef0};
	uint check_sum;//,h_num,l_num;
	uchar buf;//,buf1;
	uchar s1;

	check_sum = 0;
	for (s1=0; s1<numc; s1++){
		buf = (check_sum>>8) ^ pbuf[s1];
		//check_sum = (h_num_table[buf>>4] ^ l_num_table[buf&0x0f] ^ l_num_table[buf>>4] ) ^ (check_sum<<8);
		check_sum = h_num_table[buf>>4] ^ l_num_table[buf&0x0f] ^ (check_sum<<8);
   }
    return check_sum;
}
/*
uint crc_fun(uchar *pbuf, uchar numc)
{//计算法
	uint check_sum,h_num,l_num;
	uchar buf,buf1;
	uchar s1;
	
	check_sum = 0;
	for (s1=0; s1<numc; s1++)
	{
		 buf = (check_sum>>8) ^ pbuf[s1+1];
		 buf1 =  buf>>4;
		 h_num = (buf1 * 0x0210) ^ (buf1 * 0x1021);
		 l_num = (buf & 0x0f) * 0x1021;
		 check_sum = h_num ^ l_num ^ (check_sum<<8);
	}
	return check_sum;
}

//////纯查表方式
__flash uint CRCTable[256]={
		0, 4129, 8258,12387,16516,20645,24774,28903,
	33032,37161,41290,45419,49548,53677,57806,61935,
	 4657,  528,12915, 8786,21173,17044,29431,25302,
	37689,33560,45947,41818,54205,50076,62463,58334,
	 9314,13379, 1056, 5121,25830,29895,17572,21637,
	42346,46411,34088,38153,58862,62927,50604,54669,
	13907, 9842, 5649, 1584,30423,26358,22165,18100,
	46939,42874,38681,34616,63455,59390,55197,51132,
	18628,22757,26758,30887, 2112, 6241,10242,14371,
	51660,55789,59790,63919,35144,39273,43274,47403,
	23285,19156,31415,27286, 6769, 2640,14899,10770,
	56317,52188,64447,60318,39801,35672,47931,43802,
	27814,31879,19684,23749,11298,15363, 3168, 7233,
	60846,64911,52716,56781,44330,48395,36200,40265,
	32407,28342,24277,20212,15891,11826, 7761, 3696,
	65439,61374,57309,53244,48923,44858,40793,36728,
	37256,33193,45514,41451,53516,49453,61774,57711,
	 4224,  161,12482, 8419,20484,16421,28742,24679,
	33721,37784,41979,46042,49981,54044,58239,62302,
	  689, 4752, 8947,13010,16949,21012,25207,29270,
	46570,42443,38312,34185,62830,58703,54572,50445,
	13538, 9411, 5280, 1153,29798,25671,21540,17413,
	42971,47098,34713,38840,59231,63358,50973,55100,
	 9939,14066, 1681, 5808,26199,30326,17941,22068,
	55628,51565,63758,59695,39368,35305,47498,43435,
	22596,18533,30726,26663, 6336, 2273,14466,10403,
	52093,56156,60223,64286,35833,39896,43963,48026,
	19061,23124,27191,31254, 2801, 6864,10931,14994,
	64814,60687,56684,52557,48554,44427,40424,36297,
	31782,27655,23652,19525,15522,11395, 7392, 3265,
	61215,65342,53085,57212,44955,49082,36825,40952,
	28183,32310,20053,24180,11923,16050, 3793, 7920
	};


uint CheckCRC(uchar numc)
{//纯查表法
	uchar s1;
	uint checksum;
	checksum=0;
	for(s1=0;s1<numc;s1++)
	{
		checksum=(checksum<<8)^CRCTable[(checksum>>8)^p_data[s1+1]];
	}
	return checksum;
}

*/
