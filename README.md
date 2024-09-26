# TCP/IPv4 Server Test
## Brief Explanation
TCP/IPv4 server test application demonstrates connection-oriented client-to-server model on a Zynq-7000 SoC platform and PC setup. TCP/IPv4 server application is developed for Linux OS running on a Zynq-7000 SoC processor and utilizes socket connection APIs for client (PC) and server (Zynq-7000 SoC) interaction. Server application receives data from the client and sends same data back over the socket connection. 

Hardware block design of the Zynq chip includes processing system, ethernet interface, SD card interface and USB/UART interface. PetaLinux tools are used to configure embedded linux kernel with the application and create linux image. 

Arty-Z7 (Zynq-7000 SoC) is used for the application tests and is configured for booting embedded linux from SD card interface. 
## Hardware Block Design
Hardware block design is developed by using Vivado 2021.1 and XSA file is created to be used with PetaLinux tools.  

Block design and Zynq processing system configurations can be seen in the figures below.


![artyz7_prj_xsa](https://github.com/user-attachments/assets/51a4ee9a-1ff8-4afd-b7ef-ae5f586bdc5c)
                                Figure-1 Block Design


![interfaces_xsa](https://github.com/user-attachments/assets/3b5a7979-0f64-49ac-8dba-64e717f98396)          
                                Figure-2 Interfaces


![clock_xsa](https://github.com/user-attachments/assets/d3d7fa52-dd8d-4fc6-91db-f69b7fdcc8ba)                                        
                                Figure-3 Clocks 

## Petalinux Project
Following commands shown below can be used to create petalinux project and embedded linux image.
- Create the petalinux project.

  petalinux-create --type project --template zynq --name (project-name)
- Change current directory to  created project directory <project-name>.

  cd  /project-name
- Get hardware design contained in .XSA file.

  petalinux-config --get-hw-description (directory of the XSA file)
- Configure the petalinux project. Default configuration settings work for this project.

  petalinux-config
- Configure linux kernel. Default configuration settings work for this project.

  petalinux-config -c kernel
- Create custom application tcpsrvtest (tcp server test).

  petalinux-create -t apps --template c --name tcpsrvtest --enable
- Copy tcpsrvtest.c and Makefile files that are included in petalinux_files folder to /project-spec/meta-user/recipes-apps/tcpsrvtest/files/ folder in the petalinux project.
- Update bitbake file tcpsrvtest.bb in /project-spec/meta-user/recipes-apps/tcpsrvtest/ folder with  the same settings of the reference bitbake file included in petalinux_files folder.
- Build root file system.

  petalinux-build -c rootfs
- Build the Petalinux project.

  petalinux-build
- Generate boot files and linux image.

  petalinux-package --boot --fsbl ./images/linux/zynq_fsbl.elf --fpga ./images/linux/system.bit --u-boot --force
- Write generated boot files into an SD card that will boot the board.
Copy BOOT.BIN, image.ub and boot.scr files from /images/linux/ folder to first partition of the SD card which is in FAT32 format. Extract rootfs.tar.gz file from /images/linux/ folder to second partition of the SD card which is in ext4 format.  
