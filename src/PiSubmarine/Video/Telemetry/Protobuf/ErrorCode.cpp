#include "PiSubmarine/Video/Telemetry/Protobuf/ErrorCode.h"

#include <array>
#include <string_view>

namespace PiSubmarine::Video::Telemetry::Protobuf
{
    namespace
    {
        class ErrorCategory final : public std::error_category
        {
        public:
            [[nodiscard]] const char* name() const noexcept override
            {
                return "PiSubmarine.Video.Telemetry.Protobuf";
            }

            [[nodiscard]] std::string message(const int condition) const override
            {
                constexpr std::array<std::string_view, 4> Messages{
                    "success",
                    "serialization failed",
                    "deserialization failed",
                    "invalid payload"};

                const auto index = static_cast<std::size_t>(condition);
                if (index >= Messages.size())
                {
                    return "unknown video telemetry protobuf error";
                }

                return std::string(Messages[index]);
            }
        };
    }

    std::error_code make_error_code(const ErrorCode errorCode) noexcept
    {
        static const ErrorCategory Category;
        return {static_cast<int>(errorCode), Category};
    }
}
