//#pragma once
//
//#include <Ziben/Scene/Scene.hpp>
//#include <Ziben/Renderer/Shader.hpp>
//#include <Ziben/Renderer/PerspectiveCamera.hpp>
//
//#include "Entities/Torus.hpp"
//
//class ADSScene : public Ziben::Scene {
//public:
//    ADSScene();
//
//    void OnEvent(Ziben::Event& event) override;
//    void OnUpdate(const Ziben::TimeStep& ts) override;
//    void OnRender() override;
//    void OnImGuiRender() override;
//
//private:
//    struct Light {
//        glm::vec4 Position;
//        glm::vec3 La;
//        glm::vec3 Ld;
//        glm::vec3 Ls;
//    };
//
//    struct Material {
//        glm::vec3 Ka;
//        glm::vec3 Kd;
//        glm::vec3 Ks;
//        float     Shininess;
//    };
//
//private:
//    Ziben::Ref<Ziben::Shader> m_Shader;
//    Torus                     m_Torus;
//    float                     m_Angle;
//    glm::mat4                 m_Model;
//    glm::mat4                 m_View;
//    glm::mat4                 m_Projection;
//    Light                     m_Light;
//    Material                  m_Material;
//    glm::vec4                 m_WorldLight;
//    glm::vec3                 m_Angles;
//    Ziben::PerspectiveCamera  m_Camera;
//
//}; // class ADSScene
