cmd_/home/ubuntu/os/kernel-test/modules.order := {   echo /home/ubuntu/os/kernel-test/mymodule.ko; :; } | awk '!x[$$0]++' - > /home/ubuntu/os/kernel-test/modules.order
