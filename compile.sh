sudo make -j64;
sudo make install;
sudo make modules_install;
sudo mkinitramfs -o /boot/initrd.img-2.6.37.6ConversionOS+ 2.6.37.6ConversionOS+;
sudo grub-mkconfig -o /boot/grub/grub.cfg;
