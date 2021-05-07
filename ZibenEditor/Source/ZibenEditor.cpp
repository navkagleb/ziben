#include "ZibenEditor.hpp"

#include "EditorLayer.hpp"

namespace Ziben {

    ZibenEditor::ZibenEditor()
        : Application("ZibenEditor", 1600, 900) {

        PushLayer(new EditorLayer);
    }

} // namespace Ziben

