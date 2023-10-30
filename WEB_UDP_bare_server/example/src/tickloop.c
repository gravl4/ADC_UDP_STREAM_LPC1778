

#include "../../../../WEB_UDP_bare_server/example/inc/tickloop.h"

#include "lpc_types.h"


// check loop counter
// return 0 if need next tick
// return 1 if ready
uint8_t check_halfloop(t_s_loop *loop, uint32_t tick_loop) {
    if (loop->over_loop & 0x80) {
    	return 1;
    }
	if (!loop->over_loop)  {
		// пример начальные условия loop size 100, tick_loop 90, c_loop 100,
		// но пропустили 5 тиков до  tick_loop 4
		if ((tick_loop > loop->c_loop) || ((tick_loop < HALF_LOOP) && (loop->c_loop > HALF_LOOP))) {
			loop->over_loop |= 0x80;
			return 1;
		}
	}
	else {
		// пример начальные условия loop size 100, tick_loop 90, c_loop 10,
		// и пропустили 5 тиков до  tick_loop 15
		if ((tick_loop > loop->c_loop) && (tick_loop < HALF_LOOP)) {
			loop->over_loop |= 0x80;
			return 1;
		}
	}
	return 0;
}
// calculate loop counter and over loop bool
uint8_t get_cmploop(uint32_t delta, t_s_loop *loop, uint32_t tick_loop) {
	if (delta > HALF_LOOP)
		return 0;
	loop->c_loop = tick_loop + delta;
	loop->over_loop = 0;
	if (loop->c_loop < tick_loop) {
		// over loop
		loop->over_loop = 0x1;
	}
	return 1;
}



