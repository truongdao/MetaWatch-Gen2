//==============================================================================
//  Copyright 2012 Meta Watch Ltd. - http://www.MetaWatch.org/
// 
//  Licensed under the Meta Watch License, Version 1.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//  
//      http://www.MetaWatch.org/licenses/license-1.0.html
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//==============================================================================

/******************************************************************************/
/*! \file Wrapper.h
*
*/
/******************************************************************************/

#ifndef WRAPPER_H
#define WRAPPER_H

#include <stdarg.h>

#define CONN_TYPE_NULL      (0x00)
#define CONN_TYPE_TUNNEL    (0x01)
#define CONN_TYPE_SPP       (0x02)
#define CONN_TYPE_HFP       (0x04)
#define CONN_TYPE_MAP       (0x08)
#define CONN_TYPE_MWM       (0x10)
#define CONN_TYPE_ANCS      (0x20)
#define CONN_TYPE_ANY       (0x3F)

#define CONN_TYPE_BLE       (CONN_TYPE_TUNNEL | CONN_TYPE_MWM)
#define CONN_TYPE_MAIN      (CONN_TYPE_BLE | CONN_TYPE_SPP)
#define CONN_TYPE_BR        (CONN_TYPE_SPP | CONN_TYPE_HFP | CONN_TYPE_MAP)
#define CONN_TYPE_BLE_ANCS  (CONN_TYPE_BLE | CONN_TYPE_ANCS)

#define CONN_CHANGE_BIT     (0x80)

#define CONN_CHG_CONN       (0x80)
#define CONN_CHG_DSCONN     (0x00)

/* send current conn state to remote */
#define CONN_IND            (0x40)

/* BLE connection interval parameter */
#define LONG                0
#define SHORT               1

/*! Initaliaze the serial port profile task.  This should be called from main.
*
* This task opens the stack which in turn creates 3 more tasks that create and 
* handle the bluetooth serial port connection.
*/
void CreateWrapperTask(void);

/*! Query the serial port profile task if it is okay to put the part into LPM3
*
* This function is called by the idle task.  The idle task puts the 
* MSP430 into sleep mode.
* 
* \return 0 if micro cannot go into LPM3; 1 if micro can go into LPM3
*/
unsigned char ReadyToSleep(void);
unsigned char BlePaired(void);
unsigned char BtPaired(void);
unsigned char NotPaired(void);
unsigned char HfpPaired(void);

/******************************************************************************/

/*! The current state of the bluetooth serial port.
*
* On: Radio on, stack up or Disconnected timeout;
* Connect: BLE/SPP connected; Disconnect: BLE/SPP Disconnected and not timeout;
* Off: Radio and stack are closed.
*/
typedef enum
{
  Off,
  On,
  Connect,
  Initial,
  Boot,
  Shipping
} eBluetoothState;

/*! Determine if the bluetooth link is in the connected state.
* When the phone and watch are connected then data can be sent.
*
* \return 0 when not connected, 1 when connected
*/
unsigned char Connected(unsigned char Type);

/*! Check if been connected once
* \return 0 never been connected; 1: connected
*/
unsigned char OnceConnected(void);

/*! This function is used to determine if the radio is on and will return 1 when
* initialization has successfully completed, but the radio may or may not be 
* paired or connected.
*
* \return 1 when stack is in the connected, paired, or radio on state, 0 otherwise
*/
unsigned char RadioOn(void);

/*! This function is used to determine if the connection state of the 
 * bluetooth serial port.
 *
 * \return eBluetoothState
 */
eBluetoothState BluetoothState(void);

/*! Query Bluetooth Discoverability
 *
 * \return 0 when not discoverable, 1 when discoverable
 */
unsigned char QueryDiscoverable(void);

/*! Query Bluetooth Connectability
 *
 * \return 0 when not connectable, 1 when connectable
 */
unsigned char QueryConnectable(void);

/*! Query Bluetooth pairing information
 *
 * \return 0 when valid pairing does not exist, 1 when valid pairing information
 * exists
 */
unsigned char ValidRemoteAddr(void);

void GetBDAddrStr(char *pAddr);

char const *GetLocalName(void);

#define INTERVAL_VALUE        0
#define INTERVAL_STATE        1

#if SUPPORT_BLE
unsigned char CurrentInterval(unsigned char Prop);
#endif

/******************************************************************************/

/*! 
 * \param DelayMs is the delay for entering sniff mode 
 */
void SetSniffModeEntryDelay(unsigned int DelayMs);


/* these are actually the HCI Current Mode Types + some housekeeping */
typedef enum
{
  Active,
  Hold,
  Sniff,
  Park,
  SniffToActive,
  ActiveToSniff
} etSniffState;

etSniffState QuerySniffState(void);

/******************************************************************************/

/*! Convert milliseconds to baseband slots
 * \note stack handles making input an even number per spec requirements
*/
#define MILLISECONDS_TO_BASEBAND_SLOTS(_x)   ((_x) / (0.625))

/*! maximum sniff slot interval */
#define SNIFF_MAX_INTERVAL_SLOTS_LEVEL ( MILLISECONDS_TO_BASEBAND_SLOTS(1280) )

/*! minimum sniff slot interval */
#define SNIFF_MIN_INTERVAL_SLOTS_LEVEL ( MILLISECONDS_TO_BASEBAND_SLOTS(1270) )

/*! number of slots that the slave must listen */
#define SNIFF_ATTEMPT_SLOTS         ( 0x0002 )

/*! number of additional slots the slave must listen */
#define SNIFF_ATTEMPT_TIMEOUT_SLOTS ( 0x0001 )

/*! These are the four sniff slot parameters that can be set by user */
typedef enum
{
  MaxInterval = 0,
  MinInterval = 1,
  Attempt = 2,
  AttemptTimeout = 3
} eSniffSlotType;

/*! Allow access to the 4 sniff parameters
 *
 * \param SniffSlotType is the parameter type
 * \param Slots is the new parameter value in slots
 */
void SetSniffSlotParameter(eSniffSlotType SniffSlotType,unsigned int Slots);


/*! \return The sniff Slot parameter
 *
 * \param SniffSlotType is the parameter type
 */
unsigned int QuerySniffSlotParameter(eSniffSlotType SniffSlotType);

/******************************************************************************/

/*! If a function wishes to disable the flow of characters from the radio
 * during a long interrupt routine, it must first read the state of flow
 * pin. */
void EnableFlowControl(unsigned char Enable);

int vSprintF(char *buffer, const char *format, va_list ap);

/******************************************************************************/

#endif /* WRAPPER_H */

