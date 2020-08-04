#!/bin/sh

package_list=(autoconf automake bc binutils binutils-gold bison build-essential bzip2 cmake curl device-tree-compiler dpkg flex g++ gawk gcc gettext git gperf libc6-dev libncurses-dev libncurses5-dev libpcre3-dev libssl-dev libtool libxml-parser-perl lzma make mtd-utils npm ocaml ocaml-findlib ocaml-nox patch pkg-config python python-yaml rcs repo sharutils subversion texinfo u-boot-tools unzip vim wget xz-utils zlib1g-dev chrpath libfdt-dev coccinelle lib32z1 lib32ncurses5-dev)

install_error=0
install_log=/tmp/build_package_install.log
echo "==============================" > $install_log
for p in ${package_list[*]};
do
    echo "=============================="
    echo "Installing package <$p> ..."
    apt-get install -y $p >> $install_log
    install_error="$?"
    if [[ "$install_error" == "0" ]]; then
        echo "Package <$p> install success!"
    else
        echo "Package <$p> install error!"
    fi
done

echo "Package install over, See log at $install_log"