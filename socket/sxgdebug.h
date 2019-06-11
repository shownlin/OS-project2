#ifndef _SXG_DEBUG_H_
#define _SXG_DEBUG_H_

#ifdef  _sxg_debug_
#	define sxg_debug(fmt, args...)	printk("ksocket : %s, %s, %d, "fmt, __FILE__, __FUNCTION__, __LINE__, ##args)
#else
#	define sxg_debug(fmt, args...)
#endif

#endif
