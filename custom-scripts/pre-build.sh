#!/bin/sh

cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

cp $BASE_DIR/../custom-scripts/S50hello $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S50hello
cp $BASE_DIR/../custom-scripts/hello/a.out $BASE_DIR/target/usr/bin/a.out
chmod +x $BASE_DIR/target/usr/bin/a.out