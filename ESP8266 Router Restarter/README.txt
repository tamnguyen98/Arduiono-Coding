##Intro

This tool works with the ESP8266's onboard wifi. This code constant monitor the internet connection by pinging a server (Speedtest to be specific)
and when the ping fails 3 times, it connects to the router and request a reboot.

#Disclaimer

This code will not work straight out of the box since each router is different. So, make sure you change the code (wifi & router login, the POST data, and possibly URL)
 to work with your router.