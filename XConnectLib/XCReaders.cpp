#include "XCReaders.h"
#include "XCRead.h"
#include "Conversion.h"
#include "Common.h"
#include <stdio.h>

XPLMDataRef GetDataRef(const char* dataRef)
{
	XPLMDataRef intvar;

	StringMap::iterator it = VarMap.find(dataRef);

	if(it==VarMap.end())
	{
		intvar = XPLMFindDataRef(dataRef);
		VarMap[dataRef] = intvar;
	}
	else
	{
		intvar = it->second;
	}

	//intvar = XPLMFindDataRef(dataRef);
	return intvar;

}

float GetFloat(const char* dataRef)
{
	XPLMDataRef intvar = GetDataRef(dataRef);
	
	return XPLMGetDataf(intvar);
}

int GetInt(const char* dataRef)
{
	XPLMDataRef intvar;
	intvar = XPLMFindDataRef(dataRef);
	return XPLMGetDatai(intvar);
}

double GetDouble(const char* dataRef)
{

	XPLMDataRef intvar;
	intvar = XPLMFindDataRef(dataRef);
	return XPLMGetDatad(intvar);
}

float GetFloat(const char* dataRef, int offset)
{
	XPLMDataRef intvar;
	intvar = XPLMFindDataRef(dataRef);

	float ret;

	XPLMGetDatavf(intvar, &ret, offset, 1);

	return ret;
}
int GetInt(const char* dataRef, int offset)
{
	XPLMDataRef intvar;
	intvar = XPLMFindDataRef(dataRef);

	int ret;

	XPLMGetDatavi(intvar, &ret, offset, 1);

	return ret;
	
}

namespace xcread
{

  void GroundElev(unsigned char* target)
  {	// Ground altitude meters x 256

	  float agl = GetFloat("sim/flightmodel/position/y_agl");
	  float msl = GetFloat("sim/flightmodel/position/elevation");

	  int ga = (int)((msl - agl) * 256.0);

	  XCCopyMemory(target, &ga);
  }
	
  void HoursLocal(unsigned char* target)
  {	// Hour local

	  float sec = GetFloat("sim/time/local_time_sec");
	  int iSec = (int)(sec / 3600);
	  char cSec = (char)iSec;
	  XCCopyMemory(target, &cSec);
  }

  void MinsLocal(unsigned char* target)
  {	// minute
	  float sec = GetFloat("sim/time/local_time_sec");
	  int iHour = (int)(sec / 3600);
	  sec -= iHour * 3600;
	  int min = (int)(sec / 60);

	  char cMin = (char)min;

	  XCCopyMemory(target, &cMin);
  }

  void SecsLocal(unsigned char* target)
  {	// second
	  float sec = GetFloat("sim/time/local_time_sec");
	  int iHour = (int)(sec / 3600);
	  sec -= iHour * 3600;
	  int min = (int)(sec / 60);
	  sec -= min * 60;
	  char cSec = (char)sec;

	  XCCopyMemory(target, &cSec);
  }

  void HoursZulu(unsigned char* target)
  {	// Hour zulu

	  //fprintf(str, "Read hour\n");

		
	  float sec = GetFloat("sim/time/zulu_time_sec");

	  //fprintf(str, "Hour: %f\n", sec);

	  int iSec = (int)(sec / 3600);
	  char cSec = (char)iSec;

	  //fprintf(str, "Hour: %d\n", iSec);

	  XCCopyMemory(target, &cSec);
  }

  void MinutesZulu(unsigned char* target)
  {	// minute zulu
	  float sec = GetFloat("sim/time/zulu_time_sec");
	  int iHour = (int)(sec / 3600);
	  sec -= iHour * 3600;
	  int min = (int)(sec / 60);

	  char cMin = (char)min;

	  XCCopyMemory(target, &cMin);
  }

  void DayOfYear(unsigned char* target)
  {	// day in year

	  int day = GetInt("sim/time/local_date_days");
	  short sDay = (short)day;
	  XCCopyMemory(target, &sDay);
  }

  void Paused(unsigned char* target)
  {   // Paused
	  int pause = GetInt("sim/time/paused");
	  short sPause = (short)pause;
	  XCCopyMemory(target, &sPause);
  }

  void MagVar(unsigned char* target)
  {   // mag var
	  float magvar = GetFloat("sim/flightmodel/position/magnetic_variation") / 360.0 * 65536;
	  short sMagVar = -(short)magvar;
	  XCCopyMemory(target, &sMagVar);
  }

  void GS(unsigned char* target)
  {	// Ground speed

	  float gs = GetFloat("sim/flightmodel/position/groundspeed") * 65536.0;
	  unsigned int iGS = (unsigned int)gs;

	  XCCopyMemory(target, &iGS);

  }

  void TAS(unsigned char* target)
  {	// TAS

	  float ias = MetersToKnot(GetFloat("sim/flightmodel/position/true_airspeed") * 128.0);

	  //fprintf(str, "TAS: %e\n", ias);
	  unsigned int iIAS = (unsigned int)ias;

	  XCCopyMemory(target, &iIAS);
  }

  void IAS(unsigned char* target)
  {	// IAS

	  float ias = GetFloat("sim/flightmodel/position/indicated_airspeed2") * 128.0;
	  int iIAS = (int)ias;

	  XCCopyMemory(target, &iIAS);
  }

  void VS(unsigned char* target)
  {	// vertical speed
	  float vs = GetFloat("sim/cockpit2/gauges/indicators/vvi_fpm_pilot");
	  vs = vs / 60.0 / 3.28084 * 256;
	  int iVs = (int)vs;
	  XCCopyMemory(target, &iVs);
  }	

  void ADF2Freq(unsigned char* target)
  {	// adf2 freq
	  int freq = GetInt("sim/cockpit/radios/adf2_freq_hz");
		
	  short res = (short)GetBCD(freq);
	  XCCopyMemory(target, &res);
  }

  void Nav2Info(unsigned char* target)
  {   // nav2 identity
	  int freq = GetInt("sim/cockpit/radios/adf2_freq_hz");
	  float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  /*fprintf(str, "Freq: %d\n", freq);
	  fprintf(str, "Lat: %f\n", latitude);
	  fprintf(str, "Lon: %f\n", longitude);*/

	  XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 2);

	  char buf[6];
	  for(int i = 0;i<6;i++)
		  buf[i] = 0;


	  if(ref != XPLM_NAV_NOT_FOUND)
	  {
		  XPLMGetNavAidInfo(ref, 
							  NULL,	// type
							  &latitude,	// lat
							  &longitude,	// lon
							  NULL,	// height
							  &freq,	// freq
							  NULL,	// heading
							  buf,	// ID
							  NULL,	// name
							  NULL);	// reg
	  }

	  CopyMemory(target, buf, 6);

  }

  void QNH(unsigned char* target)
  {	// altimeter
	  float altimeter = GetFloat("sim/cockpit/misc/barometer_setting");
	  short sAlt = ((short)InchToHPA(altimeter)) * 16;
	  XCCopyMemory(target, &sAlt);
  }

  void Transponder(unsigned char* target)
  {
	  short xpdr = GetInt("sim/cockpit/radios/transponder_code");
	  short bcd = GetBCD(xpdr);
	  XCCopyMemory(target, &xpdr);
  }

  void OnGround(unsigned char* target)
  {
	  float agl = GetFloat("sim/flightmodel/position/y_agl");
	  short onground = 0;
	  if(agl<4)
	  {
		  onground = 1;
	  }
	  XCCopyMemory(target, &onground);
  }

  void ADF1Freq(unsigned char* target)
  {	// adf1 freq
	  int freq = GetInt("sim/cockpit/radios/adf1_freq_hz");
		
	  short res = (short)GetBCD(freq);
	  XCCopyMemory(target, &res);
  }

  void COM1Freq(unsigned char* target)
  {	// com1 freq
	  int freq = GetInt("sim/cockpit/radios/com1_freq_hz") - 10000;
		
	  //fprintf(str, "Nav 1 freq: %d\n", freq);

	  short res = (short)GetBCD(freq);
	  XCCopyMemory(target, &res);
  }

  void NAV1Freq(unsigned char* target)
  {	// nav1 freq
	  int freq = GetInt("sim/cockpit/radios/nav1_freq_hz") - 10000;
		
	  //fprintf(str, "Nav 1 freq: %d\n", freq);

	  short res = (short)GetBCD(freq);
	  XCCopyMemory(target, &res);
  }

  void NAV2Freq(unsigned char* target)
  {	// nav2 freq
	  int freq = GetInt("sim/cockpit/radios/nav2_freq_hz") - 10000;
		
	  short res = (short)GetBCD(freq);
	  XCCopyMemory(target, &res);
  }

  void LatitudeHi(unsigned char* target)
  {
	  double latitude = GetDouble("sim/flightmodel/position/latitude");

	  latitude = latitude * (10001750.0 * 65536.0 * 65536.0) / 90.0;

	  long long lLat = (long long) latitude;

	  BYTE* lTarget = XConnectMemBlock;
	  lTarget += 0x0560;

	  int dLo = (lLat & 0xFFFFFFFF);
	  int dHi = (lLat & 0xFFFFFFFF00000000) >> 32;

	  XCCopyMemory(lTarget, &dLo);
	  XCCopyMemory(lTarget + 0x0004, &dHi);

	  //XCCopyMemory(lTarget, &, 8);
		

	  XCCopyMemory(target, lTarget);
  }

  void LatitudeLo(unsigned char* target)
  {
	  double latitude = GetDouble("sim/flightmodel/position/latitude");

	  latitude = latitude * (10001750.0 * 65536.0 * 65536.0) / 90.0;

	  long long lLat = (long long) latitude;

	  BYTE* lTarget = XConnectMemBlock;
	  lTarget += 0x0560;

	  int dLo = (lLat & 0xFFFFFFFF);
	  int dHi = (lLat & 0xFFFFFFFF00000000) >> 32;

	  XCCopyMemory(lTarget, &dLo);
	  XCCopyMemory(lTarget + 0x0004, &dHi);

	  lTarget += 0x0004;

	  XCCopyMemory(target, lTarget);
  }

  void LongitudeHi(unsigned char* target)
  {
	  double longitude = GetDouble("sim/flightmodel/position/longitude");

	  longitude = longitude * (65536.0 * 65536.0 * 65536.0 * 65536.0) / 360.0;

	  long long lLon = (long long)longitude;
		
	  BYTE* lTarget = XConnectMemBlock;
	  lTarget += 0x0568;

	  int dLo = (lLon & 0xFFFFFFFF);
	  int dHi = (lLon & 0xFFFFFFFF00000000) >> 32;

	  XCCopyMemory(lTarget, &dLo);
	  XCCopyMemory(lTarget + 0x0004, &dHi);

	  //XCCopyMemory(lTarget, &lLon, 8);

	  XCCopyMemory(target, lTarget);
  }

  void LongitudeLo(unsigned char* target)
  {
	  double longitude = GetDouble("sim/flightmodel/position/longitude");

	  longitude = longitude * (65536.0 * 65536.0 * 65536.0 * 65536.0) / 360.0;

	  long long lLon = (long long)longitude;
		
	  BYTE* lTarget = XConnectMemBlock;
	  lTarget += 0x0568;

	  int dLo = (lLon & 0xFFFFFFFF);
	  int dHi = (lLon & 0xFFFFFFFF00000000) >> 32;

	  XCCopyMemory(lTarget, &dLo);
	  XCCopyMemory(lTarget + 0x0004, &dHi);

	  lTarget += 0x0004;

	  XCCopyMemory(target, lTarget);
  }

  void Altitude(unsigned char* target)
  {
	  double elevation = GetDouble("sim/flightmodel/position/elevation");

	  long long lAlt = (long long)(elevation * 65536.0 * 65536.0);

	  XCCopyMemory(target, &lAlt);
  }

  void Pitch(unsigned char* target)
  {	// pitch
	  float pitch = GetFloat("sim/flightmodel/position/theta");
	  pitch = pitch / 360.0 * (65536.0 * 65536.0);
	  int iPitch = (int)-pitch;
	  XCCopyMemory(target, &iPitch);
  }

  void Roll(unsigned char* target)
  {	// bank
	  float bank = GetFloat("sim/flightmodel/position/phi");
	  bank = bank / 360.0 * (65536.0 * 65536.0);
	  int iBank = (int)-bank;
	  XCCopyMemory(target, &iBank);
  }

  void Heading(unsigned char* target)
  {	// heading
	  double hdg = (double)GetFloat("sim/flightmodel/position/psi");

	  hdg = hdg / 360.0 * (65536.0 * 65536.0);

	  //fprintf(str, "Heading: %e\n", hdg);

	  unsigned int iHdg = (unsigned int)hdg;

	  //fprintf(str, "Heading int: %d\n", iHdg);
	  XCCopyMemory(target, &iHdg);
  }

  void NAV2RwyHdg(unsigned char* target)
  {	// runway heading nav2
	  short crs = (short)(WrapHeading(GetFloat("sim/cockpit/radios/nav2_course_degm") + 180) / 360.0 * 65536.0);
	  XCCopyMemory(target, &crs);
  }

  void NAV2Latitude(unsigned char* target)
  {
	  int freq = GetInt("sim/cockpit/radios/nav2_freq_hz");
	  float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  /*fprintf(str, "Freq: %d\n", freq);
	  fprintf(str, "Lat: %f\n", latitude);
	  fprintf(str, "Lon: %f\n", longitude);*/

	  XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 1036 + 16 + 32);


	  char buf[6];
	  for(int i = 0;i<6;i++)
		  buf[i] = 0;


	  if(ref != XPLM_NAV_NOT_FOUND)
	  {
		  XPLMGetNavAidInfo(ref, 
							  NULL,	// type
							  &latitude,	// lat
							  &longitude,	// lon
							  NULL,	// height
							  &freq,	// freq
							  NULL,	// heading
							  buf,	// ID
							  NULL,	// name
							  NULL);	// reg
	  }

	  int lat = (int)(latitude / 90.0 * 10001750.0);

	  XCCopyMemory(target, &lat);
  }

  void NAV1Latitude(unsigned char* target)
  {
	  int freq = GetInt("sim/cockpit/radios/nav1_freq_hz");
	  float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  /*fprintf(str, "Freq: %d\n", freq);
	  fprintf(str, "Lat: %f\n", latitude);
	  fprintf(str, "Lon: %f\n", longitude);*/

	  XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 1036 + 16 + 32);


	  char buf[6];
	  for(int i = 0;i<6;i++)
		  buf[i] = 0;


	  if(ref != XPLM_NAV_NOT_FOUND)
	  {
		  XPLMGetNavAidInfo(ref, 
							  NULL,	// type
							  &latitude,	// lat
							  &longitude,	// lon
							  NULL,	// height
							  &freq,	// freq
							  NULL,	// heading
							  buf,	// ID
							  NULL,	// name
							  NULL);	// reg
	  }

	  int lat = (int)(latitude / 90.0 * 10001750.0);

	  XCCopyMemory(target, &lat);
  }

  void NAV2Longitude(unsigned char* target)
  {
	  int freq = GetInt("sim/cockpit/radios/nav2_freq_hz");
	  float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  /*fprintf(str, "Freq: %d\n", freq);
	  fprintf(str, "Lat: %f\n", latitude);
	  fprintf(str, "Lon: %f\n", longitude);*/

	  XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 1036 + 16 + 32);


	  char buf[6];
	  for(int i = 0;i<6;i++)
		  buf[i] = 0;


	  if(ref != XPLM_NAV_NOT_FOUND)
	  {
		  XPLMGetNavAidInfo(ref, 
							  NULL,	// type
							  &latitude,	// lat
							  &longitude,	// lon
							  NULL,	// height
							  &freq,	// freq
							  NULL,	// heading
							  buf,	// ID
							  NULL,	// name
							  NULL);	// reg
	  }

	  int lon = (int)(longitude / 360.0 * (65536.0 * 65536.0));

	  XCCopyMemory(target, &lon);
  }

  void NAV1Longitude(unsigned char* target)
  {
	  int freq = GetInt("sim/cockpit/radios/nav1_freq_hz");
	  float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  /*fprintf(str, "Freq: %d\n", freq);
	  fprintf(str, "Lat: %f\n", latitude);
	  fprintf(str, "Lon: %f\n", longitude);*/

	  XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 1036 + 16 + 32);


	  char buf[6];
	  for(int i = 0;i<6;i++)
		  buf[i] = 0;


	  if(ref != XPLM_NAV_NOT_FOUND)
	  {
		  XPLMGetNavAidInfo(ref, 
							  NULL,	// type
							  &latitude,	// lat
							  &longitude,	// lon
							  NULL,	// height
							  &freq,	// freq
							  NULL,	// heading
							  buf,	// ID
							  NULL,	// name
							  NULL);	// reg
	  }

	  int lon = (int)(longitude / 360.0 * (65536.0 * 65536.0));

	  XCCopyMemory(target, &lon);
  }

  void NAV1RwyHdg(unsigned char* target)
  {	// runway heading nav1

	  short crs = (short)(WrapHeading(GetFloat("sim/cockpit/radios/nav1_course_degm") + 180) / 360.0 * 65536.0);
	  XCCopyMemory(target, &crs);
  }

  void CenterFuelLevel(unsigned char* target)
  {

	  float ratio = GetFloat("sim/aircraft/overflow/acf_tank_rat", 1);
	  float total = GetFloat("sim/aircraft/weight/acf_m_fuel_tot");
	  float weightkg = GetFloat("sim/flightmodel/weight/m_fuel", 1);

	  float proc = weightkg / (ratio * total);
	  int iProc = (int)(proc * 128.0 * 65536.0);

	  XCCopyMemory(target, &iProc);
  }

  void CenterFuelCapacity(unsigned char* target)
  {

	  float ratio = GetFloat("sim/aircraft/overflow/acf_tank_rat", 1);
	  float total = GetFloat("sim/aircraft/weight/acf_m_fuel_tot");
	  int cap = (int)KilogramsToGallons(ratio * total);

	  XCCopyMemory(target, &cap);
  }

  void LeftFuelLevel(unsigned char* target)
  {
	  float ratio = GetFloat("sim/aircraft/overflow/acf_tank_rat", 0);
	  float total = GetFloat("sim/aircraft/weight/acf_m_fuel_tot");
	  float weightkg = GetFloat("sim/flightmodel/weight/m_fuel", 0);

	  float proc = weightkg / (ratio * total);
	  int iProc = (int)(proc * 128.0 * 65536.0);

	  XCCopyMemory(target, &iProc);

	  //fprintf(str, "Fuel total weight: %e\n", total);
  }

  void LeftFuelCapacity(unsigned char* target)
  {
	  float ratio = GetFloat("sim/aircraft/overflow/acf_tank_rat", 0);
	  float total = GetFloat("sim/aircraft/weight/acf_m_fuel_tot");
	  int cap = (int)KilogramsToGallons(ratio * total);

	  XCCopyMemory(target, &cap);
  }

  void RightFuelLevel(unsigned char* target)
  {
	  float ratio = GetFloat("sim/aircraft/overflow/acf_tank_rat", 2);
	  float total = GetFloat("sim/aircraft/weight/acf_m_fuel_tot");
	  float weightkg = GetFloat("sim/flightmodel/weight/m_fuel", 2);

	  float proc = weightkg / (ratio * total);
	  int iProc = (int)(proc * 128.0 * 65536.0);

	  XCCopyMemory(target, &iProc);
  }

  void RightFuelCapacity(unsigned char* target)
  {
	  float ratio = GetFloat("sim/aircraft/overflow/acf_tank_rat", 2);
	  float total = GetFloat("sim/aircraft/weight/acf_m_fuel_tot");
	  int cap = (int)KilogramsToGallons(ratio * total);

	  XCCopyMemory(target, &cap);
  }

  void PitchInput(unsigned char* target)
  {
	  short ratio = (short)(GetFloat("sim/joystick/yoke_pitch_ratio") * 16383.0);
	  XCCopyMemory(target, &ratio);
  }

  void RollInput(unsigned char* target)
  {
	  short ratio = (short)(GetFloat("sim/joystick/yoke_roll_ratio") * 16383.0);
	  XCCopyMemory(target, &ratio);
  }

  void DME1(unsigned char* target)
  {
	  float dist = GetFloat("sim/cockpit2/radios/indicators/nav1_dme_distance_nm");
	  char dme[5];
	  if(dist>99.9)
	  {
		  sprintf(dme, "%.0f", dist);
	  }
	  else
	  {
		  sprintf(dme, "%00.1f", dist);
	  }
	  dme[4] = 0;
	  CopyMemory(target, dme, 5);
  }

  void DME2(unsigned char* target)
  {
	  float dist = GetFloat("sim/cockpit2/radios/indicators/nav2_dme_distance_nm");
	  char dme[5];
	  if(dist>99.9)
	  {
		  sprintf(dme, "%.0f", dist);
	  }
	  else
	  {
		  sprintf(dme, "%00.1f", dist);
	  }
	  dme[4] = 0;
	  CopyMemory(target, dme, 5);
  }

  void NAV1Deflection(unsigned char* target)
  {
	  char loc = (char)(GetFloat("sim/cockpit2/radios/indicators/nav1_hdef_dots_pilot") / 2.5 * 127);
	  XCCopyMemory(target, &loc);
  }

  void NAV1GS(unsigned char* target)
  {
	  char gs = (char)(GetFloat("sim/cockpit2/radios/indicators/nav1_vdef_dots_pilot") / 2.5 * 127);
	  XCCopyMemory(target, &gs);
  }

  void NAV1FromTo(unsigned char* target)
  {
	  char fromto = (char)GetInt("sim/cockpit2/radios/indicators/nav1_flag_from_to_pilot");
	  XCCopyMemory(target, &fromto);
  }

  void NAV1CDI(unsigned char* target)
  {
	  char flag = (char)GetInt("sim/cockpit/radios/nav1_CDI");
	  XCCopyMemory(target, &flag);
  }

  void NAV1OBS(unsigned char* target)
  {
	  float fObs = GetFloat("sim/cockpit/radios/nav1_obs_degm");
	  //fprintf(str, "Nav 1 obs: %f\n", fObs);

	  short obs = (short)fObs;
	  XCCopyMemory(target, &obs);

	  /*fObs = GetFloat("sim/cockpit/radios/nav1_obs_degt");
	  fprintf(str, "sim/cockpit/radios/nav1_obs_degt: %f\n", fObs);
	  fObs = GetFloat("sim/cockpit2/radios/actuators/nav1_obs_deg_mag_pilot");
	  fprintf(str, "sim/cockpit2/radios/actuators/nav1_obs_deg_mag_pilot: %f\n", fObs);
	  fObs = GetFloat("sim/cockpit2/radios/actuators/nav1_course_deg_mag_pilot");
	  fprintf(str, "sim/cockpit2/radios/actuators/nav1_course_deg_mag_pilot: %f\n", fObs);*/

  }

  void NAV1Radial(unsigned char* target)
  {	// nav1 radial
	  short bgn = (short)(WrapHeading(GetFloat("sim/cockpit2/radios/indicators/nav1_bearing_deg_mag") - 180) / 360.0 * 65536.0);
	  XCCopyMemory(target, &bgn);
  }

  void NAV2Deflection(unsigned char* target)
  {
	  char loc = (char)(GetFloat("sim/cockpit2/radios/indicators/nav2_hdef_dots_pilot") / 2.5 * 127);
	  XCCopyMemory(target, &loc);
  }

  void NAV2FromTo(unsigned char* target)
  {
	  char fromto = (char)GetInt("sim/cockpit2/radios/indicators/nav2_flag_from_to_pilot");
	  XCCopyMemory(target, &fromto);
  }

  void NAV2OBS(unsigned char* target)
  {
	  short obs = (short)GetFloat("sim/cockpit/radios/nav2_obs_degm");
	  XCCopyMemory(target, &obs);
  }

  void NAV2Radial(unsigned char* target)
  {	// nav1 radial
	  short bgn = (short)(WrapHeading(GetFloat("sim/cockpit2/radios/indicators/nav2_bearing_deg_mag") - 180) / 360.0 * 65536.0);
	  XCCopyMemory(target, &bgn);
  }

  void NAV2GS(unsigned char* target)
  {
	  char gs = (char)(GetFloat("sim/cockpit2/radios/indicators/nav2_vdef_dots_pilot") / 2.5 * 127);
	  XCCopyMemory(target, &gs);
  }

  void NAV2CDI(unsigned char* target)
  {
	  char flag = (char)GetInt("sim/cockpit/radios/nav2_CDI");
	  XCCopyMemory(target, &flag);
  }

  void Temp(unsigned char* target)
  {
	  short oat = (short)(GetFloat("sim/weatcher/temperature_ambient_c") * 256.0);
	  XCCopyMemory(target, &oat);
  }

  void WindSpeed(unsigned char* target)
  {	// wind speed
	  short wsp = (short)GetFloat("sim/weather/wind_speed_kt");
	  XCCopyMemory(target, &wsp);
  }

  void WindDir(unsigned char* target)
  {	// wind direction
	  short wdir = (short)(GetFloat("sim/weather/wind_direction_degt") / 360.0 * 65536);
	  XCCopyMemory(target, &wdir);
  }

  void Eng1N1(unsigned char* target)
  {
	  double n1 = (double)GetFloat("sim/cockpit2/engine/indicators/N1_percent", 0);
	  XCCopyMemory(target, &n1);
  }

  void Eng1N2(unsigned char* target)
  {
	  double n2 = (double)GetFloat("sim/cockpit2/engine/indicators/N2_percent", 0);
	  XCCopyMemory(target, &n2);
  }

  void Eng1Throttle(unsigned char* target)
  {
	  short thr = (short)(GetFloat("sim/flightmodel/engine/ENGN_thro", 0) * 16384.0);
	  XCCopyMemory(target, &thr);




  }

  void Eng1Temp(unsigned char* target)
  {
	  short egt = (short)(GetFloat("sim/cockpit2/engine/indicators/EGT_deg_C", 0) / 860.0 * 16384.0);
	  XCCopyMemory(target, &egt);
  }

  void Eng1OilPress(unsigned char* target)
  {
	  short oilp = (short)(GetFloat("sim/flightmodel/engine/ENGN_oil_press_psi", 0) / 55.0 * 16384.0);
	  XCCopyMemory(target, &oilp);
  }

  void Eng1OilTemp(unsigned char* target)
  {
	  short oilt = (short)(GetFloat("sim/flightmodel/engine/ENGN_oil_temp_c", 0) / 140.0 * 16384.0);
	  XCCopyMemory(target, &oilt);
  }

  void Eng1OilQty(unsigned char* target)
  {
	  int oilq = (int)(GetFloat("sim/cockpit2/engine/indicators/oil_quantity_ratio", 0) * 16384.0);
	  XCCopyMemory(target, &oilq);
  }

  void Eng1HydraulicQty(unsigned char* target)
  {
	  int hydq = (int)(GetFloat("sim/cockpit/misc/hydraulic_quantity") / 100.0 * 16384.0);
	  XCCopyMemory(target, &hydq);
  }

  void Eng1FuelFlow(unsigned char* target)
  {
	  double ff = (double)(GetFloat("sim/flightmodel/engine/ENGN_FF_", 0) * 3600 / 0.45359237);
	  XCCopyMemory(target, &ff);
  }

  void Eng2Throttle(unsigned char* target)
  {
	  short thr = (short)(GetFloat("sim/flightmodel/engine/ENGN_thro", 1) * 16384.0);
	  XCCopyMemory(target, &thr);
  }

  void Flaps1(unsigned char* target)
  {
	  int flappos = (int)(GetFloat("sim/flightmodel2/controls/flap1_deploy_ratio") * 16383.0);
	  XCCopyMemory(target, &flappos);
  }

  void Flaps2(unsigned char* target)
  {
	  int flappos = (int)(GetFloat("sim/flightmodel2/controls/flap2_deploy_ratio") * 16383.0);
	  XCCopyMemory(target, &flappos);
  }

  void MachSpeed(unsigned char* target)
  {
	  short mach = (short)(GetFloat("sim/flightmodel/misc/machno") * 20480.0);
	  XCCopyMemory(target, &mach);
  }

  void TAT(unsigned char* target)
  {
	  float oat = GetFloat("sim/weatcher/temperature_ambient_c");
	  float mach = GetFloat("sim/flightmodel/misc/machno");

	  float tat = oat * (1 + 0.2 * mach * mach) * 256.0;
	  short sTat = (short)tat;
	  XCCopyMemory(target, &sTat);
  }

  void Eng2N1(unsigned char* target)
  {
	  double n1 = (double)GetFloat("sim/cockpit2/engine/indicators/N1_percent", 1);
	  XCCopyMemory(target, &n1);
  }

  void Eng2N2(unsigned char* target)
  {
	  double n2 = (double)GetFloat("sim/cockpit2/engine/indicators/N2_percent", 1);
	  XCCopyMemory(target, &n2);
  }

  void Eng2Temp(unsigned char* target)
  {
	  short egt = (short)(GetFloat("sim/cockpit2/engine/indicators/EGT_deg_C", 0) / 860.0 * 16384.0);
	  XCCopyMemory(target, &egt);
  }

  void Eng2OilPress(unsigned char* target)
  {
	  short oilp = (short)(GetFloat("sim/flightmodel/engine/ENGN_oil_press_psi", 1) / 55.0 * 16384.0);
	  XCCopyMemory(target, &oilp);
  }

  void Eng2OilTemp(unsigned char* target)
  {
	  short oilt = (short)(GetFloat("sim/flightmodel/engine/ENGN_oil_temp_c", 1) / 140.0 * 16384.0);
	  XCCopyMemory(target, &oilt);
  }

  void Eng2OilQty(unsigned char* target)
  {
	  int oilq = (int)(GetFloat("sim/cockpit2/engine/indicators/oil_quantity_ratio", 1) * 16384.0);
	  XCCopyMemory(target, &oilq);
  }

  void Eng2HydraulicQty(unsigned char* target)
  {
	  int hydq = (int)(GetFloat("sim/cockpit/misc/hydraulic_quantity2") / 100.0 * 16384.0);
	  XCCopyMemory(target, &hydq);
  }

  void Eng2FuelFlow(unsigned char* target)
  {
	  double ff = (double)(GetFloat("sim/flightmodel/engine/ENGN_FF_", 0) * 3600 / 0.45359237);
	  XCCopyMemory(target, &ff);
  }

  void NAV1Info(unsigned char* target)
  {   // nav1 identity

	  int freq = GetInt("sim/cockpit/radios/nav1_freq_hz");
	  float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  /*fprintf(str, "Freq: %d\n", freq);
	  fprintf(str, "Lat: %f\n", latitude);
	  fprintf(str, "Lon: %f\n", longitude);*/

	  XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 1036 + 16 + 32);


	  char buf[6];
	  for(int i = 0;i<6;i++)
		  buf[i] = 0;


	  if(ref != XPLM_NAV_NOT_FOUND)
	  {
		  XPLMGetNavAidInfo(ref, 
							  NULL,	// type
							  &latitude,	// lat
							  &longitude,	// lon
							  NULL,	// height
							  &freq,	// freq
							  NULL,	// heading
							  buf,	// ID
							  NULL,	// name
							  NULL);	// reg
	  }

	  CopyMemory(target, buf, 6);

  }

  void NAV2Info(unsigned char* target)
  {   // nav2 identity
	  int freq = GetInt("sim/cockpit/radios/nav2_freq_hz");
	  float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  /*fprintf(str, "Freq: %d\n", freq);
	  fprintf(str, "Lat: %f\n", latitude);
	  fprintf(str, "Lon: %f\n", longitude);*/

	  XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 1036 + 16 + 32);

	  char buf[6];
	  for(int i = 0;i<6;i++)
		  buf[i] = 0;


	  if(ref != XPLM_NAV_NOT_FOUND)
	  {
		  XPLMGetNavAidInfo(ref, 
							  NULL,	// type
							  &latitude,	// lat
							  &longitude,	// lon
							  NULL,	// height
							  &freq,	// freq
							  NULL,	// heading
							  buf,	// ID
							  NULL,	// name
							  NULL);	// reg
	  }

	  CopyMemory(target, buf, 6);

  }

  void ADF1Info(unsigned char* target)
  {   // nav2 identity
	  int freq = GetInt("sim/cockpit/radios/adf1_freq_hz");
	  float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  /*fprintf(str, "Freq: %d\n", freq);
	  fprintf(str, "Lat: %f\n", latitude);
	  fprintf(str, "Lon: %f\n", longitude);*/

	  XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 2);

	  char buf[6];
	  for(int i = 0;i<6;i++)
		  buf[i] = 0;


	  if(ref != XPLM_NAV_NOT_FOUND)
	  {
		  XPLMGetNavAidInfo(ref, 
							  NULL,	// type
							  &latitude,	// lat
							  &longitude,	// lon
							  NULL,	// height
							  &freq,	// freq
							  NULL,	// heading
							  buf,	// ID
							  NULL,	// name
							  NULL);	// reg
	  }

	  CopyMemory(target, buf, 6);

  }

  void COM2Freq(unsigned char* target)
  {	// com2 freq
	  int freq = GetInt("sim/cockpit/radios/com2_freq_hz") - 10000;
		
	  //fprintf(str, "Nav 1 freq: %d\n", freq);

	  short res = (short)GetBCD(freq);
	  XCCopyMemory(target, &res);
  }

  void HeightAGL(unsigned char* target)
  {	// ground altitude
	  int agl = (int)(GetFloat("sim/flightmodel/position/y_agl") * 65536.0);
	  XCCopyMemory(target, &agl);
  }

  void RadioFlags(unsigned char* target)
  {	// radio status flags
		
	  bool goodnav1 = GetInt("sim/cockpit/radios/nav_type", 0) != 0; 
	  bool goodnav2 = GetInt("sim/cockpit/radios/nav_type", 1) != 0;
	  bool goodadf1 = GetInt("sim/cockpit/radios/nav_type", 2) != 0;
	  bool goodadf2 = GetInt("sim/cockpit/radios/nav_type", 3) != 0;
	  bool nav1isils = GetInt("sim/cockpit/radios/nav_type", 0) == 4;
	  bool nav2isils = GetInt("sim/cockpit/radios/nav_type", 1) == 4;
	  bool nav1hasdme = GetInt("sim/cockpit/radios/nav1_has_dme", 1) != 0;
	  bool nav2hasdme = GetInt("sim/cockpit/radios/nav2_has_dme", 1) != 0;

	  short ret = 0;

	  if(goodnav1)
		  ret += 2;
	  if(goodnav2)
		  ret += 4;
	  if(goodadf1)
		  ret += 8;
	  if(nav1hasdme)
		  ret += 16;
	  if(nav2hasdme)
		  ret += 32;
	  if(nav1isils)
		  ret += 64;
	  if(goodadf2)
		  ret += 2048;
	  if(nav2isils)
		  ret += 4096;

	  XCCopyMemory(target, &ret);
  }

  void XConnectVersion(unsigned char* target)
  {	// XConnect Version
	  int version = 0x20000000;
	  XCCopyMemory(target, &version);
  }

  void XPlaneVersion(unsigned char* target)
  {	// X-Plane Version
	  int version = 0xFADE0006;
	  XCCopyMemory(target, &version);
  }

  void PressureAltitude(unsigned char* target)
  {   // Barometric altitude
	  int alt = (int)GetFloat("sim/flightmodel/misc/h_ind2");
	  XCCopyMemory(target, &alt);
  }

  void Eng1ThrottleInput(unsigned char* target)
  {
	  short ratio = (short)(GetFloat("sim/cockpit2/engine/actuators/throttle_ratio",0) * 16384.0);
	  XCCopyMemory(target, &ratio);
  }

  void Eng2ThrottleInput(unsigned char* target)
  {
	  short ratio = (short)(GetFloat("sim/cockpit2/engine/actuators/throttle_ratio",1) * 16384.0);
	  XCCopyMemory(target, &ratio);
  }

  void StandbyQNH(unsigned char* target)
  {
	  float altimeter = GetFloat("sim/cockpit/misc/barometer_setting2");
	  short sAlt = ((short)InchToHPA(altimeter)) * 16;
	  XCCopyMemory(target, &sAlt);
  }

  void StandbyAltitude(unsigned char* target)
  {
	  int alt = (int)GetFloat("sim/flightmodel/misc/h_ind_copilot2");
	  XCCopyMemory(target, &alt);
  }

} // namespace xcread