# Laser Runs

## Calibration Unit Testing

### HE Calibration Unit
Laser runs are used to commission the Calibration Units (CU) for the HE Phase 1 Upgrade.
Each CU has 6 pin-diodes (PD) that are read out by a QIE card. The QIE card has 12 QIE chips, but only 6 are used for the pin-diodes.
Additionally, the CU distributes light (measured by Pin-Diode channels 0 and 1) to 4 Readout Modules (RM).
Each HE RM has 48 SiPMs read out by 4 QIE cards, and each QIE card has 12 QIE chips and thus 12 channels.
There are a total of 192 SiPM channels in one Readout Box (RBX), and each RBX has 4 RMs and 1 CU.

### Laser Runs
The laser emits 337nm wavelength light. The CU has 5 inputs for inserting the laser.
There is one input that distributes light to pin-diodes 0, 1 and all 192 SiPMs.
There are four inputs that each distribute light to one pin-diode, channel 2, 3, 4, or 5.
The data is collected using uHTRtool.exe. We use histogramming mode to integrate a fixed number of orbits (e.g. 200,000).
We can obtain the max ADC value (from 0 to 255) that is measure for each SiPM and pin-diode channel.
Then we can convert this max ADC value to fC and then plot this data in various forms.

