# Video.Telemetry.Protobuf

`PiSubmarine.Video.Telemetry.Protobuf` adapts `Video.Telemetry.Api` to and from
raw protobuf payloads.

- `Serializer` implements `Telemetry.Api::IRawSource` by querying a local
  `Video::Telemetry::Api::IProvider`
- `Deserializer` implements `Video::Telemetry::Api::IProvider` by reading bytes
  from a `Telemetry.Api::IRawSource`
