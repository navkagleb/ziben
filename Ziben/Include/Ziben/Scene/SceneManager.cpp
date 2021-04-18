#include "SceneManager.hpp"

namespace Ziben {

    Scene* SceneManager::GetActiveScene() {
        if (!m_Scenes.empty())
            return m_Scenes.top();

        throw std::invalid_argument("SceneManager is empty!");
    }

    void SceneManager::PushScene(Scene* scene) {
        m_Scenes.push(scene);
    }

    void SceneManager::PopScene() {
        delete m_Scenes.top();
        m_Scenes.pop();
    }

} // namespace Ziben