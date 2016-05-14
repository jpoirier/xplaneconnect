/*
This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdint.h>

#include "Common.h"
#include "Conversion.h"
#include "XCWrite.h"


static void SetByte(const char* strRef, int8_t value, int index);
static void SetFloat(const char* strRef, float value);
static void SetFloat(const char* strRef, float value, int index);
static void SetInt(const char* strRef, int value);
static void SetInt(const char* strRef, int value, int index);
static void SetDouble(const char* strRef, double value);
static void SetDouble(const char* strRef, double value, int index);



void SetByte(const char* strRef, int8_t value, int index)
{
	XPLMDataRef dataRef;
	dataRef = XPLMFindDataRef(strRef);
	XPLMSetDatavb(dataRef, (void*)&value, index, 1);
}


void SetFloat(const char* strRef, float value)
{
	XPLMDataRef dataRef;
	dataRef = XPLMFindDataRef(strRef);
	XPLMSetDataf(dataRef, value);
}

void SetFloat(const char* strRef, float value, int index)
{
	XPLMDataRef dataRef;
	dataRef = XPLMFindDataRef(strRef);
	XPLMSetDatavf(dataRef, &value, index, 1);
}

void SetInt(const char* strRef, int value)
{
	XPLMDataRef dataRef;
	dataRef = XPLMFindDataRef(strRef);
	XPLMSetDatai(dataRef, value);
}

void SetInt(const char* strRef, int value, int index)
{
	XPLMDataRef dataRef;
	dataRef = XPLMFindDataRef(strRef);
	XPLMSetDatavi(dataRef, &value, index, 1);
}

void SetDouble(const char* strRef, double value)
{
	XPLMDataRef dataRef;
	dataRef = XPLMFindDataRef(strRef);
	XPLMSetDatad(dataRef, value)
}

void SetDouble(const char* strRef, double value, int index)
{
	XPLMDataRef dataRef;
	dataRef = XPLMFindDataRef(strRef);
	XPLMSetDatavd(dataRef, &value, index , 1)
}

void WriteVariable(BYTE* source, DWORD offset, DWORD size)
{
	FILE* str = fopen("FDSConnectX.out.txt", "a+");

	if (offset == 0x0330) {
		int16_t val;
		CopyMemory(&val, source, size);
		float fVal = HPAToInch(val / 16.0f);
		SetFloat("sim/cockpit/misc/barometer_setting", fVal);
	} else if (offset == 0x088C) {
		int16_t val;
		CopyMemory(&val, source, size);
		float fVal = (float)val;
		fVal /= 16384.0;
		SetInt("sim/operation/override/override_throttles", 1);
		SetFloat("sim/flightmodel/engine/ENGN_thro_use", fVal, 0);
		SetInt("sim/operation/override/override_throttles", 0);
	} else if (offset == 0x0924) {
		int16_t val;
		CopyMemory(&val, source, size);
		float fVal = (float)val;
		fVal /= 16384.0;
		SetInt("sim/operation/override/override_throttles", 1);
		SetFloat("sim/flightmodel/engine/ENGN_thro_use", fVal, 1);
		SetInt("sim/operation/override/override_throttles", 0);
	} else if (offset == 0x0BB2) {
		if (PitchDisconnect) {
			SetInt("sim/operation/override/override_joystick_pitch", 1);
			int16_t val;
			CopyMemory(&val, source, size);
			float ratio = val / 16383.0f;
			SetFloat("sim/joystick/yoke_pitch_ratio", ratio);
		} else {
			SetInt("sim/operation/override/override_joystick_pitch", 0);
		}
	} else if (offset == 0x0BB6) {
		if (RollDisconnect) {
			SetInt("sim/operation/override/override_joystick_roll", 1);
			int16_t val;
			CopyMemory(&val, source, size);
			float ratio = val / 16383.0f;
			SetFloat("sim/joystick/yoke_roll_ratio", ratio);
		} else {
			SetInt("sim/operation/override/override_joystick_roll", 0);
		}
// TODO: rudder
 	// } else if(offset == 0x0BBA) {
	// }
	} else if (offset == 0x0C4E) {
		int16_t val;
		CopyMemory(&val, source, size);
		SetFloat("sim/cockpit/radios/nav1_obs_degm", (float)val);
		//fprintf(str, "Set Nav1 OBS: %d\n", val);
	} else if (offset == 0x0C5E) {
		int16_t val;
		CopyMemory(&val, source, size);
		SetFloat("sim/cockpit/radios/nav2_obs_degm", (float)val);
	} else if (offset == 0x3542) {
		int16_t val;
		CopyMemory(&val, source, size);
		float fVal = HPAToInch(val / 16.0f);
		SetFloat("sim/cockpit/misc/barometer_setting2", fVal);
	} else if (offset == 0x310A) {
		char val;
		CopyMemory(&val, source, size);

		Throttle1Disconnect = false;
		Throttle2Disconnect = false;
		PitchDisconnect = false;
		RollDisconnect = false;

		if (val & 1) {
			PitchDisconnect = true;
		}
		if (val & 2) {
			RollDisconnect = true;
		}
		if (val & 8) {
			Throttle1Disconnect = true;
			Throttle2Disconnect = true;
		}
		if (val & 64) {
			Throttle1Disconnect = true;
		}
		if (val & 128) {
			Throttle2Disconnect = true;
		}
		if (Throttle1Disconnect || Throttle2Disconnect) {
			SetInt("sim/operation/override/override_throttles", 1);
		} else {
			SetInt("sim/operation/override/override_throttles", 0);
		}
		if (PitchDisconnect) {
			SetInt("sim/operation/override/override_joystick_pitch", 1);
		} else
			SetInt("sim/operation/override/override_joystick_pitch", 0);
		if (RollDisconnect) {
			SetInt("sim/operation/override/override_joystick_roll", 1);
		} else
			SetInt("sim/operation/override/override_joystick_roll", 0);
// sim/flightmodel/failures/stallwarning	int	y	???	Stall Warning
// sim/operation/failures/rel_AOA	int	y	failure_enum	AOA
// sim/cockpit/autopilot/autopilot_mode	int	y	enum	The autopilot master mode (off=0, flight director=1, on=2)
// sim/cockpit/autopilot/altitude	float	y	ftmsl	Altitude dialed into the AP
// sim/flightmodel2/controls/aileron_trim	float	y	ratio	Aileron trim, in part of MAX FLIGHT CONTROL
// DEFLECTION. So, if the aileron trim is deflected enough to move the ailerons through 30% of their travel,
// then that is an aileron trim of 0.3.
		} else if (offset == 0x0300) {
			// # Created offset - 0x0300 VOR1 DME distance, 16-bit integer, nm * 10
			// # sim/cockpit/radios/nav1_dme_dist_m	float	y	nautical_miles	Our distance in nautical miles from the beacon tuned in on nav1.  override_navneedles
			// Dataref	BFF_DME_Dist	sim/cockpit/radios/nav1_dme_dist_m	float
			// Offset	0x0300 UINT16 1 rw	$BFF_DME_Dist 10 *
			uint16_t val;
			CopyMemory(&val, source, size);
			SetFloat("sim/cockpit/radios/nav1_dme_dist_m", (float)val);
		} else if (offset == 0x07FC) {
			// # Created offset - 0x07FC Gildeslope Status, 16-bit integer
			// # sim/cockpit2/autopilot/glideslope_status	int	n	enum	Autopilot glideslope status. 0=off,1=armed,2=captured
			// Dataref	BFF_GS_Hold	sim/cockpit2/autopilot/glideslope_status	int
			// Offset	0x07FC UINT32 1 rw	$BFF_GS_Hold
			uint32_t val;
			CopyMemory(&val, source, size);
			SetInt("sim/cockpit2/autopilot/glideslope_status", (int)val);
		} else if (offset == 0x0610) {
			// # Created offset - 0x0610 GPS: aircraft latitude, floating point double, in degrees (+ve = N, ve = S).
			// # sim/flightmodel/position/latitude	double	n	degrees	The latitude of the aircraft
			// Dataref	BFF_GPS_Lat	sim/flightmodel/position/latitude	double
			// Offset	0x6010 FLOAT64 1 rw	$BFF_GPS_Lat
			int64_t val;
			CopyMemory(&val, source, size);
			SetDouble("sim/flightmodel/position/latitude", (double)val);
		} else if (offset == 0x0618) {
			// # Created offset - 0x0618 GPS: aircraft longitude, floating point double, in degrees (+ve = E, ve = W).
			// # sim/flightmodel/position/longitude	double	n	degrees	The longitude of the aircraft
			// Dataref	BFF_GPS_Long	sim/flightmodel/position/longitude	double
			// Offset	0x6018 FLOAT64 1 rw	$BFF_GPS_Long
			int64_t val;
			CopyMemory(&val, source, size);
			SetDouble("sim/flightmodel/position/longitude", (double)val);
		} else if (offset == 0x0C02) {
			// # Custom offset - 0x0C02 Set Aileron Trim - data ref range +/1.0 scaled to +/- 16383
			// Dataref	BFF_ATrim	sim/flightmodel/controls/ail_trim	float
			// Offset	0x0C02 SINT16 1 rw	$BFF_ATrim 16383 * >BFF_ATrim @ 16383 /
			int16_t val;
			CopyMemory(&val, source, size);
			SetFloat("sim/flightmodel/controls/ail_trim", (float)val);
		} else if (offset == 0x0C04) {
			// # Custom offset - 0x0C04 Set Rudder Trim - data ref range +/1.0 scaled to +/- 16383
			// Dataref	BFF_RTrim	sim/flightmodel/controls/rud_trim	float
			// Offset	0x0C04 SINT16 1 rw	$BFF_RTrim 16383 * >BFF_RTrim @ 16383 /
			int16_t val;
			CopyMemory(&val, source, size);
			SetFloat("sim/flightmodel/controls/rud_trim", (float)val);
		} else {
			fprintf(str, "Unknown offset: %x\n", offset);
			BYTE* memory = XConnectMemBlock;
			memory += offset;
			CopyMemory(memory, source, size);
		}

	fclose(str);
}
