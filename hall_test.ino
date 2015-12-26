/* Copyright (C) 2015 Ben Lewis <benjf5+github@gmail.com>
 * Licensed under the MIT license.
 *
 * Core of an extensible, event-based home automation system.
 */

#include "round_robin_queue.h"
#include "event.h"

uint8_t const LED_PIN = 13;
uint8_t const HALL_PIN = 3;

bool lockEnabled = false;
uint8_t doorTimeoutCounter = 0;

Queue<Event, 8> eventQueue;

void doorSensorChanged()
{
  // Since LOW signals the presence of a magnet, we read the inverse
  // to tell if the door has been opened.
  eventQueue.enqueue(Event(EventType::DoorSensor, !digitalRead(HALL_PIN)));
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
    TIMSK1 |= (1 << OCIE1A);
    break;
  case false:
    /* If the sensor is LOW, then the door has opened. The default
     * operation should be to turn on the LED; eventually this can
     * check the light level before turning on the light.
     */
    TIMSK1 &= ~(1 << OCIE1A);
    digitalWrite(LED_PIN, HIGH);
    break;
  }
  TCNT1 = 0;
  doorTimeoutCounter = 0;
}

void handleKeyEvent(char const key)
{
  // Handle key input; this might differ based on what input is
  // required.
}

void handleMotionEvent()
{
}

void handleTimerEvent(TimerEvent timer)
{
  // Currently only one timer is tracked.
  if (timer == TimerEvent::DoorClosedTimer)
  {
    if (!lockEnabled)
    {
      digitalWrite(LED_PIN, LOW);
    }
  }
}

void initializeTimer()
{
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  // We use timer1 when the door has closed to track how long since the door was
  // opened, in order to track when we want to shut off the light (if the
  // light-off is not overridden by the button.)
  
  // We want the prescaler to be set for maximum duration, so that we can tick
  // our counter every two seconds (instead of every few milliseconds); to do
  // that, we need to set CS12 and CS10 to HIGH on TCC1B:
  TCCR1B |= ((1 << CS12) | (1 << CS10));

  // Enable CTC mode on timer1.
  TCCR1B |= (1 << WGM12);
  
  // At a 1024 prescaler, we want (2 s) / (6.4e-5 s) - 1 ticks before we
  // increment, or 31249 ticks on the CTC counter:
  OCR1A = 31249;

  interrupts();
}

ISR(TIMER1_COMPA_vect)
{
  // This timer ticks every two seconds; once ten seconds (for testing) or
  // five minutes (the more realistic window) has elapsed, queue a TimerElapsed
  // event; if the queueing succeeds, disable the timer.
  doorTimeoutCounter++;

  if (doorTimeoutCounter > 4) // above 4 -> above 8 seconds has passed.
  {
    if (eventQueue.enqueue(Event(EventType::TimerElapsed, static_cast<uint16_t>(TimerEvent::DoorClosedTimer))))
    {
      TIMSK1 &= ~(1 << OCIE1A);
      doorTimeoutCounter = 0;
    }
  }
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(HALL_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(3), doorSensorChanged, CHANGE);
  initializeTimer();
  // Additional work needs to be done for supporting a wall button.
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
    case EventType::TimerElapsed:
      {
	TimerEvent timerType = TimerEvent::INVALID;
	if (event.elapsedTimer(&timerType))
	{
	  handleTimerEvent(timerType);
	}
      }
      break;
    }
  }
}
