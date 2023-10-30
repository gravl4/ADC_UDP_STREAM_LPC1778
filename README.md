# ADC_UDP_STREAM_LPC1778
ADC to UDP tream. LPC178 based 


used with https://github.com/gravl4/ADC_UDP_LOGGER
generate ADC imitation 

lwip bare metal source
for IP101GR set #define LPC_PHYDEF_PHYADDR 1
for KSZ8863 set #define LPC_PHYDEF_PHYADDR 3

adjust transmit PC IP IP4_ADDR(&destIPAddr, 192, 168, 0, 5); and port 
udp_sendto(upcb_data, p, &destIPAddr, 2001);

device IP4_ADDR(&ipaddr, 192, 168, 0, 103); and port
err = udp_bind(upcb_command, IP_ADDR_ANY, 2002);  

MCU crystall 12MHz. 

Project with Ethernet bootloader. For input to boot before supply short GPIO 1:13 to GND next power supply. 
for update FW via bootloader:
1. short GPIO 1:13 to GND
2. supply device
3. connect via Ethernet to PC
4. adjust PC to 192.168.0.x
5. open in web browser 192.168.0.101
6. in page drug and drop x.bin file with new FW
7. click Upload ![изображение](https://github.com/gravl4/ADC_UDP_STREAM_LPC1778/assets/64896051/d4a9e57d-13d0-4ad4-a226-6deae6f8248b)

For enable DEBUG main FW need use J-Link and rebuild with default linker script. Project properties/Settings/MCU Linker/Manager Linker Script - check box.
![изображение](https://github.com/gravl4/ADC_UDP_STREAM_LPC1778/assets/64896051/ef79c583-996b-450d-8b33-d2c3c548e161)

ethernet phy circuit:
![изображение](https://github.com/gravl4/ADC_UDP_STREAM_LPC1778/assets/64896051/90fb3fa5-935f-498c-ae89-a1dd66e99d6c)
![изображение](https://github.com/gravl4/ADC_UDP_STREAM_LPC1778/assets/64896051/18ee786e-9c84-4120-a56e-6963f8502991)


