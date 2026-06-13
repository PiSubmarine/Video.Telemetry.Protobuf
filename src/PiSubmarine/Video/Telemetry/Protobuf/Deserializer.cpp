#include "PiSubmarine/Video/Telemetry/Protobuf/Deserializer.h"

#include <optional>

#include "Video.pb.h"
#include "PiSubmarine/Video/Telemetry/Protobuf/ErrorCode.h"
#include "PiSubmarine/Error/Api/MakeError.h"

namespace PiSubmarine::Video::Telemetry::Protobuf
{
    namespace
    {
        [[nodiscard]] Error::Api::Error MakeVideoTelemetryError(
            const Error::Api::ErrorCondition condition,
            const ErrorCode code)
        {
            return Error::Api::MakeError(condition, make_error_code(code));
        }

        [[nodiscard]] std::optional<Api::OperationalState> ParseOperationalState(const int32_t value)
        {
            switch (value)
            {
                case 0:
                    return Api::OperationalState::Stopped;
                case 1:
                    return Api::OperationalState::Streaming;
                case 2:
                    return Api::OperationalState::Faulted;
                default:
                    return std::nullopt;
            }
        }
    }

    Deserializer::Deserializer(const ::PiSubmarine::Telemetry::Api::IRawSource& rawSource)
        : m_RawSource(rawSource)
    {
    }

    Error::Api::Result<Api::Status> Deserializer::GetStatus() const
    {
        const auto rawResult = m_RawSource.GetRaw();
        if (!rawResult.has_value())
        {
            return std::unexpected(rawResult.error());
        }

        ::pisubmarine::video::telemetry::protobuf::Status protoStatus;
        if (!protoStatus.ParseFromArray(reinterpret_cast<const char*>(rawResult->data()), static_cast<int>(rawResult->size())))
        {
            return std::unexpected(MakeVideoTelemetryError(
                Error::Api::ErrorCondition::ContractError,
                ErrorCode::DeserializationFailed));
        }

        const auto operational = ParseOperationalState(protoStatus.operational());
        if (!operational.has_value())
        {
            return std::unexpected(MakeVideoTelemetryError(
                Error::Api::ErrorCondition::ContractError,
                ErrorCode::InvalidPayload));
        }

        Api::Status status{
            .IsStreamingEnabled = protoStatus.is_streaming_enabled(),
            .Subscribers = protoStatus.subscribers(),
            .Operational = *operational,
            .ActiveFaults = static_cast<Api::Faults>(protoStatus.active_faults())};

        return status;
    }
}
