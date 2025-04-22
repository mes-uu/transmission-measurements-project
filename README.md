# Transmission Measurements Project

TODO describe project and instrument.

<img src="testbed/images/testbed.jpeg" alt="(image not found)" height="280">
<img src="buoy/images/buoy_1.jpeg" alt="(image not found)" height="280">
<img src="testbed/images/testbed_in_water.png" alt="(image not found)" height="280">

## Repository Contents and Structure

The repository is structured according to different components of the instrument. These are

* A small [buoy](buoy/README.md) to carry the testbed, built from readily avalilable materials

* A [carrierplate](carrierplate/README.md) to position the testbed in strategic locations. The testbed is straped to the plate which in turn can be fixated to a stirdy rod or rope

* A [waterproof testbed](testbed/README.md) consisting of

  * A [conotrol and datalogging sytem](testbed/conotrol_and_datalogging_sytem/README.md) based on a Raspberry Pi. It interfaces with multiple independent devices for which it can switch the power and coomunicate via an I2C bus.

  * An [internal mouting system](testbed/internal_mounting_system/README.md) to structure the space inside the testbed

  * (will be added later) Documentation of the tranmsission modules used to test signal quality of different technologies

## Team and Funding

The research project is part of the Vici ENW programme ['Tracing Marine Macroplastics by Unraveling the Ocean's Multiscale Transport Processes'](https://www.nwo.nl/en/projects/vic222025)

* Project holder: Marc Schneiter, PhD candidate (m.e.schneiter@uu.nl)
* Contributors:
	* Co-Supervision PhD: Rolf Hut (R.W.Hut@tudelft.nl)
  * Supervision PhD: Erik van Sebille (e.vansebille@uu.nl)
  * Deployments and Measurements: Lagrangian Ocean Analysis team Utrecht (https://oceanparcels.org/utrechtteam)
  * Pictures: Isabelle Pelster José (i.pelsterjose@students.uu.nl), Goncalo Borges Simoes de Albergaria Dias (g.borgessimoesdealbergariadias@uu.nl)

## License

This project is released under the [Creative Commons - Attribution - ShareAlike 4.0](https://creativecommons.org/licenses/by-sa/4.0/) license.