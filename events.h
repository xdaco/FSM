ifndef EVENTS_H_ // include guard
#define EVENTS_H_
//Definition starts here
//#include <...> // other dependencies
//#include "..." // other dependencies

#ifdef __cplusplus //making it useable in C compiler too
extern “C” {
#endif
/*==============================================================================

                               INCLUDE FILES

==============================================================================*/


/*==============================================================================

                      DEFINITIONS AND TYPES : MACROS

==============================================================================*/


/*==============================================================================

                      DEFINITIONS AND TYPES : ENUMS

==============================================================================*/

// ... My types, methods, variables starts here
typedef enum
{
  Event_exit=0
  Event_inactive
  Event_active
  Event_pause
  Event_invalid
}all_state_events;
typedef struct
{
  char* user_input_data;
	all_state_events;
	unsigned long timestamp;
}all_event_information;

typedef struct
{
	union
	{
		struct
		{
			unsigned int received_user_input_data:1;
			unsigned int valid_user_input:1;
			unsigned int valid_event_served:1;
			unsigned int dummy3:1;
			unsigned int dummy4:1;
			unsigned int dummy5:1;
			unsigned int dummy6:1;	
			unsigned int dummy7:1;
			unsigned int dummy8:1;
			unsigned int dummy9:1;
			unsigned int dummy10:1;
			unsigned int dummy11:1;
			unsigned int dummy12:1;
			unsigned int dummy13:1;
			unsigned int dummy14:1;
			unsigned int dummy15:1;
			unsigned int dummy16:1;
			unsigned int dummy17:1;
			unsigned int dummy18:1;
			unsigned int dummy19:1;
			unsigned int dummy20:1;
			unsigned int dummy21:1;
			unsigned int dummy22:1;
			unsigned int dummy23:1;
			unsigned int dummy24:1;
			unsigned int dummy25:1;
			unsigned int dummy26:1;
			unsigned int dummy27:1;
			unsigned int dummy28:1;
			unsigned int dummy29:1;
			unsigned int dummy30:1;
			unsigned int dummy31:1;
			
		}unsigned int all_flags;

	}all_flags_u;

}all_flags_s;
  
  /*==============================================================================

                           EXTERNAL DECLARATIONS

==============================================================================*/

extern all_flags_s flags;

/*==============================================================================

                           FUNCTION PROTOTYPES

==============================================================================*/

void main_loop ( void );
  
// .... My types, methods, variables ends here

#ifdef __cplusplus // Ending extern C
}
#endif

#endif // EVENTS_H_
