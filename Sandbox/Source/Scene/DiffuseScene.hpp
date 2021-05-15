//#pragma once
//
//#include <Ziben/Scene/Scene.hpp>
//#include <Ziben/Renderer/Shader.hpp>
//
//#include "Entities/Torus.hpp"
//
//class DiffuseScene : public Ziben::Scene {
//public:
//    DiffuseScene();
//
//    void OnEvent(Ziben::Event& event) override;
//    void OnUpdate(const Ziben::TimeStep& ts) override;
//    void OnRender() override;
//    void OnImGuiRender() override;
//
//private:
//    glm::mat4                 m_Model{};
//    glm::mat4                 m_View{};
//    glm::mat4                 m_Projection{};
//    Torus                     m_Torus;
//    Ziben::Ref<Ziben::Shader> m_Shader;
//    glm::vec4                 m_LightPosition;
//    glm::vec3                 m_Kd;
//    glm::vec3                 m_Ld;
//    glm::vec3                 m_Angles;
//
//}; // class DiffuseScene