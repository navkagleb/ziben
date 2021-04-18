#pragma once

#include "Scene.hpp"

namespace Ziben {

    class SceneManager {
    public:
        SceneManager() = default;
        ~SceneManager() = default;

        inline Scene* GetActiveScene() { return m_Scenes.top(); }

        void PushScene(Scene* scene);
        void PopScene();

    private:
        std::stack<Scene*> m_Scenes;

    }; // class SceneManager

} // namespace Ziben