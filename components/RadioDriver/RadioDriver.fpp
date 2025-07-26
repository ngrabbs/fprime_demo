module SBandDemo {

  @ Radio driver serialisable command.  This structure mirrors the
  @ `radio_message_t` union used in the original FreeRTOS code.  The
  @ `type` field indicates whether the message configures the radio or
  @ sends a payload.  When `type` is 0 the config fields are valid; when
  @ `type` is 1 the payload fields are valid.
  struct RadioMessage {
    u8 type
    u32 frequencyHz
    u8  txPower
    u8  modulation
    u32 bandwidthHz
    u8  spreadingFactor
    u8  codingRate
    u8  payloadLength
    u8[256] payload
  }

  port RadioCmdPort input SBandDemo.RadioMessage

  port RadioStatusPort output u8

  active component RadioDriver {
    @ Input port carrying commands and payloads
    input port cmdIn: RadioCmdPort

    @ Optional output status port.  This can be connected to a
    @ telemetry channel or event logger.
    output port status: RadioStatusPort

    @ TODO: add events, telemetry channels and parameters as needed
  }

}