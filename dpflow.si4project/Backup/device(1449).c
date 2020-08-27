#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_sau.h"
#include "r_cg_wdt.h"
#include "r_cg_rtc.h"
#include "device.h"
#include "hum.h"
#include "lora.h"
#include "irc.h"
#include "24cxx.h"
#include "r_cg_adc.h"
#include "r_cg_rtc.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "cs123x.h"
#include "pcf857x.h"
#include "ad54x0.h"
#include "system.h"



  
			
/////////////////////////////////////////get temperature////////////////////////////////////
/////////////////////////////////////////get temperature////////////////////////////////////

static const long temp_tab[] = 
{

	583542,  554647, 526968,  500480, 475159, 450974, 427897, 405892, 384927,  364967, //-55...-46
	345975,  327915, 310751,  294448, 278969, 264279, 250344, 237130, 224603,  212733, //-45...-36
	201487,  190836, 180750,  171201, 162163, 153610, 145516, 137858, 130614,  123761,//-35....-26

	117280,  111149, 105351,  99867 , 94681,   89776,  85137,  80750,  76600,   72676,  // -25 .........-16
	68963 ,  65451 ,  62129,  58986 , 56012,   53198,  50539,  48013,  45627,   43368,   // -15.......... -6
	41229 ,  39204 ,  37285,  35468 , 33747,   32116,  30570,  29105,  27716,   26399,   // -5............4
	25150 ,  23965 ,  22842,  21776 , 20764,   19783,  18892,  18026,  17204,   16423,   //  5.......... 14
	15681 ,  14976 ,  14306,  13669 , 13063,   12487,  11939,  11418,  10921,   10449,    //  15...........24
	10000 ,  9571  ,   9164,  8775  ,  8405,   8052 ,  7716,    7396,   7090,    6798,     // 25...........34
	6520  ,   6255 ,   6002,  5760  ,  5529,    5309,  5098,    4897,   4704,    4521,     //  35..........44
	4345  ,   4177 ,   4016,  3863  ,  3716,    3588,  3440,    3311,   3188,    3069,    // 45...........54
	2956  ,   2848 ,   2744,  2644  ,  2548,    2457,  2369,    2284,   2204,    2126,2051, //  55..........65
	1980  ,    1911,   1845,  1782  ,  1721,    1663,  1606,    1552,   1500, 1450,  //66-75
	1402  ,    1356,   1312,  1269  ,  1228,    1188,  1150,    1113,   1078, 1044,  //76-85
	1011  ,     979,    948,  919   ,   891,     863,   837,     811,    787,  763,   //86-95
	740   ,     718,    697,  676   ,   657,     637,   619,     601,    584,  567,   //96-105
	551   ,     535,    520,  505   ,   491,     477,   464,     451,    439,  427,   //106-115
	415   ,     404,    393,  383   ,   373,     363,   353,     344,    335,  326   //116-125

};

static const int pt100_tab[]=
{   
    
    7633 ,7673 ,7713 ,7752 ,7792 ,7832 ,7872 ,7911 ,7951 ,7991, //-60....-51
    8031 ,8070 ,8110 ,8150 ,8189 ,8229 ,8269 ,8308 ,8348 ,8388, 
    8427 ,8467 ,8506 ,8546 ,8585 ,8625 ,8664 ,8704 ,8743 ,8783, 
    8822 ,8862 ,8901 ,8940 ,8980 ,9019 ,9059 ,9093 ,9137 ,9177, 
    9216 ,9255 ,9295 ,9334 ,9373 ,9412 ,9452 ,9491 ,9530 ,9569, 
    9609 ,9648 ,9687 ,9726 ,9765 ,9804 ,9844 ,9883 ,9922 ,9961,//-10....-1
    
    10000,10039,10078,10117,10156,10195,10234,10273,10312,10351,//0......9
    10390,10429,10468,10507,10546,10585,10624,10663,10702,10740,
    10779,10818,10875,10896,10935,10973,11012,11051,11090,11128,
    11167,11206,11245,11283,11322,11361,11499,11438,11477,11515,
    11554,11593,11631,11670,11708,11747,11785,11824,11862,11901,
    11940,11978,12016,12055,12093,12132,12170,12209,12247,12286,
    12324,12362,12401,12439,12477,12516,12554,12592,12631,12669,
    12707,12745,12784,12822,12860,12898,12937,12975,13013,13051,
    13089,13127,13166,13204,13242,13280,13318,13356,13394,13432,
    13470,13508,13546,13584,13622,13660,13698,13736,13774,13812,
    13850,13888,13926,13964,14002,14039,14077,14115,14153,14191,
    14229,14266,14304,14342,14380,14417,14455,14493,14531,14568,
    14606,14644,14681,14719,14757,14794,14832,14870,14907,14945,
    14982,15020,15057,15095,15133,15170,15208,15245,15283,15320,
    15358,15395,15432,15470,15507,15545,15582,15619,15657,15694,
    15731,15769,15806,15843,15881,15918,15955,15993,16030,16067,
    16104,16142,16179,16216,16253,16290,16327,16365,16402,16439,
    16476,16513,16550,16587,16624,16661,16698,16735,16772,16809,
    16846,16883,16920,16957,16994,17031,17068,17105,17142,17179,
    17216,17253,17290,17326,17363,17400,17437,17474,17510,17547,
    17584,17621,17657,17694,17731,17768,17804,17841,17878,17914,
    17951,17988,18024,18061,18097,18134,18171,18207,18244,18280,
    18317,18353,18390,18426,18463,18499,18536,18572,18609,18645,
    18632,18718,18754,18791,18827,18863,18900,18936,18972,19009,
    19045,19081,19118,19154,19190,19226,19263,19299,19335,19371,
    19407,19444,19480,19516,19552,19588,19624,19660,19696,19733,
    19769,19805,19841,19877,19913,19949,19985,20021,20057,20093,
    20129,20165,20201,20236,20272,20308,20344,20380,20416,20452,
    20488,20523,20559,20595,20631,20667,20702,20738,20774,20810,
    20845,20881,20917,20952,20988,21024,21059,21095,21131,21166,
    21202,21237,21273,21309,21344,21380,21415,21451,21486,21522,
    21557,21593,21628,21664,21799,21735,21770,21805,21841,21876,
    21912,21947,21982,22018,22053,22088,22124,22159,22194,22229,
    22265,22300,22335,22370,22406,22441,22476,22511,22546,22581,
    22617,22652,22687,22722,22757,22792,22827,22862,22897,22932,
    22962,23002,23042,23082,23122,23162,23202,23242,23282,23322,
    23317,23352,23387,23422,23456,23491,23526,23561,23596,23631,
    23665,23700,23735,23770,23804,23839,23874,23909,23945,23978,
    24013,24047,24082,24117,24151,24186,24220,24255,24290,24324,
    24359,24393,24428,24462,24497,24531,24566,24600,24635,24669,
    24704,24738,24773,24807,24841,24876,24910,24945,24979,25013,
    25048,25082,25116,25150,25185,25219,25253,25255,25322,25356,
    25390,25424,25459,25493,25527,25561,25595,25629,25664,25698,
    25732,25766,25800,25834,25868,25902,25936,25970,26004,26038,
    26072,26106,26140,26174,26208,26242,26276,26310,26343,26377,
    26411,26445,26479,26513,26547,26580,26614,26648,26682,26715,
    26749,26783,26817,26850,26884,26918,26951,26985,27019,27052,
    27086,27120,27153,27187,27220,27254,27288,27321,27355,27388,
    27422,27455,27489,27522,27556,27589,27623,27656,27689,27723,
    27756,27790,27823,27856,27890,27923,27956,27990,28023,28056,
    28090,28123,28156,28189,28223,28256,28289,28322,28355,28389,
    28422,28455,28488,28521,28554,28587,28621,28654,28687,28720,
    28753,28786,28819,28852,28885,28918,28951,28984,29017,29050,
    29083,29116,29149,29181,29214,29247,29280,29313,29346,29379,
    29411,29444,29477,29510,29543,29575,29608,29641,29674,29706,
    29739,29772,29804,29837,29870,29902,29935,29968,30000,30033,
    30065,30098,30131,30163,30196,30228,30261,30293,30326,30358,
    30391,30423,30456,30488,30520,30553,30585,30618,30650,30682,
    30715,30747,30779,30812,30844,30876,30909,30941,30973,31005,
    31038,31070,31102,31134,31167,31199,31231,31263,31295,31327//590...599

};

static const float Sated_vapor_density_table[]=
{
    0.5977	,0.618 	,0.6388	,0.6601	,0.6821	,0.7046	,0.7277	,0.7515	,0.7758	,0.8008	,0.8265	,0.8528	,0.8798	,0.9075	,0.9359	,
    0.965 	,0.9948	,1.025 	,1.057 	,1.089 	,1.122 	,1.155 	,1.19  	,1.225 	,1.261 	,1.298 	,1.336 	,1.375 	,1.415 	,1.455 	,
    1.497 	,1.539 	,1.583 	,1.627 	,1.672	,1.719	,1.766	,1.815	,1.864	,1.915	,1.967	,2.019	,2.073	,2.129	,2.185	,
    2.242	,2.301	,2.361	,2.422	,2.484	,2.548	,2.613	,2.679	,2.747	,2.816	,2.886	,2.958	,3.032	,3.106	,3.182	,
    3.26	,3.339	,3.42	,3.502	,3.586	,3.671	,3.758	,3.847	,3.937	,4.029	,4.123	,4.218	,4.316	,4.415	,4.515	,
    4.618	,4.723	,4.829	,4.937	,5.048	,5.16	,5.274	,5.391	,5.509	,5.629	,5.752	,5.877	,6.003	,6.131	,6.264	,
    6.397	,6.553	,6.671	,6.812	,6.955	,7.1	,7.248	,7.398	,7.551	,7.706	,7.864	,8.025	,8.188	,8.354	,8.522	,
    8.694	,8.868	,9.045	,9.225	,9.408	,9.593	,9.782	,9.974	,10.17	,10.37	,10.57	,10.77	,10.98	,11.19	,11.41	,
    11.62	,11.84	,12.07	,12.3	,12.53	,12.76	,13		,13.24	,13.49	,13.74	,14		,14.25	,14.52	,14.78	,15.05	,
    15.33	,15.61	,15.89	,16.18	,16.47	,16.76	,17.06	,17.37	,17.68	,17.99	,18.31	,18.64	,18.97	,19.3	,19.64
};

static const float Sated_vapor_press_table[]=
{
    
    0.1013	,0.105	,0.1088	,0.1127	,0.1167	,0.1208	,0.125	,0.1294	,0.1339	,0.1385	,0.1433	,0.1481	,0.1532	,0.1583	,0.1636	,
    0.1691	,0.1746	,0.1804	,0.1863	,0.1923	,0.1985	,0.2049	,0.2114	,0.2182	,0.225	,0.2321	,0.2393	,0.2467	,0.2543	,0.2621	,
    0.2701	,0.2783	,0.2867	,0.2953	,0.3041	,0.313	,0.3222	,0.3317	,0.3414	,0.3513	,0.3614	,0.3718	,0.3823	,0.3931	,0.4042	,
    0.4155	,0.4271	,0.4389	,0.451	,0.4633	,0.476	,0.4888	,0.5021	,0.5155	,0.5292	,0.5433	,0.5577	,0.5723	,0.5872	,0.6025	,
    0.6181	,0.6339	,0.6502	,0.6666	,0.6835	,0.7008	,0.7183	,0.7362	,0.7544	,0.773	,0.792	,0.8114	,0.831	,0.8511	,0.8716	,
    0.8924	,0.9137	,0.9353	,0.9573	,0.9797	,1.0197	,1.0259	,1.0496	,1.0737	,1.0983	,1.1233	,1.1487	,1.1746	,1.201	,1.2278	,
    1.2551	,1.2829	,1.3111	,1.3397	,1.369	,1.3987	,1.4289	,1.4596	,1.4909	,1.5225	,1.5548	,1.5876	,1.621	,1.6548	,1.6892	,
    1.7242	,1.7597	,1.7959	,1.8326	,1.8699	,1.9077	,1.9462	,1.9852	,2.0248	,2.065	,2.1059	,2.1474	,2.1896	,2.2323	,2.2757	,
    2.3198	,2.3645	,2.4098	,2.4559	,2.5026	,2.55	,2.5981	,2.6469	,2.6963	,2.7466	,2.7975	,2.8491	,2.901	,2.9546	,3.0085	,
    3.0631	,3.1185	,3.1746	,3.2316	,3.2892	,3.3477	,3.407	,3.467	,3.5279	,3.5897	,3.6522	,3.7155	,3.7797	,3.8448	,3.9107	

};

static const int Sated_vapor_temp_table[]=
{
    1000	,1010	,1020	,1030	,1040	,1050	,1060	,1070	,1080	,1090	,1100	,1110	,1120	,1130	,1140	,
    1150	,1160	,1170	,1180	,1190	,1200	,1210	,1220	,1230	,1240	,1250	,1260	,1270	,1280	,1290	,
    1300	,1310	,1320	,1330	,1340	,1350	,1360	,1370	,1380	,1390	,1400	,1410	,1420	,1430	,1440	,
    1450	,1460	,1470	,1480	,1490	,1500	,1510	,1520	,1530	,1540	,1550	,1560	,1570	,1580	,1590	,
    1600	,1610	,1620	,1630	,1640	,1650	,1660	,1670	,1680	,1690	,1700	,1710	,1720	,1730	,1740	,
    1750	,1760	,1770	,1780	,1790	,1800	,1810	,1820	,1830	,1840	,1850	,1860	,1870	,1880	,1890	,
    1900	,1910	,1920	,1930	,1940	,1950	,1960	,1970	,1980	,1990	,2000	,2010	,2020	,2030	,2040	,
    2050	,2060	,2070	,2080	,2090	,2100	,2110	,2120	,2130	,2140	,2150	,2160	,2170	,2180	,2190	,
    2200	,2210	,2220	,2230	,2240	,2250	,2260	,2270	,2280	,2290	,2300	,2310	,2320	,2330	,2340	,
    2350	,2360	,2370	,2380	,2390	,2400	,2410	,2420	,2430	,2440	,2450	,2460	,2470	,2480	,2490	

};


static const float Suped_vapor_press_table[]=
{
    0.1	,0.15	,0.2	,0.25	,0.3	,0.4	,0.5	,0.8	,1.1	,1.4	,1.7	,2	    ,2.5	,3	    ,
    3.5	,4		,4.5	,5		,6	    ,7		,8		,9		,10		,12.5	,15		,17.5	,20		,21.5
};

static const int Suped_vapor_temp_table[]=
{
    1500	,1700	,1900	,2100	,2300	,2500	,2700	,2900	,3100	,3300	,3500	,
    3700	,3900	,4100	,4300	,4500	,4700	,4900	,5100	,5300	,5500	,5700	,5900
};

 static const float Suped_vapor_density_table[sizeof(Suped_vapor_press_table)/sizeof(float)][sizeof(Suped_vapor_temp_table)/sizeof(int)]=
{
    0.5164		,0.4925		,0.4707		,0.4507		,0.4323		,0.4156		,0.4001		,0.3857		,0.3724		,0.36		,0.3484		,0.3375		,0.3272		,0.3176		,0.3086		,0.3		,0.2919		,0.2842		,0.2769		,0.27		,0.2634		,0.2571		,0.2512	,
    0.7781		,0.7412		,0.7079		,0.6777		,0.65		,0.6246		,0.601		,0.5795		,0.5594		,0.5404		,0.523		,0.5066		,0.4912		,0.4767		,0.4631		,0.4502		,0.4381		,0.427		,0.4156		,0.4052		,0.3953		,0.386		,0.3768	,
    1.0423		,0.9918		,0.9466		,0.9056		,0.8684		,0.8342		,0.8027		,0.7736		,0.7465		,0.7214		,0.698		,0.6759		,0.6553		,0.636		,0.6178		,0.6005		,0.5842		,0.5688		,0.55		,0.54		,0.5271		,0.5146		,0.5026	,
    1.3089		,1.2444		,1.1869		,1.1349		,1.0849		,1.0445		,1.0048		,0.9682		,0.9343		,0.9027		,0.8732		,0.8456		,0.8198		,0.7955		,0.7726		,0.7507		,0.7316		,0.7113		,0.6925		,0.6757		,0.6591		,0.644		,0.6284	,
    1.5783		,1.499		,1.4287		,1.3653		,1.3079		,1.254		,1.2077		,1.1634		,1.1224		,1.0844		,1.0488		,1.0156		,0.9845		,0.9552		,0.9277		,0.8989		,0.8856		,0.854		,0.832		,0.8108		,0.7913		,0.7724		,0.754	,
    2.1237		,2.0141		,1.9166		,1.8297		,1.7513		,1.678		,1.6152		,1.5554		,1.5		,1.449		,1.401		,1.3563		,1.3144		,1.2753		,1.2377		,1.2035		,1.1708		,1.1396		,1.1102		,1.0821		,1.0556		,1.0303		,1.0062	,
    2.6658		,2.538		,2.4121		,2.2997		,2.1992		,2.1081		,2.0255		,1.9495		,1.8802		,1.8147		,1.7545		,1.6983		,1.6456		,1.5961		,1.5498		,1.5036		,1.4648		,1.4258		,1.3888		,1.3537		,1.3204		,1.2887		,1.2585	,
    4.3966		,4.1676		,3.935		,3.74		,3.5374		,3.411		,3.2718		,3.1453		,3.0283		,2.9215		,2.8227		,2.7305		,2.644		,2.5595		,2.4884		,2.4171		,2.35		,2.2869		,2.2274		,2.172		,2.1164		,2.029		,2.0168	,
    6.1313		,5.8332		,5.5342		,5.2356		,4.981		,4.746		,4.5445		,4.3612		,4.1943		,4.041		,3.903		,3.77		,3.6512		,3.5384		,3.4335		,3.3345		,3.2402		,3.1529		,3.071		,2.9902		,2.917		,2.8449		,2.7774	,
    7.8785		,7.5163		,7.154		,6.7913		,6.4288		,6.1147		,5.8437		,5.5945		,5.3794		,5.1777		,4.9945		,4.829		,4.6673		,4.522		,4.3857		,4.2575		,4.3496		,4.2291		,3.9157		,3.8143		,3.7183		,3.6271		,3.5401	,
    9.8464		,9.3688		,9.2473		,8.413		,7.9352		,7.5219		,7.183		,6.8607		,6.5815		,6.3309		,6.0998		,5.7779		,5.6936		,5.512		,5.3441		,5.1863		,5.0374		,4.8972		,4.7665		,4.6408		,4.523		,4.4116		,4.3056	,
    11.6295		,11.0985	,10.5676	,10.0366	,9.5054		,8.9744		,8.535		,8.1447		,7.8061		,7.4955		,7.2186		,6.9619		,6.729		,6.5117		,6.313		,6.1203		,5.9419		,5.7811		,5.6204		,5.4725		,5.3322		,5.1989		,5.0745	,
    15.189		,14.4516	,13.715		,12.9776	,12.2406	,11.5036	,10.8794	,10.35		,9.8888		,9.4806		,9.1139		,8.7802		,8.475		,8.1938		,7.9332		,7.6777		,7.4937		,7.281		,7.0799		,6.8637		,6.6858		,6.5177		,6.3582	,
    18.4168		,17.5709	,16.7243	,15.8776	,15.0367	,14.1842	,13.3377	,12.6359	,11.9979	,11.5143	,11.0494	,10.6308	,10.2493	,9.9		,9.5775		,9.2642		,8.856		,8.602		,8.361		,8.133		,8.0486		,7.8437		,7.6498	,
    22.7008		,21.5713	,20.4427	,19.3131	,18.2266	,17.053		,15.9243	,15.0163	,14.2565	,13.8501	,13.0286	,12.6162	,12.0528	,11.6308	,11.2425	,10.8842	,10.5512	,10.25		,9.9499		,9.6776		,9.4197		,9.1777		,8.948	,
    27.164		,25.747		,24.3303	,22.9129	,21.4954	,20.0778	,18.6603	,17.4997	,16.5527	,15.749		,15.0539	,14.4392	,13.8862	,13.3077	,12.9991	,12.5087	,12.1835	,11.757		,11.4169	,11.0994	,10.8003	,105191		,10.2533,
    30.3852		,28.9163	,27.4475	,25.9784	,24.5096	,23.0407	,21.5717	,20.1028	,18.9333	,17.9308	,17.1279	,16.4018	,15.7527	,15.1451	,14.6679	,14.1507	,13.7009	,13.288		,12.895		,12.5315	,12.1894	,11.8683	,11.565	,
    35.4243		,33.6293	,31.8342	,30.0384	,28.2433	,26.4483	,24.6532	,22.858		,21.4221	,20.2508	,19.2627	,18.4108	,17.6565	,16.9827	,16.3719	,15.8139	,15.3017	,14.8021	,14.3859	,13.9749	,13.5885	,13.2267	,12.885	,
    43.8954		,41.7475	,39.5988	,37.4508	,35.302		,33.1541	,31.0062	,28.8574	,26.7091	,25.0502	,23.7006	,22.557		,21.5629	,20.69		,19.9062	,19.1981	,18.5495	,17.956		,17.4029	,16.8912	,16.4119	,15.9657	,15.544	,
    56.7201		,53.6991	,50.678		,47.6561	,44.6352	,41.6133	,38.5922	,35.5704	,32.5488	,30.2231	,28.4037	,27.01		,25.633		,24.5224	,23.4021	,22.6635	,21.8675	,21.146		,20.4699	,19.8506	,19.2745	,18.735		,18.2314,
    65.4713		,62.18		,58.8883	,55.5968	,52.3061	,49.0145	,45.7231	,42.4316	,39.1399	,35.8485	,33.4179	,31.4825	,29.8698	,28.4969	,27.2913	,26.017		,25.264		,24.395		,23.5905	,22.8573	,22.1742	,21.54		,20.95	,
    84.5457		,79.8261	,75.1061	,70.3863	,65.6665	,60.9465	,56.21		,51.5077	,46.7877	,42.068		,38.8083	,36.3217	,34.3044	,32.2947	,31.1593	,29.8733	,28.4637	,27.71		,26.7676	,25.9068	,25.1124	,24.3771	,23.6949,
    108.625		,102.0289	,95.4346	,88.8412	,82.2486	,75.6543	,65.7699	,62.4676	,59.6648	,49.2802	,44.756		,41.5274	,39.0006	,36.9344	,35.1684	,33.6447	,32.3002	,31.096		,30.0116	,29.0164	,28.1		,27.2557	,26.4738,
    158.3464	,148.7516	,139.1578	,129.5629	,119.9781	,110.3842	,95.7769	,91.1964	,81.6034	,72.0105	,62.4178	,56.1496	,51.8212	,48.5015	,45.8023	,43.5431	,41.5884	,39.88		,38.3537	,36.9936	,35.7414	,34.6072	,33.5541,
    206.4175	,194.4276	,182.4477	,170.4577	,158.4766	,146.4967	,127.682	,122.5268	,110.5369	,98.5531	,86.5688	,74.584		,66.8341	,61.553		,57.5137	,54.2497	,51.5265	,49.194		,47.1249	,45.3087	,43.668		,42.1936	,40.8349,
    250.3934	,236.691	,222.8603	,209.1592	,195.4568	,181.6261	,163.428	,154.2312	,140.3919	,126.6895	,116.3142	,100.8176	,85.3228	,76.6185	,70.5711	,65.9331	,62.1807	,59.052		,56.3427	,53.9875	,51.8985	,50.0237	,48.3269,
    327.8165	,309.9521	,291.2953	,273.4409	,255.5786	,236.9271	,219.0574	,201.2031	,182.5462	,164.6839	,151.12		,137.7965	,108.543	,94.4945	,85.3276	,78.7759	,73.6858	,69.601		,66.0602	,63.0674	,60.4493	,58.1253	,56.0402,
    384.6647	,363.2975	,341.9027	,320.5455	,299.188	,277.7931	,256.426	,235.0688	,213.6739	,192.3164	,171.8651	,150.0074	,128.1614	,106.636	,95.1366	,87.0939	,81.0184	,76.1667	,72.1376	,68.7108	,65.737		,63.1132	,60.7719
};

static void delay_us(int t)
{
	while(t>0)
	{
		NOP();NOP();
		t--;
	}
}



int read_time_seg_data_param(void *buf,int len )
{
    return _24cxx_comps.read(MD_TIME_SEG_DATA_PARAM_START_ADDR,buf,len);
}

int save_time_seg_data_param(void const *buf,int len )
{
     return _24cxx_comps.write(MD_TIME_SEG_DATA_PARAM_START_ADDR,buf,len);
}

int read_system_time(void *buf,int len )
{
    return _24cxx_comps.read(MD_SYSTEM_TIME_START_ADDR,buf,len);
}

int save_system_time(void const *buf,int len )
{
     return _24cxx_comps.write(MD_SYSTEM_TIME_START_ADDR,buf,len);
}


int read_calibration_param(void *buf,int len )
{
    return _24cxx_comps.read(MD_CALIBRATION_PARAM_START_ADDR,buf,len);
}
int save_calibration_param(void const *buf,int len )
{
    return _24cxx_comps.write(MD_CALIBRATION_PARAM_START_ADDR,buf,len);
}

int read_res_calibration_param(void *buf,int len )
{
    return _24cxx_comps.read(MD_RES_CALIBRATION_PARAM_START_ADDR,buf,len);
}
int save_res_calibration_param(void const *buf,int len )
{
    return _24cxx_comps.write(MD_RES_CALIBRATION_PARAM_START_ADDR,buf,len);
}
int read_deltaP_calibration_param(void *buf,int len )
{
    return _24cxx_comps.read(MD_DELTAP_CALIBRATION_PARAM_START_ADDR,buf,len);
}
int save_deltaP_calibration_param(void const *buf,int len )
{
    return _24cxx_comps.write(MD_DELTAP_CALIBRATION_PARAM_START_ADDR,buf,len);
}

static int read_device_coe(void *buf,int len )
{
    return _24cxx_comps.read(MD_DEVICE_COE_START_ADDR,buf,len);
}

int save_device_coe(void const *buf,int len )
{
    return _24cxx_comps.write(MD_DEVICE_COE_START_ADDR,buf,len);
}


int read_air_leak_param(void *buf,int len )
{
    return _24cxx_comps.read(MD_TIME_AIR_LEAK_PARAM_START_ADDR,buf,len);
}

int save_air_leak_param(void *buf,int len )
{
    return _24cxx_comps.write(MD_TIME_AIR_LEAK_PARAM_START_ADDR,buf,len);
}

int read_report_param(void *buf,int len)
{
     return _24cxx_comps.read(MD_REPORT_PARAM_START_ADDR,buf,len);
}
int save_report_param(void const *buf,int len)
{
    return _24cxx_comps.write(MD_REPORT_PARAM_START_ADDR,buf,len);
}

     
int read_access_param(void *buf,int len)
{
     return _24cxx_comps.read(MD_ACCESS_PARAM_START_ADDR,buf,len);
}
int save_access_param(void const *buf,int len)
{
     return _24cxx_comps.write(MD_ACCESS_PARAM_START_ADDR,buf,len);
}


int read_alarm_param(void *buf,int len)
{
    return _24cxx_comps.read(MD_ALARM_PARAM_START_ADDR,buf,len);
}
 
int save_alarm_param(void const *buf,int len)
{
    return _24cxx_comps.write(MD_ALARM_PARAM_START_ADDR,buf,len);
}

  
int read_device_addr(void *buf,int len)
 {
     return _24cxx_comps.read(MD_DEVICE_ADDR_START_ADDR,buf,len);
 }
 int save_device_addr(void const *buf,int len)
 {
    return _24cxx_comps.write(MD_DEVICE_ADDR_START_ADDR,buf,len);
 }


int read_device_sensor(void *buf,int len)
{
    return _24cxx_comps.read(MD_DEVICE_SENSOR_START_ADDR,buf,len);
}
int save_device_sensor(void const *buf,int len)
{
    return _24cxx_comps.write(MD_DEVICE_SENSOR_START_ADDR,buf,len);
}

 int read_dpf_param(void *buf,int len)
 {
    return _24cxx_comps.read(MD_DPF_PARAM_START_ADDR,buf,len);
 }
 int save_dpf_param(void const *buf,int len)
{
     return _24cxx_comps.write(MD_DPF_PARAM_START_ADDR,buf,len);
}

int read_dpf_backup_data(void *buf,int len)
{
  return _24cxx_comps.read(MD_DPF_BACKUP_DATA_START_ADDR,buf,len);
}
int save_dpf_backup_data(void const *buf,int len)
{
  return _24cxx_comps.write(MD_DPF_BACKUP_DATA_START_ADDR,buf,len);
}


 int read_dpf_data(void *buf,int len)
 {
    unsigned int addr=MD_DPF_DATA_START_ADDR;
    addr=addr+device_comps.dpf_backup_data.store_index*8;
    return _24cxx_comps.read(MD_DPF_DATA_START_ADDR,buf,len);
 }
  int save_dpf_data(void const *buf,int len)
 {
    unsigned int addr=MD_DPF_DATA_START_ADDR;
    addr=addr+device_comps.dpf_backup_data.store_index*8;
    return _24cxx_comps.write(MD_DPF_DATA_START_ADDR,buf,len);
 }
 
 

 static void get_batt()
 {
     unsigned int adc;
     MD_SET_BAT_CTL_ON;
     delay_us(30);
     ADCE=0;
     delay_us(2);
     ADM2|=0x80;//switch internal vref
     ADM2&=~0x40;  //1.45V
     ADS = _00_AD_INPUT_CHANNEL_0;
     delay_us(10);
     ADCE=1;
     delay_us(2);
     
 
     device_comps.sw._bit.adc_busy=1;
     R_ADC_Start();
     while(device_comps.sw._bit.adc_busy);
     R_ADC_Get_Result(&adc);
     MD_SET_BAT_CTL_OFF;
     ADCE=0;
     
     ADCE=0;       //switch extern vref
     delay_us(2);
     ADM2&=~0xc0;
     delay_us(2);
		       
     
     
   #if(MD_DEVICE_BATT ==MD_3_6V)
     device_comps.batt=(unsigned long)435*adc/10230;//0.1V  20k 10k
   #elif(MD_DEVICE_BATT ==MD_4_5V)
     device_comps.batt=(unsigned long)580*adc/10230;//0.1V  30k 10k
   #endif
   hum_comps.dis_oper_mark._bit.refresh_batt_symbol=1;
 }



/////start ntc temp

static  long calc_ad3_average(device_comps_t *const this)
{
    int   i=0;
    const int count=this->ad3_pos;
    unsigned long average=0;
	for(i=0;i<count;i++)
	{
		average+=this-> ad3_convert_result[i];
	}
    average=average*10/count;	
    //free (period);
	return average;
}

static long calc_ntc_valve(device_comps_t *const this)
{
	long  r_std=10000;
	long  ad3_average=this->ad3_average_result;
	long  ntc_valve=(r_std*ad3_average)/(10231-ad3_average);
	return ntc_valve;
}

static int calc_temp(device_comps_t  *const this)
{
    long Tem;
    long LowRValue;
    long HighRValue;        
    long   Tem0;
    int i;
    int  cBottom, cTop;
    int limit=sizeof(temp_tab)/sizeof(temp_tab[0])-1;
    

    if (this->ntc_valve >temp_tab[0])                // 电阻值小于表格最值低于量程下限。
    {
         return -550;
    }
    if (this->ntc_valve < temp_tab[limit])        // 电阻值大于表格最大值，超出量程上限 。
    {
        return 1250;
    }
    cBottom = 0; 
    cTop    = limit;
     for (i=limit/2; (cTop-cBottom)!=1; )        // 2分法查表。
    {

       if (this->ntc_valve > temp_tab[i])
        {
            cTop = i;
            i = (cTop + cBottom) / 2;
        }
        else if (this->ntc_valve < temp_tab[i])
        {
            cBottom = i;
            i = (cTop + cBottom) / 2;
        }
        else
        {
            Tem0 = i * 10 - 550;
            Tem = Tem0;
            goto ret;
        }
    }
    Tem0 = i * 10 - 550;
    LowRValue  =temp_tab[cBottom];
    HighRValue = temp_tab[cTop];
    Tem = ( ((this->ntc_valve - LowRValue)*10) / (HighRValue - LowRValue) ) + Tem0;        // 表格是以5度为一步的。
ret:
    Tem=Tem*this->coe.temp/10000;
    return Tem;
}


////////////////////////////////end get ntc temperature////////////////////////////////////
////////////////////////////////end get ntc temperature////////////////////////////////////



static long calc_temp_p_temp_n_average(device_comps_t *const this)
{
    int   i=0;
    int   j=0;
    const int count=this->temp_p_pos;
    long  temp_var;
    long average=0;
	long difference[32]={0};//=malloc(count*sizeof(float));
	for(i=0;i<count;i++)
	{
		difference[i]=this-> temp_p_convert_result[i]-this-> temp_n_convert_result[i];
	}
    /*
    for(i=0;i<count-1;i++)
    {
        for(j=0;j<count-1-i;j++)
        {
        	if(difference[j]>difference[j+1])
        	{
        		temp_var=difference[j+1];
        		difference[j+1]=difference[j];
        		difference[j]=temp_var;
        	}
        }
    }
    */
    for(i=0;i<count;i++)
	{
		average+=difference[i];
	}	
	average=(average/(count));
	//free (period);
	return average;

}

static long calc_pt_valve(device_comps_t *const this)
{
    
	long delta_y=this->res_calibration_param.y[1]-this->res_calibration_param.y[0];
	long delta_x=this->res_calibration_param.x[1]-this->res_calibration_param.x[0];
    long pt_valve=(long long)delta_y*(this->temp_p_temp_n_average_result-this->res_calibration_param.x[0])/delta_x+this->res_calibration_param.y[0];
	return pt_valve;

}

static int calc_pt_temp(device_comps_t  *const this)
{
    long Tem;
    long LowRValue;
    long HighRValue;        
    long   Tem0;
    int i;
    int  cBottom, cTop;
    int  limit=sizeof(pt100_tab)/sizeof(pt100_tab[0])-1;
    

    if (this->pt_valve <pt100_tab[0])                // 电阻值小于表格最值低于量程下限。
    {
         return -600;
    }
    if (this->pt_valve >pt100_tab[limit])        // 电阻值大于表格最大值，超出量程上限 。
    {
        return 5990;
    }
    cBottom = 0; 
    cTop    = limit;
     for (i=limit/2; (cTop-cBottom)!=1; )        // 2分法查表。
    {

       if (this->pt_valve <pt100_tab[i])
        {
            cTop = i;
            i = (cTop + cBottom) / 2;
        }
        else if (this->pt_valve > pt100_tab[i])
        {
            cBottom = i;
            i = (cTop + cBottom) / 2;
        }
        else
        {
            Tem0 = i * 10 - 600;
            Tem = Tem0;
            goto ret;
        }
    }
    Tem0 = i * 10 - 600;
    LowRValue  =pt100_tab[cBottom];
    HighRValue = pt100_tab[cTop];
    Tem = ( ((this->pt_valve - LowRValue)*10) / (HighRValue - LowRValue) ) + Tem0;        // 表格是以5度为一步的。
ret:
    Tem=Tem*this->coe.temp/10000;
    return Tem;


}


/////////////////////////////////////////get press////////////////////////////////////





static long  calc_ad1_ad2_average(device_comps_t *const this)
{
    int   i=0;
    int   j=0;
    const int count=this->ad1_pos;
    long  temp_var;
    long average=0;
	long difference[32]={0};//=malloc(count*sizeof(float));
	for(i=0;i<count;i++)
	{
		difference[i]=this-> ad1_convert_result[i]-this-> ad2_convert_result[i];
	}
    /*for(i=0;i<count-1;i++)
    {
        for(j=0;j<count-1-i;j++)
        {
        	if(difference[j]>difference[j+1])
        	{
        		temp_var=difference[j+1];
        		difference[j+1]=difference[j];
        		difference[j]=temp_var;
        	}
        }
    }
    */
    for(i=0;i<count;i++)
	{
		average+=difference[i];
	}	
	average=(average/(count));
	//free (period);
	return average;

}
static long calc_deltaP_p_deltaP_n_average(device_comps_t *const this)
{
    int   i=0;
    int   j=0;
    const int count=this->deltaP_p_pos;
    long  temp_var;
    long  average=0;
	long  difference[32]={0};//=malloc(count*sizeof(float));
	for(i=0;i<count;i++)
	{
		difference[i]=this-> deltaP_p_convert_result[i]-this-> deltaP_n_convert_result[i];
	}
    /*for(i=0;i<count-1;i++)
    {
        for(j=0;j<count-1-i;j++)
        {
        	if(difference[j]>difference[j+1])
        	{
        		temp_var=difference[j+1];
        		difference[j+1]=difference[j];
        		difference[j]=temp_var;
        	}
        }
    }
    */
    for(i=0;i<count;i++)
	{
		average+=difference[i];
	}	
	average=(average/(count));
	//free (period);
	return average;
}
static long calc_current_deltaP(device_comps_t *const this)
{
    long delta_v= this->deltaP_p_deltaP_n_average_result;
	long press=0;

    long  LowTValue;
    long  HighTValue;        
    int   i;
    int  Bottom=0;
    int  Top=sizeof(this->deltaP_calibration_param.x)/sizeof(this->deltaP_calibration_param.x[0])-1; //cal 4 points 0-3
    if (delta_v<=device_comps.deltaP_calibration_param.x[Bottom])               
    {
       // Top=Bottom+1;
       // goto insert_calc;//jmp 2 points insert_calc code
        return press=device_comps.deltaP_calibration_param.y[Bottom];
    }
    else if (delta_v>device_comps.deltaP_calibration_param.x[Top])       
    {
        Bottom=Top-1;
        goto insert_calc;
    }
	i=Top/2;
	while(Top-Bottom>1)
	{
        if (delta_v<device_comps.deltaP_calibration_param.x[i])
        {
            Top = i;
            i = (Top + Bottom) / 2;
        }
        else if (delta_v >device_comps.deltaP_calibration_param.x[i])
        {
            Bottom = i;
            i = (Top + Bottom) / 2;
        }
        else
        {
            press = device_comps.deltaP_calibration_param.y[i];
            goto  ret;
        }
    }
insert_calc:
{
    
    LowTValue  = device_comps.deltaP_calibration_param.x[Bottom];
    HighTValue = device_comps.deltaP_calibration_param.x[Top];
    press =(long)(
    ((long  long) (delta_v-LowTValue)*(device_comps.deltaP_calibration_param.y[Top]-device_comps.deltaP_calibration_param.y[Bottom]))
    /(HighTValue - LowTValue)
    )
    +device_comps.deltaP_calibration_param.y[Bottom];
}
ret:
    press-=this->coe.press_clr_value;
	if(press>this->deltaP_calibration_param.y[3]*1/100)//>fs*1/100.
	{
		press;
	}
	else
	{
		press=0;
	}
	return press*(this->coe.deltaP/10000.);
}

static  long calc_current_press(device_comps_t *const this)
{
    long delta_v= this->ad1_ad2_average_result;
	long press=0;

    long  LowTValue;
    long  HighTValue;        
    int   i;
    int  Bottom=0;
    int  Top=sizeof(this->calibration_param.x)/sizeof(this->calibration_param.x[0])-1; //cal 4 points 0-3
    if (delta_v<=device_comps.calibration_param.x[Bottom])               
    {
       // Top=Bottom+1;
       // goto insert_calc;//jmp 2 points insert_calc code
        return press=device_comps.calibration_param.y[Bottom];
    }
    else if (delta_v>device_comps.calibration_param.x[Top])       
    {
        Bottom=Top-1;
        goto insert_calc;
    }
	i=Top/2;
	while(Top-Bottom>1)
	{
        if (delta_v<device_comps.calibration_param.x[i])
        {
            Top = i;
            i = (Top + Bottom) / 2;
        }
        else if (delta_v >device_comps.calibration_param.x[i])
        {
            Bottom = i;
            i = (Top + Bottom) / 2;
        }
        else
        {
            press = device_comps.calibration_param.y[i];
            goto  ret;
        }
    }
insert_calc:
{
    
    LowTValue  = device_comps.calibration_param.x[Bottom];
    HighTValue = device_comps.calibration_param.x[Top];
    press =(long)(
    ((long long) (delta_v-LowTValue)*(device_comps.calibration_param.y[Top]-device_comps.calibration_param.y[Bottom]))
    /(HighTValue - LowTValue)
    )
    +device_comps.calibration_param.y[Bottom];
}
ret:
    press-=this->coe.press_clr_value;
	if(press>this->calibration_param.y[3]*2/1000)//>fs*1/100.
	{
		press;
	}
	else
	{
		press=0;
	}
	return press*(this->coe.press/10000.);
}


static float fpwr(int dot)
{
    dot%=7;
    if(dot==0) {return 1.     ;}
    if(dot==1) {return 10.    ;}
    if(dot==2) {return 100.   ;}
    if(dot==3) {return 1000.  ;}
    if(dot==4) {return 10000. ; }
    if(dot==5) {return 100000. ;}
    if(dot==6) {return 1000000.;}
    return 1.;
}

long lpwr(int dot)
{
    
    if(dot==0) {return 1     ;}
    if(dot==1) {return 10    ;}
    if(dot==2) {return 100   ;}
    if(dot==3) {return 1000  ;}
    return 1;
}

static float get_Sated_vapor_density_by_temp(long temp) 
{

    float density=0;
    long  LowTValue;
    long  HighTValue;        
    int   i;
    int  Bottom=0;
    int  Top=sizeof(Sated_vapor_temp_table)/sizeof(Sated_vapor_temp_table[0])-1; //cal 4 points 0-3
    if (temp<Sated_vapor_temp_table[Bottom])               
    {
       // Top=Bottom+1;
       // goto insert_calc;//jmp 2 points insert_calc code
        return density=Sated_vapor_density_table[Bottom];
    }
    else if (temp>Sated_vapor_temp_table[Top])       
    {
       // Bottom=Top-1;
        //goto insert_calc;
        return density=Sated_vapor_density_table[Top];
    }
	i=Top/2;
	while(Top-Bottom>1)
	{
        if (temp<Sated_vapor_temp_table[i])
        {
            Top = i;
            i = (Top + Bottom) / 2;
        }
        else if (temp >Sated_vapor_temp_table[i])
        {
            Bottom = i;
            i = (Top + Bottom) / 2;
        }
        else
        {
            density = Sated_vapor_density_table[i];
            goto  ret;
        }
    }
insert_calc:
{
    
    LowTValue  = Sated_vapor_temp_table[Bottom];
    HighTValue = Sated_vapor_temp_table[Top];
    density =(
    ((temp-LowTValue)*(Sated_vapor_density_table[Top]-Sated_vapor_density_table[Bottom]))
    /(HighTValue - LowTValue)
    )
    +Sated_vapor_density_table[Bottom];
}
ret:
 
	return density;

}

static float get_Sated_vapor_density_by_press(long press,float PA) 
{
    float  density=0;
    float  LowTValue;
    float  HighTValue;
    int    i;
    
    
    int  Bottom=0;
    int  Top=sizeof(Sated_vapor_press_table)/sizeof(Sated_vapor_press_table[0])-1; //cal 4 points 0-3
    float  press_abs=press/fpwr(device_comps.calibration_param.dot);
    if(device_comps.calibration_param.unit!=0)
    {
        press_abs=press_abs/1000;
    }
    press_abs=press_abs+PA;
    if (press_abs<Sated_vapor_press_table[Bottom])               
    {
       // Top=Bottom+1;
       //goto insert_calc;//jmp 2 points insert_calc code
       return Sated_vapor_density_table[Bottom];
        
    }
    else if (press_abs>Sated_vapor_press_table[Top])       
    {
       //Bottom=Top-1;
      // goto insert_calc;
       return Sated_vapor_density_table[Top];
       
    }
	else 
	{
	    i=Top/2;
    	while(Top-Bottom>1)
    	{
            if (press_abs<Sated_vapor_press_table[i])
            {
                Top = i;
                i = (Top + Bottom) / 2;
            }
            else if (press_abs >Sated_vapor_press_table[i])
            {
                Bottom = i;
                i = (Top + Bottom) / 2;
            }
            else
            {
                Bottom=i;
                Top=i+1;
                break;
            }
        }
    }
insert_calc:
{
    
    LowTValue  = Sated_vapor_press_table[Bottom];
    HighTValue = Sated_vapor_press_table[Top];
    density =(
    ((press_abs-LowTValue)*(Sated_vapor_density_table[Top]-Sated_vapor_density_table[Bottom]))
    /(HighTValue - LowTValue)
    )
    +Sated_vapor_density_table[Bottom];
}
ret:
 
	return density;

}

static float get_Suped_vapor_density_by_PT(long temp,long press,float PA)
{
  
    float  density=0;
    float  LowTValue;
    float  HighTValue;
    int    i;

    int Bottom_t=0;
    int Top_t=sizeof(Suped_vapor_temp_table)/sizeof(Suped_vapor_temp_table[0])-1;
    

    int  Bottom=0;
    int  Top=sizeof(Suped_vapor_press_table)/sizeof(Suped_vapor_press_table[0])-1; //cal 4 points 0-3
    float  press_abs=press/fpwr(device_comps.calibration_param.dot);
    if(device_comps.calibration_param.unit!=0)
    {
        press_abs=press_abs/1000;
    }
    press_abs=press_abs+PA;
    if (press_abs<Suped_vapor_press_table[Bottom])               
    {
        Top=Bottom+1;
       //goto insert_calc;//jmp 2 points insert_calc code
        
    }
    else if (press_abs>Suped_vapor_press_table[Top])       
    {
       Bottom=Top-1;
      // goto insert_calc;
       
    }
	else 
	{
	    i=Top/2;
    	while(Top-Bottom>1)
    	{
            if (press_abs<Suped_vapor_press_table[i])
            {
                Top = i;
                i = (Top + Bottom) / 2;
            }
            else if (press_abs >Suped_vapor_press_table[i])
            {
                Bottom = i;
                i = (Top + Bottom) / 2;
            }
            else
            {
                Bottom=i;
                Top=i+1;
                break;
            }
        }
    }


    if (temp<Suped_vapor_temp_table[Bottom_t])               
    {
        Top_t=Bottom_t+1;
        temp=Suped_vapor_temp_table[Bottom_t];
       // goto insert_calc;//jmp 2 points insert_calc code
       // return density=Sated_vapor_density_table[Bottom_t];
    }
    else if (temp>Suped_vapor_temp_table[Top_t])       
    {
        Bottom_t=Top_t-1;
        //goto insert_calc;
       // return density=Sated_vapor_density_table[Top_t];
    }
	i=Top_t/2;
	while(Top_t-Bottom_t>1)
	{
        if (temp<Suped_vapor_temp_table[i])
        {
            Top_t = i;
            i = (Top_t + Bottom_t) / 2;
        }
        else if (temp >Suped_vapor_temp_table[i])
        {
            Bottom_t = i;
            i = (Top_t + Bottom_t) / 2;
        }
        else
        {
             Bottom_t=i;
             Top_t=i+1;
             break;
        }
    }
    
    
insert_calc:
{   
    float fp1;
    float fp2;
    float x1_p=Suped_vapor_press_table[Bottom];
    float x2_p=Suped_vapor_press_table[Top];
    
    float y1_p=  Suped_vapor_density_table[Bottom][Bottom_t];
    float y2_p=  Suped_vapor_density_table[Top][Bottom_t];
    fp1=(y2_p-y1_p)*(press_abs-x1_p)/(x2_p-x1_p)+y1_p;

    y1_p=  Suped_vapor_density_table[Bottom][Top_t];
    y2_p=  Suped_vapor_density_table[Top][Top_t];
    fp2=(y2_p-y1_p)*(press_abs-x1_p)/(x2_p-x1_p)+y1_p;

    density=(fp2-fp1)*(temp-Suped_vapor_temp_table[Bottom_t])/(Suped_vapor_temp_table[Top_t]-Suped_vapor_temp_table[Bottom_t])+fp1;
}
ret:
 
	return density;



}

static  long dpf_calc_current_flow(device_comps_t *const this)
{

    float p;
    float P;
    float T;
    float fdelta_p;
    float A1;
    float A2;
    float M;
    float p20;
    float PA;
    long delta_p= this->current_deltaP;
    
    float k=0;
    long flow=0;
    long  LowTValue;
    long  HighTValue;        
    int   i;
    int  Bottom=0;
    int  Top=sizeof(this->dpf_param.k)/sizeof(this->dpf_param.k[0])-1; //cal 4 points 0-3

    
    if(delta_p==0)
    {
        return flow=0;
    }
    
    if (delta_p<device_comps.deltaP_calibration_param.y[0])               
    {
        Top=Bottom+1;
        goto insert_calc;//jmp 2 points insert_calc code
       // return k=0;
    }
    else if (delta_p>device_comps.deltaP_calibration_param.y[Top])       
    {
        Bottom=Top-1;
        goto insert_calc;
    }
	i=Top/2;
	while(Top-Bottom>1)
	{
        if (delta_p<device_comps.deltaP_calibration_param.y[i])
        {
            Top = i;
            i = (Top + Bottom) / 2;
        }
        else if (delta_p >device_comps.deltaP_calibration_param.y[i])
        {
            Bottom = i;
            i = (Top + Bottom) / 2;
        }
        else
        {
            k = device_comps.dpf_param.k[i]%10000000/fpwr(device_comps.dpf_param.k[i]/10000000%10);
            goto  ret;
        }
    }
insert_calc:
    {
        
        LowTValue  = device_comps.deltaP_calibration_param.y[Bottom];
        HighTValue = device_comps.deltaP_calibration_param.y[Top];
        k =(
        ((delta_p-LowTValue)
        *(device_comps.dpf_param.k[Top]%10000000/fpwr(device_comps.dpf_param.k[Top]/10000000%10)
        - device_comps.dpf_param.k[Bottom]%10000000/fpwr(device_comps.dpf_param.k[Bottom]/10000000%10)))
        /(HighTValue - LowTValue)
        )
        +device_comps.dpf_param.k[Bottom]%10000000/fpwr(device_comps.dpf_param.k[Bottom]/10000000%10);
    }
ret:
    {
      
        T=device_comps.current_temp/10.;
        P=device_comps.current_press/fpwr(device_comps.calibration_param.dot);
        PA=device_comps.dpf_param.PA%10000000/fpwr(device_comps.dpf_param.PA/10000000%10);
        fdelta_p=device_comps.current_deltaP/fpwr(device_comps.deltaP_calibration_param.dot);
        if(device_comps.calibration_param.unit!=0)
        {
            P=P/1000;
        }

        if(device_comps.dpf_param.bT==0)//other jiezhi
        {
            if(device_comps.dpf_param.tE&&device_comps.dpf_param.pE)
            {
               
                p20=device_comps.dpf_param.p20%10000000/fpwr(device_comps.dpf_param.p20/10000000%10);
                
                p=p20*(273.15+20)*(P+PA)/((T+273.15)*0.10133);
               
            }
            else if(device_comps.dpf_param.tE)
            {
                A1=device_comps.dpf_param.A[0]%10000000/fpwr(device_comps.dpf_param.A[0]/10000000%10);
                A2=device_comps.dpf_param.A[1]%10000000/fpwr(device_comps.dpf_param.A[1]/10000000%10);
                if(device_comps.dpf_param.A[0]/100000000)
                {
                    A1=-A1;
                }
                if(device_comps.dpf_param.A[1]/100000000)
                {
                    A2=-A2;
                }
                p=A1+A2*T;
            }
            else if(device_comps.dpf_param.pE)
            {
                A1=device_comps.dpf_param.A[0]%10000000/fpwr(device_comps.dpf_param.A[0]/10000000%10);
                A2=device_comps.dpf_param.A[1]%10000000/fpwr(device_comps.dpf_param.A[1]/10000000%10);
                if(device_comps.dpf_param.A[0]/100000000)
                {
                    A1=-A1;
                }
                if(device_comps.dpf_param.A[1]/100000000)
                {
                    A2=-A2;
                }
                p=A1+A2*(P+PA);
            }
            else
            {
                p=device_comps.dpf_param.p%10000000/fpwr(device_comps.dpf_param.p/10000000%10);
            }
        }

    	else if(device_comps.dpf_param.bT==1)//1 Saturated steam,
        {
            if(device_comps.dpf_param.tE)
            {
                 p=get_Sated_vapor_density_by_temp(this->current_temp);

            }
            else if(device_comps.dpf_param.pE)
            {
                 p=get_Sated_vapor_density_by_press(this->current_press,PA);
            }
            else
            {
                 p=device_comps.dpf_param.p%10000000/fpwr(device_comps.dpf_param.p/10000000%10);
            }
        }
        else if(device_comps.dpf_param.bT==2)//1 superheated steam,
        {
            if(device_comps.dpf_param.tE&&device_comps.dpf_param.pE)
            {
                p=get_Suped_vapor_density_by_PT(this->current_temp,this->current_press,PA);
            }
            else
            {
                  p=device_comps.dpf_param.p%10000000/fpwr(device_comps.dpf_param.p/10000000%10);
            }
        }
        if(k<0)
        {
            return flow=0;
        }
        else
        {
            if(p<0)
            {
                return flow=0;
            }
            M=k*sqrt(p*fdelta_p);
            device_comps.dpf_param.Fd%=4;
            if(device_comps.dpf_param.Fd==0)
            {
                flow=M;
            }
            else if(device_comps.dpf_param.Fd==1)
            {
                flow=M*10;
            }
            else if(device_comps.dpf_param.Fd==2)
            {
                flow=M*100;
            }
            else if(device_comps.dpf_param.Fd==3)
            {
                flow=M*1000;
            }
            return flow*(this->coe.flow/10000.);
        }
    }

}
static long check_total_to_zero(long total,int dot)
{
    long var;
    var=total*lpwr(dot)+(long)(device_comps.dpf_total_dec_part*lpwr(dot));
    while(var>999999999)
    {
        var-=1000000000;
    }
    total=var/lpwr(dot);
    device_comps.dpf_total_dec_part=device_comps.dpf_total_dec_part+var%lpwr(dot)/(float)lpwr(dot);
    return total;
}
static long dpf_calc_total(device_comps_t *const this)
{
    static int timer=0;
    dpf_data_t dpf_data;
    long total=this->dpf_data.total;
  if(this->current_flow==0)
    {
        return total;
    }
    this->dpf_total_dec_part+=this->current_flow/(3600*fpwr(device_comps.dpf_param.Fd));
    if(this->dpf_total_dec_part>1)
    {
        total=total+(long)this->dpf_total_dec_part;
        this->dpf_total_dec_part=this->dpf_total_dec_part-(long)this->dpf_total_dec_part;
        total=check_total_to_zero(total,device_comps.dpf_param.Td);
        device_comps.dpf_data.total=total;
        device_comps.dpf_data.cs=Check_Sum_5A(&device_comps.dpf_data, & device_comps.dpf_data.cs-(unsigned char *)&device_comps.dpf_data);
	 device_comps.save_dpf_data(&device_comps.dpf_data,sizeof(device_comps.dpf_data));
        device_comps.read_dpf_data(&dpf_data,sizeof(dpf_data));
	if(memcmp(&dpf_data,&device_comps.dpf_data,sizeof(dpf_data)))
        {
            if(device_comps.dpf_backup_data.store_index<30)
            {
               device_comps.dpf_backup_data.store_index++;
               device_comps.save_dpf_data(&device_comps.dpf_data,sizeof(device_comps.dpf_data));
               
               device_comps.dpf_backup_data.total=total;;
               device_comps.dpf_backup_data.cs=Check_Sum_5A(&device_comps.dpf_backup_data, & device_comps.dpf_backup_data.cs-(unsigned char *)&device_comps.dpf_backup_data);
               device_comps.save_dpf_backup_data(&device_comps.dpf_backup_data,sizeof(device_comps.dpf_backup_data));

            }
	}

        if(++timer>3600)
        {
            timer=0;
            device_comps.dpf_backup_data.total=total;;
            device_comps.dpf_backup_data.cs=Check_Sum_5A(&device_comps.dpf_backup_data, & device_comps.dpf_backup_data.cs-(unsigned char *)&device_comps.dpf_backup_data);
            device_comps.save_dpf_backup_data(&device_comps.dpf_backup_data,sizeof(device_comps.dpf_backup_data));
        }
        
    }
    return total;
}



//Algorithm: The actual measured value is less than 4% of full scale and can be cleared. 
//The actual value = current value + cleared value. 
//Because the current value = actual value-clear value

static int clr_press(void)
{
    long actual_value=device_comps.current_press+device_comps.coe.press_clr_value;
    int ret=0;
    if(actual_value>device_comps.calibration_param.y[3]*4/100)//
    {
        ret=1;
    }
    else
    {
        device_comps.coe.press_clr_value=actual_value;
        device_comps.coe.cs=Check_Sum_5A(&device_comps.coe, &device_comps.coe.cs-(unsigned char *)&device_comps.coe);
        device_comps.save_coe(&device_comps.coe,sizeof(device_comps.coe));
    }
    return ret;
}
////////////////////////////////end get press////////////////////////////////////



static void device_comps_output_debug_info(device_comps_t const *const this)
{
//	static int line_num=0;
//	int tx_num=0;
//	memset(this->debug_info,0,sizeof(this->debug_info));
//	if(line_num==0)
//	{
//		
//		//start output attribute name(title)
//		sprintf(this->debug_info+strlen(this->debug_info),"AD1-AD2\t\t");//
//		sprintf(this->debug_info+strlen(this->debug_info),"ad2_pos\t\t");//
//		sprintf(this->debug_info+strlen(this->debug_info),"AD3\t\t");//signal_ferq_from_timer_ch0 extern event counter 
//		sprintf(this->debug_info+strlen(this->debug_info),"ad3_pos\r\n");//signal_freq
//		//end output attribute name(title)
//	}
//	else
//	{
//		sprintf(this->debug_info+strlen(this->debug_info),"%05ld\t\t",this->ad1_ad2_average_result);
//		sprintf(this->debug_info+strlen(this->debug_info),"%05ld\t\t",this->ad2_pos);
//		sprintf(this->debug_info+strlen(this->debug_info),"%05ld\t\t",this->ad3_average_result);
//		sprintf(this->debug_info+strlen(this->debug_info),"%05ld\r\n",this->ad3_pos);
//		
//	}
//	line_num++;
//	if(line_num>=10)//Output attribute name(title) every 50 lines
//	{
//		line_num=0; 
//	}
//	tx_num=strlen(this->debug_info);
//	if(tx_num>=sizeof(this->debug_info))
//	{
//		memset(this->debug_info,0,sizeof(this->debug_info));
//		sprintf(this->debug_info,"Write sensor debug output buffer overflow\r\n"); 
//		tx_num=strlen(this->debug_info);
//	}
//	ircComps.write(this->debug_info,tx_num);
	
	
}




static unsigned char device_comps_init(device_comps_t *const this)
{
	/**************START E2PROM TEST********************/
	if(device_comps.sw._bit.e2prom_driver_err)	
	{
		char msg[32]="";// ep2rom low 32bytes use test
		_24cxx_comps.write(MD_MEMORY_TEST_START_ADDR,"this is a e2prom driver test",strlen("this is a e2prom driver test"));
		_24cxx_comps.read (MD_MEMORY_TEST_START_ADDR,msg,strlen("this is a e2prom driver test"));
		if(!strcmp(msg,"this is a e2prom driver test"))
		{
			device_comps.sw._bit.e2prom_driver_err=0;	
		}
		else
		{
			device_comps.sw._bit.e2prom_driver_err=1;
		}
	}
	if(device_comps.sw._bit.cs123x_driver_err)	
	{
		if(!cs123x_comps.Init(0x74))//74
		{
            device_comps.sw._bit.cs123x_driver_err=0;
		}
		else
		{
            device_comps.sw._bit.cs123x_driver_err=1;
		}
	}
	if(device_comps.sw._bit.cs1237_driver_err)	
	{
		if(!cs1237_comps.Init(0x7c))//7c
		{
            device_comps.sw._bit.cs1237_driver_err=0;
		}
		else
		{
            device_comps.sw._bit.cs1237_driver_err=1;
		}
	}
	
	
	if(device_comps.sw._bit.rtc_module_err)	
	{
	    if(systemComps.sw._bit.is_xt1_running)	
    	{
    	    device_comps.sw._bit.rtc_module_err=0;
    	}
	}
	
	if(device_comps.sw._bit.lora_module_err)	
	{
	    if(loraComps.sw._bit.init_ok)	
    	{
    	    device_comps.sw._bit.lora_module_err=0;
    	}
	}
	
    if(device_comps.sw._bit.pcf857x_driver_err)  
    {
        if(!pcf857x_comps.Init())
        {
            device_comps.sw._bit.pcf857x_driver_err=0;
        }
    }

    if(device_comps.sw._bit.ad54x0_driver_err)
	{
		unsigned int valve=1;
		unsigned int valve_read_back;
		ad54x0_comps.write_reset_reg(&valve);
		/**************************************************************************************
		DB15 	DB14   DB13   DB12    DB11 DB10 DB9 DB8	  DB7 DB6 DB5  DB4   DB3   DB2  DB1  DB0
	       0     0     REXT   OUTEN         R(CLK)          SR(STEP)  SREN   DCEN   R2 	 R1   R0
		   0     0      0       1      1    1   1    1     1   1   1   1      0     1   0    1(4-20ma)
		***************************************************************************************/
		valve=0x1ff5;
		ad54x0_comps.write_control_reg(&valve);
		ad54x0_comps.read_control_reg(&valve_read_back);
		if(valve==valve_read_back)
		{
			device_comps.sw._bit.ad54x0_driver_err=0;
		}
		
	}

	if(!(device_comps.sw.All&0xff))
	{
		return 0;
	}
	else
	{
		return 1;
	}
	 
}


static void check_air_param()
{
//    if(device_comps.air_leak_param.unit>3)
//    {
//        device_comps.air_leak_param.unit=0;
//    }
//    if(device_comps.air_leak_param.Constant_pressure_value>99999)
//    {
//        device_comps.air_leak_param.Constant_pressure_value=0;
//    }
//     if(device_comps.air_leak_param.buck_delta_value>99999)
//    {
//        device_comps.air_leak_param.buck_delta_value=0;
//    }
//    if(device_comps.air_leak_param.buck_percent>999)
//    {
//        device_comps.air_leak_param.buck_percent=0;
//    }
//    if(device_comps.air_leak_param.work_mode>2)
//    {
//        device_comps.air_leak_param.work_mode=0;
//    }
//     if(device_comps.air_leak_param.count_direction>1)
//    {
//        device_comps.air_leak_param.count_direction=0;
//    }
//    if(device_comps.air_leak_param.delay_timer>9999)
//    {
//        device_comps.air_leak_param.delay_timer=0;
//    }
//    if(device_comps.air_leak_param.timer>9999)
//    {
//        device_comps.air_leak_param.timer=0;
//    }
}
static void read_all_param(struct _DEVICE_COMPONENTS  *const this)
{
    if(!device_comps.sw._bit.e2prom_driver_err)
    {
//        if(!read_system_time(&device_comps.system_time,sizeof(device_comps.system_time)))
//        {
//            if(device_comps.system_time.cs!=Check_Sum_5A(&device_comps.system_time, &device_comps.system_time.cs-(unsigned char *)&device_comps.system_time))
//            {
//                device_comps.system_time.year=0x20;
//                device_comps.system_time.month=1;
//                device_comps.system_time.day=1;
//                device_comps.system_time.hour=0;
//                device_comps.system_time.min=0;
//                device_comps.system_time.sec=0;
//	    }
	    
//	    rtc_valve.year=device_comps.system_time.year;
//            rtc_valve.month=device_comps.system_time.month;
//            rtc_valve.day=device_comps.system_time.day;
//            rtc_valve.hour=device_comps.system_time.hour;
//            rtc_valve.min=device_comps.system_time.min;
//            rtc_valve.sec=device_comps.system_time.sec;
//            R_RTC_Set_CounterValue(rtc_valve);	
//        }
        
//        if(!read_time_seg_data_param(&device_comps.TimeSegData,sizeof(device_comps.TimeSegData)))
//        {
//            if(device_comps.TimeSegData.cs!=Check_Sum_5A(&device_comps.TimeSegData, &device_comps.TimeSegData.cs-(unsigned char *)&device_comps.TimeSegData))
//            {
//                device_comps.TimeSegData.store_addr=MD_TIME_SEG_DATA_START_ADDR;
//            }
//        }

        if(!read_calibration_param(&device_comps.calibration_param,sizeof(device_comps.calibration_param)))
        {
            if(device_comps.calibration_param.cs!=Check_Sum_5A(&device_comps.calibration_param, & device_comps.calibration_param.cs-(unsigned char *)&device_comps.calibration_param))
            {
                device_comps.calibration_param.is_calibrated=0;
                memset(&device_comps.calibration_param,0,sizeof(device_comps.calibration_param));
            }
        }
        if(!read_res_calibration_param(&device_comps.res_calibration_param,sizeof(device_comps.res_calibration_param)))
        {
            if(device_comps.res_calibration_param.cs!=Check_Sum_5A(&device_comps.res_calibration_param, & device_comps.res_calibration_param.cs-(unsigned char *)&device_comps.res_calibration_param))
            {
                device_comps.res_calibration_param.is_calibrated=0;
                memset(&device_comps.res_calibration_param,0,sizeof(device_comps.res_calibration_param));
            }
        }
        if(!read_deltaP_calibration_param(&device_comps.deltaP_calibration_param,sizeof(device_comps.deltaP_calibration_param)))
        {
            if(device_comps.deltaP_calibration_param.cs!=Check_Sum_5A(&device_comps.deltaP_calibration_param, & device_comps.deltaP_calibration_param.cs-(unsigned char *)&device_comps.deltaP_calibration_param))
            {
                device_comps.deltaP_calibration_param.is_calibrated=0;
                memset(&device_comps.deltaP_calibration_param,0,sizeof(device_comps.deltaP_calibration_param));
            }
        }
//        if(!read_air_leak_param(&device_comps.air_leak_param,sizeof(device_comps.air_leak_param)))
//        {
//            if(device_comps.air_leak_param.cs!=Check_Sum_5A(&device_comps.air_leak_param, & device_comps.air_leak_param.cs-(unsigned char *)&device_comps.air_leak_param))
//            {
//                memset(&device_comps.air_leak_param,0,sizeof(device_comps.air_leak_param));
//            }
//            check_air_param();
           
//        }
        if(!device_comps.read_coe(&device_comps.coe,sizeof(device_comps.coe)))
        {
            if(device_comps.coe.cs!=Check_Sum_5A(&device_comps.coe, &device_comps.coe.cs-(unsigned char *)&device_comps.coe))
            {
                device_comps.coe.press=10000;
                device_comps.coe.temp=10000;
                 device_comps.coe.deltaP=10000;
                device_comps.coe.flow=10000;
                device_comps.coe.press_clr_value=0;
            }
            if(device_comps.coe.press<7000||device_comps.coe.press>13000)
            {
                device_comps.coe.press=10000;
            }
             if(device_comps.coe.temp<7000||device_comps.coe.temp>13000)
            {
                device_comps.coe.temp=10000;
            }
            if(device_comps.coe.deltaP<7000||device_comps.coe.deltaP>13000)
            {
                device_comps.coe.deltaP=10000;
            }
             if(device_comps.coe.flow<7000||device_comps.coe.flow>13000)
            {
                device_comps.coe.flow=10000;
            }
            if(device_comps.coe.press_clr_value>device_comps.calibration_param.y[3]*4/100)
            {
                device_comps.coe.press_clr_value=0;
            }
        }
//        if(!read_alarm_param(&device_comps.alarm_param,sizeof(device_comps.alarm_param)))
//        {
//            if(device_comps.alarm_param.cs!=Check_Sum_5A(&device_comps.alarm_param, & device_comps.alarm_param.cs-(unsigned char *)&device_comps.alarm_param))
//            {
//                memset(&device_comps.alarm_param,0,sizeof(device_comps.alarm_param));
//            }
//        }

        if(!read_report_param(&device_comps.report_param,sizeof(device_comps.report_param)))
        {
            if(device_comps.report_param.cs!=Check_Sum_5A(&device_comps.report_param, & device_comps.report_param.cs-(unsigned char *)&device_comps.report_param))
            {
                device_comps.report_param.min=0;
                device_comps.report_param.hour=3;
                device_comps.report_param.hour_Interval=25;
                device_comps.report_param.disFactor=60;
            }
        }

        if(!read_device_addr(&device_comps.device_addr,sizeof(device_comps.device_addr)))
        {
            if(device_comps.device_addr.cs!=Check_Sum_5A(&device_comps.device_addr, & device_comps.device_addr.cs-(unsigned char *)&device_comps.device_addr))
            {
                const unsigned char default_addr[7]={0x61,0x05,0x01,0x00,0x00,0x00,0x00};
                memcpy(&device_comps.device_addr.addr[0],default_addr,sizeof(device_comps.device_addr.addr));
            }
        }
//        if(!read_device_sensor(&device_comps.device_sensor,sizeof(device_comps.device_sensor)))
//        {
//            if(device_comps.device_sensor.cs!=Check_Sum_5A(&device_comps.device_sensor, & device_comps.device_sensor.cs-(unsigned char *)&device_comps.device_sensor))
//            {
//                memset(&device_comps.device_sensor.ser_num[0],0,sizeof(device_comps.device_sensor.ser_num));
//            }
//        }
        if(!read_access_param(&device_comps.access_param,sizeof(device_comps.access_param)))
        {
            if(device_comps.access_param.cs!=Check_Sum_5A(&device_comps.access_param, &device_comps.access_param.cs-(unsigned char *)&device_comps.access_param))
            {
                access_param_t default_access_param=
                {
                    //{180,101,147,115},
                    {117,60,157,137},
                    5683,
                    0
                };
                 memcpy(&device_comps.access_param,&default_access_param,sizeof(device_comps.access_param));
            }
         }
        if(!read_dpf_param(&device_comps.dpf_param,sizeof(device_comps.dpf_param)))
        {
            if(device_comps.dpf_param.cs!=Check_Sum_5A(&device_comps.dpf_param, & device_comps.dpf_param.cs-(unsigned char *)&device_comps.dpf_param))
            {
                device_comps.dpf_param.k[0]=device_comps.dpf_param.k[1]=device_comps.dpf_param.k[2]=device_comps.dpf_param.k[3]=61000000;
                device_comps.dpf_param.A[0]=device_comps.dpf_param.A[1]=61000000;
                device_comps.dpf_param.p=device_comps.dpf_param.p20=61000000;
                device_comps.dpf_param.PA=50010133;
                device_comps.dpf_param._4_20ma_limit[0]=0;
                device_comps.dpf_param._4_20ma_limit[1]=0;
                device_comps.dpf_param._4_20ma_dir=0;
                device_comps.dpf_param.bT=0;
                 device_comps.dpf_param.tE=0;
                device_comps.dpf_param.pE=0;
                device_comps.dpf_param.Fu=0;
                device_comps.dpf_param.Fd=3;
                device_comps.dpf_param.Td=3;
                
            }
        }
        if(!read_dpf_backup_data(&device_comps.dpf_backup_data,sizeof(device_comps.dpf_backup_data)))
        {
            if(device_comps.dpf_backup_data.cs!=Check_Sum_5A(&device_comps.dpf_backup_data, & device_comps.dpf_backup_data.cs-(unsigned char *)&device_comps.dpf_backup_data))
            {
                device_comps.dpf_backup_data.store_index=0;
                device_comps.dpf_backup_data.total=0;
            }
        }
        if(!read_dpf_data(&device_comps.dpf_data,sizeof(device_comps.dpf_data)))
        {
            if(device_comps.dpf_data.cs!=Check_Sum_5A(&device_comps.dpf_data, & device_comps.dpf_data.cs-(unsigned char *)&device_comps.dpf_data))
            {
               device_comps.dpf_data.total=device_comps.dpf_backup_data.total;
            }
         }        
    }
    else
    {
        device_comps.calibration_param.is_calibrated=0;
        device_comps.res_calibration_param.is_calibrated=0;
        device_comps.deltaP_calibration_param.is_calibrated=0;
    }
}

//only use the highlow and low_upper 
static void pressOverloadReport(struct _DEVICE_COMPONENTS  *const this )
{
//    long temp=0;
//    if((this->calibration_param.unit&0x0f)==this->alarm_param.unit)
//    {
//        if(this->alarm_param.press_high_lower>0)
//        {
//            temp=formatData4fixDot(this->current_press);
//            if(temp>this->alarm_param.press_high_lower)
//            {
//                this->PHihgOverTimer++;
//                this->PHihgRealseTimer=0;
//                if( this->PHihgOverTimer>=40)
//                {
//                    if(!this->sw._bit.isPHighOverTriggered)
//                    {
//                        if((device_comps.device_addr.addr[4]!=0)&&(device_comps.device_addr.addr[3]!=0))
//            	        {
//                            if(device_comps.batt>30)
//                            {
////                                protocolComps.triggerIrq._bit.PHighOver=1;
//                                this->sw._bit.isPHighRealseTriggered=0;
//                                this->sw._bit.isPHighOverTriggered=1;
//                            }
//            	        }
//                    }
//                    this->PHihgOverTimer=0;
//                }
//            }
//            else if((temp<this->alarm_param.press_high_lower-this->alarm_param.press_high_lower/10)&&(this->sw._bit.isPHighOverTriggered))//dealt 10%
//            {
//                this->PHihgOverTimer=0;
//                this->PHihgRealseTimer++;
//                if( this->PHihgRealseTimer>=40)
//                {
//                    if(!this->sw._bit.isPHighRealseTriggered)
//                    {
//                        if((device_comps.device_addr.addr[4]!=0)&&(device_comps.device_addr.addr[3]!=0))
//            	        {
//                            if(device_comps.batt>30)
//                            {
// //                               protocolComps.triggerIrq._bit.PHighRealse=1;
//                                this->sw._bit.isPHighRealseTriggered=1;
//                                this->sw._bit.isPHighOverTriggered=0;
//                            }
//            	        }
//                    }
//                    this->PHihgRealseTimer=0;
//                }
//            }
//        }
//
//       if(this->alarm_param.press_low_upper>0)
//       {
//           if(temp<this->alarm_param.press_low_upper)
//           {
//               this->PLowLessTimer++;
//               this->PLowRealseTimer=0;
//               if( this->PLowLessTimer>=40)
//               {
//                   if(!this->sw._bit.isPLowLessTriggered)
//                   {
//                       if((device_comps.device_addr.addr[4]!=0)&&(device_comps.device_addr.addr[3]!=0))
//                       {
//                           if(device_comps.batt>30)
//                           {
////                               protocolComps.triggerIrq._bit.PLowLess=1;
//                               this->sw._bit.isPLowRealseTriggered=0;
//                               this->sw._bit.isPLowLessTriggered=1;
//                           }
//                       }
//                   }
//                   this->PLowLessTimer=0;
//               }
//           }
//           else if((temp>this->alarm_param.press_low_upper+this->alarm_param.press_low_upper/10)&&(this->sw._bit.isPLowLessTriggered))//dealt 10%
//           {
//               this->PLowLessTimer=0;
//               this->PLowRealseTimer++;
//               if(this->PLowRealseTimer>=40)
//               {
//                   if(!this->sw._bit.isPLowRealseTriggered)
//                   {
//                       if((device_comps.device_addr.addr[4]!=0)&&(device_comps.device_addr.addr[3]!=0))
//                       {
//                           if(device_comps.batt>30)
//                           {
//    //                           protocolComps.triggerIrq._bit.PLowRealse=1;
//                               this->sw._bit.isPLowRealseTriggered=1;
//                               this->sw._bit.isPLowLessTriggered=0;
//                           }
//                       }
//                   }
//                   this->PLowRealseTimer=0;
//               }
//           }
//       }
//        
//   }

}
static void device_comps_task_handle(void)//Execution interval is 200 ms
{
	int i=0;
	device_comps_t *this=device_comps.this;
	if(this->do_init==1)
	{
		if(this->count<40)
		{
			if(!this->init(this))
			{
				this->do_init=0;
			}
			else
			{
				this->count++;
				hum_comps.dis_oper_mark._bit.refresh_err_code=1;
			}
			
		}
		else
		{
		    #ifdef  MD_IGNORE_ALL_ERR
			    this->do_init=0;
			#else
			    WDTE='R';
			#endif
		}
		if(this->do_init==0)
		{
           // cs123x_comps.power_down();
			this->count=0;
			get_batt();
		 
			this->read_all_param(this);
			//TODO
		}
	}
	if(this->do_init==0&&hum_comps.dis_oper_mark._bit.test_ok)
	{
	    static int init_once=1;
	    if(init_once)
	    {
             #if(MD_PRODUCT_NAME==MD_LORA)
    		  {
    			  enter_lora_mode();
    		  }
              #elif (MD_PRODUCT_NAME==MD_AIR_LEAK)
              {
    			  enter_air_leak_mode();
    		  }
    		  #elif (MD_PRODUCT_NAME==MD_NORMAL)
    		  {
                    enter_normal_mode();
    		  }
    		  #endif
    		  init_once=0;
		  }
    }
	
	//if((this->do_init==0)&&(!loraComps.sw._bit.runing))
	if(this->do_init==0)//&&(!loraComps.sw._bit.runing))
	{
	    int sample_en=0;
		if(this->count==5)//every 1s calc press and temperature
		{
            long ad1_ad3_temp=0;
			this->count=0;
			get_batt();


			//NTC TEMP
			//ad1_ad3_temp=this->ad3_average_result;
            //this->ad3_average_result=this->calc_ad3_average(this);
            //this->ntc_valve=this->calc_ntc_valve(this);
           // this->current_temp_n_2=this->current_temp_n_1;
           // this->current_temp_n_1=this->current_temp;
           // this->current_temp=this->calc_temp(this);
            //ad1_ad3_temp-=this->ad3_average_result;


            //PT100 TEMP
            ad1_ad3_temp=this->temp_p_temp_n_average_result;
            this->temp_p_temp_n_average_result=this->calc_temp_p_temp_n_average(this);
            this->pt_valve=this->calc_pt_valve(this);
            this->current_temp_n_2=this->current_temp_n_1;
            this->current_temp_n_1=this->current_temp;
            this->current_temp=this->calc_pt_temp(this);
            ad1_ad3_temp-=this->temp_p_temp_n_average_result;
			
			if(ad1_ad3_temp<0)
			{
                ad1_ad3_temp=-ad1_ad3_temp;
			}
			if(ad1_ad3_temp>2)
			{
                 device_comps.sw._bit.temp_adc_stb=0;
			}
			else
			{
                 device_comps.sw._bit.temp_adc_stb=1;
			}
            hum_comps.dis_oper_mark._bit.refresh_temp=1;
			hum_comps.dis_oper_mark._bit.refresh_temp_adc=1;
			

			
            ad1_ad3_temp=this->ad1_ad2_average_result;
			this->ad1_ad2_average_result=this->calc_ad1_ad2_average(this);
			//this->ad1_ad2_average_result=this->ad1_convert_result[0];
			ad1_ad3_temp-=this->ad1_ad2_average_result;
			if(ad1_ad3_temp<0)
			{
                ad1_ad3_temp=-ad1_ad3_temp;
			}
			if(ad1_ad3_temp>2)
			{
                 device_comps.sw._bit.adc_stb=0;
			}
			else
			{
                 device_comps.sw._bit.adc_stb=1;
			}
			this->current_press_n_2=this->current_press_n_1;
			this->current_press_n_1=this->current_press;
			this->current_press=this->calc_current_press(this);
			if(this->current_press>this->max_press)
			{
                this->max_press=this->current_press;
                hum_comps.dis_oper_mark._bit.refresh_press_max=1;
			}
			pressOverloadReport(this);
			if(this->current_press>this->calibration_param.y[3])
			{
				device_comps.sw._bit.over_range=1;
			}
			else
			{
                device_comps.sw._bit.over_range=0;
			}
            hum_comps.dis_oper_mark._bit.refresh_press=1;
			hum_comps.dis_oper_mark._bit.refresh_press_adc=1;
			

            ad1_ad3_temp=this->deltaP_p_deltaP_n_average_result;
			this->deltaP_p_deltaP_n_average_result=this->calc_deltaP_p_deltaP_n_average(this);
            ad1_ad3_temp-=this->this->deltaP_p_deltaP_n_average_result;
			if(ad1_ad3_temp<0)
			{
                ad1_ad3_temp=-ad1_ad3_temp;
			}
			if(ad1_ad3_temp>2)
			{
                 device_comps.sw._bit.deltaP_adc_stb=0;
			}
			else
			{
                 device_comps.sw._bit.deltaP_adc_stb=1;
			}
			this->current_deltaP_n_2=this->current_deltaP_n_1;
			this->current_deltaP_n_1=this->current_deltaP;
			this->current_deltaP=this->calc_current_deltaP(this);
			if(this->current_deltaP>this->max_deltaP)
			{
                this->max_deltaP=this->current_deltaP;
                hum_comps.dis_oper_mark._bit.refresh_deltaP_max=1;
			}
			
			if(this->current_deltaP>this->deltaP_calibration_param.y[3])
			{
				device_comps.sw._bit.deltaP_over_range=1;
			}
			else
			{
                device_comps.sw._bit.deltaP_over_range=0;
			}
            hum_comps.dis_oper_mark._bit.refresh_deltaP=1;
			hum_comps.dis_oper_mark._bit.refresh_deltaP_adc=1;

            if(device_comps.deltaP_calibration_param.is_calibrated)
            {
                this->current_flow=dpf_calc_current_flow(this);
                this->dpf_data.total=dpf_calc_total(this);
    			hum_comps.dis_extern_oper_mark._bit.refresh_flow=1;
    			hum_comps.dis_extern_oper_mark._bit.refresh_total=1;
			}
            this->ad3_pos=0;
			this->ad1_pos=0;
			this->ad2_pos=0;
			this->temp_p_pos=0;
			this->temp_n_pos=0;
			this->deltaP_p_pos=0;
			this->deltaP_n_pos=0;
		}


		
		 if(hum_comps.current_mode==EM_CAL_MODIFY_MODE)
		 {
             sample_en=1;
		 }  
         else
         {
            if(this->count==2)
            {
                sample_en=1;
            }
           
         }
         if(sample_en)
         {
            MD_SET_REF_3030C_ON;
	    delay_us(400);//400
	    if(hum_comps.current_mode==EM_CAL_MODIFY_MODE)
        {
            cs1237_comps.sw._bit.running=1;
            cs1237_comps.restart();//pt100
            cs1237_comps.enable_interrupt();
        }
        else if(this->count==2)
        {
            cs1237_comps.sw._bit.running=1;
            cs1237_comps.restart();//pt100
            cs1237_comps.enable_interrupt();
        }
//                NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
//                while(MD_CS1237_DATA);
//                cs1237_comps.read_adc(&this->temp_p_convert_result[this->temp_p_pos++]);
//                cs1237_comps.power_down();

        cs123x_comps.sw._bit.running=1;
        cs123x_comps.restart();
        cs123x_comps.enable_interrupt();
       
//                NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
//                while(MD_CS123X_DATA);
//                cs123x_comps.write_reg(&this->ad1_convert_result[this->ad1_pos++],0x71);
//                //cs123x_comps.read_adc(&this->ad1_convert_result[this->ad1_pos++]);
//                NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
//                while(MD_CS123X_DATA);
//                cs123x_comps.write_reg(&this->deltaP_p_convert_result[this->deltaP_p_pos++],0x70);
//                cs123x_comps.power_down();



            
           
         /*      
        //        	R_ADC_Set_OperationOff();
        //	        delay_us(40);
        //	        ADM2&=~0x80;
        //	        ADM2|=0x40;//extern vref
        //	        delay_us(40);
                ADS = _00_AD_INPUT_CHANNEL_0;
                R_ADC_Set_OperationOn();
                delay_us(2);
                device_comps.sw._bit.adc_busy=1;
                R_ADC_Start();
                while(device_comps.sw._bit.adc_busy);
                R_ADC_Get_Result(&this->ad3_convert_result[this->ad3_pos++]);

        	*/
        /*
        	ADS = _10_AD_INPUT_CHANNEL_16;
        	delay_us(2);
        	device_comps.sw._bit.adc_busy=1;
        	R_ADC_Start();
        	while(device_comps.sw._bit.adc_busy);
        	R_ADC_Get_Result(&this->ad1_convert_result[this->ad1_pos++]);

        	
        	
        	ADS = _14_AD_INPUT_CHANNEL_20;
        	delay_us(2);
        	device_comps.sw._bit.adc_busy=1;
        	R_ADC_Start();
        	while(device_comps.sw._bit.adc_busy);
            	R_ADC_Get_Result(&this->ad2_convert_result[this->ad2_pos++]);


                ADS = _10_AD_INPUT_CHANNEL_16;
        	delay_us(2);
        	device_comps.sw._bit.adc_busy=1;
        	R_ADC_Start();
        	while(device_comps.sw._bit.adc_busy);
        	R_ADC_Get_Result(&this->temp_p_convert_result[this->temp_p_pos++]);

        	
        	
        	ADS = _14_AD_INPUT_CHANNEL_20;
        	delay_us(2);
        	device_comps.sw._bit.adc_busy=1;
        	R_ADC_Start();
        	while(device_comps.sw._bit.adc_busy);
        	R_ADC_Get_Result(&this->temp_n_convert_result[this->temp_n_pos++]);
        	R_ADC_Set_OperationOff();
        */ 

          // MD_SET_REF_3030C_OFF;
          }
        this->count++;
	}
}

static void nop(void)
{
    NOP();
}


device_comps_t device_comps=
{
	"device comps",                //char *desc;
    &device_comps,     //struct _DEVICE_COMPONENTS  *const this;
    1,//   int   do_init;//Whether to initialize,1:init 0,no init
    device_comps_init,//    unsigned char (*const init)(struct _DEVICE_COMPONENTS *);
    0,//    unsigned long count;            //Called counter


    //union sw;  e2prom fault +cs123x err +cs1237err +rtc_module_err
    {MD_E2PROM_DRIVER_ERR+MD_CS123X_DRIVER_ERR+MD_CS1237_DRIVER_ERR
     +MD_RTC_MODULE_ERR+MD_LORA_MODULE_ERR+MD_PCF857X_DRIVER_ERR+MD_AD54X0_DRIVER_ERR}, 
    
   0, //int PHihgOverTimer;
   0,// int PHihgRealseTimer;
   0, //int PLowLessTimer;
   0,// int PLowRealseTimer;

    {0},//    unsigned int  ad1_convert_result[MD_ADC_MAX_POS];
    0,//   unsigned int  ad1_pos;
    {0},//   unsigned int  ad2_convert_result[MD_ADC_MAX_POS];
    0,//   unsigned int  ad2_pos;
    0,//long          ad1_ad2_average_result;//(ad1-ad2)/ad1_pos

   {0}, //unsigned int  temp_p_convert_result[MD_ADC_MAX_POS];
	0,//unsigned int  temp_p_pos;
	{0},//unsigned int  temp_n_convert_result[MD_ADC_MAX_POS];
	0,//unsigned int  temp_n_pos;
	0,//long          temp_p_temp_n_average_result;//(temp_p_convert_result-temp_n_convert_result)/temp_p_pos

    {0},//    long  deltaP_p_convert_result[MD_ADC_MAX_POS];
    0,//    int   deltaP_p_pos;
    {0},//    long  deltaP_n_convert_result[MD_ADC_MAX_POS];
    0,//   int   deltaP_n_pos;
    0,//  long          deltaP_p_deltaP_n_average_result;

	
    {0}, //  unsigned int  ad3_convert_result[MD_ADC_MAX_POS];
    0, // unsigned int  ad3_pos;
    0,//unsigned long ad3_average_result



    10000,//     float ntc_valve;
    calc_ad3_average,//unsigned long ( * const calc_ad3_average)(struct _DEVICE_COMPONENTS  *const);
    calc_ntc_valve,    //float ( * const calc_ntc_valve)(struct _DEVICE_COMPONENTS  *const);//point to function calc_pt1000( device_comps_t *const this) whith param adc_result[][] 
    calc_temp,//      long  ( * const calc_temp)  (struct _DEVICE_COMPONENTS     *const);//point to function calc_temp( sensor_comp_t *const this)with param pt1000_valve

   20000,
    calc_temp_p_temp_n_average,
    calc_pt_valve,
    calc_pt_temp,
    0,//     long  current_temp;
    0,//     long  current_temp_n_1;
    0,//     long  current_temp_n_2;

    calc_deltaP_p_deltaP_n_average,//long ( *const calc_deltaP_p_deltaP_n_average)(struct _DEVICE_COMPONENTS *const ); //point to calc_signal_period(sensor_comp_t *const this)
	calc_current_deltaP,//long ( *const calc_current_deltaP)(struct _DEVICE_COMPONENTS *const ); //point to calc_signal_period(sensor_comp_t *const this)
    0,//long  current_deltaP;  //Yn
    0,//long  max_deltaP_;
	0,//long  current_deltaP_n_1;//Yn-1
	0,//long  current_deltaP_n_2;//Yn-2

   
	calc_ad1_ad2_average,//long ( *const calc_ad1_ad2_average)(struct _DEVICE_COMPONENTS *const ); //point to calc_signal_period(sensor_comp_t *const this)
	calc_current_press,//     unsigned long ( *const calc_current_press)(struct _DEVICE_COMPONENTS *const ); //point to calc_signal_period(sensor_comp_t *const this)
    0,//     long  current_press;  //Yn
    0,//long  max_press;
    0,//     long  current_press_n_1;//Yn-1
    0,//     long  current_press_n_2;//Yn-2

    {           //struct 
                //{ int press;
               //  int temp;  
        0        //   long press_clr_value 
    },         //}coe;
    read_device_coe,
    save_device_coe,  
    clr_press,
    
    0,//     unsigned int batt;//batt voltage

    {{0},{0},{0},0,0,0,0}, //struct calibration_param 
    {{0},{0},{0},0,0,0,0},//struct calibration_param_bak 
 	EM_CAL_PRESS,// cal_type
 	read_calibration_param,//     int (*read_calibration_param)(void *,int );
    save_calibration_param,//     int (*save_calibration_param)(void const *,int);


    {{0},{0},{0},0,0,0,0}, //struct res_calibration_param 
    {{0},{0},{0},0,0,0,0},//struct res_calibration_param_bak 
    read_res_calibration_param,//     int (*read_res_calibration_param)(void *,int );
    save_res_calibration_param,//     int (*save_res_calibration_param)(void const *,int);

    {{0},{0},{0},0,0,0,0},//deltaP_calibration_param_t  deltaP_calibration_param;
    {{0},{0},{0},0,0,0,0},//deltaP_calibration_param_t  deltaP_calibration_param_bak;
    read_deltaP_calibration_param,//int (*read_deltaP_calibration_param)(void *,int );
    save_deltaP_calibration_param,//int (*save_deltaP_calibration_param)(void const *,int);

    {0},//dpf_param_t dpf_param;
    read_dpf_param,
    save_dpf_param,
     0,   // current_flow;
     
     {0}, //dpf_data_t dpf_data;
     read_dpf_data,
     save_dpf_data,
     0,//float dpf_total_dec_part;
     
     {0},//dpf_backup_data_t dpf_backup_data
     read_dpf_backup_data,
     save_dpf_backup_data,

    {0},   //struct report_parm;
    read_report_param,//  int (*read_report_param)(void *,int);
    save_report_param,//  int (*save_report_param)(void const *,int);

    {{0},0,0},   //struct access_parm;
    read_access_param,//   int (*read_access_param)(void *,int);
    save_access_param,//   int (*save_access_param)(void const *,int);


    {0},// struct alarm_param;
    read_alarm_param,// int (*read_alarm_param)(void *,int);
    save_alarm_param,//int (*save_alarm_param)(void const *,int);

    {{0},0},//unsigned char addr[7];
    read_device_addr,// int (*read_device_addr)(void *,int);
    save_device_addr,//int (*save_device_addr)(void const *,int);

    {{0},0},//device_sensor_t device_sensor;
    read_device_sensor,//int (*read_device_sensor)(void *,int);
	save_device_sensor,//int (*save_device_sensor)(void const *,int);

    { MD_TIME_SEG_DATA_START_ADDR,"",0},//TimeSegData_t TimeSegData;
    read_time_seg_data_param,
    save_time_seg_data_param,

    {0},
    read_system_time,
    save_system_time,

    {0},
    read_air_leak_param,
    save_air_leak_param,

    { {0},
      0
    },
    0,//int report_interval_timer;
    0,//_1s_timr_acc

    read_all_param,//void (*read_all_param)(struct _DEVICE_COMPONENTS  *const);

	device_comps_output_debug_info,//void  ( *const output_debug_info)(struct _DEVICE_COMPONENTS   *const);//point to sensor_comps_output_debug_info(device_comps_t const *const this)
	"",                     //char debug_info[128];
	device_comps_task_handle,//void  ( *const task_handle)(void);//point to device_comps_task_handle
};



void GSMReturnTimeChk(unsigned char RHur,unsigned char RMin,unsigned char RSec)
{
	unsigned int cmp=device_comps.report_param.hour_Interval;//((unsigned int)GSM.ReturnCecle[0]<<8)+GSM.ReturnCecle[1];
    if(cmp==0)
	{
		cmp=1;
	}
	if(device_comps.report_interval_timer>=(unsigned long)cmp*60)
    {
		unsigned char reltSec=((RSec>>4)*10+(RSec&0x0f)) +60-((rtc_valve.sec>>4)*10+(rtc_valve.sec&0x0f));
		unsigned char reltMin=((RMin>>4)*10+(RMin&0x0f)) +60-1-((rtc_valve.min>>4)*10+(rtc_valve.min&0x0f));
		unsigned char reltHur=((RHur>>4)*10+(RHur&0x0f)) +24-1-((rtc_valve.hour>>4)*10+(rtc_valve.hour&0x0f));
		unsigned char equSec= reltSec%60;
		unsigned char equMin=(reltMin+(reltSec/60))%60;
		unsigned char equHur=(reltHur+((reltMin+(reltSec/60))/60))%24;
		unsigned long InterTimer=(unsigned long)equHur*3600+   (unsigned long)equMin*60    +(unsigned long)equSec;
	     if(InterTimer>(unsigned long)5*60) 
		{
			 if((device_comps.device_addr.addr[4]!=0)&&(device_comps.device_addr.addr[3]!=0))
	        {
                if(device_comps.batt>30)
                {
   //                 protocolComps.triggerIrq._bit.intervalTime=1;
                }
	        }
	    }
	    device_comps.report_interval_timer=0;
	}
}
void CalcReportTime(unsigned char *hur,unsigned char *min,unsigned char *sec)
{
	unsigned long  DisPerTimeTotalSecond=(unsigned long)((device_comps.device_addr.addr[6]>>4)*10+(device_comps.device_addr.addr[6]&0x0f))* device_comps.report_param.disFactor;
	unsigned long  Min=DisPerTimeTotalSecond/60;
	unsigned long  DisPerTimehur=Min/60;
	unsigned long  DisPerTimeMin=Min%60;//min
	
	*sec=DisPerTimeTotalSecond%60;
	*min=(device_comps.report_param.min+DisPerTimeMin)%60;
	*hur=(device_comps.report_param.hour+DisPerTimehur+(device_comps.report_param.min+DisPerTimeMin)/60)%24;
	
	*sec=(*sec/10<<4)+(*sec%10);
	*min=(*min/10<<4)+(*min%10);
	*hur=(*hur/10<<4)+(*hur%10);
}

void  Timing_interval_report(void)
{
    unsigned char ReportHur;
	unsigned char ReportMin;
	unsigned char ReportSec;
	unsigned char ReportMin_30=ReportMin;
	unsigned char temp[7];
    CalcReportTime(&ReportHur,&ReportMin,&ReportSec);
    if(rtc_valve.sec == ReportSec)
	{	
		device_comps.report_interval_timer++;
        if((ReportHur==rtc_valve.hour)&&(ReportMin==rtc_valve.min))
        {
	        device_comps.report_interval_timer=0;
	        if((device_comps.device_addr.addr[4]!=0)&&(device_comps.device_addr.addr[3]!=0))
	        {
                if(device_comps.batt>30)
                {
   //                 protocolComps.triggerIrq._bit.timeAuto=1;
                }
	        }
	     }
		//GSMReturnTimeChk(ReportHur,ReportMin,ReportSec); //检查上报时间，如果到达指定上报时间，则系统开始上报数据//	

        ReportMin_30+=0x30;
        if(ReportMin_30>=0x60)
        {
            ReportMin_30-=0x60;
        }
		if(rtc_valve.min == ReportMin  || rtc_valve.min == ReportMin_30)
		{
//			if(1)
//			{
//				unsigned int StoreAddr=device_comps.TimeSegData.store_addr;
//				
//				if((StoreAddr>= MD_TIME_SEG_DATA_START_ADDR )&&(StoreAddr<=MD_TIME_SEG_DATA_END_ADDR-4))
//				{
//                    _24cxx_comps.write(StoreAddr,&device_comps.current_press,4);
//					
//					StoreAddr+=4;
//					if(StoreAddr>=MD_TIME_SEG_DATA_END_ADDR)
//					{
//					        StoreAddr=MD_TIME_SEG_DATA_START_ADDR;
//					}
//					device_comps.TimeSegData.store_addr=StoreAddr;
//
//					
//                	temp[0]=0x20;
//                	temp[1]=rtc_valve.year;
//                	temp[2]=rtc_valve.month;
//                	temp[3]=rtc_valve.day;
//                	temp[4]=rtc_valve.hour;
//                    temp[5]=rtc_valve.min;
//                	temp[6]=rtc_valve.sec;
//					
//					memcpy(device_comps.TimeSegData.lastSampleTime,&temp,sizeof(device_comps.TimeSegData.lastSampleTime));
//					device_comps.TimeSegData.cs=Check_Sum_5A(&device_comps.TimeSegData, &device_comps.TimeSegData.cs-(unsigned char *)&device_comps.TimeSegData);
//					device_comps.save_time_seg_data_param(&device_comps.TimeSegData,sizeof(device_comps.TimeSegData));
//				}
//				else
//				{
//					StoreAddr=MD_TIME_SEG_DATA_START_ADDR;
//					device_comps.TimeSegData.store_addr=StoreAddr;
//					device_comps.TimeSegData.cs=Check_Sum_5A(&device_comps.TimeSegData, &device_comps.TimeSegData.cs-(unsigned char *)&device_comps.TimeSegData);
//					device_comps.save_time_seg_data_param(&device_comps.TimeSegData,sizeof(device_comps.TimeSegData));
//				}
//			}
		}
	}
}

static long format_all_set_unit()
{
    long num;
//    if(device_comps.air_leak_param.work_mode==0)
//    {
//       if(device_comps.air_leak_param.unit==2)//psi
//       {
//             num=device_comps.air_leak_param.Constant_pressure_value*100/145.0377439;
//       }
//       else if(device_comps.air_leak_param.unit==0)
//       {
//            if((device_comps.calibration_param.unit&0x0f)==1)
//            {
//                if(device_comps.calibration_param.dot==3)
//                {
//                    num=device_comps.air_leak_param.Constant_pressure_value*10;
//                }
//            }
//            else
//            {
//                num=device_comps.air_leak_param.Constant_pressure_value;
//            }
//       }
//       else
//       {
//            num=device_comps.air_leak_param.Constant_pressure_value;
//       }
//    }
//    else if(device_comps.air_leak_param.work_mode==1)
//    {
//       if(device_comps.air_leak_param.unit==2)//psi
//       {
//             num=device_comps.air_leak_param.buck_delta_value*100/145.0377439;
//       }
//       else if(device_comps.air_leak_param.unit==0)
//       {
//            if((device_comps.calibration_param.unit&0x0f)==1)
//            {
//                if(device_comps.calibration_param.dot==3)
//                {
//                    num=device_comps.air_leak_param.buck_delta_value*10;
//                }
//            }
//            else
//            {
//                num=device_comps.air_leak_param.buck_delta_value;
//            }
//       }
//       else
//       {
//            num=device_comps.air_leak_param.buck_delta_value;
//       }
//       
//       num=device_comps.air_leak.ref_press-num;
//       
//    }
//    else if(device_comps.air_leak_param.work_mode==2)
//    {
//        num=device_comps.air_leak.ref_press-device_comps.air_leak.ref_press*device_comps.air_leak_param.buck_percent/1000.;;
//    }
    return num;
}
static int air_leak_mode_check_warn()//return 1,warn
{
//    long press_set_format_valve=device_comps.air_leak.press_set_format_valve;
//    long current_press_format=device_comps.current_press/10;
//    return current_press_format<=press_set_format_valve;
}

void _1s_task_handle(void)
{
    R_WDT_Restart();
    if(hum_comps.current_mode!=EM_SELF_TEST_MODE)
    {
        device_comps._1s_timr_acc++;
    	if(device_comps._1s_timr_acc>9)
    	{
    		
    		 device_comps._1s_timr_acc=0;
    	}
    	
        R_RTC_Get_CounterValue(&rtc_valve);
       #if(MD_PRODUCT_NAME==MD_NBIOT)
	    Timing_interval_report();
	   #endif
	    if(rtc_valve.min==0x21&&rtc_valve.sec==0x21)
	    {
             //save systemtime();
	    }
	    hum_comps.dis_oper_mark._bit.refresh_time=1;
	    hum_comps.dis_oper_mark._bit.refresh_date=1; 
        if( (ircComps.op_window_time>0)&&(hum_comps.current_mode!=EM_CAL_MODIFY_MODE))
		{
			ircComps.op_window_time--;
			if(ircComps.op_window_time==0)
			{
				ircComps.sw._bit.runing=0;

				#ifndef MD_MODBUS
    				disable_irc_receive();
    				disable_irc_send();
				#endif
				MD_IR_VCM_OFF; 
			}
		}
	
//		if(protocolComps.timer>0)
//		{
//		    protocolComps.timer--;
//		}

//	    if(netComps.op_window_time>0)
//	    {
//	        netComps.op_window_time--;
//	    }
//
//	    if(netComps.AckTmr>0)
//	    {
//	        netComps.AckTmr--;
//	    }

	    if(hum_comps.current_mode==EM_LORA_MODE||hum_comps.current_mode==EM_AIR_LEAK_MODE
	       ||hum_comps.current_mode==EM_NORMAL_MODE)
	    {
	        if(mode_comps[hum_comps.current_mode].displayTimer>0&&mode_comps[hum_comps.current_mode].dis_option!=0)
	        {
	            mode_comps[hum_comps.current_mode].displayTimer--;
	            if(!mode_comps[hum_comps.current_mode].displayTimer)
	            {
	                  #if(MD_PRODUCT_NAME==MD_LORA)
            		  {
            			  enter_lora_mode();
            		  }
                      #elif (MD_PRODUCT_NAME==MD_AIR_LEAK)
                      {
            			  enter_air_leak_mode();
            		  }
            		  #elif (MD_PRODUCT_NAME==MD_NORMAL)
            		  {
                            //enter_normal_mode();
            		  }
            		  #endif
	            }
	        }
	    }
	    
	    if(hum_comps.current_mode==EM_DEBUG_MODE)
	    {
            if(mode_comps[hum_comps.current_mode].displayTimer>0)
	        {
                mode_comps[hum_comps.current_mode].displayTimer--;
                if(!mode_comps[hum_comps.current_mode].displayTimer)
                {
	                 #if(MD_PRODUCT_NAME==MD_LORA)
            		  {
            			  enter_lora_mode();
            		  }
                      #elif (MD_PRODUCT_NAME==MD_AIR_LEAK)
                      {
            			  enter_air_leak_mode();
            		  }
            		  #elif (MD_PRODUCT_NAME==MD_NORMAL)
            		  {
                            enter_normal_mode();
            		  }
            		  #endif
	            }
	        }
	    }
	    
	    
//	    if(device_comps.air_leak.sw._bit.count_en)
//	    {
//	            if(device_comps.air_leak.delay_timer>0)
//	            {
//	                device_comps.air_leak.delay_timer--;
//	                if(device_comps.air_leak.delay_timer==0)
//	                {
//	                    device_comps.air_leak.ref_press=device_comps.current_press/10;
//	                    device_comps.air_leak.press_set_format_valve=format_all_set_unit();
//	                }
//	            }
//	            else
//	            {
//	    	        if(device_comps.air_leak_param.count_direction==0)//up count
//	                {
//	                    
//	                    if(device_comps.air_leak.timer<device_comps.air_leak_param.timer)
//	                    {
//                            if(!air_leak_mode_check_warn())
//                            {
//	                            device_comps.air_leak.timer++;
//	                        }
//	                        else
//	                        {
//                                device_comps.air_leak.sw._bit.count_en=0;
//                                device_comps.air_leak.sw._bit.isWarn=1;
//                                device_comps.air_leak.sw._bit.test_finished=1;
//	                        }
//	                    }
//	                    else
//	                    {
//	                        device_comps.air_leak.sw._bit.count_en=0;
//	                        device_comps.air_leak.sw._bit.isWarn=0;
//	                        device_comps.air_leak.sw._bit.test_finished=1;
//	                    }
//	                }
//	                else if(device_comps.air_leak_param.count_direction==1)//down count
//	                {
//	                    if(device_comps.air_leak.timer>0)
//	                    {
//	                        if(!air_leak_mode_check_warn())
//                            {
//	                            device_comps.air_leak.timer--;
//	                        }
//	                        else
//	                        {
//                                device_comps.air_leak.sw._bit.count_en=0;
//                                device_comps.air_leak.sw._bit.isWarn=1;
//                                device_comps.air_leak.sw._bit.test_finished=1;
//	                        }
//	                    }
//	                    else
//	                    {
//	                        device_comps.air_leak.sw._bit.count_en=0;
//	                        device_comps.air_leak.sw._bit.isWarn=0;
//	                        device_comps.air_leak.sw._bit.test_finished=1;
//	                    }
//	                }
//	            }
//		    hum_comps.dis_oper_mark._bit.air_leak_mode_refresh_timer=1;
//		}
	}
}
