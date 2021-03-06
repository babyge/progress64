//Copyright (c) 2018, ARM Limited. All rights reserved.
//
//SPDX-License-Identifier:        BSD-3-Clause

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "p64_timer.h"
#include "expect.h"

void
callback(p64_timer_t tim,
	 p64_tick_t tmo,
	 void *arg)
{
    p64_tick_t tck = p64_timer_tick_get();
    printf("Timer %d expiration %#"PRIx64" now %#"PRIx64"\n", tim, tmo, tck);
    *(p64_tick_t *)arg = tck;
}

int main(void)
{
    p64_tick_t exp_a = P64_TIMER_TICK_INVALID;
    p64_timer_t tim_a = p64_timer_alloc(callback, &exp_a);
    EXPECT(tim_a != P64_TIMER_NULL)
    EXPECT(p64_timer_set(tim_a, 1));
    EXPECT(!p64_timer_set(tim_a, 1));
    p64_timer_tick_set(0);
    p64_timer_expire();
    EXPECT(exp_a == P64_TIMER_TICK_INVALID);
    p64_timer_tick_set(1);
    p64_timer_expire();
    EXPECT(exp_a == 1);
    EXPECT(p64_timer_set(tim_a, 2));
    EXPECT(p64_timer_reset(tim_a, 3))
    p64_timer_tick_set(2);
    p64_timer_expire();
    EXPECT(exp_a == 1);
    EXPECT(p64_timer_cancel(tim_a))
    p64_timer_tick_set(3);
    p64_timer_expire();
    EXPECT(exp_a == 1);
    EXPECT(!p64_timer_reset(tim_a, UINT64_C(0xFFFFFFFFFFFFFFFE)));
    EXPECT(p64_timer_set(tim_a, UINT64_C(0xFFFFFFFFFFFFFFFE)));
    EXPECT(p64_timer_reset(tim_a, UINT64_C(0xFFFFFFFFFFFFFFFE)));
    p64_timer_expire();
    EXPECT(exp_a == 1);
    p64_timer_tick_set(UINT64_C(0xFFFFFFFFFFFFFFFE));
    p64_timer_expire();
    EXPECT(exp_a == UINT64_C(0xFFFFFFFFFFFFFFFE));
    p64_timer_free(tim_a);

    printf("timer tests complete\n");
    return 0;
}
