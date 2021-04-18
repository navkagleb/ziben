#include "Scene.hpp"

#include <utility>

namespace Ziben {

    Scene::Scene(std::string name)
        : m_Name(std::move(name)) {

    }

    Scene::~Scene() {

    }

} // namespace Ziben