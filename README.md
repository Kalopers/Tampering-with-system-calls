# Syscall Modification Module

This Linux kernel module demonstrates how to modify a system call (syscall) in the kernel. The module replaces the functionality of a specific syscall with a custom function, logging information about the syscall parameters.

## Prerequisites

- This module is designed for Linux kernel development.
- Ensure that you have the necessary build tools and kernel headers installed on your system.

## Overview

The module performs the following tasks:

1. Disables write protection on the kernel page tables.
2. Modifies the specified syscall in the syscall table to point to a custom function (`hello`).
3. Enables write protection again to safeguard kernel integrity.

## Code Explanation

The main components of the code include:

- `hello`: A custom function that logs information about the parameters of the modified syscall.

- `modify_syscall`: A function that replaces the specified syscall with the custom function.

- `restore_syscall`: A function that restores the original syscall function.

- `mymodule_init`: The module initialization function that disables write protection and modifies the syscall.

- `mymodule_exit`: The module exit function that restores the original syscall and enables write protection.

## Usage

1. Ensure that your system is set up for kernel module development.

2. Compile the module:

   ```bash
   make
   ```

3. Load the module:

   ```bash
   sudo insmod mymodule.ko
   ```

4. Check kernel logs to observe the modification:

   ```bash
   dmesg
   ```

5. Unload the module:

   ```bash
   sudo rmmod mymodule
   ```

## Notes

- **Caution:** Modifying system calls can have severe consequences. This module is for educational purposes, and modifying syscalls in a production environment is discouraged.

- This module assumes a specific syscall number (`96`). Adjust the `sys_No` macro if you want to modify a different syscall.

- Ensure that you have the necessary permissions to load and unload kernel modules.

- Remember to update the 'p_sys_call_table' after each system reboot, you can get it with the following command:

```bash
cat /proc/kallsyms | grep sys_call_table 
```



## License

This module is licensed under the GNU General Public License (GPL).

## Author

- [Kalo](https://github.com/Kalopers)