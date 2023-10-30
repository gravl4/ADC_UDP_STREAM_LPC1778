
#ifndef TICKLOOP_H
#define TICKLOOP_H

#include <stdint.h>

#define HALF_LOOP (0xFFFFFFFF >> 1)
typedef struct s_loop {
	uint32_t c_loop;
	uint8_t over_loop;
} t_s_loop;

#endif //TICKLOOP_H


uint8_t check_halfloop(t_s_loop *loop, uint32_t tick_loop);
uint8_t get_cmploop(uint32_t delta, t_s_loop *loop, uint32_t tick_loop);
