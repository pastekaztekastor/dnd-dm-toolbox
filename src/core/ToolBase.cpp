#include "../../include/core/ToolBase.h"
#include "../../include/core/EventBus.h"
#include "../../include/core/Logger.h"

namespace Core {

void ToolBase::PublishEvent(const std::string& eventType, const nlohmann::json& data) {
    if (eventBus) eventBus->Publish(eventType, data, instanceID);
}

void ToolBase::Log(const std::string& actionType, const nlohmann::json& data) {
    if (logger) logger->Log(instanceID, GetID(), actionType, data);
}

ServiceID ToolBase::RegisterService(const std::string& name, ServiceHandler handler) {
    if (serviceBus) return serviceBus->RegisterService(name, std::move(handler));
    return -1;
}

void ToolBase::UnregisterService(ServiceID id) {
    if (serviceBus) serviceBus->UnregisterService(id);
}

std::optional<nlohmann::json> ToolBase::CallService(const std::string& name,
                                                     const nlohmann::json& params) {
    if (serviceBus) return serviceBus->CallService(name, params);
    return std::nullopt;
}

std::vector<nlohmann::json> ToolBase::QueryServices(const std::string& name,
                                                     const nlohmann::json& params) {
    if (serviceBus) return serviceBus->QueryService(name, params);
    return {};
}

} // namespace Core
