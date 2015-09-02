/*
  @file
  events.c

  @brief
  File handling the checking of FLAGS that control the state machine.


*/

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include "events.h"

/*==============================================================================

                             EXTERNAL DECLARATIONS

==============================================================================*/

all_flags_s all_flags;

/*==============================================================================

                      LOCAL DEFINITIONS AND TYPES : MACROS

==============================================================================*/

#define OPTO_INT_TIMEOUT		( 50 )
/*==============================================================================

                      LOCAL DEFINITIONS AND TYPES : ENUMS

==============================================================================*/


/*==============================================================================

                    LOCAL DEFINITIONS AND TYPES : STRUCTURES

==============================================================================*/


/*==============================================================================

                            LOCAL FUNCTION PROTOTYPES

==============================================================================*/


/*==============================================================================

                            LOCAL FUNCTION DEFINITIONS

==============================================================================*/

/*----------------------------------------------------------------------------*/
/* Function : add_event */
/*----------------------------------------------------------------------------*/
/*!
  @ brief
	Used to get all information about the event

  @ details
  	  Used to get all information about the event

  @ pre-requirements None

  @ param The test event

  @ return struct of the required data

*/

void add_event(all_state_events user_events, eventinformation * eventcopy )
{
	eventcopy->user_events = events;
	eventcopy->user_input_data = (char *) user_data;
	eventcopy->timestamp = getMillisecondTimestamp();

}/* add_test_event */




/*----------------------------------------------------------------------------*/
/* Function : service_event */
/*----------------------------------------------------------------------------*/
/*!
  @ brief
	Function to service the event

  @ details
	Function to service the event

  @ pre-requirements

  @ param enum - The test event

  @ return void
*/

void service_event((all_state_events user_events)
{
	eventinformation event_update;

	add_user_event ( user_events, &event_update );

	test_algorithm((eventinformation const*) &event_update);



}/* service_event */



/*----------------------------------------------------------------------------*/
/* Function : process_test_events */
/*----------------------------------------------------------------------------*/
/*!
  @ brief
  Function to process the event flags

  @ details
  Function to process the event flags

  @ pre-requirements None

  @ param whichevent
  This is the event for which the blah blah

  @ return void


*/
void process_test_events(production_test_all_events_e whichevent)
{
	if ( TEST_EVENT_IDLE != whichevent )
	{
		//UARTprintf("\nevents.c | %d | ERROR : Invalid event %d", __LINE__, whichevent );
		return;
	}

	//Interrupt flags
	if( all_flags.all_test_flags_u.test_all_flags )
	{
		//Check UART0 FLAG to see if UART0 test successful or not
		if( 1 == all_flags.all_test_flags_u.production_test_flags_s.uart0_test_flag )
		{
			all_flags.all_test_flags_u.production_test_flags_s.uart0_test_flag = 0;
			service_event(TEST_EVENT_UART0_TEST_OK);
		}


		//Check for BARCODE flag
		if( 1 == all_flags.all_test_flags_u.production_test_flags_s.is_barcode_scanned )
		{
			UARTprintf("\n barcode scanned - %s\n",test_algo_state.uart1_buffer_ptr);
			all_flags.all_test_flags_u.production_test_flags_s.is_barcode_scanned = 0;
			service_event(TEST_EVENT_BARCODE_SCANNED);
		}


		//Check for BUZZER flag
		if( 1 == all_flags.all_test_flags_u.production_test_flags_s.buzzer_check )
		{
			//UARTprintf("\n buzzer beep \n");
			all_flags.all_test_flags_u.production_test_flags_s.buzzer_check = 0;
			service_event(TEST_EVENT_BUZZER_SET);
		}


		//Check for ULN flag
		if( 1 == all_flags.all_test_flags_u.production_test_flags_s.uln_check )
		{
			//UARTprintf("\n uln flag set \n");
			all_flags.all_test_flags_u.production_test_flags_s.uln_check = 0;
			if( 1 == test_algo_state.all_uln )
			{
				service_event ( TEST_EVENT_ULN_ALGO_IMPLEMENTATION );
			}

		}


		//OPTO Interrupt flag
		if( 1 == all_flags.all_test_flags_u.production_test_flags_s.opto_int_flag  )
		{
			/* Check if the counter is 1, if not then two interrupts occured at the same time */
			if( 1 == test_algo_state.opto_int_counter )
			{
//				UARTprintf("\n came in to check opto counter");
				/* Wait for a time period to see if  another interrupt comes */
				if( OPTO_INT_TIMEOUT < ( getMillisecondTimestamp() - test_algo_state.opto_timestamp ) )
				{
					all_flags.all_test_flags_u.production_test_flags_s.opto_int_flag = 0;
					test_algo_state.opto_int_counter = 0;
					service_event ( TEST_EVENT_OPTO_INT_SERVICING );
				}
			}
			// error case if two interrupts occured at the same time
			else
			{
				all_flags.all_test_flags_u.production_test_flags_s.opto_int_flag = 0;
				test_algo_state.opto_int_counter = 0;
				error_case();
			}
		}


		//Check UART2 flag to see if UART2 test successful or not


		//check if uart 2 test ongoing
		if( 1 == all_flags.all_test_flags_u.production_test_flags_s.uart2_test_on )
		{
//			UARTprintf("\n g");

			if( ( getMillisecondTimestamp()-test_algo_state.uart2_timeout) > 3000 )
			{
				UARTprintf("\n string %s",test_algo_state.uart2_buffer_ptr);
				UARTprintf("\n ts : %d getts: %d",test_algo_state.uart2_timeout,getMillisecondTimestamp());
				all_flags.all_test_flags_u.production_test_flags_s.uart2_test_on = 0;
				service_event ( TEST_ALGORITHM_STATE_UART2_TEST_DATA_REC );

			}
		}


		//Check if the ethernet test return back an IP which will set the ethernet_ip_get flag
		if (1 == all_flags.all_test_flags_u.production_test_flags_s.ethernet_ip_get )
		{
			all_flags.all_test_flags_u.production_test_flags_s.ethernet_ip_get = 0;
			service_event(TEST_EVENT_ETHERNET_TEST_DONE);
		}

		//Check if PPTL gave a button Press
		if ( 1 == all_flags.all_test_flags_u.production_test_flags_s.pptl_button_press )
		{
			all_flags.all_test_flags_u.production_test_flags_s.pptl_button_press = 0;
			service_event (TEST_EVENT_PPTL_BUTTON_PRESS);
		}

		//Check if ethernet IP is to be checked
		if ( 1 == all_flags.all_test_flags_u.production_test_flags_s.check_for_ip )
		{
			all_flags.all_test_flags_u.production_test_flags_s.check_for_ip = 0;
			service_event (TEST_EVENT_ETHERNET_CHK_FOR_IP);
		}

	}
}/* process_test_events */







/*----------------------------------------------------------------------------*/
/* Function : uln_algo_implementation */
/*----------------------------------------------------------------------------*/
/*!
  @ brief
	Implements the ULN output algorithm

  @ details
	Implements the ULN output algorithm

  @ pre-requirements

  @ param

  @ return void


*/
void uln_algo_implementation (void)
{
	if( TEST_ALGORITHM_STATE_ULN_TEST_STARTED == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_01_HIGH;
		activate_uln(ULN1_BASE,ULN1_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_01_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_02_HIGH;
		activate_uln(ULN2_BASE,ULN2_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_02_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_03_HIGH;
		activate_uln(ULN3_BASE,ULN3_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_03_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_04_HIGH;
		activate_uln(ULN4_BASE,ULN4_PIN);
	}

	/* changes */
	else if( TEST_ALGORITHM_STATE_ULN_04_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_05_HIGH;
		deactivate_uln(ULN4_BASE,ULN4_PIN);
		activate_uln(ULN6_BASE,ULN6_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_05_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_06_HIGH;
		activate_uln(ULN5_1_BASE,ULN5_1_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_06_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_07_HIGH;
		activate_uln(ULN7_BASE,ULN7_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_07_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_08_HIGH;
		activate_uln(ULN8_BASE,ULN8_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_08_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_09_HIGH;
		activate_uln(ULN9_BASE,ULN9_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_09_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_10_HIGH;
		activate_uln(ULN10_BASE,ULN10_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_10_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_11_HIGH;
		activate_uln(ULN11_BASE,ULN11_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_11_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_12_HIGH;
		activate_uln(ULN12_BASE,ULN12_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_12_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_13_HIGH;
		activate_uln(ULN13_BASE,ULN13_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_13_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_14_HIGH;
		activate_uln(ULN14_BASE,ULN14_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_14_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_15_HIGH;
		activate_uln(ULN15_BASE,ULN15_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_15_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ULN_16_HIGH;
		activate_uln(ULN16_BASE,ULN16_PIN);
	}
	else if( TEST_ALGORITHM_STATE_ULN_16_HIGH == test_algo_state.algo_state )
	{
		test_algo_state.algo_state = TEST_ALGORITHM_STATE_ALL_ULN_LOW;
		test_algo_state.all_uln = 0;
	}

}/* uln_algo_implementation */


/*----------------------------------------------------------------------------*/
/* Function : production_test_algo_loop */
/*----------------------------------------------------------------------------*/
/*!
  @ brief
  This is the algo oop for production tests to be called in main

  @ details
  This is the algo oop for production tests to be called in main

  @ pre-requirements

  @ param void

  @ return void

*/
void production_test_algo_loop ( void )
{

	/* Local Variables */

	/* Validation */

	/* Algorithm */
	process_test_events(TEST_EVENT_IDLE);

}/* production_test_algo_loop */
