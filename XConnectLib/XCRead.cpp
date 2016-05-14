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
#include <vector>
#include <functional>
#include <utility>

#include "Common.h"
#include "XCReaders.h"
#include "Conversion.h"
#include "XCRead.h"


typedef std::function<void(unsigned char*)> VariableReader;

void GetVariableReaders(size_t offset, size_t size, std::vector<std::pair<size_t, VariableReader> >& o_readers)
{
	static bool init = true;
	static VariableReader readers[0x4000];
	if (init) {
		init = false;
		using namespace xcread;
		readers[0x0020] = GroundElev;
		readers[0x0238] = HoursLocal;
		readers[0x0239] = MinsLocal;
		readers[0x023A] = SecsLocal;
		readers[0x023B] = HoursZulu;
		readers[0x023C] = MinutesZulu;
		readers[0x023E] = DayOfYear;
		readers[0x0264] = Paused;
		readers[0x02A0] = MagVar;
		readers[0x02B4] = GS;
		readers[0x02B8] = TAS;
		readers[0x02BC] = IAS;
		readers[0x02C8] = VS;
		readers[0x02D4] = ADF2Freq;
		readers[0x02DC] = Nav2Info;
		readers[0x0330] = QNH;
		readers[0x0354] = Transponder;
		readers[0x0366] = OnGround;
		readers[0x034C] = ADF1Freq;
		readers[0x034E] = COM1Freq;
		readers[0x0350] = NAV1Freq;
		readers[0x0352] = NAV2Freq;
		readers[0x0560] = LatitudeHi;
		readers[0x0564] = LatitudeLo;
		readers[0x0568] = LongitudeHi;
		readers[0x056C] = LongitudeLo;
		readers[0x0570] = Altitude;
		readers[0x0578] = Pitch;
		readers[0x057C] = Roll;
		readers[0x0580] = Heading;
		readers[0x0844] = NAV2RwyHdg;
		readers[0x0858] = NAV2Latitude;
		readers[0x085C] = NAV1Latitude;
		readers[0x0860] = NAV2Longitude;
		readers[0x0864] = NAV1Longitude;
		readers[0x0870] = NAV1RwyHdg;
		readers[0x0B74] = CenterFuelLevel;
		readers[0x0B78] = CenterFuelCapacity;
		readers[0x0B7C] = LeftFuelLevel;
		readers[0x0B80] = LeftFuelCapacity;
		readers[0x0B94] = RightFuelLevel;
		readers[0x0B98] = RightFuelCapacity;
		readers[0x0BB4] = PitchInput;
		readers[0x0BB8] = RollInput;
		readers[0x0C29] = DME1;
		readers[0x0C33] = DME2;
		readers[0x0C48] = NAV1Deflection;
		readers[0x0C49] = NAV1GS;
		readers[0x0C4B] = NAV1FromTo;
		readers[0x0C4C] = NAV1CDI;
		readers[0x0C4E] = NAV1OBS;
		readers[0x0C50] = NAV1Radial;
		readers[0x0C59] = NAV2Deflection;
		readers[0x0C5B] = NAV2FromTo;
		readers[0x0C5E] = NAV2OBS;
		readers[0x0C60] = NAV2Radial;
		readers[0x0C6E] = NAV2GS;
		readers[0x0C6F] = NAV2CDI;
		readers[0x0E8C] = Temp;
		readers[0x0E90] = WindSpeed;
		readers[0x0E92] = WindDir;
		readers[0x2000] = Eng1N1;
		readers[0x2018] = Eng1N2;
		readers[0x088C] = Eng1Throttle;
		readers[0x08BE] = Eng1Temp;
		readers[0x08BA] = Eng1OilPress;
		readers[0x08B8] = Eng1OilTemp;
		readers[0x08D0] = Eng1OilQty;
		readers[0x08DC] = Eng1HydraulicQty;
		// readers[0x0918] = Eng1FuelFlow;
		readers[0x0924] = Eng2Throttle;
		readers[0x0BE0] = Flaps1;
		readers[0x0BE4] = Flaps2;
		readers[0x11C6] = MachSpeed;
		readers[0x11D0] = TAT;
		readers[0x2100] = Eng2N1;
		readers[0x2118] = Eng2N2;
		readers[0x0956] = Eng2Temp;
		readers[0x0952] = Eng2OilPress;
		readers[0x0950] = Eng2OilTemp;
		readers[0x0968] = Eng2OilQty;
		readers[0x0974] = Eng2HydraulicQty;
		// readers[0x09B0] = Eng2FuelFlow;
		readers[0x3000] = NAV1Info;
		readers[0x301F] = NAV2Info;
		readers[0x303E] = ADF1Info;
		readers[0x3118] = COM2Freq;
		readers[0x31E4] = HeightAGL;
		readers[0x3300] = RadioFlags;
		readers[0x3304] = XConnectVersion;
		readers[0x3308] = XPlaneVersion;
		readers[0x3324] = PressureAltitude;
		readers[0x3330] = Eng1ThrottleInput;
		readers[0x3332] = Eng2ThrottleInput;
		readers[0x3542] = StandbyQNH;
		readers[0x3544] = StandbyAltitude;
        //
		// 0x0898 2 Engine RPM ** r
		readers[0x0898] = EngRPM;
		// 0x036C 1 stall Warning ** rw
		readers[0x036C] = StallWarning;
		// 0x0BF0 4 Nose Gear Position
		readers[0x0BF0] = NoseGearPos;
		// 0x0918 8 Engine 1 Vibration
		readers[0x0918] = Eng1Vibration;
		// 0x09B0 8 Engine 2 Vibration
		readers[0x09B0] = Eng2Vibration;
		// 0x11BA 2 G-Force  (g's * 625) ** r
		readers[0x11BA] = GForce;
		// 0x11BE 2 Relative Angle of Attack ** rw
		readers[0x11BE] = RelAOA;
		// 0x3060 8 Lateral acceleration
		readers[0x3060] = LatAccel;
		// 0x3080 8 Roll acceleration
		readers[0x3080] = RollAccel;
		// 0x07BC 4 Autopilot Master switch ** rw
		readers[0x07BC] = OttoMasterSwitch;
		// 0x07D0 4 Autopilot Alitude Hold ** rw
		readers[0x07D0] = OttoAltHold;
		// 0x2ED0 8 Alpha - Radians ** r
		readers[0x2ED0] = Alpha;
		// 0x2ED8 9 Beta - Radians ** r
		readers[0x2ED8] = Beta;
		// 0x6030 8 Ground Speed - m/s ** r
		readers[0x6030] = GroundSpeed;
		// 0x2EA8 8 Aileron deflection - rads
		readers[0x2EA8] = AileronDeflection;
		// 0x0BC0 2 Elevator Trim Control  (for AP following) ** rw
		readers[0x0BC0] = ElevatorTrimCtrl;
		// 0x05DC 2 Slew mode indicator
		readers[0x05DC] = SlewModeInd;
		// 0x0588 8 Sim local time in seconds ** r
		readers[0x0588] = SimLocalTimeSecs;  // **
		// 0x66F8 8 Jet/Propwash - Engine 1 in m/s
		readers[0x66F8] = Eng1JetPropwash; // **
		// 0x66E8 8 Stall Warning AoA - Degrees
		readers[0x66E8] = StallWarningAOA; // **
		// 0x30A8 8 Pitch Rate Rad/s (+ve nose up in X-Plane)
		readers[0x30A8] = PitchRate;
		// 0x66E0 8 CG Position displacement from default, meters
		readers[0x66E0] = CGPosDispFromDflt; // **
		// 0x0C02 2 Aileron Trim Control  (for AP following)
		readers[0x0C02] = AileronTrimCtrl; // **
		// 0x0C04 2 Rudder Trim Control  (for AP following)
		readers[0x0C04] = RudderTrimCtrl; // **
		// 0x0BBA 2 Elevator Control Input  (for AP following)
		readers[0x0BBA] = ElevatorCtrlInput;
		// Elevator position  (for AP following)
		// 		Elev_FS_offset := 0x66DA 2 Ail_FS_offset := 0x66DC 2
		readers[0x66DA] = ElevatorOffsetHeli;
		readers[0x66DC] = AileronOffsetHeli;
		//		Elev_FS_offset := 0x0BB2 2 Ail_FS_offset := 0x0BB6 2
		readers[0x0BB2] = ElevatorOffset;
		readers[0x0BB6] = AileronOffset;
	}
	while (size) {
		if (readers[offset])
			o_readers.push_back(std::make_pair(offset, readers[offset]));
		size--;
		offset++;
	}
}

void GetVariable(BYTE* target, DWORD offset, DWORD size)
{
	//char debug[256];
	//sprintf(debug, "XConnect: read offset 0x%lx (%lu bytes)\n", offset, size);
	//XPLMDebugString(debug);

//	FILE* str = fopen("XConnect.out.txt", "a+");

	std::vector<std::pair<size_t, VariableReader> > readers;
	GetVariableReaders(offset, size, readers);
	for (size_t i = 0; i < readers.size(); ++i) {
		size_t off = readers[i].first;
		VariableReader& reader = readers[i].second;
		reader(XConnectMemBlock + off);
	}

	BYTE* memory = XConnectMemBlock;
	memory+=offset;
	CopyMemory(target, memory, size);

//	fclose(str);
}
