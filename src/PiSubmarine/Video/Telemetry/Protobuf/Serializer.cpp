#include "PiSubmarine/Video/Telemetry/Protobuf/Serializer.h"

#include <string>

#include "Video.pb.h"
#include "PiSubmarine/Video/Telemetry/Protobuf/ErrorCode.h"
#include "PiSubmarine/Error/Api/MakeError.h"

namespace PiSubmarine::Video::Telemetry::Protobuf
{
    Serializer::Serializer(const Api::IProvider& provider)
        : m_Provider(provider)
    {
    }

    Error::Api::Result<std::vector<std::byte>> Serializer::GetRaw() const
    {
        const auto statusResult = m_Provider.GetStatus();
        if (!statusResult.has_value())
        {
            return std::unexpected(statusResult.error());
        }

        ::pisubmarine::video::telemetry::protobuf::Status protoStatus;
        protoStatus.set_is_streaming_enabled(statusResult->IsStreamingEnabled);
        protoStatus.set_subscribers(statusResult->Subscribers);
        protoStatus.set_operational(static_cast<int32_t>(statusResult->Operational));
        protoStatus.set_active_faults(static_cast<uint32_t>(statusResult->ActiveFaults));

        std::string serialized;
        if (!protoStatus.SerializeToString(&serialized))
        {
            return std::unexpected(Error::Api::MakeError(
                Error::Api::ErrorCondition::DeviceError,
                make_error_code(ErrorCode::SerializationFailed)));
        }

        std::vector<std::byte> bytes;
        bytes.reserve(serialized.size());
        for (const char character : serialized)
        {
            bytes.push_back(static_cast<std::byte>(character));
        }

        return bytes;
    }
}
