#ifndef  SYSTEM_H
#define  SYSTEM_H

typedef void (*  pfun)(void);

typedef struct _SYSTEM_COMPONENTS
{
    int test_xt1_run_flag;
    pfun const  user_init;
    pfun const  call_back_task_remarks;
    pfun const  task_process;
    
}systemComps_t;


extern systemComps_t systemComps;

#endif
