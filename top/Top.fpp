module SBandDemo {

  @ Topology definition for the S‑Band demo.  This topology instantiates
  @ the three core components (radio driver, CAN handler and
  @ housekeeping) and connects their ports.  The housekeeping component
  @ should also be connected to a rate group driver to schedule its
  @ execution, but this is left as a TODO for the user.
  topology Topology {
    instance radio  : RadioDriver base id 0x100
    instance can    : CanHandler   base id 0x200
    instance hk     : Housekeeping base id 0x300

    connections {
      can.radioOut  -> radio.cmdIn
      hk.radioOut   -> radio.cmdIn
      # TODO: connect the `schedIn` ports of the CanHandler and
      # Housekeeping components to a rate group driver so that
      # they run periodically.  See the F´ rate group documentation
      # for details.  For example:
      #   rateGroupMemberOut[n] -> can.schedIn
      #   rateGroupMemberOut[m] -> hk.schedIn
    }
  }

}