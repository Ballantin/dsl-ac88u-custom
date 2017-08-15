#ifndef _IPT_TRIGGER_H_target 
#define _IPT_TRIGGER_H_target 
  
#define TRIGGER_TIMEOUT 600     /* 600 secs */ 

#define MAX_TRIG_RANGE	12
  
enum ipt_trigger_type 
{ 
       IPT_TRIGGER_DNAT = 1, 
       IPT_TRIGGER_IN = 2, 
        IPT_TRIGGER_OUT = 3 
}; 
 
struct ipt_trigger_info { 
        enum ipt_trigger_type type; 
        u_int16_t mport[2];     /* Related destination port range */ 
        u_int8_t proto[MAX_TRIG_RANGE];        /* Related protocol */ 
        u_int16_t rport[MAX_TRIG_RANGE][2];     /* Port range to map related destination port range to */ 
}; 
 
#endif /*_IPT_TRIGGER_H_target*/ 
