# Control and Datalogging System

This README documents hardware with a [description](#description), a [list of materials](#list-of-materials), instructions for the [assembly](#assembly), additional [remarks](#remarks), and [example use cases](#example-use-cases). The software for the main board and an example for interface boards connecting to an Arduino is provided in the [software](./software/) directory.

## Description

The hardware and software described here enables two main functionalities:

1) Remote, programmable power control by the main board (Raspberry Pi) for each module

2) A shared I2C bus with the main board (Raspberry Pi) as the root device and the modules as the node devices

In addition, the scripts in the [software](./software/) directory include

* Routines to read and write data of different types from and to the modules

* Testing and debugging of the I2C communication with a VERBOSE flag

* An example implementation of the module interface for Arduino UNO, with time synchronization, a read-write register, and a remotely invokable funtion

Since the communication is based on I2C, a module must provide this interface. For Arduinos and in case of a double occupation of the hardware I2C interface, the integration is less straight forward: In principle, using the same bus for internal and external communication is possible with the standard Arduino I2C library. However, not all libraries for I2C-based devices are compatible with the provided interface script. In addition, the I2C devices of the module will share the bus with the rest of the datalogging system, in particular any other module that is powered up at the same time. Alternatively, a software I2C library may come in handy to use alternative pins for the communication with the devices of the module, which is supported by some libraries for I2C-based devices.

<p float="left">
<img src="images/diagram_conotrol_and_datalogging_sytem.png" alt="(image not found)" height="280">
</p>

## List of Materials

### Main Board

| <div style="width:150px">Image</div> | Designator | Quantity | Price/Quantity (EUR) | Total Cost (EUR) | Source | Remarks |
| - | - | - | - | - | - | - |
| ![ (image not found)](images/materials/raspberry_pi_zero_2_w.jpg) | Raspberry Pi - Raspberry Pi® Zero 2 W 512 MB 1 x | 1 | 18.48 | 18.48 | https://www.conrad.com/en/p/raspberry-pi-zero-2-w-raspberry-pi-zero-2-w-512-mb-1-x-1-0-ghz-2482940.html |  |
| ![ (image not found)](images/materials/logic_level_converter_bi_directional.jpg) | Logic Level Converter - SparkFun Logic Level Converter Bi Directional | 2-3 | 3.50 | 2-3 * 3.50 | https://opencircuit.shop/product/sparkfun-logic-level-converter-bi-directional | Number of required channels is 2+number of modules |
| ![ (image not found)](images/materials/power_bank_30000_mah_lipo_usb.jpg) | Power Bank - Verico Power Pro PD Power bank 30000 mAh LiPo USB type | 1 | 30.24 | 30.24 | https://www.conrad.com/en/p/verico-power-pro-pd-power-bank-30000-mah-lipo-usb-type-a-usb-c-black-2583063.html |  |
| ![ (image not found)](images/materials/cable_usb_type_a_to_micro.jpg) | Micro USB cable |  |  |  |  | Should be tested to reliably provide sufficient power to the Pi |
|  | [TODO Pi-header] |  |  |  |  |  |
|  | [TODO USB connector exposing pins] |  |  |  |  |  |
| ![ (image not found)](images/materials/6pin_board_connector.jpg) | (6pin board connector - TE Pin strip AMPMODU MOD II Contact spacing: 2.54 mm) | (no. modules) | (1.08) | (N * 1.08) | https://www.conrad.com/en/p/te-connectivity-pin-strip-standard-ampmodu-mod-ii-total-number-of-pins-6-contact-spacing-2-54-mm-280379-2-1-pc-s-1079641.html | Contact spacing 2.54mm, 5pin connector also suffices |
| ![ (image not found)](images/materials/2pin_power_screw_terminal.png) | (2pin power screw terminal - PTR Hartmann 50500020134G Contact spacing: 5.08 mm) | (1) | (0.17) | (0.17) | https://www.conrad.nl/nl/p/ptr-hartmann-50500020134g-klemschroefblok-1-50-mm-aantal-polen-2-grijs-1-stuk-s-731877.html |  |
|  | PCB Prototype Board | 1 |  |  |  | Contact spacing 2.54mm, TODO approx dimensions |
| |
|  |  |  |  | [TODO] |  |  |

### Module Interface

| <div style="width:150px">Image</div> | Designator | Quantity | Price/Quantity (EUR) | Total Cost (EUR) | Source| Remarks |
| - | - | - | - | - | - | - |
| ![ (image not found)](images/materials/logic_level_converter_bi_directional.jpg) | Logic Level Converter - SparkFun Logic Level Converter Bi Directional | 1 | 3.50 | 3.50 | https://opencircuit.shop/product/sparkfun-logic-level-converter-bi-directional |  |
| ![ (image not found)](images/materials/p_mosfet_irf9540pbf.jpg) | P channel enhanced MOSFET - Vishay IRF9540PBF MOSFET | 1 | 1.23 | 1.23 | https://www.conrad.nl/nl/p/vishay-irf9540pbf-mosfet-1-p-kanaal-150-w-to-220ab-597146.html |  |
| ![ (image not found)](images/materials/n_mosfet_irfz44pbf.jpg) | N channel enhanced MOSFET - Vishay IRFZ44PBF MOSFET | 1 | 1.64 | 1.64 | https://www.conrad.nl/nl/p/vishay-irfz44pbf-mosfet-1-n-kanaal-150-w-to-220ab-597269.html |  |
| ![ (image not found)](images/materials/resistor_1k.jpg) | Resistor 1K | 1 |  |  |  |  |
| ![ (image not found)](images/materials/resistor_10k.jpg) | Resistor 10K | 2 |  |  |  |  |
| ![ (image not found)](images/materials/6pin_board_connector.jpg) | (6pin board connector - TE Pin strip AMPMODU MOD II Contact spacing: 2.54 mm) | (1) | (1.08) | (1.08) | https://www.conrad.com/en/p/te-connectivity-pin-strip-standard-ampmodu-mod-ii-total-number-of-pins-6-contact-spacing-2-54-mm-280379-2-1-pc-s-1079641.html | Contact spacing 2.54mm, 5pin connector also suffices |
| ![ (image not found)](images/materials/2pin_power_screw_terminal.png) | (2pin power screw terminal - PTR Hartmann 50500020134G Contact spacing: 5.08 mm) | (1) | (0.17) | (0.17) | https://www.conrad.nl/nl/p/ptr-hartmann-50500020134g-klemschroefblok-1-50-mm-aantal-polen-2-grijs-1-stuk-s-731877.html |  |
| ![ (image not found)](images/materials/arduino_proto_shield.jpg) | PCB Prototype Board Contact spacing: 2.54 mm - (Arduino PROTO SHIELD Development board) | 1 | (12.39) | (12.39) | https://www.conrad.nl/nl/p/arduino-proto-shield-development-board-1969858.html | Contact spacing 2.54mm |
| |
|  |  |  |  | [TODO] |  |  |

## Assembly

[TODO]

## Remarks

* The following board families have successfully been integrated in the system
  * Arduino UNO
  * Arduino MKR
  * Particle Electron

* We have used the system to take measurements with five and eight modules. All modules were based on Arduino UNO, Arduino MKR or Particle Electron boards. A small degradation of the reliability of the I2C communication was occurring when using eight boards, since I/O exceptions from failed read/write interactions were raised occasionally. Repeating these failed interactions seemed to solve the issue, and it is unclear if this degradation was caused by connecting more modules.

## Example Use Cases

[TODO]

## License

See the [README](./../../README.md) in the [root directory](./../../) of this repo for license information