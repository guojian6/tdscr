/************************************************************
 * Copyright (c) 2004-2005, POWERCOM Co., All Rights Reserved.
 * File        :"all.h"
 * Description :

 * Author      : Xu Hailun
 * Date        : 2005-12-21
 * Version     : 1.0
 *
 * Revisions   :
 
 ************************************************************/
//0－有符号数，7－无符号数 8-比例为10的2个有符号数
__flash struct ParaListStruct ParaList_TABD[] = {
//参数表， 第一个参数是表参数地址的个数
  {0x0000,1,7},
  {0x0008,1,7},{0x060c,1,7},
  //{0x0155,2,8},{0x0157,2,8},{0x0156,2,8},
  {0x0010,1,0},{0x0011,1,0},
  {0x0018,1,0},{0x0019,1,0},
  {0x0020,1,7},{0x0021,1,7},
  {0x0028,1,7},{0x0029,1,7},
  {0x0024,1,7},{0x0025,1,7},
  {0x002c,1,7},{0x002d,1,7},
  {0x7002,1,7},{0x7003,1,7},
  {0x00d0,1,0},{0x00d1,1,0},
  {0x00d8,1,0},{0x00d9,1,0},
  {0x0050,2,7},{0x0052,2,7},
  {0x0060,2,7},{0x0062,2,7},
 
  
 // {0x8104,2,8},{0x8106,2,8},{0x8105,2,8},
 // {0x810c,2,8},{0x810e,2,8},{0x810d,2,8},
 	{0x8000,1,0},{0x8001,1,0},
  {0x8004,1,0},{0x8005,1,0},

  {0x8045,1,0},
  {0x8086,2,7},{0x8088,2,7},
  {0x808e,2,7},{0x8090,2,7},
  

  {0x80bc,2,7},{0x8076,2,7},
  {0x8028,1,7},{0x80c6,1,7},
  {0x8018,1,7},{0x8019,1,7},
  {0x801c,1,7},{0x801d,1,7},


//模块基本信息项
	{0xf000,10,0xff},{0xf001,30,0xff},{0xf002,1,7},{0xf003,1,7},{0xf017,10,0xff},{0xf018,2,7},{0xf019,20,0xff},
	{0xf036,2,8},{0xf030,2,8},{0xf032,2,8},{0xf037,2,8},{0xf031,2,8},{0xf033,2,8},{0xf035,2,8},{0xf034,2,8},
	/*{0xf350,2,7}{0xf357,1,7},*/{0xf352,1,7},{0xf353,1,7},{0xffff,0xff,0xff}
};
