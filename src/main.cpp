#include <stdint.h>
#include <stdio.h>
#include "mal.hpp"
#include "iot_monitor.h"
#include <unistd.h>                                                             // Needed for sleep(), usleep()
#include "wwan_status.hpp"
#include <fstream>
#include <string>
#include <hwlib/hwlib.h>                                                        // Needed for WNCSDK GPIO API

#define USER_BUTTON     GPIO_PIN_98

using namespace std;

int c;
sysinfo  mySystem;                                                              // Get system info from MAL
wwanInfo myWwan;                                                                // Get wwan info from MAL
netInfo  myNetw;                                                                // Get network info from MAL
unsigned int dbg_flag = 0;

void writetofile(string data, string filename) {
	char* writable = new char[data.size() + 1];		// creates char array
	copy(data.begin(), data.end(), writable);		// copies string to char array
	writable[data.size()] = '\0';				// deletes the terminating 0
	ofstream rfile(filename.c_str(), ios::app);		// sets to append to file and opens file
	rfile.write(writable, data.size());			// adds string data to end of file
	rfile.close();						// closes file
	delete[] writable;					// frees the string after using it				
}

int main(void) {
    // Return variables from function calls
    int  ret;
    // Key/value pairs for parsing json
    json_keyval om[20];                                                         // JSON key/value pairs for MAL system info

    c = start_data_service();

    while ( c < 0 ) {
        printf("WAIT: starting WNC Data Module (%d)\n",c);
        sleep(10);
        c = start_data_service();
    }

    printf("\nData service running\n\n");

    do
        mySystem.model=getModelID(om, sizeof(om));
    while( mySystem.model == "service is not ready");

    string header = "Connection Time,Signal Strength,Signal Level,LTE RSRP,WCDMA RSCP,IPv4,IPv6\n\r";
    writetofile(header, "data/MonitorData.csv");

    string info;

    for(int  i = 0; i < 100000000; i++) {
    mySystem.firmVer   = getFirmwareVersion(om, sizeof(om));
    mySystem.appsVer   = getAppsVersion(om, sizeof(om));
    mySystem.malwarVer = getMALManVer(om, sizeof(om));
    mySystem.ip        = get_ipAddr(om, sizeof(om));
    mySystem.iccid     = getICCID(om, sizeof(om));
    mySystem.imei      = getIMEI(om, sizeof(om));
    mySystem.imsi      = getIMSI(om, sizeof(om));

    printf("\nNetwork Time = %s\n", get_networkTime(om, sizeof(om)));


    WWANStatus(&myWwan, &myNetw);

    printf("connType %s\n", myNetw.connType.c_str());
    printf("connState %s\n", myNetw.connState.c_str());
    printf("connTime %s\n", myNetw.connTime.c_str());
    printf("provider %s\n", myNetw.provider.c_str());
    printf("radioMode %s\n", myNetw.radioMode.c_str());
    printf("dbTech %s\n", myNetw.dbTech.c_str());
    printf("roamStatus %s\n", myNetw.roamStatus.c_str());
    printf("sigStrength %s\n", myNetw.sigStrength.c_str());
    printf("sigLevel %s\n", myNetw.sigLevel.c_str());
    printf("lte %s\n", myNetw.lte.c_str());
    printf("wcdma %s\n", myNetw.wcdma.c_str());
    printf("IPv4 %s\n", mySystem.ip.c_str());
    printf("ipv6 %s\n\n", myNetw.ipv6.c_str());
	
	// Write data to text file

	string info(myNetw.connTime.c_str());
	info += ",";
	// info += myNetw.connState.c_str();
	// info += ",";
	// info += myNetw.connTime.c_str();
	// info += ",";
	// info += myNetw.provider.c_str();
	// info += ",";
	// info += myNetw.radioMode.c_str();
	// info += ",";
	// info += myNetw.dbTech.c_str();
	// info += ",";
	// info += myNetw.roamStatus.c_str();
	// info += ",";
	info += myNetw.sigStrength.c_str();
	info += ",";
	info += myNetw.sigLevel.c_str();
	info += ",";
	info += myNetw.lte.c_str();
	info += ",";
	info += myNetw.wcdma.c_str();
	info += ",";
	info += mySystem.ip.c_str();
	info += ",";
	info += myNetw.ipv6.c_str();
	info += "\n\r";
	writetofile(info, "data/MonitorData.csv");
        

	sleep(2);
    }

    return 0;
}

/* =====================================================================
   Copyright (c) 2018, AT&T (R)
   www.att.com 
 
   Licensed under the Apache License, Version 2.0 (the "License"); 
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, 
   software distributed under the License is distributed on an 
   "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, 
   either express or implied. See the License for the specific 
   language governing permissions and limitations under the License.
======================================================================== */

