# fw_printenv-cli


Need a way to run this command on my VM for testing and having the ability to change values


## Setup

```
cat << EOF > ~/.fw_env 
baseaddr=0x21000000
baudrate=115200
bootargs=console=ttyS0,115200 panic=20 root=/dev/mtdblock3 init=/init mtdparts=NOR_FLASH:256k(boot),64k(env),2048k(kernel),\${rootmtd}(rootfs),-(rootfs_data) LX_MEM=\${memlx} mma_heap=mma_heap_name0,miu=0,sz=\${memsz}
bootcmd=sf probe 0; setenv setargs setenv bootargs \${bootargs}; run setargs; sf read \${baseaddr} \${kernaddr} \${kernsize}; bootm \${baseaddr}
bootcmdnor=sf probe 0; setenv setargs setenv bootargs \${bootargs}; run setargs; sf read \${baseaddr} \${kernaddr} \${kernsize}; bootm \${baseaddr}
bootdelay=0
ethact=sstar_emac
gatewayip=192.168.1.1
ipaddr=192.168.1.10
kernaddr=0x50000
kernsize=0x200000
memlx=0xFFE0000
memsz=0x9E9C000
netmask=255.255.255.0
rootaddr=0x250000
rootmtd=8192k
rootsize=0x500000
serverip=192.168.1.1
setsdcard=setenv updatetool fatload mmc 0
soc=ssc338q
stderr=serial
stdin=serial
stdout=serial
totalmem=256M
ubnor=sf probe 0; sf erase 0x0 \${kernaddr}; sf write \${baseaddr} 0x0 \${kernaddr}
uknor=mw.b \${baseaddr} 0xFF 0x1000000; \${updatetool} \${baseaddr} uImage.\${soc}; sf probe 0; sf erase \${kernaddr} \${kernsize}; sf write \${baseaddr} \${kernaddr} \${filesize}
updatetool=tftpboot
urnor=mw.b \${baseaddr} 0xFF 0x1000000; \${updatetool} \${baseaddr} rootfs.squashfs.\${soc}; sf probe 0; sf erase \${rootaddr} \${rootsize}; sf write \${baseaddr} \${rootaddr} \${filesize}
ver=U-Boot 2015.01 (Nov 01 2023 - 01:35:05)
sensor=imx415
ethaddr=42:F4:90:8D:D0:75
upgrade=https://github.com/OpenIPC/builder/releases/download/latest/ssc338q_fpv_openipc-urllc-aio-nor.tgz
EOF
```
