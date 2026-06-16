#include "../../include/core/ServiceBus.h"
#include <iostream>

namespace Core {

ServiceBus::ServiceBus() {}
ServiceBus::~ServiceBus() {}

ServiceID ServiceBus::RegisterService(const std::string& name, ServiceHandler handler) {
    ServiceID id = nextServiceID++;
    services[name].push_back({ id, name, std::move(handler) });
    if (debugMode)
        std::cout << "ServiceBus: registered \"" << name << "\" (id=" << id << ")" << std::endl;
    return id;
}

bool ServiceBus::FindRegistration(ServiceID id, std::string& outName, size_t& outIndex) {
    for (auto& [name, regs] : services) {
        for (size_t i = 0; i < regs.size(); ++i) {
            if (regs[i].id == id) {
                outName  = name;
                outIndex = i;
                return true;
            }
        }
    }
    return false;
}

void ServiceBus::UnregisterService(ServiceID id) {
    std::string name;
    size_t      idx;
    if (!FindRegistration(id, name, idx)) return;
    auto& regs = services[name];
    regs.erase(regs.begin() + static_cast<ptrdiff_t>(idx));
    if (regs.empty()) services.erase(name);
    if (debugMode)
        std::cout << "ServiceBus: unregistered id=" << id << " (\"" << name << "\")" << std::endl;
}

std::optional<nlohmann::json> ServiceBus::CallService(const std::string& name,
                                                       const nlohmann::json& params) {
    auto it = services.find(name);
    if (it == services.end() || it->second.empty()) {
        if (debugMode)
            std::cout << "ServiceBus: CallService(\"" << name << "\") — no provider" << std::endl;
        return std::nullopt;
    }
    if (debugMode)
        std::cout << "ServiceBus: CallService(\"" << name << "\")" << std::endl;
    try {
        return it->second[0].handler(params);
    } catch (const std::exception& e) {
        std::cerr << "ServiceBus: handler \"" << name << "\" threw: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::vector<nlohmann::json> ServiceBus::QueryService(const std::string& name,
                                                      const nlohmann::json& params) {
    std::vector<nlohmann::json> results;
    auto it = services.find(name);
    if (it == services.end()) return results;
    if (debugMode)
        std::cout << "ServiceBus: QueryService(\"" << name << "\") — "
                  << it->second.size() << " provider(s)" << std::endl;
    for (auto& reg : it->second) {
        try {
            results.push_back(reg.handler(params));
        } catch (const std::exception& e) {
            std::cerr << "ServiceBus: query handler \"" << name
                      << "\" (id=" << reg.id << ") threw: " << e.what() << std::endl;
        }
    }
    return results;
}

bool ServiceBus::HasService(const std::string& name) const {
    auto it = services.find(name);
    return it != services.end() && !it->second.empty();
}

} // namespace Core
