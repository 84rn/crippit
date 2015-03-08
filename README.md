crippit
=======
A small tool for batch-downloading images from Reddit.

By default, it uses static CRT. If you wish to build with DLL CRT, change the target settings. 
Please remember to build libcurl with proper CRT!

Dependencies
-------------
* libcurl - http://curl.haxx.se/libcurl/

TODO:
-------------

* cURL error checking (being tested)
* zlib support for downloading albums as .zip
* wildcards for file names
* .gifv support when Imgur releases some more info
* SSL support
* porting to Linux
