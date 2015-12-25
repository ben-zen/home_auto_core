#include "round_robin_queue.h"
#include "event.h"

uint8_t const LED_PIN = 13;
uint8_t const HALL_PIN = 3;

bool lockEnabled = false;

Queue<Event, 8> eventQueue;

void doorSensorChanged()
{
  eventQueue.enqueue(Event(EventType::DoorSensor, digitalRead(HALL_PIN)));
}

void buttonChanged()
{
  eventQueue.enqueue(Event(EventType::WallButton, 0));
}

void handleButtonEvent()
{
}

void handleDoorEvent(bool const opened)
{
  switch (opened)
  {
  case true:
    /* If the sensor is HIGH, then the door has been closed.
     * The simple form is to shut off the LED, but another approach is
     * to set a timeout; this could be tuned by a menu setting,
     * eventually.
     */
    digitalWrite(LED_PIN, LOW);
    break;
  case false:
    /* If the sensor is LOW, then the door has opened. The default
     * operation should be to turn on the LED; eventually this can
     * check the light level before turning on the light.
     */
    digitalWrite(LED_PIN, HIGH);
    break;
  default:
    break;
  }
}

void handleKeyEvent(char const key)
{
  // Handle key input; this might differ based on what input is
  // required.
}

void handleMotionEvent()
{
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(HALL_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(3), doorSensorChanged, CHANGE);
  // Attach an interrupt on the rising of the button pin, once that's
  // set.
}

void loop()
{
  Event event;
  if (eventQueue.dequeue(&event))
  {
    switch (event.getType())
    {
    case EventType::DoorSensor:
      {
	bool opened = false;
	if (event.doorOpened(&opened))
	{
	  handleDoorEvent(opened);
	}
      }
      break;
    case EventType::WallButton:
      handleButtonEvent();
      break;
    case EventType::MotionDetector:
      handleMotionEvent();
      break;
    case EventType::Keypress:
      {
	char key = 0;
	if (event.pressedKey(&key))
	{
	  handleKeyEvent(key);
	}
      }
      break;
    }
  }
}
