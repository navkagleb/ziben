#include "ZibenEditor.hpp"

#include "EditorLayer.hpp"

namespace Ziben {

    ZibenEditor::ZibenEditor()
        : Application("ZibenEditor", 1280, 720) {

        PushLayer(new EditorLayer);
    }

} // namespace Ziben

