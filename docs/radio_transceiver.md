# Radio transceiver

more info here : https://www.addicore.com/NRF24L01-PA-LNA-p/ad269.htm

Datasheet: https://www.sparkfun.com/datasheets/Components/SMD/nRF24L01Pluss_Preliminary_Product_Specification_v1_0.pdf

### pinout

![pinout](pinout.png)

![NRF24L01-Transceiver-Module-Pinouts-Connections](NRF24L01-Transceiver-Module-Pinouts-Connections.jpg)



### Connection Guide

```
Radio MISO -> Arduino 12 MISO
Radio MOSI -> Arduino 11 MOSI
Radio SCK  -> Arduino 13 SCK
Radio CE   -> Any GPIO Pin (can be same as CSN)
Radio CSN  -> Any GPIO Pin (pin 10 recommended)
Radio IRQ  -> Any GPIO Pin (optional)
```



**Notes**

To maximize the transmission/reception distance (range) we recommend the following:

- using a slower data rate, preferrably 250kbps
- in addition to the GND header pin we recommend also connecting your source ground to the ground through-holes or castellations near the brass antenna connector
- adding a [10uF capacitor](https://www.addicore.com/10uF-50V-Electrolytic-Capacitor-p/177.htm) across GND and V+ to assist with power demands during transmitting and receiving
- using a dedicated 3.3V power source instead of an Arduino's 3.3V supply, a good 3.3V regulator such as the [LD33V](https://www.addicore.com/product-p/165.htm) works well

### Connection Guide

https://github.com/dparson55/NRFLite
