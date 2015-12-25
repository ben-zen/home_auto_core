# Door lighting control flow

## Basic events

The basic flow is, door opens, light goes on. When the door closes, a timer is
set to shut off the light. There should be a button on the wall to lock the
light "on", when this happens. When the lock state is engaged, ignore the
request to turn off the light. If the lock state is disengaged before the five
minutes have elapsed, the light should turn off.