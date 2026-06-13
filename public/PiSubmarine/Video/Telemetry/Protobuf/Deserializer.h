#pragma once

#include "PiSubmarine/Video/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Telemetry/Api/IRawSource.h"

namespace PiSubmarine::Video::Telemetry::Protobuf
{
    class Deserializer final : public Api::IProvider
    {
    public:
        explicit Deserializer(const ::PiSubmarine::Telemetry::Api::IRawSource& rawSource);

        [[nodiscard]] Error::Api::Result<Api::Status> GetStatus() const override;

    private:
        const ::PiSubmarine::Telemetry::Api::IRawSource& m_RawSource;
    };
}
