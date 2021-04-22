#pragma once

namespace Ziben {

    using ButtonCode = int16_t;

    namespace Button {

        // From glfw3 key codes
        enum : ButtonCode {
            Unknown      = -1,

            C0           = 0,
            C1           = 1,
            C2           = 2,
            C3           = 3,
            C4           = 4,
            C5           = 5,
            C6           = 6,
            C7           = 7,

            ButtonLast   = C7,
            ButtonLeft   = C0,
            ButtonRight  = C1,
            ButtonMiddle = C2
        };

    } // namespace Button

} // namespace Ziben

