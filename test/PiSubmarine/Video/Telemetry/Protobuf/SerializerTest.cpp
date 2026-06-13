#include <gtest/gtest.h>

#include "PiSubmarine/Video/Telemetry/Api/IProviderMock.h"
#include "PiSubmarine/Video/Telemetry/Protobuf/Deserializer.h"
#include "PiSubmarine/Video/Telemetry/Protobuf/Serializer.h"
#include "PiSubmarine/Telemetry/Api/IRawSourceMock.h"

namespace PiSubmarine::Video::Telemetry::Protobuf
{
    TEST(SerializerTest, RoundTripsVideoStatus)
    {
        Api::IProviderMock providerMock;
        const Api::Status expectedStatus{
            .IsStreamingEnabled = true,
            .Subscribers = 3,
            .Operational = Api::OperationalState::Streaming,
            .ActiveFaults = static_cast<Api::Faults>(
                static_cast<uint32_t>(Api::Faults::SourceError)
                | static_cast<uint32_t>(Api::Faults::NetworkError))};

        EXPECT_CALL(providerMock, GetStatus())
            .WillOnce(testing::Return(Error::Api::Result<Api::Status>(expectedStatus)));

        Serializer serializer(providerMock);
        const auto rawResult = serializer.GetRaw();

        ASSERT_TRUE(rawResult.has_value());
        EXPECT_FALSE(rawResult->empty());

        ::PiSubmarine::Telemetry::Api::IRawSourceMock rawSourceMock;
        EXPECT_CALL(rawSourceMock, GetRaw())
            .WillOnce(testing::Return(Error::Api::Result<std::vector<std::byte>>(*rawResult)));

        Deserializer deserializer(rawSourceMock);
        const auto statusResult = deserializer.GetStatus();

        ASSERT_TRUE(statusResult.has_value());
        EXPECT_EQ(*statusResult, expectedStatus);
    }
}
