***********************************************
Photovoltaic DC Smart Energy Meter
***********************************************

Overview
========

The DC Smart Energy Meter is capable of monitoring the power production (i.e. DC current and voltage) from photovoltaics. It used a non-contact hall effect sensor, voltage divider circuit, and a WiFi-SoC to communicate real-time power production to a communication gateway.  This directory is organized into: 

    * Archive
    * Eagle Design Files
    * Enclosure Design Files
    * MC_Firmware 
    * Notes


Description (template)
======================

**Components**
    	1N5822RL Schottky Diode	
    		http://www.digikey.com/product-detail/en/stmicroelectronics/1N5822RL/497-14742-1-ND/4868910
	LM2576 Voltage Regulator
		http://www.digikey.com/product-detail/en/texas-instruments/LM2576SX-3.3-NOPB/LM2576SX-3.3-NOPBCT-ND/3526859
	100 uH Inductor	
		http://www.digikey.com/product-detail/en/bourns-inc/RLB0914-101KL/RLB0914-101KL-ND/2352772
	1000 uF Electrolytic Capacitor SMD
		http://www.mouser.com/ProductDetail/Panasonic/EEE-0JA102UP/?qs=sGAEpiMZZMtZ1n0r9vR22cS6GEbW2fto8pw8wJRO4%2fA%3d
	100 uF Electrolytic Capacitor SMD
		http://www.mouser.com/ProductDetail/Panasonic/EEE-1HA101UP/?qs=sGAEpiMZZMtZ1n0r9vR22cS6GEbW2fto6jpFqu6vaQU%3d
	47 uF Electrolytic Capacitor SMD
		http://www.mouser.com/ProductDetail/Panasonic/EEE-1CA470WR/?qs=sGAEpiMZZMtZ1n0r9vR22cS6GEbW2ftocdkiN2ijlcU%3d
	Pin Jumpers
		http://www.wholesalecables.com/ProductDetails.asp?ProductCode=WC-30J1-00100&gdffi=f6ac2ef474d5472dae5ecb55a73b8a43&gdfms=F3BEDE5BDCAE47B9A31977FF8514A2CE
	MCP3208	
		http://www.digikey.com/product-detail/en/microchip-technology/MCP3208T-CI-SL/MCP3208T-CI-SLCT-ND/5818697
	TLV2462 OP AMP	
		http://www.digikey.com/product-detail/en/texas-instruments/TLV2462CDR/296-2434-1-ND/374274
	ESP8266	
		http://www.gearbest.com/transmitters-receivers-module/pp_227650.html?wid=1
	2P Pluggable Term Block	
		http://www.mouser.com/ProductDetail/FCI/20020007-H021B01LF/?qs=sGAEpiMZZMsDddcp1dBDJNR7rNETrFyKff32nDP4Fbc%3d
	2P Pluggable Term Socket
		http://www.mouser.com/ProductDetail/FCI/20020109-H021A01LF/?qs=sGAEpiMZZMsDddcp1dBDJNR7rNETrFyKRqvksFMxIlM%3d
	3P Pluggable Term Block	
		http://www.mouser.com/ProductDetail/FCI/20020007-H031B01LF/?qs=sGAEpiMZZMsDddcp1dBDJNR7rNETrFyKs6VOdOjviT8%3d
	3P Pluggable Term Socket
		http://www.mouser.com/ProductDetail/FCI/20020110-H031A01LF/?qs=sGAEpiMZZMsDddcp1dBDJLKcJAgXGcjNxB%2fvZAiBMjo%3d
	Some 805 SMD resistors


**Algorithm**
    Voltage is scaled down with a voltage divider and fed into the ADC in input 0.
	Hall effect transducer signal is amplified with the non-inverting op-amp circuit.
	Amplified current signal is input into ADC input 1.
	Calibration data was accumulated to determine appropriate signal processing.
	Data is sampled for 15000 cycles and averaged.
	The average power is output and prepared for sending to database.

	Communication protocol is yet to be determined for sending power data.   

Dependencies (template)
=======================

**Algorithms**
    	#include <ESP8266WiFi.h>
	#include "AH_MCP320x.h"
	#include <SPI.h>
**Eagle**
    	ESP8266-ESP12.lbr
	MCP3208.lbr
	AWJLEagleLib.lbr
	Texas Instruments_By_element14_Batch_1.lbr

**Hardware**
    	The plate file in the EAGLE folder contains the brd for laser cutting.
	The design is for two plates to sandwich the hall effect transducer with the PCB sitting on top of the assembly. 
	The entire assembly is designed to fit into a marine grade PVC single gang electrical enclosure.
	Maximum input voltage is 40VDC and is regulated down to 3.3VDC.
	This is designed with the open circuit voltage of a single panel or panels in parallel in mind.
