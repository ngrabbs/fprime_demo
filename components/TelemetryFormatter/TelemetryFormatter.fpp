module SBandDemo {

  @ Passive component that encapsulates SpaceCAN formatting routines.  The
  @ original firmware implemented formatting and parsing in
  @ `protocol_spacecan.c`.  This component exposes those functions so that
  @ other components can reuse them.
  passive component TelemetryFormatter {
    @ TODO: define ports for formatting and parsing if needed
  }

}