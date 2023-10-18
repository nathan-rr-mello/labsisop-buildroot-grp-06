#!/bin/sh

# export LINUX_OVERRIDE_SRCDIR=/workspaces/linux-4.13.9
# sudo qemu-system-i386 --device e1000,netdev=eth0,mac=aa:bb:cc:dd:ee:ff --netdev tap,id=eth0,script=custom-scripts/qemu-ifup --kernel output/images/bzImage --hda output/images/rootfs.ext2 --nographic --append "console=ttyS0 root=/dev/sda"
# qemu-system-i386 --kernel output/images/bzImage --hda output/images/rootfs.ext2 --hdb sdb.bin --nographic --append "console=ttyS0 root=/dev/sda"
# modprobe sstf-iosched
# echo sstf > /sys/block/sdb/queue/scheduler
# cat /sys/block/sdb/queue/scheduler
# sector_read


# Configuração de rede
cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

# Configuração para Hello World
cp $BASE_DIR/../custom-scripts/S50hello $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S50hello
cp $BASE_DIR/../custom-scripts/hello/hello-i686 $BASE_DIR/target/usr/bin/hello-i686
chmod +x $BASE_DIR/target/usr/bin/hello-i686

# Configuração para o TP1
cp $BASE_DIR/../custom-scripts/TP1/S51server-init $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S51server-init
cp $BASE_DIR/../custom-scripts/TP1/server.py $BASE_DIR/target/usr/bin/server.py
chmod +x $BASE_DIR/target/usr/bin/server.py

# Driver teste
make -C $BASE_DIR/../modules/simple_driver/

# make sstf
make -C $BASE_DIR/../modules/T2_SSTF/