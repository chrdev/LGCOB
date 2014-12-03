LGCOB
=====

Legacy Game Controller Over Bluetooth

Copyright (c) 2014 chrdev

The LGCOB project is released under the MIT License.

The source code is available at
https://github.com/chrdev/lgcob

HTTPS clone URL:
https://github.com/chrdev/LGCOB.git

## Disclaimer

ALL TRADEMARKS MENTIONED IN LGCOB PROJECT BELONG TO THEIR RESPECTIVE OWNERS.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Introduction

LGCOB is game controller firmware for a certain types of low-cost CSR BlueCore chips. With LGCOB, a legacy gamepad can operate as a standard Bluetooth 2.1 HID game controller, working with most Bluetooth hosts including PCs and smartphones.

## Support List

All devices are tested working with CSR BlueCore4-External (BC417).

- SEGA Saturn Gamepad

## Usage

Due to copyright reasons, the built firmware cannot be published. you have to build it yourself. You may need a proprietary IDE like CSR BlueLab to achieve that.

1. Backup the original firmware and PS keys.

2. Open *source/lgcob.wix* in xIDE, go to *Project* -> *Properties* -> *Build System*, and choose your *Target Device*.

3. Build the LGCOB firmware, generates *.xdv* / *.xpv* file pair.

4. Flash the firmware .

5. Apply *source/lgcob.psr* to PS keys.

6. Power off the Bluetooth module and connect the game controller to the right PIO pins. Check out *connection* directory for more details.

7. Power up the Bluetooth module and enjoy.

## Development

To add support for a new game controller, you should follow these steps, which is  described by an example. The name of the new device is called diy.

1. Create *diy.h* in *source/devices* directory. You may start by copying *source/devices/skeleton.h* as a start point, and use *source/devices/ss.h* as reference. In the xIDE, Put *source/devices/diy.h* in *Devices* folder. 

2. Edit *source/devices/device.h*, make sure it conditionally includes the new device header file, like this
```
#ifdef DEVICE_DIY
#include "diy.h"
#endif
```

3. Edit *source/lgcob_vmprops.xml*, insert a new line to inform the xIDE there is a new target device to choose from. The new line should come just before
```
<option internalval="DEVICE_SKELETON">Skeleton</option>
```
and have the contents of
```
<option internalval="DEVICE_DIY">DIY Game Controller</option>
```

## Acknowledgment

My first smartphone, Smartisan T1 inspired the creation of this project.

## Contact

chrdev@zoho.com
(English and Chinese preferred)

## History

### 2014-12-03 (UTC+8)

First release

Known Issue:

- Won't reconnect to some paired hosts, this function is simply not implemented. Walk-around: delete saved device from  the host and re-pair. 
