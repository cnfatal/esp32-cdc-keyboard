# esp-cdc-keyboard

ESP32 USB HID+CDC implementation, it is a composite device that supports HID and CDC on a single USB port.

It recieve messages from the CDC interface, then send back HID reports from the HID interfaces.

The commonly used to simulate keyboard and mouse the hardware way.

## Usage

### Hardware Required

An ESP32-S3 development board that have USB-OTG supported.

The board has two USB ports, one is USB-OTG, the other is USB-UART(for debugging).
Connect both of them to your computer.

### Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```bash
idf.py -p PORT flash monitor
```

### Take Control from Host

python example:

```python
def key_report(modifiers: int, keys: List[int]) -> bytes:
    keys += [0] * (6 - len(keys))
    return bytes(
        [0x01, modifiers, 0x00, keys[0], keys[1], keys[2], keys[3], keys[4], keys[5]]
    )

def mouse_report(buttons: int, x: int, y: int) -> bytes:
    return bytes([0x02, buttons, x, y, 0x00, 0x00])

def main():
    ser = serial.serial_for_url("alt://COM1")
    # move mouse relatively to x=20, y=30
    report = mouse_report(0, 20, 30)
    ser.write(report)
```

