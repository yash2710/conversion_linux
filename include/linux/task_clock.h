
#ifndef TASK_CLOCK_FUNC_H
#define TASK_CLOCK_FUNC_H

#include <linux/irq_work.h>


#define TASK_CLOCK_MAX_THREADS 1024

//used by userspace to know what is going on
struct task_clock_user_status{
	uint64_t lowest_clock; //set when you inactivate the clock
	uint64_t ticks;
	uint64_t notifying_clock;
	uint64_t notifying_id;
	uint64_t notifying_sample;
	uint64_t notifying_diff;
};__attribute__ ((aligned (8), packed));

struct task_clock_info{
  uint32_t tid;
  struct task_clock_user_status * user_status;
  int fd;
};

struct task_clock_entry_info{
  uint8_t initialized;
  uint8_t sleeping;
  uint8_t waiting;
  uint8_t inactive;
 //when the counter is stopped, the count gets reset to 0...but we need
  //that to persist. So, "ticks" is the current counter value and base_ticks
  //stores the values in previous counter sessions
  uint64_t ticks;
  uint64_t base_ticks;
  struct perf_event * event;
  //debugging stuff
  struct timespec debug_last_enable;
  struct timespec debug_last_disable;
  uint64_t debug_last_enable_ticks;
  uint64_t debug_last_sample_period;
  uint64_t debug_last_overflow_ticks;
};

struct task_clock_group_info{
  spinlock_t nmi_lock;
  spinlock_t lock;
  int32_t lowest_tid;
  uint64_t lowest_ticks;
  struct task_clock_entry_info clocks[TASK_CLOCK_MAX_THREADS];
  struct irq_work pending_work;
  uint8_t notification_needed;
  uint8_t nmi_new_low;
  struct task_clock_user_status * user_status_arr;
};


struct task_clock_func{
  void (*task_clock_overflow_handler) (struct task_clock_group_info *);
  struct task_clock_group_info * (*task_clock_group_init) (void);
  void (*task_clock_entry_init) (struct task_clock_group_info *, struct perf_event *);
  void (*task_clock_on_disable) (struct task_clock_group_info *);
  void (*task_clock_on_enable) (struct task_clock_group_info *);
  void (*task_clock_entry_activate) (struct task_clock_group_info *);
  void (*task_clock_entry_halt) (struct task_clock_group_info *);
  void (*task_clock_on_wait) (struct task_clock_group_info *);
  void (*task_clock_entry_activate_other) (struct task_clock_group_info *, int32_t id);
  void (*task_clock_entry_wait) (struct task_clock_group_info *);
  void (*task_clock_entry_sleep) (struct task_clock_group_info *);
  void (*task_clock_entry_woke_up) (struct task_clock_group_info *);
  void (*task_clock_overflow_update_period) (struct task_clock_group_info *);
  void (*task_clock_add_ticks) (struct task_clock_group_info *, int32_t ticks);
  void (*task_clock_debug_add_event) (struct task_clock_group_info *, int32_t event);
};

#define TASK_CLOCK_MAX_THREADS 1024

extern struct task_clock_func task_clock_func;

#endif
