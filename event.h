/* Copyright (C) 2015 Ben Lewis <benjf5+github@gmail.com>
 *
 * Defines a compact event structure for use in a home automation
 * system, with a bit of type-aware optimization for space.
 */

#include <stdint.h>

// Note: EventType has a maximum value of 31, since there are five
// bits of space it can occupy.
enum class EventType : uint16_t
{
  INVALID,
  DoorSensor,     // Hall effect sensor for door open/close
  WallButton,     // Button for locking the entryway light on or off
  MotionDetector, // Motion detector for sensing presence/security alert
  Keypress        // Press on the monitor module's keypad
};

class Event
{
private:
  uint16_t m_data;

public:
  Event(EventType eType, uint16_t value)
  {
    m_data = 0x0;
    m_data |= (value & 0x03FF);
    m_data |= (static_cast<uint16_t>(eType) & 0x003F) << 10;
  }

  Event() : m_data(0)
  {
  }

  Event(Event const &other) : m_data(other.m_data)
  {
  }

  EventType getType() const
  {
    return static_cast<EventType>((m_data & 0xFC00) >> 10);
  }

  uint16_t getValue() const
  {
    return (m_data & 0x03FF);
  }

  bool doorOpened(bool *out) const
  {
    bool properAccess = (getType() == EventType::DoorSensor);
    *out = false;
    if (properAccess)
    {
      *out = (getValue() != 0);
    }
    return properAccess;
  }

  bool pressedKey(char *out) const
  {
    bool properAccess = (getType() == EventType::Keypress);
    *out = 0;
    if (properAccess)
    {
      *out = static_cast<char>(m_data & 0x00FF);
    }
    return properAccess;
  }

};
