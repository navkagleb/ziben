#include "SceneManager.hpp"

namespace Ziben {

    void SceneManager::PushScene(Scene* scene) {
        m_Scenes.push(scene);
    }

    void SceneManager::PopScene() {
        delete m_Scenes.top();
        m_Scenes.pop();
    }

} // namespace Ziben