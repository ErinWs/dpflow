#ifndef PCF857X_H
#define PCF857X_H


typedef struct _PCF857X_COMPONENTS
{
	char *desc;
	struct  _PCF857X_COMPONENTS *this;
	volatile char in_data;
	volatile char out_data;
    int  (*const Init )(struct  _PCF857X_COMPONENTS *this);
    int  (*const read )(struct  _PCF857X_COMPONENTS *this);
	int  (*const write)(struct  _PCF857X_COMPONENTS *this);
	
}pcf857x_comps_t;
extern pcf857x_comps_t pcf857x_comps;
#endif

