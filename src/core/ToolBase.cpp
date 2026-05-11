#include "../../include/core/ToolBase.h"
#include "../../include/core/EventBus.h"
#include "../../include/core/Logger.h"

namespace Core {

void ToolBase::PublishEvent(const std::string& eventType, const nlohmann::json& data) {
    if (eventBus) {
        eventBus->Publish(eventType, data, instanceID);
    }
}

void ToolBase::Log(const std::string& actionType, const nlohmann::json& data) {
    if (logger) {
        logger->Log(instanceID, GetID(), actionType, data);
    }
}

} // namespace Core
