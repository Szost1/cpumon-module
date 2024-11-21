#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/cpumask.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include <linux/cpu.h>
#include <linux/kernel_stat.h>
#include <linux/timekeeping.h>

int t = 10;
module_param(t,int,0644);
MODULE_PARM_DESC(t,"czas pomiedzy zapisami w logach");

static struct timer_list timerr;
static u64 prevuser = 0;
static u64 previdle = 0;
static u64 prevsystem = 0;
static u64 prevnice = 0;


static void log_cpu_load(struct timer_list *time) {
  u64 calkobc=0;
  int cpu;
  u64 user=0;
  u64 systemm=0;
  u64 nice=0;
  u64 idle=0;
  struct tm currtime;
  struct timespec64 seconds;
  ktime_get_real_ts64(&seconds);
  time64_to_tm(seconds.tv_sec,0,&currtime);
  for_each_online_cpu(cpu) {
    user += kcpustat_cpu(cpu).cpustat[CPUTIME_USER];
    systemm += kcpustat_cpu(cpu).cpustat[CPUTIME_SYSTEM];
    nice += kcpustat_cpu(cpu).cpustat[CPUTIME_NICE];
    idle += kcpustat_cpu(cpu).cpustat[CPUTIME_IDLE]; 
  }
  calkobc = (user - prevuser) + (systemm - prevsystem) + (nice - prevnice) + (idle - previdle);
  if(calkobc) {
  u64 percentag = ((calkobc-(idle - previdle))*100)/calkobc;
    printk(KERN_INFO "[%02d-%02d-%04ld %02d:%02d:%02d] aktualne obciazenie procesora to %llu%%",currtime.tm_mday,currtime.tm_mon+1, currtime.tm_year+1900, currtime.tm_hour+1, currtime.tm_min, currtime.tm_sec, percentag);
    prevuser = user;
    prevsystem = systemm;
    prevnice = nice;
    previdle = idle;

  }
  else {
    printk(KERN_ERR "Dzielenie przez 0!");
  }
  mod_timer(&timerr,jiffies + t * HZ );
}

static int __init cpumon_init(void) {
  printk(KERN_INFO "Modul cpumon zaczyna swoje dzialanie, interwal miedzy pomiarami to: %d", t);
  timer_setup(&timerr,log_cpu_load,0);
  mod_timer(&timerr, jiffies + t * HZ);
  log_cpu_load(&timerr);
  return 0;
}

static void __exit cpumon_exit(void) {
  printk(KERN_INFO "Modul cpumon konczy swoje dzialanie");
  del_timer(&timerr);
}

module_init(cpumon_init);
module_exit(cpumon_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("279504 i 264119");
MODULE_DESCRIPTION("Modul co 't' sekund zapisuje w logach systemowych obiazenie CPU");
