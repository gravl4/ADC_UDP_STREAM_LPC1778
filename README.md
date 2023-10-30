# ADC_UDP_STREAM_LPC1778
ADC to UDP tream. LPC1778 based 


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
