#pragma once

#include "Scene.hpp"

namespace Ziben {

    class SceneManager {
    public:
        SceneManager() = default;
        ~SceneManager() = default;

        [[nodiscard]] inline bool HasActiveScene() const { return !m_Scenes.empty(); }

        Scene* GetActiveScene();

        void PushScene(Scene* scene);
        void PopScene();

    private:
        std::stack<Scene*> m_Scenes;

    }; // class SceneManager

} // namespace Ziben