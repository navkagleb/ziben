#pragma once

#include <Ziben/Scene/Layer.hpp>

class MenuLayer : public Ziben::Layer {
public:
    MenuLayer();
    ~MenuLayer() override = default;

    void OnImGuiRender() override;

}; // class MenuLayer