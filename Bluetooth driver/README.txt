The fix is to update the driver with the Prolific PL2303 driver v3.3.2.102 (2008-24-09) Win8_x64_x86. 
This is done by opening Device Manager (windows) -> Ports(COM &LPT) -> PL2303HXA PHASED OUT SINCE 2012. 
You right-click this PL2303HXA PHASED OUT SINCE 2012 and update the driver -> Browse computer for drivers -> Let me pick from a list of available drivers 
-> Have Disk -> Browse -> Navigate to where you downloaded the file -> Then click ser2pl -> Then Ok.

Please note you can only 'listen' from the HC06 as it is a slave module unlike the HC05. So dont bother trying to implement remote control like I tried.

Follow me on insta @Genghis_Kuan #shamlessplug