#include <allegro5/allegro5.h>
#include "t3f/t3f.h"
#include "t3f/controller.h"

static char t3f_binding_return_name[256] = {0};
static char t3f_controller_return_name[256] = {0};
static float t3f_controller_axis_threshold = 0.3;

T3F_CONTROLLER * t3f_create_controller(int bindings)
{
	T3F_CONTROLLER * cp;
	
	cp = malloc(sizeof(T3F_CONTROLLER));
	if(!cp)
	{
		return NULL;
	}
	memset(cp, 0, sizeof(T3F_CONTROLLER));
	cp->bindings = bindings;
	return cp;
}

void t3f_destroy_controller(T3F_CONTROLLER * cp)
{
	free(cp);
}

const char * t3f_get_controller_name(T3F_CONTROLLER * cp, int binding)
{
	switch(cp->binding[binding].type)
	{
		case T3F_CONTROLLER_BINDING_KEY:
		{
			sprintf(t3f_controller_return_name, "Keyboard");
			return t3f_controller_return_name;
		}
		case T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON:
		case T3F_CONTROLLER_BINDING_JOYSTICK_AXIS:
		{
			if(t3f_joystick[cp->binding[binding].joystick])
			{
				sprintf(t3f_controller_return_name, "%s (%d)", al_get_joystick_name(t3f_joystick[cp->binding[binding].joystick]), cp->binding[binding].joystick);
			}
			else
			{
				sprintf(t3f_controller_return_name, "N/A");
			}
			return t3f_controller_return_name;
		}
	}
	t3f_controller_return_name[0] = 0;
	return t3f_controller_return_name;
}

const char * t3f_get_controller_binding_name(T3F_CONTROLLER * cp, int binding)
{
	const char * name;
	switch(cp->binding[binding].type)
	{
		case T3F_CONTROLLER_BINDING_KEY:
		{
			sprintf(t3f_binding_return_name, "%s", al_keycode_to_name(cp->binding[binding].button));
			return t3f_binding_return_name;
		}
		case T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON:
		{
			if(t3f_joystick[cp->binding[binding].joystick])
			{
				name = al_get_joystick_button_name(t3f_joystick[cp->binding[binding].joystick], cp->binding[binding].button);
				sprintf(t3f_binding_return_name, "Button %s", name ? name : "???");
			}
			else
			{
				sprintf(t3f_binding_return_name, "N/A");
			}
			return t3f_binding_return_name;
		}
		case T3F_CONTROLLER_BINDING_JOYSTICK_AXIS:
		{
			if(t3f_joystick[cp->binding[binding].joystick])
			{
				name = al_get_joystick_axis_name(t3f_joystick[cp->binding[binding].joystick], cp->binding[binding].stick, cp->binding[binding].axis);
				sprintf(t3f_binding_return_name, "Stick %d %s%s%s", cp->binding[binding].stick, name ? name : "???", (cp->binding[binding].flags & T3F_CONTROLLER_FLAG_AXIS_NEGATIVE) ? "-" : "+", (cp->binding[binding].flags & T3F_CONTROLLER_FLAG_AXIS_INVERT) ? "(I)" : "");
			}
			else
			{
				sprintf(t3f_binding_return_name, "N/A");
			}
			return t3f_binding_return_name;
		}
	}
	t3f_binding_return_name[0] = 0;
	return t3f_binding_return_name;
}

void t3f_write_controller_config(ALLEGRO_CONFIG * acp, const char * section, T3F_CONTROLLER * cp)
{
	char temp_string[1024] = {0};
	char temp_string2[1024] = {0};
	int j;
	
	for(j = 0; j < cp->bindings; j++)
	{
		sprintf(temp_string, "Binding %d Type", j);
		sprintf(temp_string2, "%d", cp->binding[j].type);
		al_set_config_value(acp, section, temp_string, temp_string2);
		sprintf(temp_string, "Binding %d SubType", j);
		sprintf(temp_string2, "%d", cp->binding[j].sub_type);
		al_set_config_value(acp, section, temp_string, temp_string2);
		sprintf(temp_string, "Binding %d Joystick", j);
		sprintf(temp_string2, "%d", cp->binding[j].joystick);
		al_set_config_value(acp, section, temp_string, temp_string2);
		sprintf(temp_string, "Binding %d Button", j);
		sprintf(temp_string2, "%d", cp->binding[j].button);
		al_set_config_value(acp, section, temp_string, temp_string2);
		sprintf(temp_string, "Binding %d Stick", j);
		sprintf(temp_string2, "%d", cp->binding[j].stick);
		al_set_config_value(acp, section, temp_string, temp_string2);
		sprintf(temp_string, "Binding %d Axis", j);
		sprintf(temp_string2, "%d", cp->binding[j].axis);
		al_set_config_value(acp, section, temp_string, temp_string2);
		sprintf(temp_string, "Binding %d Flags", j);
		sprintf(temp_string2, "%d", cp->binding[j].flags);
		al_set_config_value(acp, section, temp_string, temp_string2);
	}
}

bool t3f_read_controller_config(ALLEGRO_CONFIG * acp, const char * section, T3F_CONTROLLER * cp)
{
	char temp_string[1024] = {0};
	const char * item;
	int j;
	
	for(j = 0; j < cp->bindings; j++)
	{
		sprintf(temp_string, "Binding %d Type", j);
		item = al_get_config_value(acp, section, temp_string);
		if(item)
		{
			cp->binding[j].type = atoi(item);
		}
		else
		{
			return false;
		}
		sprintf(temp_string, "Binding %d SubType", j);
		item = al_get_config_value(acp, section, temp_string);
		if(item)
		{
			cp->binding[j].sub_type = atoi(item);
		}
		else
		{
			return false;
		}
		sprintf(temp_string, "Binding %d Joystick", j);
		item = al_get_config_value(acp, section, temp_string);
		if(item)
		{
			cp->binding[j].joystick = atoi(item);
		}
		else
		{
			return false;
		}
		sprintf(temp_string, "Binding %d Button", j);
		item = al_get_config_value(acp, section, temp_string);
		if(item)
		{
			cp->binding[j].button = atoi(item);
		}
		else
		{
			return false;
		}
		sprintf(temp_string, "Binding %d Stick", j);
		item = al_get_config_value(acp, section, temp_string);
		if(item)
		{
			cp->binding[j].stick = atoi(item);
		}
		else
		{
			return false;
		}
		sprintf(temp_string, "Binding %d Axis", j);
		item = al_get_config_value(acp, section, temp_string);
		if(item)
		{
			cp->binding[j].axis = atoi(item);
		}
		else
		{
			return false;
		}
		sprintf(temp_string, "Binding %d Flags", j);
		item = al_get_config_value(acp, section, temp_string);
		if(item)
		{
			cp->binding[j].flags = atoi(item);
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool t3f_bind_controller(T3F_CONTROLLER * cp, int binding)
{
	ALLEGRO_EVENT_QUEUE * queue;
	ALLEGRO_EVENT event;
	const char * val;
	float ppos = 0.0;
	int i, jn;
	
	queue = al_create_event_queue();
	if(!queue)
	{
		return false;
	}
	if(t3f_flags & T3F_USE_KEYBOARD)
	{
		al_register_event_source(queue, al_get_keyboard_event_source());
	}
	if(t3f_flags & T3F_USE_MOUSE)
	{
		al_register_event_source(queue, al_get_mouse_event_source());
	}
	if(t3f_flags & T3F_USE_JOYSTICK)
	{
		al_register_event_source(queue, al_get_joystick_event_source());
		for(i = 0; i < al_get_num_joysticks() - 1; i++)
		{
			t3f_joystick[i] = al_get_joystick(i);
			if(t3f_joystick[i])
			{
				al_get_joystick_state(t3f_joystick[i], &t3f_joystick_state[i]); // read initial state
			}
		}
	}
	val = al_get_config_value(t3f_config, "T3F", "joystick_axis_threshold");
	if(val)
	{
		t3f_controller_axis_threshold = atof(val);
	}
	while(1)
	{
		al_wait_for_event(queue, &event);
		switch(event.type)
		{
			/* key was pressed or repeated */
			case ALLEGRO_EVENT_KEY_DOWN:
			{
				if(event.keyboard.keycode != ALLEGRO_KEY_ESCAPE)
				{
					cp->binding[binding].type = T3F_CONTROLLER_BINDING_KEY;
					cp->binding[binding].button = event.keyboard.keycode;
					al_destroy_event_queue(queue);
					return true;
				}
				else
				{
					al_destroy_event_queue(queue);
					return false;
				}
			}
			case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
			{
				cp->binding[binding].type = T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON;
				cp->binding[binding].joystick = t3f_get_joystick_number(event.joystick.id);
				cp->binding[binding].stick = event.joystick.stick;
				cp->binding[binding].button = event.joystick.button;
				al_destroy_event_queue(queue);
				return true;
			}
			case ALLEGRO_EVENT_JOYSTICK_AXIS:
			{
				jn = t3f_get_joystick_number(event.joystick.id);
				if(jn >= 0)
				{
					if(event.joystick.pos < -T3F_CONTROLLER_AXIS_THRESHOLD && t3f_joystick_state[jn].stick[event.joystick.stick].axis[event.joystick.axis] >= -T3F_CONTROLLER_AXIS_THRESHOLD)
					{
						cp->binding[binding].type = T3F_CONTROLLER_BINDING_JOYSTICK_AXIS;
						cp->binding[binding].joystick = jn;
						cp->binding[binding].stick = event.joystick.stick;
						cp->binding[binding].axis = event.joystick.axis;
						cp->binding[binding].flags = T3F_CONTROLLER_FLAG_AXIS_NEGATIVE;
						al_destroy_event_queue(queue);
						return true;
					}
					else if(event.joystick.pos > T3F_CONTROLLER_AXIS_THRESHOLD && t3f_joystick_state[jn].stick[event.joystick.stick].axis[event.joystick.axis] <= T3F_CONTROLLER_AXIS_THRESHOLD)
					{
						cp->binding[binding].type = T3F_CONTROLLER_BINDING_JOYSTICK_AXIS;
						cp->binding[binding].joystick = jn;
						cp->binding[binding].stick = event.joystick.stick;
						cp->binding[binding].axis = event.joystick.axis;
						cp->binding[binding].flags = T3F_CONTROLLER_FLAG_AXIS_POSITIVE;
						al_destroy_event_queue(queue);
						return true;
					}
				}
				if(event.joystick.pos >= -T3F_CONTROLLER_AXIS_THRESHOLD && event.joystick.pos <= T3F_CONTROLLER_AXIS_THRESHOLD)
				{
					ppos = event.joystick.pos;
				}
				if(al_get_joystick_stick_flags(event.joystick.id, event.joystick.stick) & ALLEGRO_JOYFLAG_DIGITAL)
				{
					ppos = 0.1;
				}
				break;
			}
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			{
				cp->binding[binding].type = T3F_CONTROLLER_BINDING_MOUSE_BUTTON;
				cp->binding[binding].button = event.mouse.button;
				al_destroy_event_queue(queue);
				return true;
			}
		}
	}
	return false;
}

/* see which buttons are held */
void t3f_read_controller(T3F_CONTROLLER * cp)
{
	int i;
	
	for(i = 0; i < cp->bindings; i++)
	{
		switch(cp->binding[i].type)
		{
			case T3F_CONTROLLER_BINDING_KEY:
			{
				if(t3f_key[cp->binding[i].button])
				{
					cp->state[i].down = true;
				}
				else
				{
					cp->state[i].down = false;
				}
				break;
			}
			case T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON:
			{
				if(t3f_joystick_state[cp->binding[i].joystick].button[cp->binding[i].button])
				{
					cp->state[i].down = true;
				}
				else
				{
					cp->state[i].down = false;
				}
				break;
			}
			case T3F_CONTROLLER_BINDING_JOYSTICK_AXIS:
			{
				bool held = false;
				cp->state[i].pos = t3f_joystick_state[cp->binding[i].joystick].stick[cp->binding[i].stick].axis[cp->binding[i].axis];
				if(cp->binding[i].flags & T3F_CONTROLLER_FLAG_AXIS_INVERT)
				{
					cp->state[i].pos = -cp->state[i].pos;
				}
				if((cp->binding[i].flags & T3F_CONTROLLER_FLAG_AXIS_NEGATIVE) && t3f_joystick_state[cp->binding[i].joystick].stick[cp->binding[i].stick].axis[cp->binding[i].axis] < -T3F_CONTROLLER_AXIS_THRESHOLD)
				{
					held = true;
				}
				else if((cp->binding[i].flags & T3F_CONTROLLER_FLAG_AXIS_POSITIVE) && t3f_joystick_state[cp->binding[i].joystick].stick[cp->binding[i].stick].axis[cp->binding[i].axis] > T3F_CONTROLLER_AXIS_THRESHOLD)
				{
					held = true;
				}
				if(held)
				{
					cp->state[i].down = true;
				}
				else
				{
					cp->state[i].down = false;
				}
				break;
			}
		}
	}
}

/* update pressed/released variables */
void t3f_update_controller(T3F_CONTROLLER * cp)
{
	int i;
	
	for(i = 0; i < cp->bindings; i++)
	{
		cp->state[i].was_held = cp->state[i].held;
		if(cp->state[i].down)
		{
			if(!cp->state[i].was_held)
			{
				cp->state[i].pressed = true;
				cp->state[i].released = false;
			}
			else
			{
				cp->state[i].pressed = false;
			}
			cp->state[i].held = true;
		}
		else
		{
			if(cp->state[i].was_held)
			{
				cp->state[i].released = true;
			}
			else
			{
				cp->state[i].released = false;
			}
			cp->state[i].pressed = false;
			cp->state[i].held = false;
		}
	}
}

void t3f_clear_controller_state(T3F_CONTROLLER * cp)
{
	int i;
	
	for(i = 0; i < cp->bindings; i++)
	{
		cp->state[i].down = false;
		cp->state[i].held = false;
		cp->state[i].was_held = false;
		cp->state[i].pressed = false;
		cp->state[i].released = false;
	}
}
