#include <stdio.h>
struct state; // declearing a structure  named state ;
typedef void state_fn(struct state *); 	// this typedef simplyfies the readability of the decleared 
					// function 'state_fn'; The return type of the function is void, 
					// the argument type is a pointer to a structure; So next time 
					// when we define a function as 'state_fn foo', it is clear that
					// 'foo' is a function whose return type is void and can take an 
					// argument of pointer to a structure 

struct state  // Here we are defining the previously decleared structure 'state' ; 
{
    state_fn * next; // This defines a function pointer named 'next' whose type is 'state_fn'; 
    		     // Interestingly  this also tells us that the reurn type of 'next' is 
    		     // void and 'next' can take an argument of pointer to a structure;
    int i; // data
};

state_fn state1, state2; // This decleares two functions as 'state1' and 'state2' whose type is
			 // state_fn; Therefore 'state1' and 'state2' can take an argument type of pointer to structure

void state1(struct state * state) // Here we are defining the activity of the function 'state1'
{
    printf("%s%s%s %i\n"," :", __func__," :", ++state->i); // This line prints the  function name itself and the value of 
    							   // the member 'i' of the passed structure argument state;
    state->next = state2; // Here we are pointing function pointer 'next' of the passed argument (which is a structure
    			 // called 'state')  to the function 'state2' 
}

void state2(struct state * state) // Here we are defining the activity of the function 'state2'
{
    printf("%s%s%s %i\n"," :", __func__," :", ++state->i); // This line prints the  function name itself and the value 
    							   // of the member 'i' of the passed structure argument state;
    state->next = state->i < 10 ? state1 : 0 ; // Here we are pointing function pointer 'next' of the passed argument
    					       // (which is a structure called 'state')  to the state1 if the value of 
    					       // i <10 else  passes 0 which terminates the  loopl;
}

int main(void)
{
    struct state state = { state1,0 }; // creating a  variable 'state' of type 'state'; also intializing the variable 
    				       // with state1 and 0;thereby 'state.next' becomes state1 and 'state.i' becomes 0; 
    while(state.next) // If the state.next is true then program will enter into loop
	{	
		state.next(&state);//
	}
}
