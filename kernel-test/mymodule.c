#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/task_stack.h>
#include <linux/kallsyms.h>
#include <linux/module.h>

#define __force_order (*(volatile char*)0)
#define sys_No 96
unsigned long long old_sys_call_func;
unsigned long long p_sys_call_table = 0xffffffffb9800300; //= (unsigned long long)kallsyms_lookup_name("sys_call_table");

inline void mywrite_cr0(unsigned long cr0) {
  asm volatile("mov %0,%%cr0" : "+r"(cr0), "+m"(__force_order));
}

void enable_write_protection(void) {
  unsigned long cr0 = read_cr0();
  set_bit(16, &cr0);
  mywrite_cr0(cr0);
}

void disable_write_protection(void) {
  unsigned long cr0 = read_cr0();
  clear_bit(16, &cr0);
  mywrite_cr0(cr0);
}

asmlinkage long long hello(void)
{
    long long a, b;
    //register int a asm("rdi");
    //register int b asm("rsi");
    struct pt_regs *regs = task_pt_regs(current);
    a = regs->di;
    b = regs->si;
    printk("No 96 syscall has changed to hello, a = %lld, b = %lld, \n", a, b);
    return a + b;
}

/*static struct {
  void **sys_call_table;
  void *orig_fn;
} tinfo;

static int __init mod_init(void) {
  printk(KERN_INFO "Init syscall hook\n");
  tinfo.sys_call_table = (void **)kallsyms_lookup_name("sys_call_table");
  tinfo.orig_fn = tinfo.sys_call_table[your_syscall_num];
  disable_write_protection();
  // modify sys_call_table directly
  tinfo.sys_call_table[your_syscall_num] = sys_yourcall;
  enable_write_protection();
  return 0;
}*/

void modify_syscall(void)
{
    unsigned long long *sys_call_addr;
    sys_call_addr = (unsigned long long *)(p_sys_call_table + sys_No * sizeof(void *));
    old_sys_call_func = *(sys_call_addr);
    printk(KERN_INFO "old_sys_call_func: %llx\n", (unsigned long long)sys_call_addr);
    *(sys_call_addr) = (unsigned long long)&hello; // 指向新函数
    printk(KERN_INFO "new_sys_call_func: %llx\n", (unsigned long long)sys_call_addr);
}

void restore_syscall(void)
{
    unsigned long long *sys_call_addr;
    sys_call_addr = (unsigned long long *)(p_sys_call_table + sys_No * sizeof(void *));
    *(sys_call_addr) = old_sys_call_func; // 指向原始函数
}

static int mymodule_init(void)
{
    unsigned long long *sys_call_addr;
    printk(KERN_INFO "Module 'mymodule' initialization\n");
    sys_call_addr = (unsigned long long *)(p_sys_call_table + sys_No * sizeof(void *));
    printk(KERN_INFO "sys_call_addr: %llx\n", *sys_call_addr);
    disable_write_protection();
    modify_syscall();
    enable_write_protection();
    return 0;
}

static void mymodule_exit(void)
{
    printk(KERN_INFO "Module 'mymodule' exit\n");
    disable_write_protection();
    restore_syscall();
    enable_write_protection();
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kalo");
