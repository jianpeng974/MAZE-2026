/*
 *	Author:			Sebastien Parent-Charette (support@robotshop.com)
 *	Version:		1.1.0
 *	Licence:		LGPL-3.0 (GNU Lesser General Public License version 3)
 *	
 *	Description:	Basic example of the LSS moving back and forth.
 */

#include <LSS.h>

// ID set to default LSS ID = 0
#define MT_EF_ID (1) //motor esq frente id
#define MT_DF_ID (2) //motor dir frente id
#define MT_ET_ID (3) //motor esq tras id
#define MT_DT_ID (4) // motor dir tras id

#define LSS_BAUD	(LSS_DefaultBaud)
// Choose the proper serial port for your platform
#define LSS_SERIAL	(Serial2)	// serial 2 rx1 tx1
//#define LSS_SERIAL	(Serial1)	// ex: Teensy


// Create one LSS object
LSS MT_EF = LSS(MT_EF_ID);  //motor esq frente
LSS MT_DF = LSS(MT_DF_ID);  //motor dir frente 
LSS MT_ET = LSS(MT_ET_ID);  //motor esq tras 
LSS MT_DT = LSS(MT_DT_ID);  // motor dir tras 

//OBS: VER SINAL DO MOTOR DEPENDENDO MUDAR DE A CORDO COM O ROBO (ver os sentidos da roda)

void frente(int ang){   
	MT_EF.move(ang);
	MT_DF.move(ang);
	MT_ET.move(ang);
	MT_DT.move(ang);

	delay(2000)
}

void tras(int ang){
	MT_EF.move(-ang);
	MT_DF.move(-ang);
	MT_ET.move(-ang);
	MT_DT.move(-ang);

	delay(2000)
}

void esq(int ang){
	MT_EF.move(-ang);
	MT_DF.move(ang);
	MT_ET.move(-ang);
	MT_DT.move(ang);

	delay(2000)

}

void dir(int ang){
	MT_EF.move(ang);
	MT_DF.move(ang);
	MT_ET.move(ang);
	MT_DT.move(ang);

	delay(2000)

}

/*
void setup()
{
	// Initialize the LSS bus
	LSS::initBus(LSS_SERIAL, LSS_BAUD);

	// Initialize LSS to position 0.0 °
	myLSS.move(0);

	// Wait for it to get there
	delay(2000);
}

// Loops between -180.0° and 180°, taking 1 second pause between each half-circle move.
void loop()
{
	// Send LSS to half a turn counter-clockwise from zero (assumes gyre = 1)
	myLSS.move(-1800);

	// Wait for one second
	delay(2000);

	// Send LSS to half a turn clockwise from zero (assumes gyre = 1)
	myLSS.move(1800);

	// Wait for one second
	delay(2000);
} */