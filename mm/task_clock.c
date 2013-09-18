
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/syscalls.h>
#include <linux/task_clock.h>

struct task_clock_func task_clock_func;
EXPORT_SYMBOL(task_clock_func);

SYSCALL_DEFINE3(task_clock_open, int, fd, unsigned long, user_status, int, tid)
{
  current->task_clock.tid=tid;
  current->task_clock.fd=fd;
  current->task_clock.user_status=(struct task_clock_user_status *)user_status;
}

SYSCALL_DEFINE2(task_clock_close, int, fd, int, tid)
{
  
}
