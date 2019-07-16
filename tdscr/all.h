/************************************************************
 * Copyright (c) 2004-2005, POWERCOM Co., All Rights Reserved.
 * File        :"all.h"
 * Description :

 * Author      : Xu Hailun
 * Date        : 2003-08-26
 * Version     : 1.0
 *
 * Revisions   :
 * 	20050616, by xhl
 *		增加光模块部门,关键字:__OPT_MODULE__
 ************************************************************/
#ifdef __MAIN_PROGRAM__

	schar temp1;
	uchar ctri;
	uchar fast_flag,slow_flag,sum_paras;//,slow_10ms,slow_50ms,slow_1s,slow_5s;
	uchar reset_f;//1,enter test status;2, upgrade
	uchar po_cnt;
	uint eewp;
	uint led_flag;
    struct module_addr own;
	struct PA_Para_Stru my;
    struct ADC_Para_Stru	adct[8];//adc table
    struct Calibration_Struct amplifier;    //正向功率
    struct Calibration_Struct amplifier_ro; //反向功率 
    struct Calibration_Struct amplifier_pi; //输入功率
    
	uchar rvb[MAX_RVBLEN];
	uchar sdb[MAX_SDBLEN];
	uchar rbr, rbw, sbr, sbw;

	uchar adc_mux;
	uint adc_rel;
   	uchar ADC_state[8],ADC_count[8];
    	uint uiADC[MAX_SAMPLING];
	uchar rbc,rv_len,rv_ver;
        uchar cmp_flag;
	//智能化表格
	struct ITab_Struct ITab;
	
#else
	extern schar temp1;;
	extern uchar ctri;
    extern uchar po_cnt;
	extern uchar fast_flag,slow_flag,sum_paras;//,slow_10ms,slow_50ms,slow_1s,slow_5s;
	extern uchar reset_f;//1,enter test status;2, upgrade
	extern uint eewp;
	extern struct module_addr own;
	extern struct PA_Para_Stru my;
        extern uchar cmp_flag;
	extern uchar adc_mux;
	extern uint  adc_rel;
    extern uchar ADC_state[8],ADC_count[8];
    extern uint uiADC[MAX_SAMPLING];
	extern uchar rbc,rv_len,rv_ver;
	extern uchar rvb[MAX_RVBLEN],rbr,rbw;
	extern uchar sdb[MAX_SDBLEN],sbr,sbw;
    extern struct ADC_Para_Stru	adct[8];//adc table
    extern struct Calibration_Struct amplifier;
    extern struct Calibration_Struct amplifier_ro;  
    extern struct Calibration_Struct amplifier_pi;
   	extern struct Current_Para_Stru cp;
	extern struct ITab_Struct ITab;	
#endif
