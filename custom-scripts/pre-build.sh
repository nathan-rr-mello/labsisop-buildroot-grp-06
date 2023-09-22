#!/bin/sh

# Configuração de rede
cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

# Configuração para Hello World
cp $BASE_DIR/../custom-scripts/S50hello $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S50hello
cp $BASE_DIR/../custom-scripts/hello/hello-i686 $BASE_DIR/target/usr/bin/hello-i686
chmod +x $BASE_DIR/target/usr/bin/hello-i686

# Configuração para o TP1
#cp $BASE_DIR/../custom-scripts/TP1/server.py $BASE_DIR/target/root
#chmod +x $BASE_DIR/target/root

cp $BASE_DIR/../custom-scripts/TP1/S51server-init $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S51server-init
cp $BASE_DIR/../custom-scripts/TP1/server.py $BASE_DIR/target/usr/bin/server.py
chmod +x $BASE_DIR/target/usr/bin/server.py
