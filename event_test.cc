/* Copyright (C) 2015 Ben Lewis <benjf5+github@gmail.com>
 * Licensed under the MIT license.
 *
 * Test code for the Event class. Compile with any C++ compiler to test the
 * Event's functionality.
 */

#include "event.h"
#include <iostream>

int main()
{
  Event doorOpenedEvent(EventType::DoorSensor, static_cast<uint16_t>(true));
  bool opened = false;
  if (doorOpenedEvent.doorOpened(&opened))
  {
    std::cout << "doorOpenedEvent states that the door was "
	      << ((opened) ? "opened" : "closed") << std::endl;
  }
  else
  {
    std::cout << "doorOpenedEvent does not have EventType::DoorSensor as its event type."
	      << std::endl;
  }

  Event doorClosedEvent(EventType::DoorSensor, static_cast<uint16_t>(false));
  opened = false;
  if (doorClosedEvent.doorOpened(&opened))
  {
    std::cout << "doorClosedEvent states that the door was "
	      << ((opened) ? "opened" : "closed") << std::endl;
  }
  else
  {
    std::cout << "doorClosedEvent does not have EventType::DoorSensor as its event type."
	      << std::endl;
  }

  Event keypressEvent(EventType::Keypress, 'a');
  char content = '\0';
  if (keypressEvent.pressedKey(&content))
  {
    std::cout << "keypressEvent contains character " << content << std::endl;
  }
  else
  {
    std::cout << "keypressEvent does not have EventType::Keypress as its event type."
	      << std::endl;
  }

  return 0;
}
