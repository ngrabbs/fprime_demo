module SBandDemo {

  @ Active component that periodically collects housekeeping data and
  @ emits it as a payload to the radio.  This maps to the
  @ `task_housekeeping.c` FreeRTOS task【740037136390698†L14-L37】.
  active component Housekeeping {
    @ Sched port used by the rate group driver to trigger data collection.  Using
    @ the `Svc.Sched` port type allows the component to be driven by a rate
    @ group scheduler in F´.
    sync input port schedIn: Svc.Sched

    @ Output port carrying formatted beacon frames
    output port radioOut: RadioCmdPort
  }

}