**NOTE:** All 0.x releases are kind of "alpha-versions" without expectations of API backward-compatibility.

## Table of Contents
[mdz_ansi_dyn Overview](#mdz_ansi-Overview)<br>
[mdz_ansi_dyn Advantages](#mdz_ansi-Advantages)<br>
[mdz_ansi_dyn Usage](#mdz_ansi_dyn-Usage)<br>
[Licensing info](#Licensing-info)<br>

## mdz_ansi_dyn Overview
Please take a look at *"mdz_ansi_dyn.h"* C header file or [mdz_ansi_dyn Wiki] site for detailed functions descriptions.

[mdz_ansi_dyn Wiki]: https://github.com/maxdz-gmbh/mdz_ansi_dyn/wiki

[mdz_ansi_dyn] - is a very lightweight, versatile and speedy C  library for handling single-byte (ASCII/ANSI) strings, developed by [maxdz Software GmbH]. Source code of library is highly-portable, conforms to ANSI C 89/90 Standard.

Only shared/dynamically loaded libraries (*.so* and *.dll* files with import libraries) are available for evaluation testing purposes. Static libraries are covered by our commercial licenses.

**Win32** binaries are built using Visual Studio Platform Toolset "v90", thus are compatible with Windows versions from Windows 2000.<br>
**Win64** binaries are built using Visual Studio Platform Toolset "v100", thus are compatible with Windows versions from Windows XP (64-bit).<br>

**Linux** binaries are built against Linux Kernel 2.6.17 - and thus should be compatible with Debian (from ver. 4), Ubuntu (from ver. 6.10), Fedora (from ver. 9), Red Hat/CentOS (from ver. 5)

**FreeBSD** binaries - may be used from FreeBSD ver. 7.0 (for x86) and ver. 8.0 (for x64)

~~**Android** x86/armeabi-v7a binaries - may be used from Android API level 16 ("Jelly Bean" ver. 4.1.x)<br>~~
~~**Android** x86_64/arm64-v8a binaries - may be used from Android API level 21 ("Lollipop" ver. 5.0)~~

~~**macOS** binaries - x86_64, from *MacOS X v10.6.0*~~

[mdz_ansi_dyn]: https://github.com/maxdz-gmbh/mdz_ansi_dyn
[maxdz Software GmbH]: https://maxdz.com/

## mdz_ansi_dyn Advantages

**1. High Portability:** The entire codebase conforms to the ANSI C 89/90 standard.

**2. Minimal Dependencies:** *mdz_ansi_dyn* functions solely depend on standard C-library functions. This means you can integrate the library into your code without any additional dependencies beyond the standard platform libraries/APIs.

**3. Performance:** The library functions are highly optimized for speed, particularly for operations like searching. Performance is very similar to [mdz_ansi_alg] functions.

**4. Flexibilty:** Most library functions offer both "left position" and "right position" parameters, allowing you to limit the processing area. Additionally, the library provides more string functions than comparable libraries such as STL, Boost, or GLib.

**5. Extended Error-Checking:** All functions use strict error-checking mechanisms and return specific error codes pointing the problem precisely.

**6. Extended Control:** The functions perform only explicit operations. For example, when an "insert" function is called, it will return an error if there is not enough capacity in string - no implicit memory allocations/reservations will be made.

**7. Optional dynamic memory:** There is possibility to set dynamic memory management functions like malloc, realloc, free to use dynamic memory for storing/processing very long strings.

**8. Metadata encapsulated:** String controls its Size and Capacity, thus making automatical checking and if necessary adjustment during each string function call.

[mdz_ansi_alg]: https://github.com/maxdz-gmbh/mdz_ansi_alg

## mdz_ansi_dyn Usage

**Test license generation:** - in order to get free test-license, please proceed to our Shop page [maxdz Shop] and register an account. After registration you will be able to obtain free 30-days test-licenses for our products using "Obtain for free" button. 
Test license data should be used in *mdz_ansi_dyn_init()* call for library initialization.

**NOTE:** All 0.x releases are kind of "beta-versions" and can be used 1) only with test-license (during test period of 30 days, with necessity to re-generate license for the next 30 days test period) and 2) without expectations of interface backward-compatibility.

[mdz_ansi_dyn Wiki]: https://github.com/maxdz-gmbh/mdz_ansi_dyn/wiki
[maxdz Shop]: https://maxdz.com/shop.php

## Licensing info

Use of **mdz_ansi_dyn** library is regulated by license agreement in *LICENSE.txt*

Basically private non-commercial "test" usage is unrestricted. Commercial usage of library (incl. its source code) will be regulated by according license agreement.
