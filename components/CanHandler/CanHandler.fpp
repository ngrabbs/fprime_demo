module SBandDemo {

  @ Active component that reads CAN frames and converts them into
  @ `RadioMessage` structures for the `RadioDriver`.  This mirrors the
  @ functionality of the original `task_can.c` and the SpaceCAN parser.
  active component CanHandler {
    @ Output port carrying radio messages to the driver
    output port radioOut: RadioCmdPort

    @ The rate group scheduler input.  The scheduler will call this port
    @ periodically to poll the CAN controller for new frames.
    sync input port schedIn: Svc.Sched
  }

}