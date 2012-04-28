Autoconnect
===========

Automatically connect to saved WPA Enterprise networks on the Nokia N900 (workaround for [bug 3399](https://bugs.maemo.org/show_bug.cgi?id=3399))

It scans for networks periodically when there is no current connection. If the requested network becomes available, it tries to connect.

Compilation
-----------

Compile with Qt Creator / MADDE / Scratchbox:
* `qmake`
* `make`

Execution
---------

Copy binary to device and run as:
* `./autoconnect <network SSID> <scan interval (secs)>`

Tested with Qt 4.6.2 and MADDE 0.6.72.

Requires PR1.2 and Qt Mobility libs on the device (apt-get install libqtm*)

Note
----
Ideally we would listen for discovered networks just with the
QNetworkConfigurationManager::configurationChanged signal but this is only
emitted when QNetworkConfigurationManager::updateConfiguration() is called from
the program, not when scans are performed by other applications. It turns out
ICD_SCAN_REQUEST_PASSIVE is not implemented in icd2.